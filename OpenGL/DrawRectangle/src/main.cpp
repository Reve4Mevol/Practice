#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

//#define DRAW_RECTANGLE
#define USE_FADE
//#define USE_UNIFORM

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

const int width = 800;
const int height = 600;

const char *vertexShaderSrc = "#version 330 core\n"
                              "layout (location = 0) in vec3 aPos;\n"
                              "layout (location = 1) in vec3 aColor;\n"
                              "out vec4 ourColor;\n"
                              "void main()\n"
                              "{\n"
                              "    gl_Position = vec4(aPos,1.0);\n"
                              "    ourColor = vec4(aColor,1.0);\n"
                              "}\0";
const char *fragmentShaderSrc = "#version 330 core\n"
                                "out vec4 FragColor;\n"
                                "in vec4 ourColor;\n"
                                "void main()\n"
                                "{\n"
                                "    FragColor = ourColor;\n"
                                "}\0";
const char *fragment2ShaderSrc = "#version 330 core\n"
                                "out vec4 FragColor;\n"
                                "uniform vec4 changeGreenColor;\n"
                                "void main()\n"
                                "{\n"
                                "    FragColor = changeGreenColor;\n"
                                "}\0";

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(width,height,"DrawRectangle",NULL,NULL);
    if(!window)
    {
        printf("window init failed!\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("glad init failed!\n");
        glfwTerminate();
        return -1;
    }
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
    unsigned int fragmentShaderFd[2];
    fragmentShaderFd[0] = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderFd[0],1,&fragmentShaderSrc,NULL);
    glCompileShader(fragmentShaderFd[0]);
    fragmentShaderFd[1] = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderFd[1],1,&fragment2ShaderSrc,NULL);
    glCompileShader(fragmentShaderFd[1]);
    glGetShaderiv(fragmentShaderFd[0],GL_COMPILE_STATUS,&success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderFd[0],512,NULL,infoLog);
        std::cout<<"ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"<<infoLog<<std::endl;
    }
    unsigned int shaderProgram[2];
    shaderProgram[0] = glCreateProgram();
    glAttachShader(shaderProgram[0],vertexShaderFd);
    glAttachShader(shaderProgram[0],fragmentShaderFd[0]);
    glLinkProgram(shaderProgram[0]);
    shaderProgram[1] = glCreateProgram();
    glAttachShader(shaderProgram[1],vertexShaderFd);
    glAttachShader(shaderProgram[1],fragmentShaderFd[1]);
    glLinkProgram(shaderProgram[1]);
    // 链接着色器成为着色器程序之后就可以删除着色器了
    glDeleteShader(vertexShaderFd);
    glDeleteShader(fragmentShaderFd[0]);
    glDeleteShader(fragmentShaderFd[1]);
#ifdef DRAW_RECTANGLE
    float vertices[] =
            {
            // 左上
            -0.5f,0.5f,0.0f,
            // 右上
            0.5f,0.5f,0.0f,
            // 右下
            0.5f,-0.5f,0.0f,
            // 左下
            -0.5f,-0.5f,0.0f
            };
    unsigned int indices[] =
            {
            0,1,2,
            0,2,3
            };
    unsigned int VAO,VBO,EBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float ),NULL);
    glEnableVertexAttribArray(0);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE); // 只画线
    while(!glfwWindowShouldClose(window))
    {
        static int i = 0;
        i++;
        if(i>10000)
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); // 改为填充
        processInput(window);
        glUseProgram(shaderProgram[0]);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
    glfwTerminate();
#elif defined(USE_UNIFORM)
    // 三角形顶点数据，存到VBO
    float vertices[] =
            {
                    // 左1
                    -0.5f,0.0f,0.0f,
                    // 右1 左2
                    0.0f,0.0f,0.0f,
                    // 中1
                    -0.25f,0.5f,0.0f,
                    // 右2
                    0.5f,0.0f,0.0f,
                    // 中2
                    0.25f,-0.5f,0.0f
            };
    // 三角形顶点的索引，存到EBO
    unsigned int indices[] =
            {
                    0,1,2,
                    1,3,4
            };
    unsigned int VAO,VBO,EBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float ),NULL);
    glEnableVertexAttribArray(0);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    // 定义时间与随时间变化的色彩值
    float timeValue,color;
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        timeValue= glfwGetTime();
        // 色彩随时间变化
        color = (sin(timeValue/2.0f)+0.5f);
        // 定位两个着色器使用的uniform变量位置
        int redColorPos = glGetUniformLocation(shaderProgram[0],"changeRedColor");
        int greenColorPos = glGetUniformLocation(shaderProgram[1],"changeGreenColor");
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram[0]);
        glUseProgram(shaderProgram[1]);
        glBindVertexArray(VAO);
        glUniform4f(redColorPos,color,0.0f,0.0f,1.0f);
        glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);
        glUniform4f(greenColorPos,0.0f,color,0.0f,1.0f);
        glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,(void*)12);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
    glfwTerminate();
#elif defined(USE_FADE)
    // 三角形顶点数据，存到VBO
    float vertices[] =
            {
                    // 左1
                    -0.5f,0.0f,0.0f,
                    1.0f,0.0f,0.0f,
                    // 右1
                    0.0f,0.0f,0.0f,
                    0.0f,1.0f,0.0f,
                    // 中1
                    -0.25f,0.5f,0.0f,
                    0.0f,0.0f,1.0f
            };
    unsigned int VAO,VBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float ),NULL);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float ),(void *)12);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    // 定义时间与随时间变化的色彩值
    float timeValue,color;
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram[0]);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glfwTerminate();

#endif
    return 0;
}
