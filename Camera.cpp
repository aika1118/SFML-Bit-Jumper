#include "Camera.h"
#include "Game.h" // ��ȯ���� ȸ��

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

	// �� �߽����� �� ��踦 ���� �ʵ��� Ŭ����
	FloatRect mapBound = Game::getInstance().getMapBound();
	if (mapBound.width <= 0.f || mapBound.height <= 0.f) // ���� ���� mapBound�� �ʱ�ȭ���� ���� ���
		return View(_position, _viewSize);

	// ���߿� clamping �� �� �ּҰ� ������ �ִ밪 ������ ���� �ʵ��� �����ؾ��� �� ��
	float clampX = clamp(_position.x, mapBound.left + _viewSize.x / 2, mapBound.left + mapBound.width - _viewSize.x / 2);
	float clampY = clamp(_position.y, mapBound.top + _viewSize.y / 2, mapBound.top + mapBound.height - _viewSize.y / 2);

	return View(Vector2f(clampX, clampY), _viewSize); // _position�� �߽����� _viewSize ��ŭ �����ֵ��� ��
	//return View(_position, _viewSize);
}

View Camera::getUIView()
{
	float aspect = _viewSize.x / _viewSize.y;
	_viewSize = Vector2f(UI_VIEW_SIZE, UI_VIEW_SIZE / aspect); // â ���ΰ� �پ��� x���� �׿� �°� �Ȱ��� ���� �پ����, �׷����� y���� aspect�� ���� �����Ǿ� ���� ����ϰ� ������ �ʰ� ��
	return View(Vector2f(0.f, 0.f), _viewSize); // (0, 0)�� �߽����� _viewSize ��ŭ �����ִ� view
	// ���� player view�� ������� ���ο� ���̾� �����Ǿ� center = 0,0 �������� view�� �����شٰ� �����غ���
}
