#include "light.h"

Light::Light(glm::vec3 _intensity, glm::vec3 _position) : material(LIGHT)
{
    // 先绑定强度和位置
    intensity = _intensity;
    position = _position;
    // 完成mesh和material的默认创造
    mesh = Mesh();
}

glm::vec3 Light::getIntensity()
{
    return intensity;
}

glm::vec3 Light::getPosition()
{
    return position;
};
