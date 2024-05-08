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
    // λ��
    glm::vec3 position;

    glm::vec3 color;

    Light(float _intensity, glm::vec3 _position, glm::vec3 _color); // ���캯�� ����ǿ�Ⱥ�λ��

    void draw(const Shader& pipeline); // ��Ⱦ����

    float getIntensity(); // ��ó�Ա�ӿ�
    glm::vec3 getPosition();

    void setPosition(const glm::vec3& _position); // ������λ��

};
