// File: Camera.h
// Purpose: View를 관리

#pragma once

#include <SFML/Graphics.hpp>
#include "Config.h"

using namespace sf;
using namespace std;


class Camera
{
public:
	static Camera& getInstance(); // 싱글톤

	Vector2f getViewSize();
	View getView(Vector2u windowSize); // 현재 windowSize 종횡비에 맞는 view 반환, 맵 바깥을 벗어나지 않는 view 반환
	View getUIView(); // UI 렌더링을 위한 view 반환
	float _zoomLevel = CAMERA_ZOOM_LEVEL; // view 크기
	Vector2f _position; // view 중심

private:
	Camera() = default; // 외부에서 생성자 호출 불가하도록 설정, 기본생성자로 지정
	Camera(const Camera&) = delete; // 복사생성자 삭제
	Camera& operator=(const Camera&) = delete; // 대입연산자 삭제

	Vector2f _viewSize;
};