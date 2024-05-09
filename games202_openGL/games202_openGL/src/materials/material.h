#pragma once
#include "glframework/core.h"
#include <vector>
#include <string>
using namespace std;

enum Type
{
    PHONG = 0,
    LIGHT = 1
};

struct Texture {
    unsigned int id;
    string type;
    string path;
};

class Material
{
public:
    // 类型
    Type type;
    // 环境光参数
    glm::vec3 ka;
    // 漫反射参数
    glm::vec3 kd;
    // 镜面反射参数
    glm::vec3 ks;
    // 纹理数组
    vector<Texture> textures;

    Material(const Type& _type); // 构造函数 默认

    Material(const Type& _type, const glm::vec3& _ka, const glm::vec3& _kd, const glm::vec3& _ks); // 赋值构造函数

    void addTexture(const Texture& _texture); // 传入该材质纹理
    void setTextures(const vector<Texture>& _textures); // 直接赋值

    bool isTexture();


    // 获取纹理参数接口
    Type getType();
    bool getPhongPara(glm::vec3& _ka, glm::vec3& _kd, glm::vec3& _ks); // 判断参数是否更新 默认构造函数的时候 我会全部赋值成0
    int getTexturesSize();
    Texture getTexture(const int& id);

    bool compare(Material& _material); // 比较与另外一个材质是否一样

};
