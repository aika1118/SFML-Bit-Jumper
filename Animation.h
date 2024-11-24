// File: Animation.h
// Purpose: �ִϸ��̼� ����

#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

struct AnimFrame
{
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
	Animation(float length = 0.f, vector<AnimFrame> frames = {}); // length : �ִϸ��̼� �� ����, frames : ������ ������
	void Update(float deltaTime);

	Texture GetTexture(); // ���� time�� �´� frame texture ��ȯ

private:
	float _time = 0.f; // �ð��� ���� �ش��ϴ� frame texture ��ȯ�� �˻��ϱ� ���� ����
	float _length; // �ִϸ��̼� �� ����Ŭ ����
	vector<AnimFrame> _frames; // �ִϸ��̼� ������ ������
};

