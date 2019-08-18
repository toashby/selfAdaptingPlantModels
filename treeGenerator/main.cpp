#include <include/glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/shader_m.h"
#include "include/camera.h"
#include "include/model.h"
#include "include/linemodel.h"
#include "include/collisionmanager.h" //to pass to detector in plant
#include "sceneloader.h"

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window, LineModel LineModel);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 5.0f, 18.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//is the left mouse down?
bool lbutton_down;

unsigned int selectedTree = 0;

std::vector<Model> models;
std::vector<LineModel> lineModels;
std::vector<LineModel> plantOnes;

collisionManager cMan;

sceneLoader sLoader;

int main()
{   
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Forest Generator", NULL, NULL);
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
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glfwSetKeyCallback(window, key_callback);//for keypress new


    // tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
    Shader triShader("shaders/1.model_loading.vs", "shaders/1.model_loading.fs");
    Shader lineShader("shaders/lines.vs", "shaders/lines.fs");

    //seed the random generator
    srand(time(NULL));

    //how they were loaded before being pulled from a json file
    //Model houseModel("objects/column/column.obj");
    //houseModel.modelMatrix = glm::translate(houseModel.modelMatrix, glm::vec3(15.0f, 0.0f, 0.0f)); //
    //houseModel.modelMatrix = glm::scale(houseModel.modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));	//
    //models.push_back(houseModel);

    //load models from the scene.json
    sLoader.loadScene(&models, &lineModels, &cMan);

    //add the models to the collision manager
    for(unsigned int i = 0; i < models.size(); i++){//for each model
        for(unsigned int j = 0; j < models.at(i).meshes.size(); j++)//for each mesh
        {
            cMan.m_vertLists.push_back(&models.at(i).meshes.at(j).vertices);
            cMan.m_indLists.push_back(&models.at(i).meshes.at(j).indices);
            cMan.m_modelMatrices.push_back(models.at(i).modelMatrix);
        }
    }

    //load the terrain (floor)
    Model terrainModel("objects/terrain/terrain.obj");
    terrainModel.modelMatrix = glm::translate(terrainModel.modelMatrix, glm::vec3(0.0f, -0.1f, 0.0f)); // translate it down so it's at the center of the scene
    terrainModel.modelMatrix = glm::scale(terrainModel.modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
    //models.push_back(terrainModel);

    //MAKE PLANTS
    LineModel lineModel("trees/plant1.txt", &cMan);
    lineModel.createModel(1);
    lineModel.setupMesh();
    lineModel.modelMatrix = glm::translate(lineModel.modelMatrix, glm::vec3(-5.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    lineModel.modelMatrix = glm::scale(lineModel.modelMatrix, glm::vec3(0.9f, 0.9f, 0.9f));	// it's a bit too big for our scene, so scale it down
    //lineModels.push_back(lineModel);

    for(unsigned int i = 0; i < lineModels.size(); i++)
    {
        lineModels.at(i).react();
    }

    // draw everything in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window, lineModel);

        // render
        glClearColor(0.6f, 0.8f, 0.95f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLineWidth(1);

        //enable shader before setting the uniforms
        triShader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        triShader.setMat4("projection", projection);
        triShader.setMat4("view", view);
        //draw our models
        for(std::size_t i=0; i<models.size(); ++i)
        {
            triShader.setMat4("model", models[i].modelMatrix);
            models[i].Draw(triShader);
        }

        //switch to the line shader
        lineShader.use();

        lineShader.setMat4("projection", projection);
        lineShader.setMat4("view", view);

        //lineShader.setVec4("uColor", 0.2f, 0.3f, 0.15f, 1.0f);
        //draw the line models(trees)
        for(std::size_t i=0; i<lineModels.size(); ++i)
        {
            if(selectedTree == i)
            {
                lineShader.setVec4("uColor", 1.0f, 0.55f, 0.0f, 1.0f);
            }else{
                lineShader.setVec4("uColor", lineModels.at(i).m_color);
            }
            lineShader.setMat4("model", lineModels[i].modelMatrix);
            lineModels[i].Draw(lineShader);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, LineModel lineModel)
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

    //move the model
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        lineModels.at(selectedTree).modelMatrix = glm::translate(lineModels.at(selectedTree).modelMatrix, glm::vec3(0.2f, 0.0f, 0.0f));
        //lineModels.at(0).react();
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        lineModels.at(selectedTree).modelMatrix = glm::translate(lineModels.at(selectedTree).modelMatrix, glm::vec3(-0.2f, 0.0f, 0.0f));
        //lineModels.at(0).react();
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        lineModels.at(selectedTree).modelMatrix = glm::translate(lineModels.at(selectedTree).modelMatrix, glm::vec3(0.0f, 0.0f, -0.2f));
        //lineModels.at(0).react();
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        lineModels.at(selectedTree).modelMatrix = glm::translate(lineModels.at(selectedTree).modelMatrix, glm::vec3(0.0f, 0.0f, 0.2f));
        //lineModels.at(0).react();
    }

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
        lineModels.at(selectedTree).react();
    if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
        lineModels.at(selectedTree).react();
    if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
        lineModels.at(selectedTree).react();
    if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
        lineModels.at(selectedTree).react();

    if (key == GLFW_KEY_T && action == GLFW_PRESS)
    {
        unsigned int offset = 0;
        for(int i = 0; i < lineModels.size(); i++)
        {
            string tree = "tree";
            offset += lineModels.at(i).createMesh(tree += std::to_string(i), offset, camera.Position);
        }
    }

    //change selected tree for repositioning here
    if (key == GLFW_KEY_RIGHT_BRACKET && action == GLFW_RELEASE)
    {
        std::cout<<"left bracket pushed"<<std::endl;
        selectedTree++;
        if(selectedTree > lineModels.size()-1)
            selectedTree = 0;
    }

    if (key == GLFW_KEY_LEFT_BRACKET && action == GLFW_RELEASE)
    {
        std::cout<<"right bracket pushed"<<std::endl;
        selectedTree--;
        if(selectedTree < 0)
            selectedTree == lineModels.size() - 1;
    }
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
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(lbutton_down) {//if left mouse button is down
         // do your drag here
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
    }else{
        firstMouse = true;
    }
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if(GLFW_PRESS == action)
            lbutton_down = true;
        else if(GLFW_RELEASE == action)
            lbutton_down = false;
    }
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
