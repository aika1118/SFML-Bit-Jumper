// File: Renderer.h
// Purpose: target에 렌더링

#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Renderer
{
public:
	/**
	* 렌더러 초기화 함수입니다.
	* 렌더러를 초기화합니다.
	* @param target 렌더링 대상
	*/
	Renderer(RenderTarget& target);
	/**
	* 텍스처를 렌더링합니다.
	* @param texture 렌더링할 텍스처
	* @param position 렌더링할 위치
	* @param size 렌더링할 크기
	* @param angle 렌더링할 각도 (기본값: 0.f)
	*/
	void Draw(const Texture& texture, const Vector2f& position, const Vector2f& size, float angle = 0.f); // texture를 position에 size 크기, angle 각도로 target에 그림
	RenderTarget& _target; // 렌더링 될 target (ex. window)

private:
	Sprite _sprite; // texture가 입혀질 sprite
};
