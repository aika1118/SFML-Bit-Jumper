// File: Camera.h
// Purpose: View�� ����

#pragma once

#include <SFML/Graphics.hpp>
#include "Config.h"

using namespace sf;

class Camera
{
public:
	static Camera& getInstance(); // �̱���

	Vector2f getViewSize();
	View getView(Vector2u windowSize); // ���� windowSize ��Ⱦ�� �´� view ��ȯ
	
	float _zoomLevel; // view ũ��
	Vector2f _position; // view �߽�

private:
	Camera(); // �ܺο��� ������ ȣ�� �Ұ��ϵ��� ����
	Camera(const Camera&) = delete; // ��������� ����
	Camera& operator=(const Camera&) = delete; // ���Կ����� ����

	Vector2f _viewSize;
};