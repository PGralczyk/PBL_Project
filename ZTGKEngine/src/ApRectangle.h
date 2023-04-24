#ifndef RECTANGLE_H
#define RECTANGLE_H


#include "Primitive.h"


class ApRectangle : public Primitive
{
private:


public:
    
    ApRectangle(int xPos, int yPos, int width, int height, glm::vec3 color)
    {
        //glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(xPos, yPos, 0));
        this->Transform = new glm::mat4(1.0);
        this->color = color;

        /*vector<Vertex> vertices =
        {
            {{ -0.5f, -0.5f, -0.9f}, { 0, 0, 0}, { 0, 0}, { 0, 0, 0}, { 0, 0, 0}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f} },
            {{ -0.5f, 0.5f, -0.9f}, { 0, 0, 0}, { 0, 1}, { 0, 0, 0}, { 0, 0, 0}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f} },
            {{ 0.5f, 0.5f, -0.9f}, { 0, 0, 0}, { 1, 1}, { 0, 0, 0}, { 0, 0, 0}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f} },
            {{0.5, -0.5, -0.9f}, {0, 0, 0}, {1, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f} }
        };*/

        vector<Vertex> vertices =
        {
            {{ xPos, yPos, -0.999}, { 0, 0, 0}, { 0, 0}, { 0, 0, 0}, { 0, 0, 0}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f} },
            {{ xPos, yPos + height, -0.999}, { 0, 0, 0}, { 0, 1}, { 0, 0, 0}, { 0, 0, 0}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f} },
            {{ xPos + width, yPos + height, -0.999}, { 0, 0, 0}, { 1, 1}, { 0, 0, 0}, { 0, 0, 0}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f} },
            {{xPos + width, yPos, -0.999}, {0, 0, 0}, {1, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f} }
        };

        vector<unsigned int> indices = {0, 1, 2, 2, 3, 0};

        Mesh m(vertices, indices, this->textures);

        this->mesh = m;

    }

    ApRectangle(int xPos, int yPos, int width, int height, string path)
    {
        //glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(xPos, yPos, 0));
        this->Transform = new glm::mat4(1.0);
        this->color = glm::vec3(0.0f, 0.0f, 0.0f);

        /*vector<Vertex> vertices =
        {
            {{ -0.5f, -0.5f, -0.9f}, { 0, 0, 0}, { 0, 0}, { 0, 0, 0}, { 0, 0, 0}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f} },
            {{ -0.5f, 0.5f, -0.9f}, { 0, 0, 0}, { 0, 1}, { 0, 0, 0}, { 0, 0, 0}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f} },
            {{ 0.5f, 0.5f, -0.9f}, { 0, 0, 0}, { 1, 1}, { 0, 0, 0}, { 0, 0, 0}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f} },
            {{0.5, -0.5, -0.9f}, {0, 0, 0}, {1, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f} }
        };*/

        vector<Vertex> vertices =
        {
            {{ xPos, yPos, -0.999}, { 0, 0, 0}, { 0, 1}, { 0, 0, 0}, { 0, 0, 0}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f} },
            {{ xPos, yPos + height, -0.999}, { 0, 0, 0}, { 0, 0}, { 0, 0, 0}, { 0, 0, 0}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f} },
            {{ xPos + width, yPos + height, -0.999}, { 0, 0, 0}, { 1, 0}, { 0, 0, 0}, { 0, 0, 0}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f} },
            {{xPos + width, yPos, -0.999}, {0, 0, 0}, {1, 1}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0, 0}, {0.0f, 0.0f, 0.0f, 0.0f} }
        };

        vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0 };

        Texture texture;
        texture.id = TextureFromFile(path.c_str());
        texture.isEmbedded = false;
        texture.type = "texture_diffuse";
        textures.push_back(texture);

        Mesh m(vertices, indices, this->textures);

        this->mesh = m;

    }

    ~ApRectangle()
    {
        delete Transform;
    };

    
    void Draw() override
    {
        //glDepthFunc(GL_GREATER);
        shader->use();
        shader->setVec3("color", color);
        mesh.Draw(*shader, Transform);
       //glDepthFunc(GL_LESS);
        //std::cout << "Draw Rect!\n";
    }
    
    
    GraphNode* CreateNode(int objectId)
    {
        Model* model = new Model(&mesh, objectId);
        model->SetShader(shader);
        return new GraphNode(model);
    }
};

#endif

