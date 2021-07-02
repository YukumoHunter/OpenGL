#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

// Some GLSL code (need to find a better way to compile this)
const char *vertexShaderSource = "#version 460 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 460 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.5f, 0.2f, 0.2f, 1.0f);;\n"
    "}\0";

unsigned int vertexShader;
unsigned int fragmentShader;

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // create a window
    GLFWwindow* window = glfwCreateWindow(1280, 720, "🅱️ello world", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
 
 
    // initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD");
        return -1;
    }    
 
    // set viewport
    glViewport(0, 0, 1280, 720);

    // adjust viewport on resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // CREATE VERTEX SHADER:
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // bind vertex shader to source code and compile shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
    }


    // CREATE FRAGMENT SHADER:
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // bind fragment shader to source code and compile shader
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
    }

    // create shader program and link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for shader linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s", infoLog);
    }

    // delete shaders after use
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader); 


    // vertices of the triangle
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    unsigned int VBO, VAO;
    // Vertex Buffer Object:
    // store vertices in GPU memory for fast access
    // create buffer with buffer ID of 1
    glGenBuffers(1, &VBO); 

    // Vertex Array Object:
    // used to store vertex attribute calls
    // create VAO with ID of 1
    glGenVertexArrays(1, &VAO);

    // bind the VAO 
    glBindVertexArray(VAO);

    // bind the VBO to GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  
    // copy buffer into GPU memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // specify how to interpret vertex data
    // attribute params: location, size, data type, normalized (t/f), stride, offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // enable vertex attribute at position 0
    glEnableVertexAttribArray(0);

    // call to glVertexAttribPointer registered VBO as the vertex attribute's bound
    // therefore we can safely unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // unbind the VAO to not accidentally modify it
    // usually not needed since you need to call glBindVertexArray for this 
    glBindVertexArray(0);

    while(!glfwWindowShouldClose(window))
    {
        // process inputs
        processInput(window);

        // perform rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // use shaders
        glUseProgram(shaderProgram);
        // bind VAO
        glBindVertexArray(VAO);
        // draw!!!!!!!!!!
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // swap buffers and poll events
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // resize viewport on window resize
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    // close window on ESC press
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}
