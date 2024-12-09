#include "Demo.h"

Vector3 cameraPosition = { -0.5, 3.0f, 18.0f };

float currentFrame = 0.0f;
float lastFrame = 0.0f;
float deltaTime = 0.0f;

float cameraSpeed = 3;
bool firstMouse = true;

float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;

float lastX = 800.0f / 2.0;
float lastY = 600.0f / 2.0;

int width = 1280, height = 720;

float aspect_ratio = float(width)/float(height);
float fov = 45.0f;

Smoothie::Camera demoCamera;
Smoothie::Scene demoScene;



Demo::Demo()
{

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(1280, 720, "Smoothie Demo", NULL, NULL);


    glfwSetFramebufferSizeCallback(window, windowChagneSizeCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(window);

    //Load scene
    demoScene = Smoothie::Scene("resources//DemoScene//Demo.sscene");
    SmoothieCore::useScene(&demoScene);

    demoCamera = Smoothie::Camera(
        cameraPosition,
        { 0.0f, 0.0f, -1.0f },
        { 0.0f, 1.0f, 0.0f },
        45.0f, aspect_ratio, 0.1f, 100.0f
    );

    SmoothieCore::useCamera(demoCamera);
    SmoothieCore::initilize();

    //Will be moved to Debug class later
    SmoothieCore::GlDebugOutputSynchronous();
}

Demo::~Demo()
{
    glfwTerminate();
}

void Demo::render()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwSwapInterval(0);
        currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processKeyboardEvent(window, demoCamera);

        SmoothieCore::render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void processKeyboardEvent(GLFWwindow* window, Smoothie::Camera& camera)
{
    //Press ESC to close Demo
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    float speed = cameraSpeed * deltaTime; // adjust accordingly

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPosition = cameraPosition + (camera.cameraFront * speed);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {

        cameraPosition = cameraPosition - (camera.cameraFront * speed);
    }
    

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        auto d = cross(camera.cameraFront, camera.cameraUp);
        cameraPosition = cameraPosition - (normalize(d) * speed);
    }
    

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        auto b = cross(camera.cameraFront, camera.cameraUp);
        cameraPosition = cameraPosition + (normalize(b) * speed);
    }

    camera.updateCameraPosition(cameraPosition);

}

void windowChagneSizeCallback(GLFWwindow * window, int width, int height)
{
    SmoothieCore::updateRenderResolution(width, height);
    lastX = float(width) / 2;
    lastY = float(height) / 2;
    aspect_ratio = float(width) / float(height);
}

void MouseCallback(GLFWwindow* window, double xposIn, double yposIn)
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

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    Vector3 front;
    front.x = cos(toRadians(yaw)) * cos(toRadians(pitch));
    front.y = sin(toRadians(pitch));
    front.z = sin(toRadians(yaw)) * cos(toRadians(pitch));
    front.normalizeVector();
    demoCamera.updateCameraFront(front);
}
