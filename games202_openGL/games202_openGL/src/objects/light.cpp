#include "light.h"
#include "application/Application.h"

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


void Light::setTargetAndUp(const glm::vec3& _target, const glm::vec3& _up)
{
    target = _target;
    up = _up;
}

glm::mat4 Light::getMVP() // m没有的
{
    glm::mat4 result = glm::mat4(1.0f);
    // M
    // result = glm::translate(result, position);
    // V
    glm::mat4 view = glm::lookAt(position, target, up);
    result = view * result;
    // P
    //glm::mat4 prospective = glm::perspective(glm::radians(75.f), float(SCR_WIDTH/SCR_HEIGHT), 1.f, 200.f);
    float ratio = 0.15;
    float k = myapp->getWidth() / myapp->getHeight();
    glm::mat4 prospective = glm::ortho(-770 * ratio * k, 770 * ratio * k, -ratio * (770), ratio * (770), 0.01f, 200.f);
    // glm::mat4 prospective = glm::ortho(-100.f, 100.f, -100.f, 100.f, 0.01f, 200.f);
    result = prospective * result;
    return result;
}
