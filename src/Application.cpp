#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Shader/shader.h>
//Including mathematics library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//Learnopengl Supports
#include <learnopengl/camera.h>
#include <Learnopengl/model.h>

//Texture header file importing last 
#include <Texture/texture.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;



int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Galaxy Solar System", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader shader("resources/modelloader.vs", "resources/shader.fs");
    Shader skyboxShader("resources/shaderSkybox.vs", "resources/shaderSkybox.fs");

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

   
    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // load textures
    // -------------
    Texture texture1("resources/Sun/sun.jpg", true);
    Texture texture2("resources/Sun/earth.jpg", true);
    Texture texture3("resources/Sun/mercury.jpg", true);
    Texture texture4("resources/Sun/venus.jpg", true);
    Texture texture5("resources/Sun/mars.jpg", true);
    Texture texture6("resources/Sun/jupiter.jpg", true);
    Texture texture7("resources/Sun/saturn.jpg", true);
    Texture texture8("resources/Sun/uranus.jpg", true);
    Texture texture9("resources/Sun/neptune.jpg", true);

    std::vector<std::string> faces
    {
        "resources/skybox/GalaxyTex_PositiveX.png",
            "resources/skybox/GalaxyTex_NegativeX.png",
            "resources/skybox/GalaxyTex_PositiveY.png",
            "resources/skybox/GalaxyTex_NegativeY.png",
            "resources/skybox/GalaxyTex_PositiveZ.png",
            "resources/skybox/GalaxyTex_NegativeZ.png"
    };

    Texture cubeMapTexture(faces, true);

    
    Model sun("resources/Sun/sun.obj");


    // shader configuration
    // --------------------

 

    skyboxShader.useShaderProgram();
    skyboxShader.setInt("skybox", 0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        //Relative Rotation Speed
        float jupiter = (float)glfwGetTime(); // i.e 45,583 km/h
        float rotationSpeedJupiter = 45583.0f;

        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        shader.useShaderProgram();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // render the loaded model
        //Sun
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
        shader.setMat4("model", model);
        glBindTexture(GL_TEXTURE_2D, texture1.ID);
        sun.Draw(shader);

        glBindTexture(GL_TEXTURE_2D, 0);

        //Mercury
        glm::mat4 orbitmodel = glm::mat4(1.0f);
        float OrbitRadius = 5.0f;
        float OrbitSpeed = 47.9f;  // Adjust the speed as needed
        // Update the position of Mercury in its orbit
        float OrbitAngle = glm::radians(glfwGetTime() * OrbitSpeed);
        glm::vec3 OrbitPosition = glm::vec3(cos(OrbitAngle) * OrbitRadius, 0.0f, sin(OrbitAngle) * OrbitRadius);
        orbitmodel = glm::translate(orbitmodel, OrbitPosition);

        // Render Mercury on its orbit
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
        model = orbitmodel * model;  // Combine the orbit and rotation
        model = glm::rotate(model,(jupiter / rotationSpeedJupiter) * 10.8f, glm::vec3(0.0, 1.0f, 0.0));
        shader.setMat4("model", model);
        glBindTexture(GL_TEXTURE_2D, texture3.ID);
        sun.Draw(shader);
        glBindTexture(GL_TEXTURE_2D, 0);


        //Venus
        orbitmodel = glm::mat4(1.0f);
        OrbitRadius = 7.0f;
        OrbitSpeed = 35.0f;  // Adjust the speed as needed
        OrbitAngle = glm::radians(glfwGetTime() * OrbitSpeed);
        OrbitPosition = glm::vec3(cos(OrbitAngle) * OrbitRadius, 0.0f, sin(OrbitAngle) * OrbitRadius);
        orbitmodel = glm::translate(orbitmodel, OrbitPosition);
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
        model = orbitmodel * model;  // Combine the orbit and rotation
        model = glm::rotate(model, (jupiter / rotationSpeedJupiter) * 6.5f, glm::vec3(0.0, 1.0f, 0.0));
        shader.setMat4("model", model);
        glBindTexture(GL_TEXTURE_2D, texture4.ID);
        sun.Draw(shader);

        glBindTexture(GL_TEXTURE_2D, 0);

        //Earth
        orbitmodel = glm::mat4(1.0f);
        OrbitRadius = 9.0f;
        OrbitSpeed = 29.8f;  // Adjust the speed as needed
        OrbitAngle = glm::radians(glfwGetTime() * OrbitSpeed);
        OrbitPosition = glm::vec3(cos(OrbitAngle) * OrbitRadius, 0.0f, sin(OrbitAngle) * OrbitRadius);
        orbitmodel = glm::translate(orbitmodel, OrbitPosition);
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
        model = orbitmodel * model;  // Combine the orbit and rotation
        model = glm::rotate(model, (jupiter / rotationSpeedJupiter) * 1574, glm::vec3(0.0, 1.0f, 0.0));
        shader.setMat4("model", model);
        glBindTexture(GL_TEXTURE_2D, texture2.ID);
        sun.Draw(shader);
        glBindTexture(GL_TEXTURE_2D, 0);


        //Mars
        orbitmodel = glm::mat4(1.0f);
        OrbitRadius = 11.0f;
        OrbitSpeed = 24.0f;  // Adjust the speed as needed
        OrbitAngle = glm::radians(glfwGetTime() * OrbitSpeed);
        OrbitPosition = glm::vec3(cos(OrbitAngle) * OrbitRadius, 0.0f, sin(OrbitAngle) * OrbitRadius);
        orbitmodel = glm::translate(orbitmodel, OrbitPosition);
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
        model = orbitmodel * model;  // Combine the orbit and rotation
        model = glm::rotate(model, (jupiter / rotationSpeedJupiter) * 866, glm::vec3(0.0, 1.0f, 0.0));
        shader.setMat4("model", model);
        glBindTexture(GL_TEXTURE_2D, texture5.ID);
        sun.Draw(shader);
        glBindTexture(GL_TEXTURE_2D, 0);

        //Jupiter
        orbitmodel = glm::mat4(1.0f);
        OrbitRadius = 14.0f;
        OrbitSpeed = 13.1f;  // Adjust the speed as needed
        OrbitAngle = glm::radians(glfwGetTime() * OrbitSpeed);
        OrbitPosition = glm::vec3(cos(OrbitAngle) * OrbitRadius, 0.0f, sin(OrbitAngle) * OrbitRadius);
        orbitmodel = glm::translate(orbitmodel, OrbitPosition);
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
        model = orbitmodel * model;  // Combine the orbit and rotation
        model = glm::rotate(model, jupiter, glm::vec3(0.0, 1.0f, 0.0));
        shader.setMat4("model", model);
        glBindTexture(GL_TEXTURE_2D, texture6.ID);
        sun.Draw(shader);
        glBindTexture(GL_TEXTURE_2D, 0);

        //Saturn
        orbitmodel = glm::mat4(1.0f);
        OrbitRadius = 17.0f;
        OrbitSpeed = 9.69f;  // Adjust the speed as needed
        OrbitAngle = glm::radians(glfwGetTime() * OrbitSpeed);
        OrbitPosition = glm::vec3(cos(OrbitAngle) * OrbitRadius, 0.0f, sin(OrbitAngle) * OrbitRadius);
        orbitmodel = glm::translate(orbitmodel, OrbitPosition);
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
        model = orbitmodel * model;  // Combine the orbit and rotation
        model = glm::rotate(model, (jupiter/rotationSpeedJupiter) * 36480.0f, glm::vec3(0.0, 1.0f, 0.0));
        shader.setMat4("model", model);
        glBindTexture(GL_TEXTURE_2D, texture7.ID);
        sun.Draw(shader);
        glBindTexture(GL_TEXTURE_2D, 0);

        //Uranus
        orbitmodel = glm::mat4(1.0f);
        OrbitRadius = 20.0f;
        OrbitSpeed = 6.81f;  // Adjust the speed as needed
        OrbitAngle = glm::radians(glfwGetTime() * OrbitSpeed);
        OrbitPosition = glm::vec3(cos(OrbitAngle) * OrbitRadius, 0.0f, sin(OrbitAngle) * OrbitRadius);
        orbitmodel = glm::translate(orbitmodel, OrbitPosition);
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
        model = orbitmodel * model;  // Combine the orbit and rotation
        model = glm::rotate(model, (jupiter / rotationSpeedJupiter) * 14794.0f, glm::vec3(0.0, 1.0f, 0.0));
        shader.setMat4("model", model);
        glBindTexture(GL_TEXTURE_2D, texture8.ID);
        sun.Draw(shader);
        glBindTexture(GL_TEXTURE_2D, 0);

        //Neptune
        orbitmodel = glm::mat4(1.0f);
        OrbitRadius = 23.0f;
        OrbitSpeed = 5.43f;  // Adjust the speed as needed
        OrbitAngle = glm::radians(glfwGetTime() * OrbitSpeed);
        OrbitPosition = glm::vec3(cos(OrbitAngle) * OrbitRadius, 0.0f, sin(OrbitAngle) * OrbitRadius);
        orbitmodel = glm::translate(orbitmodel, OrbitPosition);
        model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
        model = orbitmodel * model;  // Combine the orbit and rotation
        model = glm::rotate(model, (jupiter / rotationSpeedJupiter) * 9719.0f, glm::vec3(0.0, 1.0f, 0.0));
        shader.setMat4("model", model);
        glBindTexture(GL_TEXTURE_2D, texture9.ID);
        sun.Draw(shader);
        glBindTexture(GL_TEXTURE_2D, 0);

        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.useShaderProgram();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);

        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture.ID);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "OpenGL Error: " << error << std::endl;
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    
    glDeleteVertexArrays(1, &skyboxVAO);
    
    glDeleteBuffers(1, &skyboxVBO);

    glfwTerminate();
    return 0;
}



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset * 0.1));
}


