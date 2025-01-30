// File: FixtureData.h
// Brief: fixture 세부 정보 관리
// 데이터를 저장하고 관리하기 위한 구조체
// 다른 클래스나 구조체의 멤버로 사용될 수 있음

#pragma once

class ContactListener;
class Player;
class Object;

enum class FixtureDataType
{
	Player,
	PlayerSensor,
	PlayerHeadSensor,
	MapTile,
	SaveTile,
	Object,
	Attack
};

struct FixtureData
{
	ContactListener* listener;
	FixtureDataType type;
	bool isSkillAttacked;

	union // 한 공간을 공유하는 데이터 타입
	{
		Player* player; // Fixture가 플레이어일 떄 저장
		Object* object;
		struct // Fixture가 Map일 떄 저장
		{
			int mapX;
			int mapY;
		};
	};

};