#include "Camera.h"

Camera::Camera()
	:_zoomLevel(CAMERA_ZOOM_LEVEL)
{
}

Camera& Camera::getInstance()
{
	static Camera instance; // 인스턴스는 최초 호출 시에만 생성됨
	return instance;
}

Vector2f Camera::getViewSize()
{
	return _viewSize;
}

View Camera::getView(Vector2u windowSize)
{
	// 종횡비에 따라 Rendering 물체의 원본 비율이 유지되게 보이는 작업 (view 사용)
	// 이 작업이 없으면 화면 크기 변경 시 원본 비율이 깨져 찌그러져 보이게 됨

	float aspect = (float)windowSize.x / (float)windowSize.y;
	if (aspect < 1.f) // 세로화면
		_viewSize = Vector2f(_zoomLevel, _zoomLevel / aspect);
	else // 가로화면
		_viewSize = Vector2f(_zoomLevel * aspect, _zoomLevel);

	return View(_position, _viewSize); // _position을 중심으로 _viewSize 만큼 보여주도록 함
}
