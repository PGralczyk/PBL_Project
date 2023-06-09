#include "./managers/SceneManager.h"
#include "SoundBuffer.h"
#include "./UI/Text.h"
#include "./UI/ApRectangle.h"
#include <thread>
#include "SoundDevice.h"
#include "Music.h"
//#include "./utils/soundUtils.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window, Music* sneakyTheme);
void setGlobalVolume(float v);
float getGlobalVolume();

int oldMouseButtonState = GLFW_RELEASE,
    objectID = 1,
    volumeMode = 0;

unsigned int SCR_WIDTH = 1920,
             SCR_HEIGHT = 1009,
             currentlyPicked = 0;

float lastX = SCR_WIDTH / 2.0f,
      lastY = SCR_HEIGHT / 2.0f,
      texOffset = 0;

bool isMouseActive = false,
      firstMouse = true,
      lightVersion = true,
      isHoldingMouseButton = false,
      singleClick = true,
      canChangeMusic = true,
      buzzerChangeState = false;

glm::vec3 castedRay = glm::vec3(1);
ClickPicker picker = ClickPicker();
Camera camera(glm::vec3(0.75f, 0.5f, 0.0f));
SceneManager sceneManager;


struct PLight {
    //glm::vec3 position = { 0.4f, 0.5f, 0.0f };
    glm::vec3 position = { 0.5f, 1.4f, 0.0f };
    float color[3] = { 0.6f, 0.6f, 0.6f };
    float color2[3] = { 0.2f, 0.9f, 0.9f };

    float constant;
    float linear;
    float quadratic;

} pointLight;

struct CameraPosition {
    glm::vec3 position;
    glm::mat4 projection;
    glm::mat4 view;

} defaultCameraPosition, chessCameraPosition, deskCameraPosition;

Text text;

//MAIN
int main(void)
{
#pragma region Camera Positions

    defaultCameraPosition.position = { 0.97314, 0.423791, -0.04889 };
    defaultCameraPosition.projection = glm::mat4(
        1.27064, 0, 0, 0,
        0, 2.41421, 0, 0,
        0, 0, -1.002, -1,
        0, 0, -0.2002, 0);
    defaultCameraPosition.view = glm::mat4(
        -0.00174269, -0.292371, 0.956303, 0,
        -3.49246e-10, 0.956305, 0.292372, 0,
        -0.999999, 0.000509512, -0.00166654, 0,
        -0.0471941, -0.12073, -1.0546, 1);

    chessCameraPosition.position = { 0.223529, 0.426999, 0.0527502 };
    chessCameraPosition.projection = glm::mat4(
        1.27064, 0, 0, 0,
        0, 2.41421, 0, 0,
        0, 0, -1.002, -1,
        0, 0, -0.2002, 0);
    chessCameraPosition.view = glm::mat4(
        0.0157156, -0.720102, 0.25549, 0,
        0, 0.455546, 0.790212, 0,
        -0.999877, -0.0139902, 0.00715917, 0,
        0.0492308, 0.00518422, -0.482313, 1);

    deskCameraPosition.position = { -0.205362, 0.372362, 0.308516 };
    deskCameraPosition.projection = glm::mat4(
        1.26872, 0, 0, 0,
        0, 2.41421, 0, 0,
        0, 0, -1.002, -1,
        0, 0, -0.2002, 0);
    deskCameraPosition.view = glm::mat4(
        0.00523436, -0.531391, 0.84711, 0,
        -5.3551e-09, 0.847122, 0.531399, 0,
        -0.999986, -0.00278154, 0.00443414, 0,
        0.309587, -0.423705, -0.0252765, 1);

#pragma endregion

#pragma region Initialization

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

    //Sound init
    SoundDevice* mysounddevice = SoundDevice::get();

    SoundSource speaker;

#pragma endregion

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_STENCIL_TEST);
    //glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0f, 0.0f, 0.0f, 1.00f);

#pragma region Resource and scene setup

    //Resource and scene setup
    Shader basicShader("res/shaders/basic/basic.vert", "res/shaders/basic/basic.frag");
    Shader defaultShader("res/shaders/enlightened/enlightened.vert", "res/shaders/enlightened/enlightened.frag");
    Shader lightShader("res/shaders/light/light.vert", "res/shaders/light/light.frag");
    Shader pickShader("res/shaders/clickpick/clickpick.vert", "res/shaders/clickpick/clickpick.frag");
    Shader texturePickShader("res/shaders/primitiveTexture/primitiveTexture.vert", "res/shaders/texturePickShader/texturePickShader.frag");
    Shader primitiveColorShader("res/shaders/primitiveColor/primitiveColor.vert", "res/shaders/primitiveColor/primitiveColor.frag");
    Shader primitiveAnimTextureShader("res/shaders/primitiveTexture/primitiveTexture.vert", "res/shaders/primitiveAnimTexture/primitiveAnimTexture.frag");
    Shader primitiveTextureShader("res/shaders/primitiveTexture/primitiveTexture.vert", "res/shaders/primitiveTexture/primitiveTexture.frag");
    Shader rainbowPrimitiveShader("res/shaders/primitiveColor/primitiveColor.vert", "res/shaders/primitiveRainbowColor/primitiveRainbowColor.frag");
    Shader textShader("res/shaders/text/text.vert", "res/shaders/text/text.frag");
    Shader outlineShader("res/shaders/outline/outline.vert", "res/shaders/outline/outline.frag");
    Shader blurShader("res/shaders/blur/blur.vert", "res/shaders/blur/blur.frag");
    Shader mixShader("res/shaders/mixer/mixer.vert", "res/shaders/mixer/mixer.frag");
    Shader fadeShader("res/shaders/fade.vert", "res/shaders/fade.frag");
    Shader brightShader("res/shaders/bright/bright.vert", "res/shaders/bright/bright.frag");
    Shader bloomMixShader("res/shaders/bloomMixer/bloomMixer.vert", "res/shaders/bloomMixer/bloomMixer.frag");

    //Sounds
    SoundBuffer::get()->addSoundEffect("res/sounds/test1.wav","test");
    SoundBuffer::get()->addSoundEffect("res/sounds/spell.ogg", "spell");
    SoundBuffer::get()->addSoundEffect("res/sounds/hotelBell.wav", "bell");
    SoundBuffer::get()->addSoundEffect("res/sounds/bookOpen.wav", "bookOpen");
    SoundBuffer::get()->addSoundEffect("res/sounds/bookClose.wav", "bookClose");
    SoundBuffer::get()->addSoundEffect("res/sounds/plantGrow.wav", "plantGrow");
    SoundBuffer::get()->addSoundEffect("res/sounds/plantGrowReverse.wav", "plantShrink");
    SoundBuffer::get()->addSoundEffect("res/sounds/metalCreak.wav", "scalesCreak");
    SoundBuffer::get()->addSoundEffect("res/sounds/select.wav", "selectItem");
    SoundBuffer::get()->addSoundEffect("res/sounds/metalCreak.wav", "weightTake");
    SoundBuffer::get()->addSoundEffect("res/sounds/codeType.wav", "typeCode");
    SoundBuffer::get()->addSoundEffect("res/sounds/correct.wav", "correctCode");
    SoundBuffer::get()->addSoundEffect("res/sounds/slash.wav", "cutRope");
    SoundBuffer::get()->addSoundEffect("res/sounds/glassBreak.wav", "breakGlass");
    SoundBuffer::get()->addSoundEffect("res/sounds/walking.wav", "tuptup");
    SoundBuffer::get()->addSoundEffect("res/sounds/doorOpen.wav", "door");
    SoundBuffer::get()->addSoundEffect("res/sounds/doorClosed.wav", "doorClosed");
    SoundBuffer::get()->addSoundEffect("res/sounds/vaultDoorOpen.wav", "vaultDoor");
    SoundBuffer::get()->addSoundEffect("res/sounds/placeChessPiece.wav", "placePiece");
    SoundBuffer::get()->addSoundEffect("res/sounds/spell.ogg", "drawerOpen");
    SoundBuffer::get()->addSoundEffect("res/sounds/pourWater.wav", "tap");
    SoundBuffer::get()->addSoundEffect("res/sounds/wateringPlant.wav", "wateringPlant");
    Music lightBuzz("res/sounds/lightBuzz.wav");
    //Music menuMusic("res/sounds/menu.wav");
    SoundBuffer::get()->addSoundEffect("res/sounds/menu.wav", "menuTheme");
    lightBuzz.EnableLooping();
    std::cout << "Volume: " << getGlobalVolume() << std::endl;
    setGlobalVolume(1.0);
    std::cout << "Change volume to: " << getGlobalVolume() << std::endl;

    // Better don't touch this !!!! - Why???? No idea (Maybe Mona Lise fond of text)
    text.init("res/fonts/arial/arial.ttf");
    //-----------------------------------------------------------------------------
    
    speaker.Play(SoundBuffer::get()->getSound("menuTheme"));

    sceneManager.text = &text;
    sceneManager.textShader = &textShader;

    sceneManager.defaultShader = &defaultShader;
    sceneManager.lightShader = &lightShader;
    sceneManager.textureShader = &primitiveTextureShader;
    sceneManager.fadeShader = &fadeShader;
    sceneManager.outlineShader = &outlineShader;
    sceneManager.blurShader = &blurShader;
    sceneManager.mixShader = &mixShader;
    sceneManager.brightShader = &brightShader;
    sceneManager.bloomMixShader = &bloomMixShader;
    sceneManager.singleClick = &singleClick;

    ApTime::instance().isEasyMode = false;

    sceneManager.Setup(window, &lightVersion, &SCR_WIDTH, &SCR_HEIGHT, &basicShader);

    sceneManager.Update(0, false, false);

    float time = 0;

    //Scene1Dark->SetActive(false);

#pragma endregion
    lightBuzz.Play();

#pragma region Game Loop

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        //music update
        lightBuzz.UpdateBufferStream();
        if(!ApTime::instance().isBuzzzing && !buzzerChangeState)
        {
            //std::cout << "State buzzer change!" << std::endl;
            buzzerChangeState = true;
            lightBuzz.Stop();
            //std::cout << "Ambient Stopped!" << std::endl;
        }
        else if (buzzerChangeState)
        {
            //std::cout << "Ambient Replayed!" << std::endl;
            lightBuzz.Replay();
            buzzerChangeState = false;
        }

        time += ApTime::instance().deltaTime;

        //Counting new deltaTime
        ApTime::instance().Update();



#pragma region Camera Setup

        glm::mat4 projection, view;
        if (ApTime::instance().isChessPosition)
        {
            projection = chessCameraPosition.projection;
            view = chessCameraPosition.view;
            camera.Position = chessCameraPosition.position;

        }
        else if (ApTime::instance().isDeskPosition)
        {
            projection = deskCameraPosition.projection;
            view = deskCameraPosition.view;
            camera.Position = deskCameraPosition.position;
        }
        else
        {
            projection = defaultCameraPosition.projection;
            view = defaultCameraPosition.view;
            camera.Position = defaultCameraPosition.position;
        }

#pragma endregion

#pragma region Camera Setup Debug

        //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 view = camera.GetViewMatrix();

        /*std::cout << "Projection:\n";
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                std::cout  << projection[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "---------------------------------------------------------------\n";
        std::cout << "View:\n";
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                std::cout << view[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "---------------------------------------------------------------\n";
        std::cout << "Position:\n";
        std::cout << camera.Position.x << " " << camera.Position.y << " " << camera.Position.z << "\n";*/

#pragma endregion

        glm::mat4 projectionPrimitive = glm::ortho(0.0f, float(SCR_WIDTH), 0.0f, float(SCR_HEIGHT));
        glm::mat4 viewPrimitive = glm::mat4(1.0);

#pragma region Setting Shaders

        outlineShader.use();
        outlineShader.setMat4("projection", projection);
        outlineShader.setMat4("view", view);
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

        basicShader.use();
        basicShader.setMat4("projection", projection);
        basicShader.setMat4("view", view);

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

        fadeShader.use(); // lol
        fadeShader.setMat4("projection", projectionPrimitive);
        fadeShader.setMat4("view", viewPrimitive);

        rainbowPrimitiveShader.use();
        rainbowPrimitiveShader.setMat4("projection", projectionPrimitive);
        rainbowPrimitiveShader.setMat4("view", viewPrimitive);

        textShader.use();
        textShader.setMat4("projection", projectionPrimitive);

        //Setting shaders for picking models and textures
        pickShader.use();
        pickShader.setMat4("projection", projection);
        pickShader.setMat4("view", view);
        texturePickShader.use();
        texturePickShader.setMat4("projection", projectionPrimitive);
        texturePickShader.setMat4("view", viewPrimitive);

#pragma endregion

#pragma region Mouse Picking

        //RENDER FOR MOUSE PICKING
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
        {
            isHoldingMouseButton = true;
        }
        else
        {
            isHoldingMouseButton = false;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        //Performing mouse picking render
        sceneManager.RenderMousePicking(pickShader, texturePickShader);
        //Checking mouse position
        double mouseXd;
        double mouseYd;
        glfwGetCursorPos(window, &mouseXd, &mouseYd);
        //Reading ObjectId fo pixel below the mouse
        ClickPicker::PixelData pixel = picker.Read(mouseXd, SCR_HEIGHT - mouseYd);
        //Saving id of the currently picked object
        currentlyPicked = pixel.ObjectID + 255 * pixel.DrawID;

#pragma endregion
        

        //Processing input here
        processInput(window, &lightBuzz);

#pragma region Render

        //Whenever mouse i button isn't pressed we make sure that the next frame it's pressed
        //it will be a single click. Later on, after that dirst frame singleClick is set to false,
        //and it will be mouseDragging, not clicking. Player will have to release the button and press
        //again for that to be click.
        if (!(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)))
        {
            singleClick = true;
        }

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        sceneManager.Update(currentlyPicked, singleClick, isHoldingMouseButton);

        //After the first frame singleClick is set to false(mouse clicking function won't be used,
        //only the dragging function. It will be set to true again after te button is released.
        if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)))
        {
            singleClick = false;
        }

        glEnable(GL_BLEND);
        //glStencilFunc(GL_ALWAYS, 1, 0xFF);
        //glStencilMask(0xFF);
        //sceneManager.EnableFramebuffer();
        //sceneManager.RenderWithShader(outlineShader, 2);
        //sceneManager.DisableFramebuffer();
        //glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        //glStencilMask(0x00);
        //glDisable(GL_DEPTH_TEST);
        //sceneManager.Render(currentlyPicked);

        //glStencilMask(0xFF);
        //glStencilFunc(GL_ALWAYS, 0, 0xFF);
        //glEnable(GL_DEPTH_TEST);
        //sceneManager.BlurRender(currentlyPicked);
        sceneManager.Render(currentlyPicked);
          
        glDepthFunc(GL_LESS);
        glDisable(GL_BLEND);

#pragma endregion

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

#pragma endregion

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window, Music* sneakyTheme)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        if (canChangeMusic)
        {
            ALint state = sneakyTheme->GetSourceState();
            if (state == AL_PLAYING)
            {
               
                switch (volumeMode)
                {
                case 0:
                    setGlobalVolume(1.0);
                    std::cout << "Volume: 1.0" << std::endl;
                    volumeMode++;
                    break;
                case 1:
                    setGlobalVolume(0.7);
                    std::cout << "Volume: 0.7" << std::endl;
                    volumeMode++;
                    break;
                case 2:
                    setGlobalVolume(0.5);
                    std::cout << "Volume: 0.5" << std::endl;
                    volumeMode++;
                    break;
                case 3:
                    setGlobalVolume(0.2);
                    std::cout << "Volume: 0.2" << std::endl;
                    volumeMode++;
                    break;
                case 4:
                    setGlobalVolume(0.02);
                    std::cout << "Volume: 0.02" << std::endl;
                    volumeMode++;
                    break;
                case 5:
                    setGlobalVolume(0.0);
                    std::cout << "Volume: Muted" << std::endl;
                    volumeMode = 0;
                    break;
                }
            
            }
            canChangeMusic = false;
            //std::cout << "Can changed: false" << std::endl;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE)
    {
        canChangeMusic = true;
        //std::cout << "Can changed: true" << std::endl;
    }

    //if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    //    camera.ProcessKeyboard(FORWARD, ApTime::instance().deltaTime);
    //if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    //    camera.ProcessKeyboard(BACKWARD, ApTime::instance().deltaTime);
    //if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    //    camera.ProcessKeyboard(LEFT, ApTime::instance().deltaTime);
    //if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    //    camera.ProcessKeyboard(RIGHT, ApTime::instance().deltaTime);

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
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    std::cout << SCR_WIDTH << " " << SCR_HEIGHT << std::endl;
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

    //camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    //camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
