#include "Camera.h"

Camera::Camera()
	:_zoomLevel(CAMERA_ZOOM_LEVEL)
{
}

Camera& Camera::getInstance()
{
	static Camera instance; // �ν��Ͻ��� ���� ȣ�� �ÿ��� ������
	return instance;
}

Vector2f Camera::getViewSize()
{
	return _viewSize;
}

View Camera::getView(Vector2u windowSize)
{
	// ��Ⱦ�� ���� Rendering ��ü�� ���� ������ �����ǰ� ���̴� �۾� (view ���)
	// �� �۾��� ������ ȭ�� ũ�� ���� �� ���� ������ ���� ��׷��� ���̰� ��

	float aspect = (float)windowSize.x / (float)windowSize.y;
	if (aspect < 1.f) // ����ȭ��
		_viewSize = Vector2f(_zoomLevel, _zoomLevel / aspect);
	else // ����ȭ��
		_viewSize = Vector2f(_zoomLevel * aspect, _zoomLevel);

	return View(_position, _viewSize); // _position�� �߽����� _viewSize ��ŭ �����ֵ��� ��
}
