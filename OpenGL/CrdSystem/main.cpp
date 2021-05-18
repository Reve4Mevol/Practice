#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

const float viewWidth = 800.0;
const float viewHeight = 600.0;

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    glfwInit();
    glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwInitHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwInitHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(viewWidth,viewHeight,"CrdSystem",NULL,NULL);
    if(!window)
    {
        printf("init failed!");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("init glad failed");
        glfwTerminate();
        return -1;
    }

    float vertices[] = {
            0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,
            -0.5f,  0.5f, 0.0f,   0.0f, 1.0f
    };

    const char *vertexShaderSrc = "#version 330 core\n"
                                  "layout (location = 0) in vec3 aPos;\n"
                                  "layout (location = 1) in vec2 st;\n"
                                  "out vec2 outst;\n"
                                  "uniform mat4 model;\n"
                                  "uniform mat4 view;\n"
                                  "uniform mat4 toushi;\n"
                                  "void main()\n"
                                  "{\n"
//                                  "    gl_Position = vec4(aPos,1.0);\n"
                                  "    gl_Position = toushi * view * model * vec4(aPos,1.0);\n"
                                  "    outst = vec2(st.x,1-st.y);\n"
                                  "}\0";
    unsigned int vertexShaderFd;
    vertexShaderFd = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderFd,1,&vertexShaderSrc,NULL);
    glCompileShader(vertexShaderFd);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShaderFd,GL_COMPILE_STATUS,&success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderFd,512,NULL,infoLog);
        std::cout<<"ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"<<infoLog<<std::endl;
    }
    const char *fragmentShaderSrc = "#version 330 core\n"
                                    "in vec2 outst;\n"
                                    "out vec4 FragColor;\n"
                                    "uniform sampler2D firstTexture;\n"
                                    "uniform sampler2D secondTexture;\n"
                                    "void main()\n"
                                    "{\n"
                                    "    FragColor = mix(texture(firstTexture, outst),texture(secondTexture, outst),0.7);\n"
                                    "}\0";
    unsigned int fragmentShaderFd;
    fragmentShaderFd = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderFd,1,&fragmentShaderSrc,NULL);
    glCompileShader(fragmentShaderFd);
    glGetShaderiv(fragmentShaderFd,GL_COMPILE_STATUS,&success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderFd,512,NULL,infoLog);
        std::cout<<"ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"<<infoLog<<std::endl;
    }
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram,vertexShaderFd);
    glAttachShader(shaderProgram,fragmentShaderFd);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShaderFd);
    glDeleteShader(fragmentShaderFd);

    unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,  // first Triangle
            1, 2, 3   // second Triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void *)(0 * sizeof(float)));
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    int width = 0,height = 0,nrChannels = 0;
    unsigned char *data = stbi_load("./shidizai.jpg",&width,&height,&nrChannels,0);
    unsigned int texture;
    glGenTextures(1,&texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_R,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D,texture);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("invalid data\n");
    }
    stbi_image_free(data);

    unsigned char *data2 = stbi_load("./container.jpg",&width,&height,&nrChannels,0);
    unsigned int texture2;
    glGenTextures(1,&texture2);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_R,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D,texture2);
    if(data2)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("invalid data\n");
    }
    stbi_image_free(data2);

    glUseProgram(shaderProgram);
    int pos = glGetUniformLocation(shaderProgram,"firstTexture");
    glUniform1i(pos,0);
    int pos2 = glGetUniformLocation(shaderProgram,"secondTexture");
    glUniform1i(pos,1);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), viewWidth/viewHeight, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "toushi"), 1, GL_FALSE, glm::value_ptr(projection));

    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,texture2);
        // draw our first triangle
        glUseProgram(shaderProgram);
        int pos = glGetUniformLocation(shaderProgram,"firstTexture");
        glUniform1i(pos,0);
        int pos2 = glGetUniformLocation(shaderProgram,"secondTexture");
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "toushi"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform1i(pos,1);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}

