#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

unsigned int positionBufferObject;
GLuint theProgram;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

const float VertexPositions[] = {
    0.75f, 0.75f, 0.0f, 1.0f,
    0.75f, -0.75f, 0.0f, 1.0f,
    -0.75f, -0.75f, 0.0f, 1.0f
};

void initializeVertexBuffer() {

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositions), VertexPositions, GL_STATIC_DRAW);
}

const char *chVertexShader = "#version 330\n"

    "layout(location = 0) in vec4 position;\n"
    "void main() {\n"
        "gl_position = position;\n"
    "}\0";

const char *chFragmentShader = "#version 330\n"
    "out vec4 outputColor;\n"
    "void main() {\n"
        "outputColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

GLuint compileShader(GLenum shaderType, const char *shaderFile){

    unsigned int comShader;

    comShader = glCreateShader(shaderType);
    glShaderSource(comShader, 1, &shaderFile, NULL);
    glCompileShader(comShader);

    int success;
    char infoLog[512];
    glGetShaderiv(comShader, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(comShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return comShader;
}

GLuint CreateProgram(const std::vector<GLuint> &shaderList) {

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    for(size_t i = 0; i < shaderList.size(); i++)
        glAttachShader(shaderProgram, shaderList[i]);

    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }
    return shaderProgram;
}


int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if( window == NULL) {
        
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    unsigned int vshader1;
    unsigned int fshader1;

    vshader1 = compileShader(GL_VERTEX_SHADER, chVertexShader);
    fshader1= compileShader(GL_FRAGMENT_SHADER, chFragmentShader);

    std::vector<GLuint> shaderList;
    shaderList.push_back(vshader1);
    shaderList.push_back(fshader1);

    theProgram = CreateProgram(shaderList);

    for(unsigned int shader : shaderList) {
        glDeleteShader(shader);
    }

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    initializeVertexBuffer();
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0); 

    while(!glfwWindowShouldClose(window)) {
        
        //input
        processInput(window);

        //rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(theProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
