#ifndef PRIMITIVE_H
#define PRIMITIVE_H


#include "../Mesh.h"


class Primitive 
{
private:
	

public:
    //transformation matrix(translation, scaling, rotation)
    glm::mat4* Transform;

    //textures
    vector<Texture> textures;
    //color
    glm::vec3 color;
    //mesh
    Mesh mesh;
    //pointer to shader program used for this primitive
    Shader* shader;
    

    //Simple setters for whole transformation matrix and shader program
    void setTransform(glm::mat4* matrix) { Transform = matrix; }
    void SetShader(Shader* s) { shader = s; }
    void SetColor(glm::vec3 c) { color = c; }

    Primitive() {};
    virtual ~Primitive() {};

    virtual void Draw() = 0;


};

#endif
