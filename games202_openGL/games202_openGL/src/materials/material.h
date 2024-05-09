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
    // ����
    Type type;
    // ���������
    glm::vec3 ka;
    // ���������
    glm::vec3 kd;
    // ���淴�����
    glm::vec3 ks;
    // ��������
    vector<Texture> textures;

    Material(const Type& _type); // ���캯�� Ĭ��

    Material(const Type& _type, const glm::vec3& _ka, const glm::vec3& _kd, const glm::vec3& _ks); // ��ֵ���캯��

    void addTexture(const Texture& _texture); // ����ò�������
    void setTextures(const vector<Texture>& _textures); // ֱ�Ӹ�ֵ

    bool isTexture();


    // ��ȡ��������ӿ�
    Type getType();
    bool getPhongPara(glm::vec3& _ka, glm::vec3& _kd, glm::vec3& _ks); // �жϲ����Ƿ���� Ĭ�Ϲ��캯����ʱ�� �һ�ȫ����ֵ��0
    int getTexturesSize();
    Texture getTexture(const int& id);

    bool compare(Material& _material); // �Ƚ�������һ�������Ƿ�һ��

};
