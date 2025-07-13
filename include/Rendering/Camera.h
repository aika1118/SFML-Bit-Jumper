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
	/**
	 * 카메라 객체의 인스턴스를 반환합니다.
	 *
	 * @return 카메라 객체의 인스턴스입니다.
	 *
	 * @throws 없음
	 */
	static Camera& getInstance(); // 싱글톤
	/**
	 * 현재 뷰의 크기를 반환합니다.
	 *
	 * @return 현재 뷰의 크기입니다.
	 *
	 * @throws 없음
	 */
	Vector2f getViewSize();
	/**
	 * 현재 창 크기에 맞는 뷰를 반환합니다.
	 *
	 * @param windowSize 창의 크기입니다.
	 *
	 * @return 창 크기에 맞는 뷰입니다.
	 *
	 * @throws 없음
	 */
	View getView(Vector2u windowSize);
	/**
	 * UI 렌더링을 위한 뷰를 반환합니다.
	 *
	 * @return UI 렌더링을 위한 뷰입니다.
	 *
	 * @throws 없음
	 */
	View getUIView();
	float _zoomLevel = CAMERA_ZOOM_LEVEL; // view 크기
	Vector2f _position; // view 중심

private:
	Camera() = default; // 외부에서 생성자 호출 불가하도록 설정, 기본생성자로 지정
	Camera(const Camera&) = delete; // 복사생성자 삭제
	Camera& operator=(const Camera&) = delete; // 대입연산자 삭제

	Vector2f _viewSize;
};