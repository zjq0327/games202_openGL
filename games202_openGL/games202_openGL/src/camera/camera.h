#pragma once

#include "glframework/core.h"


enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    STILLNESS
};

class Camera
{
public:
    // λ��
    glm::vec3 position;
    // Ŀ��
    glm::vec3 target;
    // ���Ϸ���
    glm::vec3 lookUp;

    // ��������
    // -z �������ʵ�ʷ����෴
    glm::vec3 dir;
    glm::vec3 front; // ��Z���� ����������ڴ���target��
    // x
    glm::vec3 right;
    // y
    glm::vec3 up;

    // ����������ƶ���Ҫ������
    // ŷ���� ���ҵļܹ����ò���
    float yaw;
    float pitch;
    // �ƶ�����
    float moveSpeed;
    float mouseSensitivity;

    Camera(); // Ĭ�Ϲ���
    Camera(const glm::vec3& _position, const glm::vec3& _target, const glm::vec3& _lookup); // ����

    void getViewMatrix(glm::mat4& view); // �õ�view����

    void translateToTarget(float dis); // ��Ŀ�귽���ƶ�

    void translatetToUp(float dis); // ���Ϸ����ƶ�

    void processKeyboard(Camera_Movement movement, float deltaTime); // ���������ӿ�

    void log(); // �����Ϣ

    void updateByPitch(float _pitch); // �������ת
    void updateByPitch();
    void updateByPitchNeg();

    void updateByYaw(float _yaw);
    void updateByYaw();
    void updateByYawNeg();

};

