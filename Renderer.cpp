#include "Renderer.h"

Renderer::Renderer(RenderTarget& target)
	: _target(target)
{
}

void Renderer::Draw(const Texture& texture, const Vector2f& position, const Vector2f& size, float angle)
{
	_sprite.setTexture(texture, true); // 새로운 텍스처의 크기에 맞춰서 스프라이트가 렌더링
	_sprite.setOrigin((Vector2f)texture.getSize() / 2.f); // texture 중앙으로 origin 조정
	_sprite.setPosition(position);
	_sprite.setRotation(angle);

	// 기존 texture 크기에 scale적용
	// x 크기 : texture.getSize().x * (size.x / texture.getSize().x) = size.x
	// y 크기 : texture.getSize().y * (size.y / texture.getSize().y) = size.y
	_sprite.setScale(Vector2f(size.x / texture.getSize().x, size.y / texture.getSize().y));
	_target.draw(_sprite);
}
