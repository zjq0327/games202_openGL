#include "gameCameraControl.h"
#include <iostream>

GameCameraControl::GameCameraControl() {

}

GameCameraControl::~GameCameraControl() {

}

void GameCameraControl::onCursor(double xpos, double ypos) {
	float deltaX = (xpos - mCurrentX) * mSensitivity;
	float deltaY = (ypos - mCurrentY) * mSensitivity;

	if (mLeftMouseDown) {
		//pitch(-deltaY);
		//yaw(-deltaX);
		mCamera->updateByPitch(-0.01* deltaY);
		mCamera->updateByYaw(-0.01 * deltaX);

	}


	mCurrentX = xpos;
	mCurrentY = ypos;

}

void GameCameraControl::pitch(float angle) {
	mPitch += angle;
	if (mPitch > 89.0f || mPitch < -89.0f) {
		mPitch -= angle;
		return;
	}

	//��gameCameraControl������£�pitch����Ӱ��mPosition
	auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), mCamera->right);
	mCamera->up = mat * glm::vec4(mCamera->up, 0.0f);
}

void GameCameraControl::yaw(float angle) {
	auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	mCamera->up = mat * glm::vec4(mCamera->up, 0.0f);
	mCamera->right = mat * glm::vec4(mCamera->right, 0.0f);
}


void GameCameraControl::update() {
	//�����ƶ�����
	glm::vec3 direction(0.0f);

	auto front = glm::cross(mCamera->up, mCamera->right);
	auto right = mCamera->right;



	if (mKeyMap[GLFW_KEY_W]) {
		direction += front;
	}

	if (mKeyMap[GLFW_KEY_S]) {
		direction -= front;
	}

	if (mKeyMap[GLFW_KEY_A]) {
		direction -= right;
	}

	if (mKeyMap[GLFW_KEY_D]) {
		direction += right;
	}

	//��ʱdirection�п��ܲ�Ϊ1�ĳ��ȣ�Ҳ�п�����0�ĳ���
	if (glm::length(direction) != 0) {
		direction = glm::normalize(direction);
		mCamera->position += direction * mSpeed;
	}
}