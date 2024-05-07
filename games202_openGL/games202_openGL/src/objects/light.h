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
    glm::vec3 intensity;
    // 位置
    glm::vec3 position;

    Light(glm::vec3 _intensity, glm::vec3 _position); // 构造函数 传入强度和位置

    void draw(const Shader& pipeline); // 渲染启动

    glm::vec3 getIntensity(); // 获得成员接口
    glm::vec3 getPosition();


};
