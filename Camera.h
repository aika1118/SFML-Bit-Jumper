// File: Camera.h
// Purpose: View를 관리

#pragma once

#include <SFML/Graphics.hpp>
#include "Config.h"

using namespace sf;

class Camera
{
public:
	static Camera& getInstance(); // 싱글톤

	Vector2f getViewSize();
	View getView(Vector2u windowSize); // 현재 windowSize 종횡비에 맞는 view 반환
	
	float _zoomLevel; // view 크기
	Vector2f _position; // view 중심

private:
	Camera(); // 외부에서 생성자 호출 불가하도록 설정
	Camera(const Camera&) = delete; // 복사생성자 삭제
	Camera& operator=(const Camera&) = delete; // 대입연산자 삭제

	Vector2f _viewSize;
};