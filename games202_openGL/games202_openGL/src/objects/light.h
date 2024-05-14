#pragma once

#include "mesh.h"
#include "materials/material.h"
#include "glframework/shader.h"

class Light
{
public:
    // 网格体积
    Mesh mesh;
    // 材质
    Material material;
    // 强度
    float intensity;

    glm::vec3 color;

    glm::vec3 position;
    // 以光源为摄像机角度，target
    glm::vec3 target;
    // up
    glm::vec3 up;

    Light(float _intensity, glm::vec3 _position, glm::vec3 _color); // 构造函数 传入强度和位置

    void setTargetAndUp(const glm::vec3& _target, const glm::vec3& _up);

    void draw(const Shader& pipeline); // 渲染启动

    float getIntensity(); // 获得成员接口
    glm::vec3 getPosition();

    void setPosition(const glm::vec3& _position); // 设置新位置

    glm::mat4 getMVP(); // 得到光源角度下的 MVP变换矩阵

};
