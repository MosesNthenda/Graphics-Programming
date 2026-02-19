#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

GLuint positionBufferObject;
GLuint theProgram;

// function that resizes the window when dragged
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
}

// input command to close the window when a character on the keyboard is pressed
void processInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// data of positions of vertices and their coordinates to be given to shaders
// for drawing a triangle
const float VertexPositions[] = {
    0.75f, 0.75f, 0.0f, 1.0f,
    0.75f, -0.75f, 0.0f, 1.0f,
    -0.75f, -0.75f, 0.0f, 1.0f
};

// initializing buffers that will store data in memory for fast access
// by the gpu when processing
void initializeVertexBuffer() {

    glGenBuffers(1, &positionBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositions), VertexPositions, GL_STATIC_DRAW);
}


const char *chVertexShader = "#version 330\n"

    "layout(location = 0) in vec4 position;\n"
    "void main() {\n"
        "gl_Position = position;\n"
    "}\0";

const char *chFragmentShader = "#version 330\n"

    "out vec4 outputColor;\n"
    "float lerpValue = gl_FragCoord.y / 500.0f;\n"

    "void main() {\n"
        "outputColor = mix(vec4(1.0f, 1.0f, 1.0f, 1.0f),\n"
        "vec4(0.2f, 0.2f, 0.2f, 0.2f), lerpValue);\n"
    "}\0";

// compiling shaders based on shader type
GLuint compileShader(GLenum shaderType, const char *shaderFile){

    unsigned int comShader;

    //create object based on shadertype passed in to the function
    comShader = glCreateShader(shaderType);

    //retrieving the C-style string from shaderfile
    //to be fed into the shader object and compiling them
    glShaderSource(comShader, 1, &shaderFile, NULL);
    glCompileShader(comShader);

    int success;
    char infoLog[512];

    //retreiving compile status 
    glGetShaderiv(comShader, GL_COMPILE_STATUS, &success);

    //printing out the error if GL_FALSE is returned
    //for failed compilation
    if(!success) {
        glGetShaderInfoLog(comShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return comShader;
}

// creating a shader program from all shader objects
GLuint CreateProgram(const std::vector<GLuint> &shaderList) {

    //creating an empty program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    //attaching all shaders to the program
    for(size_t i = 0; i < shaderList.size(); i++)
        glAttachShader(shaderProgram, shaderList[i]);

    //linking together all the attached shaders
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];

    //printing out the error if GL_FALSE is returned
    //for failed linking
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::LINKING::FAILED\n" << infoLog << std::endl;
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
