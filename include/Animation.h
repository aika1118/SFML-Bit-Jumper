// File: Animation.h
// Purpose: �ִϸ��̼� ����

#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

struct AnimFrame
{	
	/**
	 * ������ �ð��� �ؽ�ó�� AnimFrame ��ü�� �����մϴ�.
	 *
	 * @param time �������� �����ϴ� �ð��Դϴ�.
	 * @param texture �����ӿ� �����ϴ� �ؽ�ó�Դϴ�.
	 *
	 */
	AnimFrame(float time = 0.f, Texture texture = Texture()) // time : �ش� ������ ���� �ð�, texture : �ش� �����ӿ� �����ϴ� texture
		: _time(time), _texture(texture)
	{
	}

	float _time = 0.f;
	Texture _texture;
};

class Animation
{
public:
	/**
	 * �ִϸ��̼� ��ü�� �����մϴ�.
	 *
	 * @param length �ִϸ��̼��� �� �����Դϴ�.
	 * @param frames �ִϸ��̼��� ������ �������Դϴ�.
	 *
	 * @return ����
	 *
	 * @throws ����
	 */
	Animation(float length = 0.f, vector<AnimFrame> frames = {}); // length : �ִϸ��̼� �� ����, frames : ������ ������
	/**
	 * �ִϸ��̼��� ������Ʈ�մϴ�.
	 *
	 * @param deltaTime �ִϸ��̼� ������Ʈ�� ���Ǵ� �ð� �����Դϴ�.
	 *
	 * @return ����
	 *
	 * @throws ����
	 */
	void Update(float deltaTime);
	/**
	 * ���� �ð��� �´� �������� �ؽ�ó�� ��ȯ�մϴ�.
	 *
	 * @return ���� �ð��� �´� �������� �ؽ�ó�Դϴ�.
	 *
	 * @throws ����
	 */
	Texture GetTexture(); // ���� time�� �´� frame texture ��ȯ

private:
	float _time = 0.f; // �ð��� ���� �ش��ϴ� frame texture ��ȯ�� �˻��ϱ� ���� ����
	float _length; // �ִϸ��̼� �� ����Ŭ ����
	vector<AnimFrame> _frames; // �ִϸ��̼� ������ ������
};

