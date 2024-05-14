#pragma once

#include "mesh.h"
#include "materials/material.h"
#include "glframework/shader.h"

class Light
{
public:
    // �������
    Mesh mesh;
    // ����
    Material material;
    // ǿ��
    float intensity;

    glm::vec3 color;

    glm::vec3 position;
    // �Թ�ԴΪ������Ƕȣ�target
    glm::vec3 target;
    // up
    glm::vec3 up;

    Light(float _intensity, glm::vec3 _position, glm::vec3 _color); // ���캯�� ����ǿ�Ⱥ�λ��

    void setTargetAndUp(const glm::vec3& _target, const glm::vec3& _up);

    void draw(const Shader& pipeline); // ��Ⱦ����

    float getIntensity(); // ��ó�Ա�ӿ�
    glm::vec3 getPosition();

    void setPosition(const glm::vec3& _position); // ������λ��

    glm::mat4 getMVP(); // �õ���Դ�Ƕ��µ� MVP�任����

};
