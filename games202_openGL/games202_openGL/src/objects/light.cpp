#include "light.h"

Light::Light(float _intensity, glm::vec3 _position, glm::vec3 _color) : material(LIGHT)
{
    // �Ȱ�ǿ�Ⱥ�λ��
    intensity = _intensity;
    position = _position;
    color = _color;
    // ���mesh��material��Ĭ�ϴ���
    mesh = Mesh();
}

float Light::getIntensity()
{
    return intensity;
}

glm::vec3 Light::getPosition()
{
    return position;
};

void Light::setPosition(const glm::vec3& _position)
{
    position = _position;
};
