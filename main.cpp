#include "glad.h"
#include "glfw3.h"
#include <iostream>
#include <cmath>

// Prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Global state for animation logic
bool redPermanent = false;

// Shaders
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos; \n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos, 1.0); \n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor; \n"
"uniform vec4 ourColor; \n"
"void main()\n"
"{\n"
" FragColor = ourColor; \n"
"}\n\0";

int main()
{
    // 1. Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Window Title set to your ID
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "0432320005101063", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 2. Load GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    // 3. Build Shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 4. Set up Triangle (Single triangle as per instructions)
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // bottom left
         0.5f, -0.5f, 0.0f, // bottom right
         0.0f,  0.5f, 0.0f  // top center
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 5. Render Loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // Dark background
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        int colorLoc = glGetUniformLocation(shaderProgram, "ourColor");

        // Color Logic
        float r, g, b;

        if (redPermanent) 
        {
            // R pressed: Red permanently
            r = 1.0f; g = 0.0f; b = 0.0f;
        }
        else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
        {
            // W held: White
            r = 1.0f; g = 1.0f; b = 1.0f;
        }
        else 
        {
            // Default: Cyan (0,1,1) to Magenta (1,0,1) transition
            float timeValue = glfwGetTime();
            // Sin oscillates -1 to 1, we map it to 0 to 1
            float factor = (sin(timeValue) + 1.0f) / 2.0f; 
            
            r = factor;        // 0 to 1
            g = 1.0f - factor; // 1 to 0
            b = 1.0f;          // Always 1
        }

        glUniform4f(colorLoc, r, g, b, 1.0f);
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    // Close on first letter of name (Z)
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Permanent Red trigger
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        redPermanent = true;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}