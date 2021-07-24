#include <stdio.h>
#include <math.h>

// image loading
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "camera.h"

#include <cglm/cglm.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void process_input(GLFWwindow *window);

unsigned int vertexShader;
unsigned int fragmentShader;

// window size
const unsigned int SCR_WIDTH = 720;
const unsigned int SCR_HEIGHT = 720;

// init camera
Camera camera;
float last_x = SCR_WIDTH / 2.0f;
float last_y = SCR_HEIGHT / 2.0f;
bool first_mouse = true;

// timing
float delta = 0.0f;
float last_frame = 0.0f;

int main()
{
    vec3 pos = { 0.0f, 0.0f, 10.0f };
    vec3 world_up = { 0.0f, 1.0f, 0.0f };

    camera = new_camera(pos, world_up, CAM_DEFAULT_YAW, CAM_DEFAULT_PITCH);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create a window 🅱
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "bello world", NULL, NULL);
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
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // capture cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // callbacks on window interaction
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);


    // old vertex data
    // float vertices[] = {
    //     // positions          // colors           // texture coords
    //     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
    //     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    //     -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    //     -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    // };

    // vertex draw order (indexed drawing)
    // unsigned int indices[] = {
    //     0, 3, 2,
    //     2, 1, 0
    // };

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    vec3 cubePositions[] = {
        { 0.0f,  0.0f,  0.0f }, 
        {  2.0f,  5.0f, -5.0f }, 
        { -1.5f, -2.2f, -2.5f },  
        { -3.8f, -2.0f, -8.3f },  
        {  2.4f, -0.4f, -3.5f },  
        { -1.7f,  3.0f, -7.5f },  
        {  1.3f, -2.0f, -2.5f },  
        {  1.5f,  2.0f, -2.5f }, 
        {  1.5f,  0.2f, -1.5f }, 
        { -1.3f,  1.0f, -1.5f }  
    };

    Shader shader;
    init_shader(&shader, "src/shaders/vertex_shader.glsl", "src/shaders/fragment_shader.glsl");

    // create a texture
    unsigned int texture, texture2;
    glGenTextures(1, &texture);

    // bind the texture
    glBindTexture(GL_TEXTURE_2D, texture);  

    // set texture wrapping/filtering options on bound texture object
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // texture loading
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);  
    unsigned char *data = stbi_load("src/assets/grass.jpg", &width, &height, &nrChannels, 0);

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
    data = stbi_load("src/assets/pete.png", &width, &height, &nrChannels, 0);

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
    // glGenBuffers(1, &EBO);

    // bind the EBO to GL_ELEMENT_ARRAY_BUFFER
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // copy buffer into GPU memory
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    // Vertex Array Object:
    // used to store vertex attribute calls
    // create VAO with ID of 1
    glGenVertexArrays(1, &VAO);

    // bind the VAO 
    glBindVertexArray(VAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // // specify how to interpret position in vertex shader
    // // attribute params: location, size, data type, normalized (t/f), stride, offset
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // // enable vertex attribute at position 0
    // glEnableVertexAttribArray(0);

    // // specify how to interpret color in vertex shader
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // // glEnableVertexAttribArray(1);

    // // specify how to interpret texture coordinates
    // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    // glEnableVertexAttribArray(2);

    // call to glVertexAttribPointer registered VBO as the vertex attribute's bound
    // therefore we can safely unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // unbind the VAO to not accidentally modify it
    // usually not needed since you need to call glBindVertexArray for this 
    glBindVertexArray(0);

    // enable depth testing 
    glEnable(GL_DEPTH_TEST);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        float current_frame = glfwGetTime();
        delta = current_frame - last_frame;
        last_frame = current_frame;

        // process inputs
        process_input(window);

        // perform rendering commands
        glClearColor(1.0f, 0.2f, 0.5f, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // apply simple transformation
        // mat4 trans;
        // glm_mat4_identity(trans);

        // glm_translate(trans, (vec3) { 0.5f, -0.5f, 0.0f });
        // glm_rotate(trans, (float) glfwGetTime(), (vec3) { 0.0f, 0.0f, 1.0f });

        // unsigned int transform_loc = glGetUniformLocation(shader.ID, "transform");
        // glUniformMatrix4fv(transform_loc, 1, GL_FALSE, *trans);

        // get clip coordinates using the formula:
        // vec_clip = mat_projection * mat_view * mat_model * vec_local

        for(unsigned int i = 0; i < 10; i++)
        {
            mat4 model;
            glm_mat4_identity(model);
            glm_translate(model, cubePositions[i]);

            float angle = 20.0f * i; 
            glm_rotate(model, glm_rad(angle+10) * (float) glfwGetTime(), (vec3) { 1.0f, 0.3f, 0.5f });
            
            set_mat4(&shader, "model", model);
            
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        mat4 model, view, projection;

        // model matrix
        glm_mat4_identity(model);
        // glm_rotate(model, glm_rad(-55.0f), (vec3) { 1.0f, 0.0f, 0.0f }); 
        glm_rotate(model, (float) glfwGetTime() * glm_rad(90.0f), (vec3) { 0.0f, 1.0f, 0.0f });  

        // old view matrix
        // glm_mat4_identity(view);
        // glm_translate(view, (vec3) { 0.0f, 0.0f, -3.0f });

        // projection matrix
        glm_perspective(glm_rad(camera.zoom), (float) width / (float) (height), 0.1f, 100.0f, projection);

        // set uniforms
        int model_loc = glGetUniformLocation(shader.ID, "model");
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, *model);

        // use camera for view
        get_view_matrix(&camera, view);
        int view_loc = glGetUniformLocation(shader.ID, "view");
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view[0]);

        int projection_loc = glGetUniformLocation(shader.ID, "projection");
        glUniformMatrix4fv(projection_loc, 1, GL_FALSE, *projection);

        // use shaders
        use_shader(&shader);

        glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0); // setting uniform manually
        set_int(&shader, "texture2", 1); // or with using the header function

        // bind textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // bind VAO
        glBindVertexArray(VAO);

        // draw wireframe
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // do indexed drawing with new bound EBO
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // glDrawArrays(GL_TRIANGLES, 0, 36);

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

void mouse_callback(GLFWwindow *window, double x_pos, double y_pos)
{
    float x_offset = x_pos - last_x;
    float y_offset = last_y - y_pos; // y coords go from bottom to top
    last_x = x_pos;
    last_y = y_pos;

    process_mouse_movement(&camera, x_offset, y_offset);
}

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
    process_mouse_scroll(&camera, y_offset);
}

void process_input(GLFWwindow *window)
{
    // close window on ESC
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, 1);
        destroy_camera(&camera);
    }

    // process WASD
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        process_keyboard(&camera, FORWARD, delta);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        process_keyboard(&camera, BACKWARD, delta);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        process_keyboard(&camera, LEFT, delta);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        process_keyboard(&camera, RIGHT, delta);
}

