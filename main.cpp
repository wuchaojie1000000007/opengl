#include <iostream>
#include <sstream>

#include "ShaderProgram.h"
#include "Texture.h"
#include "Camera.h"
#include "Mesh.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Do not need this  ->  #include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

const char* APP_TITLE = "Learning OpenGL";
int windowWidth = 800;
int windowHeight = 600;
GLFWwindow* window = NULL;
bool fullScreen = false;
bool wireFrame = false;
const std::string texture1Filename = "/Users/chaojiewu/Downloads/cat.jpg";
const std::string texture2Filename = "/Users/chaojiewu/Downloads/cat2.jpg";


// There's some problem, start look position different each run, and can not init look position to box.
FPSCamera fpsCamera(glm::vec3(0.0f, 0.0f, 5.0f));
const double ZOOM_SENSITIVITY = -3.0;
const float MOVE_SPEED = 5.0;   // unit per second
const float MOUSE_SENSITIVITY = 0.003;


//OrbitCamera orbitCamera;
//float yaw = 0.0f;
//float pitch = 0.0f;
//float radius = 10.0f;
//const float MOUSE_SENSITIVITY = 0.2;

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_onFrameBufferSize(GLFWwindow* window, int width, int height);
void glfw_onMouseMove(GLFWwindow* window, double posX, double posY);
void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY);
void update(double elapsedTime);
void showFPS(GLFWwindow* window);
bool initOpenGL();

int main()
{

    // Initialize OpenGl.
    if (!initOpenGL()) {
        std::cerr << "GLFW initialization failed. \n";
        return -1;
    }
 
    
    // Create ShaderProgram to handle filename -> source code -> shader -> program
    ShaderProgram shaderProgram;
    shaderProgram.loadShaders("/Users/chaojiewu/Downloads/opengl_learn/opengl_learn/opengl_learn/shader.vs","/Users/chaojiewu/Downloads/opengl_learn/opengl_learn/opengl_learn/shader.fs");
    
    // Model position
    glm::vec3 modelPos[] = {
        glm::vec3(-1.0f, -1.0f, -1.0f)
    };
    
    // Load meshes and textures
    const int numModels = 1;
    Mesh mesh[numModels];
    Texture2D texture[numModels];
    
    mesh[0].loadOBJ("/Users/chaojiewu/Downloads/obj/dog.obj");
    texture[0].loadTexture("/Users/chaojiewu/Downloads/cat.jpg", true);
    
    double lastTime = glfwGetTime();;
    
    // Main loop, called each frame.
    while (!glfwWindowShouldClose(window))
    {
        showFPS(window);
        glfwPollEvents();
        
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        update(deltaTime);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // model -> view -> projection
        glm::mat4 model, view, projection;
        
        view = fpsCamera.getViewMatrix();
        
        projection = glm::perspective(glm::radians(fpsCamera.getFOV()), (float)windowWidth/(float)windowHeight, 0.1f, 100.0f);
        
        
        shaderProgram.use();
        
        shaderProgram.setUniform("view", view);
        shaderProgram.setUniform("projection", projection);
        
        // Different have diff position.
        for (int i = 0; i < numModels; i++) {
            
            model = glm::translate(glm::mat4(1.0f), modelPos[i]);
            shaderProgram.setUniform("model", model);
            
            texture[i].bind(0);
            mesh[i].draw();
            texture[i].unbind(0);
        }
        
        glfwSwapBuffers(window);
        lastTime = currentTime;
    }
    
    glfwTerminate();
    return 0;
}

// Initialize library about to use.
bool initOpenGL()
{
    if (!glfwInit()) {
        std::cerr << "GLFW initialization failed \n";
        return false;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    if (fullScreen)
    {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        if (mode != NULL) {
            window = glfwCreateWindow(mode -> width, mode -> height, APP_TITLE, monitor, NULL);
        }
    }
    else
    {
        window = glfwCreateWindow(windowWidth, windowHeight, APP_TITLE, NULL, NULL);
    }
    
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window \n";
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(window);
    
    glfwSetKeyCallback(window, glfw_onKey);
    glfwSetCursorPosCallback(window, glfw_onMouseMove);
    glfwSetScrollCallback(window, glfw_onMouseScroll);
    
    // Hides and grabs cursor, unlimited movement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // But position is not in the middle
    glfwSetCursorPos(window, windowWidth/ 2.0, windowHeight / 2.0);
    
    
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW initialization failed \n";
        return false;
    }
    
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    // But times 2 make it looks normal
    glViewport(0, 0, windowWidth * 2, windowHeight * 2);
    glEnable(GL_DEPTH_TEST);
    return true;
}

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        wireFrame = !wireFrame;
        if (wireFrame) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }else{
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
}

void glfw_onFrameBufferSize(GLFWwindow* window, int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, windowWidth, windowHeight);
}

void glfw_onMouseMove(GLFWwindow* window, double posX, double posY)
{
   //static glm::vec2 lastMousePos = glm::vec2(0.0f, 0.0f);
   //
   //if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == 1) {
   //    yaw -= ((float)posX - lastMousePos.x) * MOUSE_SENSITIVITY;
   //    pitch += ((float)posY - lastMousePos.y) * MOUSE_SENSITIVITY;
   //    std::cerr << "yaw is: "<<yaw<<"   pitch is: "<<pitch<<"\n";
   //}
   //
   //if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == 1) {
   //    float dx = 0.01f * ((float)posX - lastMousePos.x);
   //    float dy = 0.01f * ((float)posY - lastMousePos.y);
   //    radius += dx - dy;
   //}
   //
   //lastMousePos.x = (float)posX;
   //lastMousePos.y = (float)posY;
}

// Called by GLFW when the mouse wheel is rotated
void glfw_onMouseScroll(GLFWwindow* window, double deltaX, double deltaY)
{
    double fov = fpsCamera.getFOV() + deltaY * ZOOM_SENSITIVITY;
    
    fov = glm::clamp(fov, 1.0, 120.0);
    
    fpsCamera.setFOV((float)fov);
}

// Update stuff every frame
void update(double elapsedTime)
{
    // Camera orientation
    double mouseX, mouseY;
    
    // Get the current mouse cursor position delta
    glfwGetCursorPos(window, &mouseX, &mouseY);
    
    // Rotate the camera the difference in mouse distance from the center screen.
    // Multiply this delta by a speed scaler
    fpsCamera.rotate((float)(windowWidth / 2.0 - mouseX) * MOUSE_SENSITIVITY , (float)(windowHeight / 2.0 - mouseY) * MOUSE_SENSITIVITY);
    
    // Camera FPS movement
    
    // Forward/backward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getLook());
    }else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getLook());
    }
    
    // Strafe left/right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getRight());
    }else if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getRight());
    }
    
    // Up/down
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * fpsCamera.getUp());
    }else if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS){
        fpsCamera.move(MOVE_SPEED * (float)elapsedTime * -fpsCamera.getUp());
    }
    
}

void showFPS(GLFWwindow* window)
{
    // function internal variable.
    static double previousTime = 0.0f;
    static int frameCount = 0;
    double currentTime = glfwGetTime(); // return number of seconds as double since GLFW started
    double elapsedTime = currentTime - previousTime;
    
    // limit text called 5 times per second.
    if (elapsedTime > 0.2) {
        previousTime = currentTime;
        double fps = (double)frameCount / elapsedTime;
        double msPerFrame = 1000.0 / fps;
        
        std::ostringstream outs;
        outs.precision(3);
        outs << std::fixed << APP_TITLE << "   FPS: " << fps << "   Frame Time: "
        << msPerFrame << "(ms)";
        glfwSetWindowTitle(window, outs.str().c_str());
        
        frameCount = 0;
    }
    
    frameCount++;
}
