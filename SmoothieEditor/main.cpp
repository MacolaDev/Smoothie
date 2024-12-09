#include "AppContext.h"


int main(int argc, char* argv[])
{

    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Smoothie Editor", nullptr, nullptr);
    if (window == nullptr) return -1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetFramebufferSizeCallback(window, windowChagneSizeCallback);
    
    AppContext::loadedScene = Smoothie::Scene("resources//DemoScene//Demo.sscene");
    
    SmoothieCore::useScene(&AppContext::loadedScene);
    SmoothieCore::useCamera(AppContext::camera);
    SmoothieCore::initilize();

    //Will be moved to Debug class later
    SmoothieCore::GlDebugOutputSynchronous();
    

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
    
    ImGui::StyleColorsDark();
    
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;
    io.Fonts->AddFontFromFileTTF("resources\\fonts\\Roboto-Medium.ttf", 16.0f);
  
    colors[ImGuiCol_Button] = { 0.3f, 0.3f, 0.3f, 0.4f };
    colors[ImGuiCol_ButtonHovered] = { 0.4f, 0.4f, 0.4f, 0.5f };
    colors[ImGuiCol_ButtonActive] = { 0.35f, 0.35f, 0.35f, 0.45f };

    colors[ImGuiCol_Header] = { 0.2f, .2f, .2f, 0.83f };
    colors[ImGuiCol_HeaderHovered] = { 0.3f, .3f, .3f, 0.83f };
    colors[ImGuiCol_HeaderActive] = { 0.35f, .35f, .35f, 0.83f };

    colors[ImGuiCol_FrameBg] = { 0.2f, .2f, .2f, 0.83f };
    colors[ImGuiCol_FrameBgHovered] = { 0.3f, .3f, .3f, 0.83f };
    colors[ImGuiCol_FrameBgActive] = { 0.35f, .35f, .35f, 0.83f };

    colors[ImGuiCol_TitleBgActive] = {.3f, .0f, .0f, 1.0f};
    colors[ImGuiCol_CheckMark] = { 0.0f, 1.0f, 0.0f, 1.0f };

    colors[ImGuiCol_SliderGrab] = { 0.0f, 0.027f, 1.0f, 1.0f };
    colors[ImGuiCol_SliderGrab] = { 0.0f, 0.027f, 0.8f, 1.0f };

    colors[ImGuiCol_Tab] = { 0.3f, 0.3f, 0.3f, 0.4f };
    colors[ImGuiCol_TabHovered] = { 0.4f, 0.4f, 0.4f, 0.5f };
    colors[ImGuiCol_TabSelected] = { 0.45f, 0.45f, 0.45f, 0.55f };

    colors[ImGuiCol_DockingEmptyBg] = { 0,0,0,0 };
    colors[ImGuiCol_Border] = { 0, 0, 0, 1 };
    style.WindowBorderSize = 0;
    style.ChildBorderSize = 0;
    style.PopupBorderSize = 0;
    style.FrameBorderSize = 0;
    style.TabBorderSize = 0;
    style.TabBarBorderSize = 0;
    style.TabBarOverlineSize = 0;

    style.WindowRounding = 4;
    style.ChildRounding = 4;
    style.FrameRounding = 3;
    style.PopupRounding = 4;
    style.ScrollbarRounding = 12;
    style.GrabRounding = 2;
    style.TabRounding = 4;

    style.WindowPadding = { 0, 0 };


    bool show_demo_window = true;
    bool show_another_window = false;

    float array[3] = { 0, 0, 0 };
    bool showDockspace = true;

    float currentFrame = 0, lastFrame = 0;
    while (!glfwWindowShouldClose(window)) 
    {

        currentFrame = (float)glfwGetTime();
        AppContext::deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processKeyboardEvent(window);

        SmoothieCore::render();


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
        {
            io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
        }
        else
        {
            io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
        }
        

        colors[ImGuiCol_WindowBg] = { 0, 0, 0, 0 };
        ImGui::Begin("DockSpace Demo", &showDockspace, windowFlags);
        ImGui::DockSpace(ImGui::GetID("MyDockSpace"), ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
        ImGui::End();
        colors[ImGuiCol_WindowBg] = { 0.06f, 0.06f, 0.06f, 0.940f };

        bool opendemowindow = true;
        ImGui::ShowDemoWindow(&opendemowindow);
        SmoothieDebugData();
        SceneGraph();
        ModelEditor();
        CameraInfo();
        
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwPollEvents();

        glfwSwapBuffers(window);
    }
    
}