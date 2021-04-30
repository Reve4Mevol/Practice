#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//#version 330 core
//layout (location = 0) in vec3 aPos;
//void main()
//{
//    gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0);
//}

//#version 330 core
//out vec4 FragColor;
//void main()
//{
//    FragColor = vec4(1.0f,0.5f,0.2f,1.0f);
//}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800,600,"DrawTriaangle",NULL,NULL);
    if(!window)
    {
        std::cout<<"init window err!"<<std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout<<"init glad err!"<<std::endl;
        glfwTerminate();
        return -1;
    }
    // 顶点输入
    float vertices[] = {
            -0.5f,-0.5f,0.0f,
            0.5f,-0.5f,0.0f,
            0.0f,0.5f,0.0f,
            };
    // 顶点着色器硬编码 vec3数据类型，in关键字，查看编译错误信息
    const char *vertexShaderSrc = "#version 330 core\n"
                                  "layout (location = 0) in vec3 aPos;\n"
                                  "void main()\n"
                                  "{\n"
                                  "    gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0);\n"
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
                                  "out vec4 FragColor;\n"
                                  "void main()\n"
                                  "{\n"
                                  "    FragColor = vec4(1.0f,0.5f,0.2f,1.0f);\n"
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

    // 链接着色器成为着色器程序之后就可以删除着色器了
    glDeleteShader(vertexShaderFd);
    glDeleteShader(fragmentShaderFd);

    // 将顶点从顶点缓冲对象传入到顶点着色器中
    // 只传入一个顶点？
    // GL_ARRAY_BUFFER已经绑定VBO，所有不需要传入VBO对象？
    unsigned  int VAO;
    unsigned int VBO;
//    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
//    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void *)0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void *)12);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void *)24);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void *)0);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void *)12);
        glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void *)24);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
//        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteProgram(shaderProgram);
    return 0;
}
