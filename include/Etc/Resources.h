// File: Resources.h
// Purpose: resource 불러온 후 unordered_map에 저장

#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>

using namespace sf;
using namespace std;

class Resources
{
public:
	static inline unordered_map<string, Texture> _textures; // texture 불러와 저장, 게임 내 1개만 관리하면 되니 static 선언
	static inline unordered_map<string, Sound> _sounds; // texture와 sprite 관계처럼 SoundBuffer와 Sound 관계도 동일
	static inline unordered_map<string, SoundBuffer> soundBuffers; // soundBuffer : sound 데이터를 저장
	static inline unordered_map<string, Music> _musics;
};