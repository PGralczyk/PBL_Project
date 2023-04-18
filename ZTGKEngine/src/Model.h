#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <iostream>
using namespace std;

unsigned int TextureFromFile(const char* path, const string& directory = "", bool gamma = false);
unsigned int TextureFromMemory(const aiTexture* texture);

//What is this class?
//It holds a model (graphical representation(not necessary) and position of an object)

class Model
{
public:
    // Model data 
    //transformation matrix(translation, scaling, rotation)
    glm::mat4* Transform;
    //textures
    vector<Texture> textures_loaded;
    //meshes
    vector<Mesh> meshes;
    string directory;
    //pointer to shader program used for this model
    Shader* shader;
    //For now we don't instantiate objects but it's very helpful for optimization
    bool instanced;
    //Some objects may use gamma correction
    bool gammaCorrection;
    
    int objectID;

    //Simple setters for whole transformation matrix and shader program
    void setTransform(glm::mat4* matrix) { Transform = matrix; }
    void SetShader(Shader* s) 
    { 
        shader = s; 
    }

    //Constructor. We have to specify path to main object file(obj, fbx...) and can set optional stuff
    //like instantiation or gamma correction
    Model(string const& path, int objectId, bool instance = false, bool gamma = false) : gammaCorrection(gamma)
    {
        instanced = instance;
        loadModel(path);
        Transform = new glm::mat4(1);
        this->objectID = objectId;
    }

    //We simply draw every mesh using Mesh class functionality
    void Draw()
    {
        //std::cout << (shader != NULL) << std::endl;
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(*shader, Transform, &objectID);
    }

    void pickDraw(Shader& pickShader) {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(pickShader, Transform, &objectID, true);
    }

private:
    //This function uses Assimp library to load model from given file
    void loadModel(string const& path)
    {
        Assimp::Importer importer;

        unsigned int processFlags =
            aiProcess_CalcTangentSpace | // calculate tangents and bitangents if possible
            aiProcess_JoinIdenticalVertices | // join identical vertices/ optimize indexing
            //aiProcess_ValidateDataStructure  | // perform a full validation of the loader's output
            aiProcess_Triangulate | // Ensure all verticies are triangulated (each 3 vertices are triangle)
            //aiProcess_ConvertToLeftHanded | // convert everything to D3D left handed space (by default right-handed, for OpenGL)
            //aiProcess_SortByPType | // ?
            //aiProcess_ImproveCacheLocality | // improve the cache locality of the output vertices
            //aiProcess_RemoveRedundantMaterials | // remove redundant materials
            //aiProcess_FindDegenerates | // remove degenerated polygons from the import
            //aiProcess_FindInvalidData | // detect invalid model data, such as invalid normal vectors
            //aiProcess_GenUVCoords | // convert spherical, cylindrical, box and planar mapping to proper UVs
            //aiProcess_TransformUVCoords | // preprocess UV transformations (scaling, translation ...)
            //aiProcess_FindInstances | // search for instanced meshes and remove them by references to one master
            //aiProcess_LimitBoneWeights | // limit bone weights to 4 per vertex
            //aiProcess_OptimizeMeshes | // join small meshes, if possible;
            aiProcess_PreTransformVertices | //-- fixes the transformation issue.
            //aiProcess_SplitByBoneCount | // split meshes with too many bones. Necessary for our (limited) hardware skinning shader
            aiProcess_GenSmoothNormals | // ?
            aiProcess_FlipUVs | // flip texture vertically
            0;

        const aiScene* scene = importer.ReadFile(path, processFlags);
        // Check for errors
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }

        directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);
    }

    //Objects are stored in hierarchy. We look for all the nodes in that object tree and
    //process every mesh in every single one,then we look it that node has children and use
    //this function on them as well
    void processNode(aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }

    }

    //Finally a function that allows as to create mesh from this spot
    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {
        // Data to fill
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

        // Walk through each of the mesh's vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector;

            // Positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            // Normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }

            // Texture coordinates
            if (mesh->mTextureCoords[0])
            {
                glm::vec2 vec;
                // Use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;

                // Tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;

                // Bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // Process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];


        // 1. Diffuse maps
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. Specular maps
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", scene);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. Normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal", scene);
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. Height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height", scene);
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        //We create mesh from loaded data 
        return Mesh(vertices, indices, textures, instanced);
    }

    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName, const aiScene* scene)
    {
        vector<Texture> textures;
        bool isEmbedded = false;

        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            if (mat->Get(AI_MATKEY_TEXTURE(type, i), str) == AI_SUCCESS) {


                bool skip = false;
                for (unsigned int j = 0; j < textures_loaded.size(); j++)
                {
                    if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                    {
                        textures.push_back(textures_loaded[j]);
                        skip = true;
                        break;
                    }
                }
                if (!skip)
                {
                    Texture texture;
                    //std::cout << str.C_Str() << std::endl;

                    if (auto embedded = scene->GetEmbeddedTexture(str.C_Str())) {
                        //aiTexture* embedded = scene->mTextures[index];

                        texture.id = TextureFromMemory(embedded);
                        isEmbedded = true;
                    }
                    else
                    {     
                        texture.id = TextureFromFile(str.C_Str(), this->directory);
                    }

                    
                    texture.type = typeName;
                    texture.path = str.C_Str();
                    texture.isEmbedded = isEmbedded;
                    textures.push_back(texture);
                    textures_loaded.push_back(texture);
                }
            }
        }
        return textures;
    }
};



#endif