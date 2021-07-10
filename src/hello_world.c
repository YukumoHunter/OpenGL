#include <stdio.h>
#include <math.h>

// image loading
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"

#include <cglm/cglm.h>

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

    Shader shader = new_shader("src/shaders/vertex_shader.glsl", "src/shaders/fragment_shader.glsl");

    // create a texture
    unsigned int texture, texture2;
    glGenTextures(1, &texture);

    // bind the texture
    glBindTexture(GL_TEXTURE_2D, texture);  

    // set texture wrapping/filtering options on bound texture object
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // texture loading
    int width, height, nrChannels;
    unsigned char *data = stbi_load("src/assets/container.jpg", &width, &height, &nrChannels, 0);

    if (data)
    {
        // generate the bound texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        // generate mipmapped texture
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load texture!\n");
    }

    // free loaded image data
    stbi_image_free(data);

    // create a second texture
    glGenTextures(1, &texture2);

    // bind the texture
    glBindTexture(GL_TEXTURE_2D, texture2);  

    // set texture wrapping/filtering options on bound texture object
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // texture loading
    // flip image vertically
    stbi_set_flip_vertically_on_load(true);  
    data = stbi_load("src/assets/awesomeface.png", &width, &height, &nrChannels, 0);

    if (data)
    {
        // generate the bound texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        // generate mipmapped texture
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load texture!\n");
    }

    // free loaded image data
    stbi_image_free(data);

    // vertex data
    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    // vertex draw order
    unsigned int indices[] = {
        0, 3, 2,
        2, 1, 0
        
    };

    unsigned int VBO, VAO, EBO;
    // Vertex Buffer Object:
    // store vertices in GPU memory for fast access
    // create buffer with buffer ID of 1
    glGenBuffers(1, &VBO); 

    // bind the VBO to GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  
    // copy buffer into GPU memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Element Buffer Object:
    // store indices (draw order)
    // will reduce overhead by not including duplicate vertices
    glGenBuffers(1, &EBO);

    // bind the EBO to GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // copy buffer into GPU memory
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    // Vertex Array Object:
    // used to store vertex attribute calls
    // create VAO with ID of 1
    glGenVertexArrays(1, &VAO);

    // bind the VAO 
    glBindVertexArray(VAO);

    // specify how to interpret position in vertex shader
    // attribute params: location, size, data type, normalized (t/f), stride, offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // enable vertex attribute at position 0
    glEnableVertexAttribArray(0);

    // specify how to interpret color in vertex shader
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // specify how to interpret texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // call to glVertexAttribPointer registered VBO as the vertex attribute's bound
    // therefore we can safely unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // unbind the VAO to not accidentally modify it
    // usually not needed since you need to call glBindVertexArray for this 
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        
        // process inputs
        processInput(window);

        // perform rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // apply transformation
        mat4 trans;
        glm_mat4_identity(trans);

        glm_translate(trans, (vec3) { 0.5f, -0.5f, 0.0f });
        glm_rotate(trans, (float) glfwGetTime(), (vec3) { 0.0f, 0.0f, 1.0f });

        unsigned int transform_loc = glGetUniformLocation(shader.ID, "transform");
        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, *trans);

        // use shaders
        use_shader(&shader);

        glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0); // set it manually
        set_int(&shader, "texture2", 1); // or with shader class

        // bind textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // bind VAO
        glBindVertexArray(VAO);

        // draw wireframe
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // glDrawElements to do indexed drawing with currently bound buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


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
