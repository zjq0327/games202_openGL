#include "light.h"

Light::Light(float _intensity, glm::vec3 _position, glm::vec3 _color) : material(LIGHT)
{
    // 先绑定强度和位置
    intensity = _intensity;
    position = _position;
    color = _color;
    // 完成mesh和material的默认创造
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
