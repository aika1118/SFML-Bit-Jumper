#include "Animation.h"

Animation::Animation(float length, vector<AnimFrame> frames)
	: _length(length), _frames(frames)
{
}

void Animation::Update(float deltaTime)
{
	_time += deltaTime; // _time에 따라 animation 프레임 결정함
}

Texture Animation::GetTexture()
{
	// _time을 _length로 나눈 나머지를 사용해 애니메이션이 반복되도록 함
	_time = fmod(_time, _length);

	for (const AnimFrame& frame : _frames) // 현재 _time에 맞는 frame을 반환하기 위한 for문
	{
		if (_time <= frame._time)
			return frame._texture;
	}

	return _frames.back()._texture;  // 마지막 프레임
}
