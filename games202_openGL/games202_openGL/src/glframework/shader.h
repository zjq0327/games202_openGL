#pragma once

#include "core.h"
#include <vector>
#include <iostream>

using namespace std;

class Shader
{
    public:
    // 上下文里的id
    unsigned int id;
    // 顶点属性名称数组
    vector<string> attributeName;
    // unifrom属性名称数组
    vector<string> uniformName;
    // 纹理属性名称数组
    vector<string> textureName;

    Shader(); // 根据GLSL文件 创建着色器 设置一个默认构造函数
    Shader(const string &path, int _type); 

    // 设置数组
    void setAttributeName(const vector<string> &_attributeName);
    void setUniformName(const vector<string> &_unifromName);
    void setTextureName(const vector<string> &_textureName);

    // 得到变量
    unsigned int getId();
    vector<string> getAttributeName();
    vector<string> getUniformName();
    vector<string> getTextureName();


};

