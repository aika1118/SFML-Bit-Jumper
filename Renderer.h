// File: Renderer.h
// Purpose: target�� ������

#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Renderer
{
public:
	/**
	* ������ �ʱ�ȭ �Լ��Դϴ�.
	* �������� �ʱ�ȭ�մϴ�.
	* @param target ������ ���
	*/
	Renderer(RenderTarget& target);
	/**
	* �ؽ�ó�� �������մϴ�.
	* @param texture �������� �ؽ�ó
	* @param position �������� ��ġ
	* @param size �������� ũ��
	* @param angle �������� ���� (�⺻��: 0.f)
	*/
	void Draw(const Texture& texture, const Vector2f& position, const Vector2f& size, float angle = 0.f); // texture�� position�� size ũ��, angle ������ target�� �׸�
	RenderTarget& _target; // ������ �� target (ex. window)

private:
	Sprite _sprite; // texture�� ������ sprite
};
