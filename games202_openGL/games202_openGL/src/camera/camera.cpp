#include "camera.h"
#include <iostream>

using namespace std;

float speed = 0.025;

Camera::Camera()
{
    // 默认构造
}

Camera::Camera(const glm::vec3& _position, const glm::vec3& _target, const glm::vec3& _lookup)
{
    position = _position;
    target = _target;
    lookUp = _lookup;

    dir = glm::normalize(_position - _target);
    front = glm::normalize(_target - _position);
    right = glm::normalize(glm::cross(_lookup, dir));
    up = glm::cross(dir, right);

    moveSpeed = 8.f;
    mouseSensitivity = 0.1f;
    yaw = -90.f;
    pitch = 0.f;

}

void Camera::getViewMatrix(glm::mat4& view)
{
    view = glm::lookAt(position, position + front, lookUp);
}

void Camera::translateToTarget(float dis)
{
    position = position + dis * front;
}

void Camera::translatetToUp(float dis)
{
    position = position + dis * up;
}

void Camera::processKeyboard(Camera_Movement movement, float deltaTime)
{
    float velocity = moveSpeed * deltaTime;
    if (movement == FORWARD)
        position += front * velocity;
    if (movement == BACKWARD)
        position -= front * velocity;
    if (movement == LEFT)
        position -= right * velocity;
    if (movement == RIGHT)
        position += right * velocity;
}

void Camera::log()
{
    cout << "position:" << position.x << " " << position.y << " " << position.z << endl;
    cout << "front:" << front.x << " " << front.y << " " << front.z << endl;
    // cout << "yaw:" << yaw << endl;
    // cout << "pitch:" << pitch << endl;
}

void Camera::updateByPitch(float _pitch)
{
    glm::mat4 _rotate(1.0f);
    _rotate = glm::rotate(_rotate, _pitch, right);
    glm::vec4 _result_front(front, 1.f);
    glm::vec4 _result_up(up, 1.0f);

    _result_front = _rotate * _result_front;
    _result_up = _rotate * _result_up;
    // 完成更新
    front = glm::vec3(_result_front.x, _result_front.y, _result_front.z);
    up = glm::vec3(_result_up.x, _result_up.y, _result_up.z);
    dir = -front;

}

void Camera::updateByPitch()
{
    glm::mat4 _rotate(1.0f);
    _rotate = glm::rotate(_rotate, speed, right);
    glm::vec4 _result_front(front, 1.f);
    glm::vec4 _result_up(up, 1.0f);

    _result_front = _rotate * _result_front;
    _result_up = _rotate * _result_up;
    // 完成更新
    front = glm::vec3(_result_front.x, _result_front.y, _result_front.z);
    up = glm::vec3(_result_up.x, _result_up.y, _result_up.z);
    dir = -front;
}

void Camera::updateByPitchNeg()
{
    glm::mat4 _rotate(1.0f);
    _rotate = glm::rotate(_rotate, -speed, right);
    glm::vec4 _result_front(front, 1.f);
    glm::vec4 _result_up(up, 1.0f);

    _result_front = _rotate * _result_front;
    _result_up = _rotate * _result_up;
    // 完成更新
    front = glm::vec3(_result_front.x, _result_front.y, _result_front.z);
    up = glm::vec3(_result_up.x, _result_up.y, _result_up.z);
    dir = -front;
}

void Camera::updateByYaw(float _yaw)
{
    glm::mat4 _rotate(1.0f);
    _rotate = glm::rotate(_rotate, _yaw, up);
    glm::vec4 _result_front(front, 1.f);
    glm::vec4 _result_right(right, 1.0f);

    _result_front = _rotate * _result_front;
    _result_right = _rotate * _result_right;
    // 完成更新
    front = glm::vec3(_result_front.x, _result_front.y, _result_front.z);
    right = glm::vec3(_result_right.x, _result_right.y, _result_right.z);
    dir = -front;
}

void Camera::updateByYaw()
{
    glm::mat4 _rotate(1.0f);
    _rotate = glm::rotate(_rotate, speed, up);
    glm::vec4 _result_front(front, 1.f);
    glm::vec4 _result_right(right, 1.0f);

    _result_front = _rotate * _result_front;
    _result_right = _rotate * _result_right;
    // 完成更新
    front = glm::vec3(_result_front.x, _result_front.y, _result_front.z);
    right = glm::vec3(_result_right.x, _result_right.y, _result_right.z);
    dir = -front;
}

void Camera::updateByYawNeg()
{
    glm::mat4 _rotate(1.0f);
    _rotate = glm::rotate(_rotate, -speed, up);
    glm::vec4 _result_front(front, 1.f);
    glm::vec4 _result_right(right, 1.0f);

    _result_front = _rotate * _result_front;
    _result_right = _rotate * _result_right;
    // 完成更新
    front = glm::vec3(_result_front.x, _result_front.y, _result_front.z);
    right = glm::vec3(_result_right.x, _result_right.y, _result_right.z);
    dir = -front;
}
