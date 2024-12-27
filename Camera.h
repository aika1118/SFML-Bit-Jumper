// File: Camera.h
// Purpose: View�� ����

#pragma once

#include <SFML/Graphics.hpp>
#include "Config.h"

using namespace sf;
using namespace std;


class Camera
{
public:
	static Camera& getInstance(); // �̱���

	Vector2f getViewSize();
	View getView(Vector2u windowSize); // ���� windowSize ��Ⱦ�� �´� view ��ȯ, �� �ٱ��� ����� �ʴ� view ��ȯ
	View getUIView(); // UI �������� ���� view ��ȯ
	float _zoomLevel = CAMERA_ZOOM_LEVEL; // view ũ��
	Vector2f _position; // view �߽�

private:
	Camera() = default; // �ܺο��� ������ ȣ�� �Ұ��ϵ��� ����, �⺻�����ڷ� ����
	Camera(const Camera&) = delete; // ��������� ����
	Camera& operator=(const Camera&) = delete; // ���Կ����� ����

	Vector2f _viewSize;
};