#include "Resources.h"

unordered_map<string, Texture> Resources::_textures; // static 변수 정의 (객체 없이 접근 가능, 클래스당 1개만 초기화되어 관리)
unordered_map<string, SoundBuffer> Resources::_sounds; // static 변수 정의
