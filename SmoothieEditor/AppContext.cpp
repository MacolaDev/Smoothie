#include "AppContext.h"


float lastX = 800.0f / 2.0;
float lastY = 600.0f / 2.0;

int AppContext::width = 1280, AppContext::height = 720;
float AppContext::aspect_ratio = float(width) / float(height);

Smoothie::Scene AppContext::loadedScene;
SmoothieMath::Vector3 AppContext::cameraPosition = { -0.5, 3.0f, 18.0f };
Smoothie::Camera AppContext::camera = Smoothie::Camera(
        cameraPosition,
        { 0.0f, 0.0f, -1.0f },
        { 0.0f, 1.0f, 0.0f },
        45.0f, aspect_ratio, 0.1f, 100.0f
    );

float AppContext::cameraSpeed = 3.0f, AppContext::deltaTime = 0.0f, AppContext::sensitivity = 0.1f;

void windowChagneSizeCallback(GLFWwindow* window, int width, int height) {
	SmoothieCore::updateRenderResolution(width, height);
	lastX = float(width) / 2;
	lastY = float(height) / 2;
	AppContext::aspect_ratio = float(width) / float(height);
    AppContext::width = width;
    AppContext::height = height;

}

void processKeyboardEvent(GLFWwindow* window)
{
    float speed = AppContext::cameraSpeed * AppContext::deltaTime; // adjust accordingly

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        AppContext::cameraPosition = AppContext::cameraPosition + (AppContext::camera.cameraFront * speed);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {

        AppContext::cameraPosition = AppContext::cameraPosition - (AppContext::camera.cameraFront * speed);
    }


    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        auto d = cross(AppContext::camera.cameraFront, AppContext::camera.cameraUp);
        AppContext::cameraPosition = AppContext::cameraPosition - (normalize(d) * speed);
    }


    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        auto b = cross(AppContext::camera.cameraFront, AppContext::camera.cameraUp);
        AppContext::cameraPosition = AppContext::cameraPosition + (normalize(b) * speed);
    }

    AppContext::camera.updateCameraPosition(AppContext::cameraPosition);
}

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;

double lastMouseCoordinateX = 0, lastMouseCoordinateY = 0;
bool updateLastPos = true;
void MouseCallback(GLFWwindow* window, double xposIn, double yposIn)
{

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        if (updateLastPos) 
        {
            glfwSetCursorPos(window, lastMouseCoordinateX, lastMouseCoordinateY);
            updateLastPos = false;
        }
        
    }
    
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        if (updateLastPos == false) 
        {
            lastMouseCoordinateX = xposIn, lastMouseCoordinateY = yposIn;
            glfwSetCursorPos(window, AppContext::width/2, AppContext::height / 2);
            updateLastPos = true;
            return;
        }
        return;
    }
    
    double currXpos, currYpos;
    glfwGetCursorPos(window, &currXpos, &currYpos);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    float xpos = static_cast<float>(currXpos);
    float ypos = static_cast<float>(currYpos);

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

    
    xoffset *= AppContext::sensitivity;
    yoffset *= AppContext::sensitivity;

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
    AppContext::camera.updateCameraFront(front);
}


void SmoothieDebugData()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Begin("Debug Info:");
    ImGui::Text("Frames Per Second: %.3f ms/frame (%.1f)FPS", 1000.0f / io.Framerate, io.Framerate);
    ImGui::Text("Rendering resoultion: %d %d", AppContext::width, AppContext::height);
    ImGui::End();
}

void CameraInfo()
{
    ImGui::Begin("Camera Info");
    ImGui::Text("Camera position: %.3f %.3f %.3f", AppContext::cameraPosition.x, AppContext::cameraPosition.y, AppContext::cameraPosition.z);
    ImGui::DragFloat("Camera sensitivity: ", &AppContext::sensitivity, 0.1f, 0.01f, 1.0f);
    ImGui::DragFloat("Camera speed: ", &AppContext::cameraSpeed, 0.1f, 0.01f, 10.0f);
    ImGui::End();
}


static int selectedModelId = 0;
static std::vector<std::string> pbrModelNames;
static bool modelUpdate = true;
void SceneGraph()
{
    ImGui::Begin("Scene");
    if (ImGui::CollapsingHeader("Models")) 
    {
        Smoothie::Scene* currentScene = SmoothieCore::getSceneInstance();
        currentScene->getAllModelNames(pbrModelNames);
        {
            for (int i = 0; i < currentScene->getNumberOfModels(); i++)
            {
                if (ImGui::Selectable(pbrModelNames[i].c_str(), selectedModelId == i))
                {
                    selectedModelId = i;
                    AppContext::selectedModel = currentScene->getModel(selectedModelId);
                }
            }
        }
    }
    if (ImGui::CollapsingHeader("Terrain"))
    {
        ImGui::Text("Not yet implemented :D");

    }
    if (ImGui::CollapsingHeader("Particles"))
    {
        ImGui::Text("Not yet implemented :D");
    }
    if (ImGui::CollapsingHeader("Lights"))
    {
        ImGui::Text("Not yet implemented :D");
    }
    ImGui::End();
}

Smoothie::Model* AppContext::selectedModel = nullptr;

void ModelEditor()
{
    ImGui::Begin("Model Editor");
    if (AppContext::selectedModel == nullptr) 
    {
        ImGui::Text("No model selected!");
        ImGui::End();
        return;
    }
    else
    {   
        SmoothieMath::Vector3 modelPosition = AppContext::selectedModel->getModelPosition();
        SmoothieMath::Vector3 modelScale = AppContext::selectedModel->getModelScale();
        SmoothieMath::Vector3 modelRotation = AppContext::selectedModel->getModelRotationEuler();

        ImGui::Text("Scale:");
        ImGui::DragFloat3("##scale", &modelScale.x, 0.1f, 0.01f);
        ImGui::Text("\n");

        ImGui::Text("Position:");
        ImGui::DragFloat3("##position", &modelPosition.x, 0.1f);
        ImGui::Text("\n");
        
        ImGui::Text("Rotation: ");
        ImGui::DragFloat3("##rotation", &modelRotation.x, 0.1f);
        ImGui::Text("\n");

        AppContext::selectedModel->setModelPosition(modelPosition);
        AppContext::selectedModel->setModelScale(modelScale);
        AppContext::selectedModel->setModelRotationEuler(modelRotation);

        AppContext::selectedModel->updateModelTransformMatrix();
        AppContext::selectedModel->drawBoundBox();
    }


    ImGui::End();
}
