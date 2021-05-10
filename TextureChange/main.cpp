#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

const char *vertexSrc = "#version 330 core\n"
                            "layout (location = 0) in vec3 aPos;\n"
                            "layout (location = 1) in vec2 aTexPosReverse;\n"
                            "out vec2 aTexPos;\n"
                            "uniform mat4 transform;\n"
                            "void main()\n"
                            "{\n"
                            "// 让输入坐标与全局矩阵运算实现坐标变化\n"
                            "gl_Position = transform * vec4(aPos,1.0f);\n"
                                "aTexPos = vec2(aTexPosReverse.x,1-aTexPosReverse.y);\n"
                            "}\0";

const char *fragSrc = "#version 330 core\n"
                        "in vec2 aTexPos;\n"
                        "out vec4 FragColor;\n"
                        "uniform sampler2D aTexture1;\n"
                        "uniform sampler2D aTexture2;\n"
                        "void main()\n"
                        "{\n"
                        "// 真正使用的texture是纹理与坐标的组合\n"
                        "FragColor = mix(texture(aTexture1,aTexPos),texture(aTexture2,aTexPos),0.2);\n"
                        "}\0";

int main() {
    glfwInit();
    glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwInitHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwInitHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(800,600,"texturechange", nullptr, nullptr);
    if (!window)
    {
        std::cout<<"window init failed"<<std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout<<"glad init failed"<<std::endl;
        glfwTerminate();
        return -1;
    }
    float vertices[] = {
            // positions         // texture coords
            0.5f,  0.5f, 0.0f,  1.0f, 1.0f, // top right
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // bottom left
            -0.5f,  0.5f, 0.0f,  0.0f, 1.0f  // top left
    };
    unsigned int VAO,VBO,vertexShader,fragmentShader,texture1,texture2,shaderProgram;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof (GL_FLOAT),0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof (GL_FLOAT),(void *)(3*sizeof (GL_FLOAT)));
    glEnableVertexAttribArray(0);

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexSrc,NULL);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragSrc,NULL);
    shaderProgram = glCreateProgram();
    glAttachShader(GL_VERTEX_SHADER,vertexShader);
    glAttachShader(GL_FRAGMENT_SHADER,fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    glm::vec4 vec(1.0f,0.0f,0.0f,1.0f);
    glm::mat4 trans;
    trans = glm::rotate(trans,glm::radians(90.0f),glm::vec3(0.0,0.0,1.0));
    trans = glm::scale(trans,glm::vec3(0.5f,0.5f,0.5f));


    while(!glfwWindowShouldClose(window))
    {
        processInput(window);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

    return 0;
}

