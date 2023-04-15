#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#include <string>
#include <vector>
using namespace std;

#define MAX_BONE_INFLUENCE 4

//What is this class?
//Mesh/meshes are components that can be included in model(we can use model to create objects with
//no graphical representation). This class uses assimp library to load data about our shape and then
//it can be used to display said objects.

//First, very important struct to store data about Vertecies
//That includes its position, normals, coordinates of 2D teture
//that is interpolated for the whole objects based on coordinates,
//tangents and bitangents.
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    float m_Weights[MAX_BONE_INFLUENCE];
};

//And this struct is for storing texture (2D image applied to our mesh)
struct Texture {
    unsigned int id;
    string type;
    string path;
    bool isEmbedded;
};

class Mesh {
public:
    // Mesh Data
    //Mesh has multiple verticies (we use our structure from above)
    vector<Vertex>       vertices;
    //Indicies are a special type of data. They allow you to go back to a vertex defined before to use it as
    //vertex of currently drawn triangle. Without indicies you would have to define such verticies again,
    //which is much more data than a single uint
    vector<unsigned int> indices;
    //We can use multiple textures for our object
    vector<Texture>      textures;
    //ID for VAO used in rendering in 3D
    unsigned int VAO;
    //For now we don't instantiate objects but it's very helpful for optimization
    bool instanced;
    // Constructor used in Model class

    Mesh() {};

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, bool instance = false)
    {
        instanced = instance;
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        setupMesh();
    }

    // Render the mesh
    void Draw(Shader& shader, glm::mat4* model, int* objectID = nullptr, bool picking = false)
    {
        //For now we only use this part, because we don't instantiate objects
        //(instantiate is about having a single object reused multiple times),
        //we can transform it, but we would still have to use just one chunk of data
        //instead of creating multiple meshes
        if (!instanced) {
            unsigned int diffuseNr = 1;
            unsigned int specularNr = 1;
            unsigned int normalNr = 1;
            unsigned int heightNr = 1;
            //We want to draw an object, so we have to tell OpenGL to us shader program we have already 
            //prepared for this object
            shader.use();
            //We set our model matrix (used for transformations) as uniform for shader program
            shader.setMat4("model", *model);

            if (picking && objectID != nullptr) {
                shader.setInt("gObjectIndex", *objectID);
            }

            //We apply every texture that is in our object
            for (unsigned int i = 0; i < textures.size(); i++)
            {
                //We have to activate the texture we are currently working with
                glActiveTexture(GL_TEXTURE0 + i);

                string number;
                string name = textures[i].type;
                if (name == "texture_diffuse")
                    number = std::to_string(diffuseNr++);
                else if (name == "texture_specular")
                    number = std::to_string(specularNr++);
                else if (name == "texture_normal")
                    number = std::to_string(normalNr++);
                else if (name == "texture_height")
                    number = std::to_string(heightNr++);

                //Specyfify the value of a uniform variable for the current program object
                glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
                glBindTexture(GL_TEXTURE_2D, textures[i].id);
            }

            //We finally bind the vertex array and draw the mesh using triangles
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            glActiveTexture(GL_TEXTURE0);
        }
        //ONLY FOR INSTANTIONING:
        else {
            glBindVertexArray(VAO);
            // set attribute pointers for matrix (4 times vec4)
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
            glEnableVertexAttribArray(5);
            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

            glVertexAttribDivisor(3, 1);
            glVertexAttribDivisor(4, 1);
            glVertexAttribDivisor(5, 1);
            glVertexAttribDivisor(6, 1);

            glBindVertexArray(0);

            shader.use();
            shader.setInt("texture_diffuse1", 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textures[0].id);

            glBindVertexArray(VAO);
            glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0, 40000);
            glBindVertexArray(0);
        }
    }

private:
    // Here we have id of buffers
    unsigned int VBO, EBO;

    // Initialization
    void setupMesh()
    {
        //We generate Vertex array, vertex buffer and element buffer
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        //We have to bind the array and then VBO to it
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //We start buffering data. We tell our buffer to get space for all verticies ready and to
        //buffer them all
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        //Now we do similiarly with EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        //All that's left is to divide data in buffers based on what they are(for now they are just
        //a stream of stupid numbers). These "attributes" are passed every time we draw the object
        //(hence their difference from uniforms) via "layout" in vertex shader. We have space for
        //all types of data(tengents, bitangents), but for now we only use position, normals and
        //texCoords. 
        //HOW TO DO THAT:
        //glVertexAttribPointer(attribID(used in layout(location = *)),
        //how many things are passed(for example: TexCoords are just 2, because they are 2D coordinates)
        //what are those things we pass,
        //is normalized?,
        //stride (length of the whole thing),
        //pointer tho that particular chunk of data (that's what this weird (void*) is for);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
        //Unbinding
        glBindVertexArray(0);
    }
};
#endif