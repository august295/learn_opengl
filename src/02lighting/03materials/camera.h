#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// 定义摄像机移动的几种可能选项
enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// 摄像机默认参数
const float YAW         = -55.0f;
const float PITCH       = 15.0f;
const float SPEED       = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM        = 45.0f;

// 抽象摄像机类，用于处理输入并计算相应的欧拉角、矢量和矩阵，供 OpenGL 使用
class Camera
{
public:
    // 摄像机属性
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // 欧拉角
    float Yaw;
    float Pitch;
    // 摄像机选项
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // 向量构造函数
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    // 标量构造函数
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    // 返回使用欧拉角和视图矩阵计算出的视图矩阵
    glm::mat4 GetViewMatrix();

    // 处理从任何键盘输入系统接收到的输入。 接受摄像机定义的 ENUM 形式的输入参数（以便从窗口系统中抽象出来）
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    // 处理从鼠标输入系统接收到的输入。 预计在 x 和 y 方向上都会有偏移值。
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    // 处理从鼠标滚轮事件接收到的输入。只需要垂直滚轮轴上的输入
    void ProcessMouseScroll(float yoffset);

private:
    // 根据相机（更新的）欧拉角计算前向量（方向向量）
    void updateCameraVectors();
};

#endif
