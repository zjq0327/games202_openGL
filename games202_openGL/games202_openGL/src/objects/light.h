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
    glm::vec3 intensity;
    // λ��
    glm::vec3 position;

    Light(glm::vec3 _intensity, glm::vec3 _position); // ���캯�� ����ǿ�Ⱥ�λ��

    void draw(const Shader& pipeline); // ��Ⱦ����

    glm::vec3 getIntensity(); // ��ó�Ա�ӿ�
    glm::vec3 getPosition();


};
