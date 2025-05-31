// File: FixtureData.h
// Brief: fixture ���� ���� ����
// �����͸� �����ϰ� �����ϱ� ���� ����ü
// �ٸ� Ŭ������ ����ü�� ����� ���� �� ����

#pragma once

// ���漱��
class ContactListener;
class Player;
class Object;

// Fixture�� Ÿ��
enum class FixtureDataType
{
	Player,
	PlayerSensor,
	PlayerHeadSensor,
	MapTile,
	SaveTile,
	ExitTile,
	LockTile,
	Object,
	Attack
};

struct FixtureData
{
	ContactListener* listener;
	FixtureDataType type;
	bool isSkillAttacked;
	bool canJump = false;

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