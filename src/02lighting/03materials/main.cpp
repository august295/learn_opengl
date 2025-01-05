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

// 设置
const unsigned int SCR_WIDTH  = 800; // 屏幕宽度
const unsigned int SCR_HEIGHT = 600; // 屏幕高度

// 时间
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

// 摄像机
Camera camera(glm::vec3(-2.5f, -1.0f, 5.0f)); //
bool   firstMouse  = true;                    //
float  lastX       = SCR_WIDTH / 2.0f;        //
float  lastY       = SCR_HEIGHT / 2.0f;       //
bool   isMouseLeft = false;                   // 标志变量，表示是否启用鼠标控制

// 光源
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// clang-format off
float vertices[] = {
    // ---- 位置 ----   
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};
// clang-format on

enum MaterialsType
{
    DEFAULT,        // 默认
    EMERALD,        // 绿宝石
    JADE,           // 玉
    OBSIDIAN,       // 黑曜石
    PEARL,          // 珍珠
    RUBY,           // 红宝石
    TURQUOISE,      // 绿松石
    BRASS,          // 黄铜
    BRONZE,         // 青铜
    CHROME,         // 铬
    COPPER,         // 铜
    GOLD,           // 金
    SILVER,         // 银
    BLACK_PLASTIC,  // 黑色塑料
    CYAN_PLASTIC,   // 青色塑料
    GREEN_PLASTIC,  // 绿色塑料
    RED_PLASTIC,    // 红色塑料
    WHITE_PLASTIC,  // 白色塑料
    YELLOW_PLASTIC, // 黄色塑料
    BLACK_RUBBER,   // 黑色橡胶
    CYAN_RUBBER,    // 青色橡胶
    GREEN_RUBBER,   // 绿色橡胶
    RED_RUBBER,     // 红色橡胶
    WHITE_RUBBER,   // 白色橡胶
    YELLOW_RUBBER,  // 黄色橡胶
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
    Shader lightingShader("02lighting/3.1.materials.vs", "02lighting/3.1.materials.fs");
    Shader lightCubeShader("02lighting/2.1.light_cube.vs", "02lighting/2.1.light_cube.fs");

    // 物体
    unsigned int VBO, cubeVAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 位置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 单位向量
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 光源
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    // 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 允许深度测试
    glEnable(GL_DEPTH_TEST);

    // imgui 变量
    int imgui_combo_materials = 0;
    // 材质
    glm::vec3 material_ambient(1.0f, 0.5f, 0.31f);
    glm::vec3 material_diffuse(1.0f, 0.5f, 0.31f);
    glm::vec3 material_specular(0.5f, 0.5f, 0.5f);
    float     material_shininess = 32.0 / 128.0;
    // 光源
    glm::vec3 lightColor           = glm::vec3(1.0f);
    glm::vec3 diffuseColor         = lightColor * glm::vec3(0.5f);
    glm::vec3 ambientColor         = diffuseColor * glm::vec3(0.2f);
    float     lightColorArray[3]   = {1.0f, 1.0f, 1.0f};
    float     diffuseColorArray[3] = {0.5f, 0.5f, 0.5f};
    float     ambientColorArray[3] = {0.2f, 0.2f, 0.2f};

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
            ImGui::Begin("Materials");

            // 不同材质
            const char* items[] = {
                "默认",
                "绿宝石",
                "玉",
                "黑曜石",
                "珍珠",
                "红宝石",
                "绿松石",
                "黄铜",
                "青铜",
                "铬",
                "铜",
                "金",
                "银",
                "黑色塑料",
                "青色塑料",
                "绿色塑料",
                "红色塑料",
                "白色塑料",
                "黄色塑料",
                "黑色橡胶",
                "青色橡胶",
                "绿色橡胶",
                "红色橡胶",
                "白色橡胶",
                "黄色橡胶",
            };
            if (ImGui::Combo("材质类型", &imgui_combo_materials, items, IM_ARRAYSIZE(items)))
            {
                switch (imgui_combo_materials)
                {
                case DEFAULT:
                    material_ambient   = glm::vec3(0.0215, 0.1745, 0.0215);
                    material_diffuse   = glm::vec3(1.0f, 0.5f, 0.31f);
                    material_specular  = glm::vec3(0.5f, 0.5f, 0.5f);
                    material_shininess = 32.0f;
                    break;
                case EMERALD:
                    material_ambient   = glm::vec3(0.0215, 0.1745, 0.0215);
                    material_diffuse   = glm::vec3(0.07568, 0.61424, 0.07568);
                    material_specular  = glm::vec3(0.633, 0.727811, 0.633);
                    material_shininess = 0.6;
                    break;
                case JADE:
                    material_ambient   = glm::vec3(0.135, 0.2225, 0.1575);
                    material_diffuse   = glm::vec3(0.54, 0.89, 0.63);
                    material_specular  = glm::vec3(0.316228, 0.316228, 0.316228);
                    material_shininess = 0.1;
                    break;
                case OBSIDIAN:
                    material_ambient   = glm::vec3(0.05375, 0.05, 0.06625);
                    material_diffuse   = glm::vec3(0.18275, 0.17, 0.22525);
                    material_specular  = glm::vec3(0.332741, 0.328634, 0.346435);
                    material_shininess = 0.3;
                    break;
                case PEARL:
                    material_ambient   = glm::vec3(0.25, 0.20725, 0.20725);
                    material_diffuse   = glm::vec3(1.0, 0.829, 0.829);
                    material_specular  = glm::vec3(0.296648, 0.296648, 0.296648);
                    material_shininess = 0.088;
                    break;
                case RUBY:
                    material_ambient   = glm::vec3(0.1745, 0.01175, 0.01175);
                    material_diffuse   = glm::vec3(0.61424, 0.04136, 0.04136);
                    material_specular  = glm::vec3(0.727811, 0.626959, 0.626959);
                    material_shininess = 0.6;
                    break;
                case TURQUOISE:
                    material_ambient   = glm::vec3(0.1, 0.18725, 0.1745);
                    material_diffuse   = glm::vec3(0.396, 0.74151, 0.69102);
                    material_specular  = glm::vec3(0.297254, 0.30829, 0.306678);
                    material_shininess = 0.1;
                    break;
                case BRASS:
                    material_ambient   = glm::vec3(0.329412, 0.223529, 0.027451);
                    material_diffuse   = glm::vec3(0.780392, 0.568627, 0.113725);
                    material_specular  = glm::vec3(0.992157, 0.941176, 0.807843);
                    material_shininess = 0.21794872;
                    break;
                case BRONZE:
                    material_ambient   = glm::vec3(0.2125, 0.1275, 0.054);
                    material_diffuse   = glm::vec3(0.714, 0.4284, 0.18144);
                    material_specular  = glm::vec3(0.393548, 0.271906, 0.166721);
                    material_shininess = 0.2;
                    break;
                case CHROME:
                    material_ambient   = glm::vec3(0.25, 0.25, 0.25);
                    material_diffuse   = glm::vec3(0.4, 0.4, 0.4);
                    material_specular  = glm::vec3(0.774597, 0.774597, 0.774597);
                    material_shininess = 0.4;
                    break;
                case COPPER:
                    material_ambient   = glm::vec3(0.19125, 0.0735, 0.0225);
                    material_diffuse   = glm::vec3(0.7038, 0.27048, 0.0828);
                    material_specular  = glm::vec3(0.256777, 0.137622, 0.086014);
                    material_shininess = 0.1;
                    break;
                case GOLD:
                    material_ambient   = glm::vec3(0.24725, 0.1995, 0.0745);
                    material_diffuse   = glm::vec3(0.75164, 0.60648, 0.22648);
                    material_specular  = glm::vec3(0.628281, 0.555802, 0.366065);
                    material_shininess = 0.4;
                    break;
                case SILVER:
                    material_ambient   = glm::vec3(0.19225, 0.19225, 0.19225);
                    material_diffuse   = glm::vec3(0.50754, 0.50754, 0.50754);
                    material_specular  = glm::vec3(0.508273, 0.508273, 0.508273);
                    material_shininess = 0.4;
                    break;
                case BLACK_PLASTIC:
                    material_ambient   = glm::vec3(0.0, 0.0, 0.0);
                    material_diffuse   = glm::vec3(0.01, 0.01, 0.01);
                    material_specular  = glm::vec3(0.50, 0.50, 0.50);
                    material_shininess = 0.25;
                    break;
                case CYAN_PLASTIC:
                    material_ambient   = glm::vec3(0.0, 0.1, 0.06);
                    material_diffuse   = glm::vec3(0.0, 0.50980392, 0.50980392);
                    material_specular  = glm::vec3(0.50196078, 0.50196078, 0.50196078);
                    material_shininess = 0.25;
                    break;
                case GREEN_PLASTIC:
                    material_ambient   = glm::vec3(0.0, 0.0, 0.0);
                    material_diffuse   = glm::vec3(0.1, 0.35, 0.1);
                    material_specular  = glm::vec3(0.45, 0.55, 0.45);
                    material_shininess = 0.25;
                    break;
                case RED_PLASTIC:
                    material_ambient   = glm::vec3(0.0, 0.0, 0.0);
                    material_diffuse   = glm::vec3(0.5, 0.0, 0.0);
                    material_specular  = glm::vec3(0.7, 0.6, 0.6);
                    material_shininess = 0.25;
                    break;
                case WHITE_PLASTIC:
                    material_ambient   = glm::vec3(0.0, 0.0, 0.0);
                    material_diffuse   = glm::vec3(0.55, 0.55, 0.55);
                    material_specular  = glm::vec3(0.70, 0.70, 0.70);
                    material_shininess = 0.25;
                    break;
                case YELLOW_PLASTIC:
                    material_ambient   = glm::vec3(0.0, 0.0, 0.0);
                    material_diffuse   = glm::vec3(0.5, 0.5, 0.0);
                    material_specular  = glm::vec3(0.60, 0.60, 0.50);
                    material_shininess = 0.25;
                    break;
                case BLACK_RUBBER:
                    material_ambient   = glm::vec3(0.02, 0.02, 0.02);
                    material_diffuse   = glm::vec3(0.01, 0.01, 0.01);
                    material_specular  = glm::vec3(0.4, 0.4, 0.4);
                    material_shininess = 0.078125;
                    break;
                case CYAN_RUBBER:
                    material_ambient   = glm::vec3(0.0, 0.05, 0.05);
                    material_diffuse   = glm::vec3(0.4, 0.5, 0.5);
                    material_specular  = glm::vec3(0.04, 0.7, 0.7);
                    material_shininess = 0.078125;
                    break;
                case GREEN_RUBBER:
                    material_ambient   = glm::vec3(0.0, 0.05, 0.0);
                    material_diffuse   = glm::vec3(0.4, 0.5, 0.4);
                    material_specular  = glm::vec3(0.04, 0.7, 0.04);
                    material_shininess = 0.078125;
                    break;
                case RED_RUBBER:
                    material_ambient   = glm::vec3(0.05, 0.0, 0.0);
                    material_diffuse   = glm::vec3(0.5, 0.4, 0.4);
                    material_specular  = glm::vec3(0.7, 0.04, 0.04);
                    material_shininess = 0.078125;
                    break;
                case WHITE_RUBBER:
                    material_ambient   = glm::vec3(0.05, 0.05, 0.05);
                    material_diffuse   = glm::vec3(0.5, 0.5, 0.5);
                    material_specular  = glm::vec3(0.7, 0.7, 0.7);
                    material_shininess = 0.078125;
                    break;
                case YELLOW_RUBBER:
                    material_ambient   = glm::vec3(0.05, 0.05, 0.0);
                    material_diffuse   = glm::vec3(0.5, 0.5, 0.4);
                    material_specular  = glm::vec3(0.7, 0.7, 0.04);
                    material_shininess = 0.078125;
                    break;
                default:
                    break;
                }
            }

            // 光照强度
            ImGui::SliderFloat("光照强度", &material_shininess, 0.0f, 1.0f);

            // 光源特性
            ImGui::InputFloat3("lightColor", lightColorArray);
            ImGui::InputFloat3("diffuseColor", diffuseColorArray);
            ImGui::InputFloat3("ambientColor", ambientColorArray);
            lightColor   = glm::vec3(lightColorArray[0], lightColorArray[1], lightColorArray[2]);
            diffuseColor = glm::vec3(diffuseColorArray[0], diffuseColorArray[1], diffuseColorArray[2]);
            ambientColor = glm::vec3(ambientColorArray[0], ambientColorArray[1], ambientColorArray[2]);

            ImGui::End();
        }

        // 计算时间差
        float currentFrame = glfwGetTime();
        deltaTime          = currentFrame - lastFrame;
        lastFrame          = currentFrame;

        // 输入
        processInput(window);

        // 渲染指令
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 在此之前不要忘记首先 use 对应的着色器程序（来设定uniform）
        lightingShader.use();
        lightingShader.setVec3("light.position", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);

        // 光特性
        lightingShader.setVec3("light.ambient", ambientColor);
        lightingShader.setVec3("light.diffuse", diffuseColor);
        lightingShader.setVec3("light.specular", lightColor);

        // 材料特性
        lightingShader.setVec3("material.ambient", material_ambient);
        lightingShader.setVec3("material.diffuse", material_diffuse);
        lightingShader.setVec3("material.specular", material_specular);
        lightingShader.setFloat("material.shininess", material_shininess * 128.0);

        // 投影
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // 视图
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("view", view);

        // 绘制图形
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 光源
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightCubeShader.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

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