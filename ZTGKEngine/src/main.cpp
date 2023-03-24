#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image/stb_image.h>
#include <assimp/Importer.hpp>

#include "Shader.h"
#include "Model.h"
#include "GraphNode.h"
#include "Camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
int oldMouseButtonState = GLFW_RELEASE;
bool isMouseActive = false;

const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 600;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

struct PLight {
    glm::vec3 position = { 0.0f, 0.0f, 0.0f };
    float color[3] = { 1.0f, 1.0f, 1.0f };

    float constant;
    float linear;
    float quadratic;

} pointLight;

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    //Below I set the function to be used in case our window changed size
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    //I try to use Glad as adresses loader and i realy hope it works
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);

    //Resource and scene setup
    Shader lightShader("res/shaders/enlightened.vert", "res/shaders/enlightened.frag");
    Shader bulbShader("res/shaders/light.vert", "res/shaders/light.frag");

    Model brick("res/models/House.obj");
    Model bulb("res/models/House.obj");

    brick.SetShader(&lightShader);
    bulb.SetShader(&bulbShader);
    
    GraphNode* brickNode = new GraphNode(&brick);
    GraphNode* bulbNode = new GraphNode(&bulb);
    GraphNode* world = new GraphNode();

    world->AddChild(brickNode);
    world->AddChild(bulbNode);

    brickNode->Translate(glm::vec3(-1.0f, -1.0f, -1.0f));
    brickNode->Scale(0.5f);
    brickNode->Rotate(45, glm::vec3(0.0f, 1.0f, 0.0f));

    bulbNode->setTranslate(&pointLight.position);
    bulbNode->Scale(0.1);

    world->RenderTransform();
    world->Update();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        lightShader.use();
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);
        lightShader.setVec3("viewPos", camera.Position);
        lightShader.setVec3("pointLightPos", pointLight.position);
        lightShader.setVec3("pointLightColor", glm::vec3({ pointLight.color[0], pointLight.color[1], pointLight.color[2] }));
        lightShader.setFloat("LightConstant", 1.0f);
        lightShader.setFloat("LightLinear", 0.09f);
        lightShader.setFloat("LightQuadratic", 0.032f);
        lightShader.setInt("texture_diffuse1", 0);

        bulbShader.use();
        bulbShader.setMat4("projection", projection);
        bulbShader.setMat4("view", view);

        //Processing input here
        processInput(window);

        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        world->Draw();
        //bulbNode->Draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime * 5);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime * 5);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime * 5);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime * 5);

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS && oldMouseButtonState == GLFW_RELEASE) {
        if (isMouseActive) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            isMouseActive = !isMouseActive;
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            isMouseActive = !isMouseActive;
            firstMouse = true;

        }
    }
    oldMouseButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}