#include "material.h"

Material::Material(const Type& _type)
{
    type = _type;
    ka = glm::vec3();
    kd = glm::vec3();
    ks = glm::vec3();
}

Material::Material(const Type& _type, const glm::vec3& _ka, const glm::vec3& _kd, const glm::vec3& _ks)
{
    type = _type;
    ka = _ka;
    kd = _kd;
    ks = _ks;
}

void Material::addTexture(const Texture& _texture)
{
    textures.push_back(_texture);
}

void Material::setTextures(const vector<Texture>& _textures)
{
    textures = _textures;
}

Type Material::getType()
{
    return type;
}

bool Material::getPhongPara(glm::vec3& _ka, glm::vec3& _kd, glm::vec3& _ks)
{
    if (ka == glm::vec3() && kd == glm::vec3() && ks == glm::vec3()) {
        return false; // û�и�����������ֵ����ȫ��Ϊ0�����������Ϣ
    }
    _ka = ka;
    _kd = kd;
    _ks = ks;
    return true;
}

int Material::getTexturesSize()
{
    return textures.size();
}

Texture Material::getTexture(const int& id)
{
    return textures[id];
}



bool Material::compare(Material& _material)
{
    bool _return = true;
    glm::vec3 _ka, _kd, _ks;
    _material.getPhongPara(_ka, _kd, _ks);
    if (ka != _ka || kd != _kd || ks != _ks) { // �Ƚ���������
        _return = false;
    }

    if (textures.size() != _material.getTexturesSize()) { // �Ƚϲ�����洢������ͼƬ����
        _return = false;
    }

    for (int i = 0; i < _material.getTexturesSize(); i++) {
        if (textures[i].id != _material.getTexture(i).id) { // �Ƚ�ÿһ���Ƿ�һ��
            _return = false;
        }
    }

    return _return;

}

