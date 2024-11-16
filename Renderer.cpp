#include "Renderer.h"

Renderer::Renderer(RenderTarget& target)
	: _target(target)
{
}

void Renderer::Draw(const Texture& texture, const Vector2f& position, const Vector2f& size, float angle)
{
	_sprite.setTexture(texture, true); // ���ο� �ؽ�ó�� ũ�⿡ ���缭 ��������Ʈ�� ������
	_sprite.setOrigin((Vector2f)texture.getSize() / 2.f); // texture �߾����� origin ����
	_sprite.setPosition(position);
	_sprite.setRotation(angle);

	// ���� texture ũ�⿡ scale����
	// x ũ�� : texture.getSize().x * (size.x / texture.getSize().x) = size.x
	// y ũ�� : texture.getSize().y * (size.y / texture.getSize().y) = size.y
	_sprite.setScale(Vector2f(size.x / texture.getSize().x, size.y / texture.getSize().y));
	_target.draw(_sprite);
}
