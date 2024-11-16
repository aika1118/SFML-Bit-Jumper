// File: Renderer.h
// Purpose: target에 렌더링

#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Renderer
{
public:
	Renderer(RenderTarget& target);
	void Draw(const Texture& texture, const Vector2f& position, const Vector2f& size, float angle = 0.f); // texture를 position에 size 크기, angle 각도로 target에 그림
	RenderTarget& _target; // 렌더링 될 target (ex. window)

private:
	Sprite _sprite; // texture가 입혀질 sprite
};
