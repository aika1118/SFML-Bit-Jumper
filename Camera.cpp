#include "Camera.h"
#include "Game.h" // 순환참조 회피

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

	// 뷰 중심점이 맵 경계를 넘지 않도록 클램핑
	FloatRect mapBound = Game::getInstance().getMapBound();
	if (mapBound.width <= 0.f || mapBound.height <= 0.f) // 아직 현재 mapBound가 초기화되지 않은 경우
		return View(_position, _viewSize);

	// 나중에 clamping 할 때 최소값 범위가 최대값 범위를 넘지 않도록 수정해야할 듯 함
	float clampX = clamp(_position.x, mapBound.left + _viewSize.x / 2, mapBound.left + mapBound.width - _viewSize.x / 2);
	float clampY = clamp(_position.y, mapBound.top + _viewSize.y / 2, mapBound.top + mapBound.height - _viewSize.y / 2);

	return View(Vector2f(clampX, clampY), _viewSize); // _position을 중심으로 _viewSize 만큼 보여주도록 함
	//return View(_position, _viewSize);
}

View Camera::getUIView()
{
	float aspect = _viewSize.x / _viewSize.y;
	_viewSize = Vector2f(UI_VIEW_SIZE, UI_VIEW_SIZE / aspect); // 창 가로가 줄어들면 x축은 그에 맞게 똑같이 같이 줄어들음, 그렇지만 y축은 aspect에 따라 조정되어 글자 어색하게 보이지 않게 함
	return View(Vector2f(0.f, 0.f), _viewSize); // (0, 0)을 중심으로 _viewSize 만큼 보여주는 view
	// 기존 player view와 관계없이 새로운 레이어 생성되어 center = 0,0 기준으로 view를 보여준다고 생각해보자
}
