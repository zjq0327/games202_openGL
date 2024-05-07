#include "cameraControl.h"
#include <iostream>

CameraControl::CameraControl() {

}

CameraControl::~CameraControl() {

}

void CameraControl::onMouse(int button, int action, double xpos, double ypos) {

	//1 �жϵ�ǰ�İ������Ƿ���
	bool pressed = action == GLFW_PRESS ? true : false;

	//2 ������£���¼��ǰ���µ�λ��
	if (pressed) {
		mCurrentX = xpos;
		mCurrentY = ypos;
	}

	//3 ���ݰ��µ���갴����ͬ�����İ���״̬
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		mLeftMouseDown = pressed;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		mRightMouseDown = pressed;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		mMiddleMouseDown = pressed;
		break;
	}
	std::cout << "onMouse" << std::endl;
}

void CameraControl::onCursor(double xpos, double ypos) {
}

void CameraControl::onKey(int key, int action, int mods) {

	//���˵�repeat�����
	if (action == GLFW_REPEAT) {
		return;
	}

	//1 ��ⰴ�»���̧�𣬸���һ������
	bool pressed = action == GLFW_PRESS ? true : false;

	//2 ��¼��keyMap
	mKeyMap[key] = pressed;
}

void CameraControl::update() {
}

void CameraControl::onScroll(float offset) {

}