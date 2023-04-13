#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image/stb_image.h>
#include <assimp/Importer.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Shader.h"
#include "Model.h"
#include "GraphNode.h"
#include "Camera.h"
#include "TestRealtimeScript.h"
#include "OtherTestRealtimeScript.h"
#include "ApTime.h"
#include "RoomSwapManager.h"
#include "ClickPicker.h"
#include "Animation.h"
#include "Animator.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
glm::vec3 rayCast(GLFWwindow* window, glm::mat4 projection, glm::mat4 view);

int oldMouseButtonState = GLFW_RELEASE;
bool isMouseActive = false;

const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

glm::vec3 castedRay = glm::vec3(1);

bool lightVersion = true;

ClickPicker picker = ClickPicker();

int objectID = 1;

unsigned int currentlyPicked = 0;
bool singleClick = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

struct PLight {
    glm::vec3 position = { -2.0f, -0.8f, 0.0f };
    float color[3] = { 1.0f, 1.0f, 1.0f };
    float color2[3] = { 0.70f, 1.0f, 1.0f };

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
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GAME", NULL, NULL);
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

    glEnable(GL_DEPTH_TEST);

    //Resource and scene setup
    Shader lightShader("res/shaders/enlightened.vert", "res/shaders/enlightened.frag");
    Shader bulbShader("res/shaders/light.vert", "res/shaders/light.frag");
    Shader pickShader("res/shaders/clickpick.vert", "res/shaders/clickpick.frag");
    Shader animationShader("res/shaders/skinner.vert", "res/shaders/skinner.frag");

    Model brick("res/models/krzeselko.fbx", objectID++);
    Model brick2("res/models/krzeselko.fbx", objectID++);
    Model bulb("res/models/House.obj", objectID++);
    Model animated("res/models/thezelda3.fbx", objectID++);

    brick.SetShader(&lightShader);
    brick2.SetShader(&lightShader);
    bulb.SetShader(&bulbShader);
    animated.SetShader(&animationShader);

    Animation animation("res/models/Defeated.dae", &animated);
    Animator animator(&animation);
    
    //Handles the whole game world
    GraphNode* world = new GraphNode();
    //------SCENE-1------
    //Each location is presented as scene
    GraphNode* Scene1 = new GraphNode();
    //Bright
    GraphNode* Scene1Bright = new GraphNode();
    GraphNode* brickNode = new GraphNode(&brick);
    GraphNode* bulbNode = new GraphNode(&bulb);
    //Dark
    GraphNode* Scene1Dark = new GraphNode();
    GraphNode* brickNode2 = new GraphNode(&brick2);

    //Animated model node
    GraphNode* animationNode = new GraphNode(&animated);

    //Adding script here
    brickNode->AddScript(new TestRealtimeScript(brickNode));
    brickNode2->AddScript(new OtherTestRealtimeScript(brickNode2));
    Scene1->AddScript(new RoomSwapManager(Scene1, Scene1Bright, Scene1Dark, window, &lightVersion));

    world->AddChild(Scene1);
    Scene1->AddChild(Scene1Bright);
    Scene1->AddChild(Scene1Dark);
    Scene1->AddChild(bulbNode);
    Scene1->AddChild(animationNode);
    Scene1Bright->AddChild(brickNode);
    Scene1Dark->AddChild(brickNode2);

    //brickNode->Translate(glm::vec3(-2.0f, -2.0f, -2.0f));
    brickNode2->Scale(0.005f);
    brickNode->Scale(0.005f);
    //brickNode2->Rotate(45, glm::vec3(0.0f, 1.0f, 0.0f));
    brickNode2->Translate(glm::vec3(2.0f, -2.0f, 0.0f));

    bulbNode->Scale(0.1f);
    bulbNode->Translate(pointLight.position);

    world->RenderTransform();
    world->Update();

    //Before entering the loop we activate setup functions in all the scripts
    world->ExecuteStartScripts();

    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);

    //Scene1Dark->SetActive(false);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        //Animation stuff
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //Counting new deltaTime
        ApTime::instance().Update();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        glm::vec3 newCastedRay = rayCast(window, projection, view);
        if (castedRay != newCastedRay)
        {
            castedRay = newCastedRay;
            //std::cout << "X: " << castedRay.x << "   ";
            //std::cout << "Y: " << castedRay.y << "   ";
            //std::cout << "Z: " << castedRay.y << std::endl;
        }

        lightShader.use();
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);
        lightShader.setVec3("viewPos", camera.Position);
        lightShader.setVec3("pointLightPos", pointLight.position);
        if(lightVersion)
            lightShader.setVec3("pointLightColor", glm::vec3({ pointLight.color[0], pointLight.color[1], pointLight.color[2] }));
        else
            lightShader.setVec3("pointLightColor", glm::vec3({ pointLight.color2[0], pointLight.color2[1], pointLight.color2[2] }));
        lightShader.setFloat("LightConstant", 1.0f);
        lightShader.setFloat("LightLinear", 0.09f);
        lightShader.setFloat("LightQuadratic", 0.032f);

        bulbShader.use();
        bulbShader.setMat4("projection", projection);
        bulbShader.setMat4("view", view);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
        {
            picker.Enable();
            pickShader.use();
            pickShader.setMat4("projection", projection);
            pickShader.setMat4("view", view);
            world->nPickDraw(pickShader);
            double mouseXd;
            double mouseYd;
            glfwGetCursorPos(window, &mouseXd, &mouseYd);
            glFlush();
            glFinish();
            ClickPicker::PixelData pixel = picker.Read(mouseXd, 600 - mouseYd);
            picker.Disable();
            currentlyPicked = pixel.ObjectID;
            //std::cout << currentlyPicked << std::endl;
        }
        else
        {
            currentlyPicked = 0;
            singleClick = true;
        }

        //Processing input here
        processInput(window);
        //Animation update
        animator.UpdateAnimation(deltaTime);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        animationShader.use();
        animationShader.setMat4("projection", projection);
        animationShader.setMat4("view", view);

        auto transforms = animator.GetFinalBoneMatrices();
        for (int i = 0; i < transforms.size(); ++i) {
            animationShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
            //std::cout << glm::to_string(transforms[i]) <<std::endl;
        }

        world->Update(currentlyPicked, singleClick);
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
        {
            singleClick = false;
        }
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
        camera.ProcessKeyboard(FORWARD, ApTime::instance().deltaTime * 5);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, ApTime::instance().deltaTime * 5);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, ApTime::instance().deltaTime * 5);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, ApTime::instance().deltaTime * 5);

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

glm::vec3 rayCast(GLFWwindow* window, glm::mat4 projection, glm::mat4 view)
{
    //GETTING MOUSE POSITION
    double mouseXd;
    double mouseYd;
    glfwGetCursorPos(window, &mouseXd, &mouseYd);
    float mouseX = (float)mouseXd;
    float mouseY = (float)mouseYd;

    //NORMALIZING THEM
    float normalizedX = (2.0f * mouseX) / SCR_WIDTH - 1.0f;
    float normalizedY = 1.0f - (2.0f * mouseY) / SCR_HEIGHT;
    glm::vec2 normalizedCoords = glm::vec2(normalizedX, normalizedY);

    //CONVERTING TO CLIP SPACE(we just add 1 at z, so that our ray faces into the screen)
    glm::vec4 clipCoords = glm::vec4(normalizedCoords.x, normalizedCoords.y, 1.0f, 1.0f);

    //CONVERTING TO EYE SPACE (by using inverse projection matrix)
    glm::mat4 invProjection = glm::inverse(projection);
    glm::vec4 eyeCoords = invProjection * clipCoords;

    //CONVERTING TO WORLD SPACE
    glm::mat4 invView = glm::inverse(view);
    glm::vec4 worldCoords = invView * eyeCoords;

    //CONVERTING 4D VECTOR TO 3D(since we no longer need it to be in 4D)
    glm::vec3 castedRay = glm::vec3(worldCoords.x, worldCoords.y, worldCoords.z);
    castedRay = glm::normalize(castedRay);
    return castedRay;
}