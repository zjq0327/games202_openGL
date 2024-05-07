#pragma once

#include "glframework/core.h"


// ��Ҫдcamera��λ��
class Camera {
public:
	Camera();
	~Camera();

	glm::mat4 getViewMatrix();
	virtual glm::mat4 getProjectionMatrix();
	virtual void scale(float deltaScale);

public:
	glm::vec3 mPosition{ 0.0f,0.0f,3.0f };
	glm::vec3 mUp{ 0.0f, 1.0f, 0.0f };
	glm::vec3 mRight{ 1.0f,0.0f,0.0f };
};