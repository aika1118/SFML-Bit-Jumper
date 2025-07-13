// File: Resources.h
// Purpose: resource �ҷ��� �� unordered_map�� ����

#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>

using namespace sf;
using namespace std;

class Resources
{
public:
	static inline unordered_map<string, Texture> _textures; // texture �ҷ��� ����, ���� �� 1���� �����ϸ� �Ǵ� static ����
	static inline unordered_map<string, Sound> _sounds; // texture�� sprite ����ó�� SoundBuffer�� Sound ���赵 ����
	static inline unordered_map<string, SoundBuffer> soundBuffers; // soundBuffer : sound �����͸� ����
	static inline unordered_map<string, Music> _musics;
};