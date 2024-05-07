#include "light.h"

Light::Light(glm::vec3 _intensity, glm::vec3 _position) : material(LIGHT)
{
    // �Ȱ�ǿ�Ⱥ�λ��
    intensity = _intensity;
    position = _position;
    // ���mesh��material��Ĭ�ϴ���
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
