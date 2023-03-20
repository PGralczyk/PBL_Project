#include <iostream>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image/stb_image.h>
#include <assimp/Importer.hpp>
#include <fstream>
#include <sstream>
#include <string>


#include "VBO.h"
#include "EBO.h"

struct ShaderSources 
{
    std::string vertexShader;
    std::string fragmentShader;
};

//For now I keep those functions in here
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
static unsigned int createShaderProgram(std::string& vertexShader, std::string& fragmentShader);
static unsigned int compileShader(unsigned int type, std::string& source);
static std::string loadSource(const std::string& path);

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   

    Assimp::Importer importer;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    //Below I set the function to be used in case our window changed size
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //I try to use Glad as adresses loader and i realy hope it works
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    float rectangle[] = {
        -0.5 ,  0.5 ,  // 1 vert. left-up corner (position x, position y)
         0.5 ,  0.5 ,  // 2 vert. right-up corner (position x, position y)
        -0.5 , -0.5 ,  // 3 vert. left-down corner (position x, position y)
         0.5 , -0.5    // 4 vert. right-down corner (position x, position y)
    };

    unsigned int indices[] = { 3, 1, 2, 3, 4, 2 };

    unsigned int va;

    glGenVertexArrays(1, &va);
    glBindVertexArray(va);

    VBO vb(rectangle, 4 * 2 * sizeof(float));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    EBO eb(indices, (unsigned int)6);
    std::string vertexSourcePath = "res/shaders/basic.vert";
    std::string fragmentSourcePath = "res/shaders/basic.frag";

    ShaderSources sources = {loadSource(vertexSourcePath), loadSource(fragmentSourcePath)};

    unsigned int programShader = createShaderProgram(sources.vertexShader, sources.fragmentShader);
    

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        //Processing input here
        processInput(window);

        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(programShader);
        vb.Bind();

        

        eb.Bind();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, &indices);

        vb.Unbind();
        eb.Unbind();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(programShader);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

static unsigned int createShaderProgram(std::string& vertexShader, std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

static unsigned int compileShader(unsigned int type, std::string& source)
{
    unsigned int id = glCreateShader(type);

    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) 
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)_malloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "COMPILE ERROR::" << (type == GL_VERTEX_SHADER ? "VERTEX SHADER" : "FRAGMENT SHADER") << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static std::string loadSource(const std::string& path)
{
    std::ifstream stream(path);
    std::stringstream ss;

    std::string line;
    while (getline(stream, line)) 
    {
        ss << line << "\n";
    }

    return ss.str();
}