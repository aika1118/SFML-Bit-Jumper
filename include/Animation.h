// File: Animation.h
// Purpose: 애니메이션 관리

#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

struct AnimFrame
{	
	/**
	 * 지정된 시간과 텍스처로 AnimFrame 객체를 생성합니다.
	 *
	 * @param time 프레임이 등장하는 시간입니다.
	 * @param texture 프레임에 등장하는 텍스처입니다.
	 *
	 */
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
	/**
	 * 애니메이션 객체를 생성합니다.
	 *
	 * @param length 애니메이션의 총 길이입니다.
	 * @param frames 애니메이션의 프레임 정보들입니다.
	 *
	 * @return 없음
	 *
	 * @throws 없음
	 */
	Animation(float length = 0.f, vector<AnimFrame> frames = {}); // length : 애니메이션 총 길이, frames : 프레임 정보들
	/**
	 * 애니메이션을 업데이트합니다.
	 *
	 * @param deltaTime 애니메이션 업데이트에 사용되는 시간 간격입니다.
	 *
	 * @return 없음
	 *
	 * @throws 없음
	 */
	void Update(float deltaTime);
	/**
	 * 현재 시간에 맞는 프레임의 텍스처를 반환합니다.
	 *
	 * @return 현재 시간에 맞는 프레임의 텍스처입니다.
	 *
	 * @throws 없음
	 */
	Texture GetTexture(); // 현재 time에 맞는 frame texture 반환

private:
	float _time = 0.f; // 시간에 따라 해당하는 frame texture 반환을 검사하기 위한 변수
	float _length; // 애니메이션 한 사이클 길이
	vector<AnimFrame> _frames; // 애니메이션 프레임 정보들
};

