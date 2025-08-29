#include "../external/GLAD/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>

#include "../external/shader_s.h"
#include "Grid.hpp"

// initializations
void processInput(GLFWwindow* window);

//settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void checkFileExists(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cout << "File not found: " << path << std::endl;
    } else {
        std::cout << "File exists: " << path << std::endl;
    }
}

int main()
{

    std::cout << "0";

    checkFileExists("./shaders/shader.vs");
    checkFileExists("./shaders/shader.fs");

    

    float vertices[] = {
        // Front face
        -0.5f, -0.5f,  0.5f, // Bottom-left
         0.5f, -0.5f,  0.5f, // Bottom-right
         0.5f,  0.5f,  0.5f, // Top-right
        -0.5f,  0.5f,  0.5f, // Top-left

        // Back face
        -0.5f, -0.5f, -0.5f, // Bottom-left
         0.5f, -0.5f, -0.5f, // Bottom-right
         0.5f,  0.5f, -0.5f, // Top-right
        -0.5f,  0.5f, -0.5f  // Top-left
    };

    unsigned int indices[] = {
        // Front face
        0, 1, 2,
        2, 3, 0,
        // Back face
        4, 5, 6,
        6, 7, 4,
        // Left face
        4, 7, 3,
        3, 0, 4,
        // Right face
        1, 5, 6,
        6, 2, 1,
        // Bottom face
        0, 1, 5,
        5, 4, 0,
        // Top face
        3, 2, 6,
        6, 7, 3
    };


    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "PacBoy", NULL, NULL);

    if(window == NULL)
    {
        std::cout << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Grid gameGrid;
    bool success = gameGrid.loadFromFile("./assets/levels/level1.txt");
    if(!success)
    {
        std::cerr << "Failed to load game grid from file." << std::endl;
    }

    glEnable(GL_DEPTH_TEST);
    Shader ourShader("./shaders/shader.vs", "./shaders/shader.fs");


    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);

    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(6 * sizeof(float)));
    //glEnableVertexAttribArray(2);

    while(!glfwWindowShouldClose(window))
    {   
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ourShader.use();
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE))
    {
        glfwSetWindowShouldClose(window, true);
    }
}