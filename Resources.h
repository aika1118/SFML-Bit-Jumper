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
	static unordered_map<string, Texture> _textures; // texture �ҷ��� ����, ���� �� 1���� �����ϸ� �Ǵ� static ����
	static unordered_map<string, SoundBuffer> _sounds; // texture�� sprite ����ó�� SoundBuffer�� Sound ���赵 ����
};