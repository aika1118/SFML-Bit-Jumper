#include "Animation.h"

Animation::Animation(float length, vector<AnimFrame> frames)
	: _length(length), _frames(frames)
{
}

void Animation::Update(float deltaTime)
{
	_time += deltaTime; // _time�� ���� animation ������ ������
}

Texture Animation::GetTexture()
{
	// _time�� _length�� ���� �������� ����� �ִϸ��̼��� �ݺ��ǵ��� ��
	_time = fmod(_time, _length);

	for (const AnimFrame& frame : _frames) // ���� _time�� �´� frame�� ��ȯ�ϱ� ���� for��
	{
		if (_time <= frame._time)
			return frame._texture;
	}

	return _frames.back()._texture;  // ������ ������
}
