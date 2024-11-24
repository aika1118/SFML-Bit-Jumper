// File: Animation.h
// Purpose: 애니메이션 관리

#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

struct AnimFrame
{
	AnimFrame(float time = 0.f, Texture texture = Texture()) // time : 해당 프레임 등장 시간, texture : 해당 프레임에 등장하는 texture
		: _time(time), _texture(texture)
	{
	}

	float _time = 0.f;
	Texture _texture;
};

class Animation
{
public:
	Animation(float length = 0.f, vector<AnimFrame> frames = {}); // length : 애니메이션 총 길이, frames : 프레임 정보들
	void Update(float deltaTime);

	Texture GetTexture(); // 현재 time에 맞는 frame texture 반환

private:
	float _time = 0.f; // 시간에 따라 해당하는 frame texture 반환을 검사하기 위한 변수
	float _length; // 애니메이션 한 사이클 길이
	vector<AnimFrame> _frames; // 애니메이션 프레임 정보들
};

