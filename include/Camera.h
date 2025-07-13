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
	/**
	 * ī�޶� ��ü�� �ν��Ͻ��� ��ȯ�մϴ�.
	 *
	 * @return ī�޶� ��ü�� �ν��Ͻ��Դϴ�.
	 *
	 * @throws ����
	 */
	static Camera& getInstance(); // �̱���
	/**
	 * ���� ���� ũ�⸦ ��ȯ�մϴ�.
	 *
	 * @return ���� ���� ũ���Դϴ�.
	 *
	 * @throws ����
	 */
	Vector2f getViewSize();
	/**
	 * ���� â ũ�⿡ �´� �並 ��ȯ�մϴ�.
	 *
	 * @param windowSize â�� ũ���Դϴ�.
	 *
	 * @return â ũ�⿡ �´� ���Դϴ�.
	 *
	 * @throws ����
	 */
	View getView(Vector2u windowSize);
	/**
	 * UI �������� ���� �並 ��ȯ�մϴ�.
	 *
	 * @return UI �������� ���� ���Դϴ�.
	 *
	 * @throws ����
	 */
	View getUIView();
	float _zoomLevel = CAMERA_ZOOM_LEVEL; // view ũ��
	Vector2f _position; // view �߽�

private:
	Camera() = default; // �ܺο��� ������ ȣ�� �Ұ��ϵ��� ����, �⺻�����ڷ� ����
	Camera(const Camera&) = delete; // ��������� ����
	Camera& operator=(const Camera&) = delete; // ���Կ����� ����

	Vector2f _viewSize;
};