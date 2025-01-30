// File: FixtureData.h
// Brief: fixture ���� ���� ����
// �����͸� �����ϰ� �����ϱ� ���� ����ü
// �ٸ� Ŭ������ ����ü�� ����� ���� �� ����

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

	union // �� ������ �����ϴ� ������ Ÿ��
	{
		Player* player; // Fixture�� �÷��̾��� �� ����
		Object* object;
		struct // Fixture�� Map�� �� ����
		{
			int mapX;
			int mapY;
		};
	};

};