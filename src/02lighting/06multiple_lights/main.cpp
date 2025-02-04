#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "shader.h"
#include "camera.h"

/* 改变窗口改变大小 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
/* 处理输入 */
void processInput(GLFWwindow* window);
/* 处理鼠标 */
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
/* 处理鼠标按键 */
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
/* 处理鼠标滚轮 */
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
/* 加载纹理文件 */
unsigned int loadTexture(char const* path);

// 设置
const unsigned int SCR_WIDTH  = 800; // 屏幕宽度
const unsigned int SCR_HEIGHT = 600; // 屏幕高度

// 时间
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

// 摄像机
Camera camera(glm::vec3(0.0f, 0.0f, 6.0f)); //
bool   firstMouse  = true;                  //
float  lastX       = SCR_WIDTH / 2.0f;      //
float  lastY       = SCR_HEIGHT / 2.0f;     //
bool   isMouseLeft = false;                 // 标志变量，表示是否启用鼠标控制

// 光源
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// clang-format off
// 设置顶点数据（和缓冲区）并配置顶点属性
float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};
// 所有容器位置
glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};
// 光源位置
glm::vec3 pointLightPositions[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};
// clang-format on

// 氛围
enum ATMOSPHERE
{
    DEFAULT,     // 默认
    DESERT,      // 沙漠
    FACTORY,     // 工厂
    HORROR,      // 恐怖
    BIOCHEMICAL, // 生化
};

int primary()
{
    // 初始化，指定版本 3.3，仅使用 Core profile
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // 同步显示器的垂直刷新率
    glfwSwapInterval(1);

    // 获取 OpenGL 函数地址
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);          // 注册鼠标移动回调
    glfwSetMouseButtonCallback(window, mouse_button_callback); // 注册鼠标按键回调
    glfwSetScrollCallback(window, scroll_callback);            // 注册鼠标滚轮回调

    // 参考 imgui/example_glfw_opengl3 配置
    const char* glsl_version = "#version 330";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    // 设置中文
    io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/msyh.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());

    // imgui 启用 glfw + opensl3
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // 着色器类
    Shader lightingShader("02lighting/6.multiple_lights.vs", "02lighting/6.multiple_lights.fs");
    Shader lightCubeShader("02lighting/2.1.light_cube.vs", "02lighting/2.1.light_cube.fs");

    // 物体
    unsigned int VBO, cubeVAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 位置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 单位向量
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // 纹理
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // 光源
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    // 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 加载纹理文件
    unsigned int diffuseMap  = loadTexture("02lighting/container2.png");
    unsigned int specularMap = loadTexture("02lighting/container2_specular.png");

    // 允许深度测试
    glEnable(GL_DEPTH_TEST);

    // imgui 变量
    int   imgui_combo_item   = 0;
    float material_shininess = 32.0 / 128.0;

    // 循环渲染
    while (!glfwWindowShouldClose(window))
    {
        // 事件轮询与处理
        glfwPollEvents();

        //  帧初始化
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver);

        // imgui 界面
        {
            ImGui::Begin("投光物");

            ImGui::SetNextItemOpen(true, ImGuiCond_FirstUseEver);
            if (ImGui::CollapsingHeader("投光物"))
            {
                const char* items[] = {
                    "DEFAULT",
                    "DESERT",
                    "FACTORY",
                    "HORROR",
                    "BIOCHEMICAL",
                };
                ImGui::Combo("投光物类型", &imgui_combo_item, items, IM_ARRAYSIZE(items));
            }

            ImGui::End();
        }

        // 计算时间差
        float currentFrame = glfwGetTime();
        deltaTime          = currentFrame - lastFrame;
        lastFrame          = currentFrame;

        // 输入
        processInput(window);

        // 渲染指令
        switch (imgui_combo_item)
        {
        case DEFAULT:
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            break;
        case DESERT:
            glClearColor(0.75f, 0.52f, 0.3f, 1.0f);
            break;
        case FACTORY:
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            break;
        case HORROR:
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            break;
        case BIOCHEMICAL:
            glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
            break;
        default:
            break;
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 在此之前不要忘记首先 use 对应的着色器程序（来设定uniform）
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        // 材料特性
        lightingShader.setFloat("material.shininess", material_shininess * 128.0);

        /**
         * 在这里，我们要为 5/6 种类型的灯光设置所有制服。 我们必须手动设置它们，并在数组中索引适当的 PointLight 结构，以设置每个统一变量。
         * 通过将光线类型定义为类并在其中设置它们的值，或者通过使用 "Uniform buffer objects（统一缓冲对象）"这种更高效的统一方法，
         * 可以更方便地编写代码，但我们将在 "Advanced GLSL（高级 GLSL）"教程中讨论这一点。
         */
        switch (imgui_combo_item)
        {
        case DEFAULT:
            // directional light
            lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
            lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
            lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
            lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
            // point light 1
            lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
            lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
            lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
            lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
            lightingShader.setFloat("pointLights[0].constant", 1.0f);
            lightingShader.setFloat("pointLights[0].linear", 0.09f);
            lightingShader.setFloat("pointLights[0].quadratic", 0.032f);
            // point light 2
            lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
            lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
            lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
            lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
            lightingShader.setFloat("pointLights[1].constant", 1.0f);
            lightingShader.setFloat("pointLights[1].linear", 0.09f);
            lightingShader.setFloat("pointLights[1].quadratic", 0.032f);
            // point light 3
            lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
            lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
            lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
            lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
            lightingShader.setFloat("pointLights[2].constant", 1.0f);
            lightingShader.setFloat("pointLights[2].linear", 0.09f);
            lightingShader.setFloat("pointLights[2].quadratic", 0.032f);
            // point light 4
            lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
            lightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
            lightingShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
            lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
            lightingShader.setFloat("pointLights[3].constant", 1.0f);
            lightingShader.setFloat("pointLights[3].linear", 0.09f);
            lightingShader.setFloat("pointLights[3].quadratic", 0.032f);
            // spotLight
            lightingShader.setVec3("spotLight.position", camera.Position);
            lightingShader.setVec3("spotLight.direction", camera.Front);
            lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
            lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
            lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
            lightingShader.setFloat("spotLight.constant", 1.0f);
            lightingShader.setFloat("spotLight.linear", 0.09f);
            lightingShader.setFloat("spotLight.quadratic", 0.032f);
            lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
            lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
            break;
        case DESERT: {
            glm::vec3 pointLightColors[] = {
                glm::vec3(1.0f, 0.6f, 0.0f),
                glm::vec3(1.0f, 0.0f, 0.0f),
                glm::vec3(1.0f, 1.0, 0.0),
                glm::vec3(0.2f, 0.2f, 1.0f),
            };
            // Directional light
            lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
            lightingShader.setVec3("dirLight.ambient", 0.3f, 0.24f, 0.14f);
            lightingShader.setVec3("dirLight.diffuse", 0.7f, 0.42f, 0.26f);
            lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
            // Point light 1
            lightingShader.setVec3("pointLights[0].position", pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
            lightingShader.setVec3("pointLights[0].ambient", pointLightColors[0].x * 0.1, pointLightColors[0].y * 0.1, pointLightColors[0].z * 0.1);
            lightingShader.setVec3("pointLights[0].diffuse", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
            lightingShader.setVec3("pointLights[0].specular", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
            lightingShader.setFloat("pointLights[0].constant", 1.0f);
            lightingShader.setFloat("pointLights[0].linear", 0.09);
            lightingShader.setFloat("pointLights[0].quadratic", 0.032);
            // Point light 2
            lightingShader.setVec3("pointLights[1].position", pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
            lightingShader.setVec3("pointLights[1].ambient", pointLightColors[1].x * 0.1, pointLightColors[1].y * 0.1, pointLightColors[1].z * 0.1);
            lightingShader.setVec3("pointLights[1].diffuse", pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
            lightingShader.setVec3("pointLights[1].specular", pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
            lightingShader.setFloat("pointLights[1].constant", 1.0f);
            lightingShader.setFloat("pointLights[1].linear", 0.09);
            lightingShader.setFloat("pointLights[1].quadratic", 0.032);
            // Point light 3
            lightingShader.setVec3("pointLights[2].position", pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
            lightingShader.setVec3("pointLights[2].ambient", pointLightColors[2].x * 0.1, pointLightColors[2].y * 0.1, pointLightColors[2].z * 0.1);
            lightingShader.setVec3("pointLights[2].diffuse", pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
            lightingShader.setVec3("pointLights[2].specular", pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
            lightingShader.setFloat("pointLights[2].constant", 1.0f);
            lightingShader.setFloat("pointLights[2].linear", 0.09);
            lightingShader.setFloat("pointLights[2].quadratic", 0.032);
            // Point light 4
            lightingShader.setVec3("pointLights[3].position", pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
            lightingShader.setVec3("pointLights[3].ambient", pointLightColors[3].x * 0.1, pointLightColors[3].y * 0.1, pointLightColors[3].z * 0.1);
            lightingShader.setVec3("pointLights[3].diffuse", pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
            lightingShader.setVec3("pointLights[3].specular", pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
            lightingShader.setFloat("pointLights[3].constant", 1.0f);
            lightingShader.setFloat("pointLights[3].linear", 0.09);
            lightingShader.setFloat("pointLights[3].quadratic", 0.032);
            // SpotLight
            lightingShader.setVec3("spotLight.position", camera.Position.x, camera.Position.y, camera.Position.z);
            lightingShader.setVec3("spotLight.direction", camera.Front.x, camera.Front.y, camera.Front.z);
            lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
            lightingShader.setVec3("spotLight.diffuse", 0.8f, 0.8f, 0.0f);
            lightingShader.setVec3("spotLight.specular", 0.8f, 0.8f, 0.0f);
            lightingShader.setFloat("spotLight.constant", 1.0f);
            lightingShader.setFloat("spotLight.linear", 0.09);
            lightingShader.setFloat("spotLight.quadratic", 0.032);
            lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
            lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(13.0f)));
        }
        break;
        case FACTORY: {
            glm::vec3 pointLightColors[] = {
                glm::vec3(0.2f, 0.2f, 0.6f),
                glm::vec3(0.3f, 0.3f, 0.7f),
                glm::vec3(0.0f, 0.0f, 0.3f),
                glm::vec3(0.4f, 0.4f, 0.4f),
            };
            // Directional light
            lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
            lightingShader.setVec3("dirLight.ambient", 0.3f, 0.24f, 0.14f);
            lightingShader.setVec3("dirLight.diffuse", 0.7f, 0.42f, 0.26f);
            lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
            // Point light 1
            lightingShader.setVec3("pointLights[0].position", pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
            lightingShader.setVec3("pointLights[0].ambient", pointLightColors[0].x * 0.1, pointLightColors[0].y * 0.1, pointLightColors[0].z * 0.1);
            lightingShader.setVec3("pointLights[0].diffuse", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
            lightingShader.setVec3("pointLights[0].specular", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
            lightingShader.setFloat("pointLights[0].constant", 1.0f);
            lightingShader.setFloat("pointLights[0].linear", 0.09);
            lightingShader.setFloat("pointLights[0].quadratic", 0.032);
            // Point light 2
            lightingShader.setVec3("pointLights[1].position", pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
            lightingShader.setVec3("pointLights[1].ambient", pointLightColors[1].x * 0.1, pointLightColors[1].y * 0.1, pointLightColors[1].z * 0.1);
            lightingShader.setVec3("pointLights[1].diffuse", pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
            lightingShader.setVec3("pointLights[1].specular", pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
            lightingShader.setFloat("pointLights[1].constant", 1.0f);
            lightingShader.setFloat("pointLights[1].linear", 0.09);
            lightingShader.setFloat("pointLights[1].quadratic", 0.032);
            // Point light 3
            lightingShader.setVec3("pointLights[2].position", pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
            lightingShader.setVec3("pointLights[2].ambient", pointLightColors[2].x * 0.1, pointLightColors[2].y * 0.1, pointLightColors[2].z * 0.1);
            lightingShader.setVec3("pointLights[2].diffuse", pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
            lightingShader.setVec3("pointLights[2].specular", pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
            lightingShader.setFloat("pointLights[2].constant", 1.0f);
            lightingShader.setFloat("pointLights[2].linear", 0.09);
            lightingShader.setFloat("pointLights[2].quadratic", 0.032);
            // Point light 4
            lightingShader.setVec3("pointLights[3].position", pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
            lightingShader.setVec3("pointLights[3].ambient", pointLightColors[3].x * 0.1, pointLightColors[3].y * 0.1, pointLightColors[3].z * 0.1);
            lightingShader.setVec3("pointLights[3].diffuse", pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
            lightingShader.setVec3("pointLights[3].specular", pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
            lightingShader.setFloat("pointLights[3].constant", 1.0f);
            lightingShader.setFloat("pointLights[3].linear", 0.09);
            lightingShader.setFloat("pointLights[3].quadratic", 0.032);
            // SpotLight
            lightingShader.setVec3("spotLight.position", camera.Position.x, camera.Position.y, camera.Position.z);
            lightingShader.setVec3("spotLight.direction", camera.Front.x, camera.Front.y, camera.Front.z);
            lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
            lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
            lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
            lightingShader.setFloat("spotLight.constant", 1.0f);
            lightingShader.setFloat("spotLight.linear", 0.09);
            lightingShader.setFloat("spotLight.quadratic", 0.032);
            lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(10.0f)));
            lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(12.5f)));
        }
        break;
        case HORROR: {
            glm::vec3 pointLightColors[] = {
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.1f, 0.1f, 0.1f),
                glm::vec3(0.3f, 0.1f, 0.1f),
            };
            // Directional light
            lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
            lightingShader.setVec3("dirLight.ambient", 0.3f, 0.24f, 0.14f);
            lightingShader.setVec3("dirLight.diffuse", 0.7f, 0.42f, 0.26f);
            lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
            // Point light 1
            lightingShader.setVec3("pointLights[0].position", pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
            lightingShader.setVec3("pointLights[0].ambient", pointLightColors[0].x * 0.1, pointLightColors[0].y * 0.1, pointLightColors[0].z * 0.1);
            lightingShader.setVec3("pointLights[0].diffuse", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
            lightingShader.setVec3("pointLights[0].specular", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
            lightingShader.setFloat("pointLights[0].constant", 1.0f);
            lightingShader.setFloat("pointLights[0].linear", 0.09);
            lightingShader.setFloat("pointLights[0].quadratic", 0.032);
            // Point light 2
            lightingShader.setVec3("pointLights[1].position", pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
            lightingShader.setVec3("pointLights[1].ambient", pointLightColors[1].x * 0.1, pointLightColors[1].y * 0.1, pointLightColors[1].z * 0.1);
            lightingShader.setVec3("pointLights[1].diffuse", pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
            lightingShader.setVec3("pointLights[1].specular", pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
            lightingShader.setFloat("pointLights[1].constant", 1.0f);
            lightingShader.setFloat("pointLights[1].linear", 0.09);
            lightingShader.setFloat("pointLights[1].quadratic", 0.032);
            // Point light 3
            lightingShader.setVec3("pointLights[2].position", pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
            lightingShader.setVec3("pointLights[2].ambient", pointLightColors[2].x * 0.1, pointLightColors[2].y * 0.1, pointLightColors[2].z * 0.1);
            lightingShader.setVec3("pointLights[2].diffuse", pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
            lightingShader.setVec3("pointLights[2].specular", pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
            lightingShader.setFloat("pointLights[2].constant", 1.0f);
            lightingShader.setFloat("pointLights[2].linear", 0.09);
            lightingShader.setFloat("pointLights[2].quadratic", 0.032);
            // Point light 4
            lightingShader.setVec3("pointLights[3].position", pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
            lightingShader.setVec3("pointLights[3].ambient", pointLightColors[3].x * 0.1, pointLightColors[3].y * 0.1, pointLightColors[3].z * 0.1);
            lightingShader.setVec3("pointLights[3].diffuse", pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
            lightingShader.setVec3("pointLights[3].specular", pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
            lightingShader.setFloat("pointLights[3].constant", 1.0f);
            lightingShader.setFloat("pointLights[3].linear", 0.09);
            lightingShader.setFloat("pointLights[3].quadratic", 0.032);
            // SpotLight
            lightingShader.setVec3("spotLight.position", camera.Position.x, camera.Position.y, camera.Position.z);
            lightingShader.setVec3("spotLight.direction", camera.Front.x, camera.Front.y, camera.Front.z);
            lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
            lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
            lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
            lightingShader.setFloat("spotLight.constant", 1.0f);
            lightingShader.setFloat("spotLight.linear", 0.09);
            lightingShader.setFloat("spotLight.quadratic", 0.032);
            lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(10.0f)));
            lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
        }
        break;
        case BIOCHEMICAL: {
            glm::vec3 pointLightColors[] = {
                glm::vec3(0.4f, 0.7f, 0.1f),
                glm::vec3(0.4f, 0.7f, 0.1f),
                glm::vec3(0.4f, 0.7f, 0.1f),
                glm::vec3(0.4f, 0.7f, 0.1f),
            };
            // Directional light
            lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
            lightingShader.setVec3("dirLight.ambient", 0.3f, 0.24f, 0.14f);
            lightingShader.setVec3("dirLight.diffuse", 0.7f, 0.42f, 0.26f);
            lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
            // Point light 1
            lightingShader.setVec3("pointLights[0].position", pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
            lightingShader.setVec3("pointLights[0].ambient", pointLightColors[0].x * 0.1, pointLightColors[0].y * 0.1, pointLightColors[0].z * 0.1);
            lightingShader.setVec3("pointLights[0].diffuse", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
            lightingShader.setVec3("pointLights[0].specular", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
            lightingShader.setFloat("pointLights[0].constant", 1.0f);
            lightingShader.setFloat("pointLights[0].linear", 0.09);
            lightingShader.setFloat("pointLights[0].quadratic", 0.032);
            // Point light 2
            lightingShader.setVec3("pointLights[1].position", pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
            lightingShader.setVec3("pointLights[1].ambient", pointLightColors[1].x * 0.1, pointLightColors[1].y * 0.1, pointLightColors[1].z * 0.1);
            lightingShader.setVec3("pointLights[1].diffuse", pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
            lightingShader.setVec3("pointLights[1].specular", pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
            lightingShader.setFloat("pointLights[1].constant", 1.0f);
            lightingShader.setFloat("pointLights[1].linear", 0.09);
            lightingShader.setFloat("pointLights[1].quadratic", 0.032);
            // Point light 3
            lightingShader.setVec3("pointLights[2].position", pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
            lightingShader.setVec3("pointLights[2].ambient", pointLightColors[2].x * 0.1, pointLightColors[2].y * 0.1, pointLightColors[2].z * 0.1);
            lightingShader.setVec3("pointLights[2].diffuse", pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
            lightingShader.setVec3("pointLights[2].specular", pointLightColors[2].x, pointLightColors[2].y, pointLightColors[2].z);
            lightingShader.setFloat("pointLights[2].constant", 1.0f);
            lightingShader.setFloat("pointLights[2].linear", 0.09);
            lightingShader.setFloat("pointLights[2].quadratic", 0.032);
            // Point light 4
            lightingShader.setVec3("pointLights[3].position", pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
            lightingShader.setVec3("pointLights[3].ambient", pointLightColors[3].x * 0.1, pointLightColors[3].y * 0.1, pointLightColors[3].z * 0.1);
            lightingShader.setVec3("pointLights[3].diffuse", pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
            lightingShader.setVec3("pointLights[3].specular", pointLightColors[3].x, pointLightColors[3].y, pointLightColors[3].z);
            lightingShader.setFloat("pointLights[3].constant", 1.0f);
            lightingShader.setFloat("pointLights[3].linear", 0.09);
            lightingShader.setFloat("pointLights[3].quadratic", 0.032);
            // SpotLight
            lightingShader.setVec3("spotLight.position", camera.Position.x, camera.Position.y, camera.Position.z);
            lightingShader.setVec3("spotLight.direction", camera.Front.x, camera.Front.y, camera.Front.z);
            lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
            lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
            lightingShader.setVec3("spotLight.specular", 0.0f, 1.0f, 0.0f);
            lightingShader.setFloat("spotLight.constant", 1.0f);
            lightingShader.setFloat("spotLight.linear", 0.07);
            lightingShader.setFloat("spotLight.quadratic", 0.017);
            lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(7.0f)));
            lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(10.0f)));
        }
        break;
        default:
            break;
        }

        // 投影
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // 视图
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("view", view);

        // 绘制图形
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // 绑定贴图
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        glBindVertexArray(cubeVAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model           = glm::translate(model, cubePositions[i]);
            float angle     = 20.0f * i;
            model           = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // 光源
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);

        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            lightCubeShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // 绘制
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // 检查并调用事件，交换缓冲
        glfwSwapBuffers(window);
    }

    // 释放资源
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

int main()
{
    primary();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    float cameraSpeed = static_cast<float>(2.5 * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (ImGui::GetIO().WantCaptureMouse || !isMouseLeft)
        return;

    if (firstMouse)
    {
        lastX      = xpos;
        lastY      = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX         = xpos;
    lastY         = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            isMouseLeft = true;

            // 同步鼠标初始位置
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            lastX = xpos;
            lastY = ypos;
        }
        else if (action == GLFW_RELEASE)
        {
            isMouseLeft = false;
        }
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (ImGui::GetIO().WantCaptureMouse)
        return;

    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int            width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}