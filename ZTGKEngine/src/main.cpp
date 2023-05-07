#include "SceneManager.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
glm::vec3 rayCast(GLFWwindow* window, glm::mat4 projection, glm::mat4 view);

int oldMouseButtonState = GLFW_RELEASE;
bool isMouseActive = false;

unsigned int SCR_WIDTH = 1900;
unsigned int SCR_HEIGHT = 1000;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
float texOffset = 0;
bool firstMouse = true;

bool lightVersion = true;

//rainbow anim----
float r = 1.0f, g = 0.0f, b = 0.0f;
float animSpeed = 1.5f;
int animStage = 1;
bool isRunning = true;
glm::vec3 rainbowColor = { 1.0, 0.0, 0.0 };
//----------------
Text text;

glm::vec3 castedRay = glm::vec3(1);

ClickPicker picker = ClickPicker();

int objectID = 1;

unsigned int currentlyPicked = 0;
bool singleClick = true;

SceneManager sceneManager;

struct PLight {
    glm::vec3 position = { 0.4f, 0.5f, 0.0f };
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

    //glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    //Resource and scene setup
    Shader defaultShader("res/shaders/enlightened.vert", "res/shaders/enlightened.frag");
    Shader lightShader("res/shaders/light.vert", "res/shaders/light.frag");
    Shader pickShader("res/shaders/clickpick.vert", "res/shaders/clickpick.frag");
    Shader texturePickShader("res/shaders/primitiveTexture.vert", "res/shaders/texturePickShader.frag");
    Shader primitiveColorShader("res/shaders/primitiveColor.vert", "res/shaders/primitiveColor.frag");
    Shader primitiveAnimTextureShader("res/shaders/primitiveTexture.vert", "res/shaders/primitiveAnimTexture.frag");
    Shader primitiveTextureShader("res/shaders/primitiveTexture.vert", "res/shaders/primitiveTexture.frag");
    Shader rainbowPrimitiveShader("res/shaders/primitiveColor.vert", "res/shaders/primitiveRainbowColor.frag");
    Shader textShader("res/shaders/text.vert", "res/shaders/text.frag");
    Shader outlineShader("res/shaders/basic.vert", "res/shaders/basic.frag");

    ApRectangle rec(0, 0, SCR_WIDTH, SCR_HEIGHT, glm::vec3{1.0, 0.0, 1.0});
    ApRectangle recTex(0, 0, SCR_WIDTH, SCR_HEIGHT, "res/models/everest.jpg");
    //ApRectangle bottomPanel(0, 0, SCR_WIDTH, SCR_HEIGHT, "res/models/gui_panel.png");
    ApRectangle rainbowSquare(35, SCR_HEIGHT - 75, 300, 50, rainbowColor);
    text.init("res/fonts/arial/arial.ttf");

    rec.SetShader(&primitiveColorShader);
    recTex.SetShader(&primitiveAnimTextureShader);
    //bottomPanel.SetShader(&primitiveTextureShader);
    rainbowSquare.SetShader(&rainbowPrimitiveShader);

    sceneManager.defaultShader = &defaultShader;
    sceneManager.lightShader = &lightShader;
    sceneManager.textureShader = &primitiveTextureShader;

    sceneManager.Setup( window, &lightVersion, &SCR_WIDTH, &SCR_HEIGHT);

    sceneManager.Update();

    //Before entering the loop we activate setup functions in all the scripts
    sceneManager.ExecuteStartScripts();

    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);

    float time = 0;

    //Scene1Dark->SetActive(false);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        time += ApTime::instance().deltaTime;

        //Counting new deltaTime
        ApTime::instance().Update();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        glm::mat4 projectionPrimitive = glm::ortho(0.0f, float(SCR_WIDTH), 0.0f, float(SCR_HEIGHT));
        glm::mat4 viewPrimitive = glm::mat4(1.0);

        glm::vec3 newCastedRay = rayCast(window, projection, view);
        if (castedRay != newCastedRay)
        {
            castedRay = newCastedRay;
            //std::cout << "X: " << castedRay.x << "   ";
            //std::cout << "Y: " << castedRay.y << "   ";
            //std::cout << "Z: " << castedRay.y << std::endl;
        }

        defaultShader.use();
        defaultShader.setMat4("projection", projection);
        defaultShader.setMat4("view", view);
        defaultShader.setVec3("viewPos", camera.Position);
        defaultShader.setVec3("pointLightPos", pointLight.position);
        if (lightVersion)
            defaultShader.setVec3("pointLightColor", glm::vec3({ pointLight.color[0], pointLight.color[1], pointLight.color[2] }));
        else
            defaultShader.setVec3("pointLightColor", glm::vec3({ pointLight.color2[0], pointLight.color2[1], pointLight.color2[2] }));
        defaultShader.setFloat("LightConstant", 1.0f);
        defaultShader.setFloat("LightLinear", 0.09f);
        defaultShader.setFloat("LightQuadratic", 0.032f);

        outlineShader.use();
        outlineShader.setMat4("projection", projection);
        outlineShader.setMat4("view", view);

        lightShader.use();
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);

        primitiveColorShader.use();
        primitiveColorShader.setMat4("projection", projectionPrimitive);
        primitiveColorShader.setMat4("view", viewPrimitive);

        primitiveAnimTextureShader.use();
        primitiveAnimTextureShader.setMat4("projection", projectionPrimitive);
        primitiveAnimTextureShader.setMat4("view", viewPrimitive);
        primitiveAnimTextureShader.setFloat("texOffset", texOffset);

        primitiveTextureShader.use();
        primitiveTextureShader.setMat4("projection", projectionPrimitive);
        primitiveTextureShader.setMat4("view", viewPrimitive);

        rainbowPrimitiveShader.use();
        rainbowPrimitiveShader.setMat4("projection", projectionPrimitive);
        rainbowPrimitiveShader.setMat4("view", viewPrimitive);

        textShader.use();
        textShader.setMat4("projection", projectionPrimitive);
        
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
        {
            picker.Enable();
            pickShader.use();
            pickShader.setMat4("projection", projection);
            pickShader.setMat4("view", view);
            texturePickShader.use();
            texturePickShader.setMat4("projection", projectionPrimitive);
            texturePickShader.setMat4("view", viewPrimitive);
            sceneManager.RenderMousePicking(pickShader,texturePickShader);
            double mouseXd;
            double mouseYd;
            glfwGetCursorPos(window, &mouseXd, &mouseYd);
            glFlush();
            glFinish();
            ClickPicker::PixelData pixel = picker.Read(mouseXd, SCR_HEIGHT - mouseYd);
            picker.Disable();
            currentlyPicked = pixel.ObjectID;
        }
        else
        {
            currentlyPicked = 0;
            singleClick = true;
        }
        
        //Processing input here
        processInput(window);

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        sceneManager.Update(currentlyPicked, singleClick);
        
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
        {
            singleClick = false;
        }
        glEnable(GL_BLEND);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        sceneManager.Render();
        
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);
        sceneManager.world->Scale(1.3f);
        sceneManager.world->Update();
        sceneManager.RenderWithShader(outlineShader, 2);
        sceneManager.world->Scale(1.0f);
        sceneManager.world->Update();
        //sceneManager.Render();
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glEnable(GL_DEPTH_TEST);

        //sceneManager.Render();
        //recTex.Draw();
        texOffset += 0.1 * ApTime::instance().deltaTime;
        glDepthFunc(GL_ALWAYS);
        //rainbowSquare.Draw();

        //Rainbow animation-----
        if (isRunning)
        {
            switch (animStage)
            {
            case 1:
                g += animSpeed * ApTime::instance().deltaTime;
                if (g >= 1.0f)
                {
                    g = 1.0f;
                    animStage++;
                }
                break;
            case 2:
                r -= animSpeed * ApTime::instance().deltaTime;
                if (r <= 0.0f)
                {
                    r = 0.0f;
                    animStage++;
                }
                break;
            case 3:
                b += animSpeed * ApTime::instance().deltaTime;
                if (b >= 1.0f)
                {
                    b = 1.0f;
                    animStage++;
                }
                break;
            case 4:
                g -= animSpeed * ApTime::instance().deltaTime;
                if (g <= 0.0f)
                {
                    g = 0.0f;
                    animStage++;
                }
                break;
            case 5:
                r += animSpeed * ApTime::instance().deltaTime;
                if (r >= 1.0f)
                {
                    r = 1.0f;
                    animStage++;
                }
                break;
            case 6:
                b -= animSpeed * ApTime::instance().deltaTime;
                if (b <= 0.0f)
                {
                    b = 0.0f;
                    animStage = 1;
                }
                break;
            }

            rainbowSquare.SetColor(glm::vec3(r,g,b));
        }
        //----------------------

        //bottomPanel.Draw();
        //text.RenderText(textShader, to_string(time), float(SCR_WIDTH) - 200, float(SCR_HEIGHT) - 75, 0.5, glm::vec3(0.9, 0.1f, 0.1f));
        glDepthFunc(GL_LESS);
        glDisable(GL_BLEND);
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
        camera.ProcessKeyboard(FORWARD, ApTime::instance().deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, ApTime::instance().deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, ApTime::instance().deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, ApTime::instance().deltaTime);

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
    SCR_WIDTH= width;
    SCR_HEIGHT = height;
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