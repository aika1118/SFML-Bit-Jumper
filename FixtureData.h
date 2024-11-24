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
	MapTile,
	SaveTile,
	Object
};

struct FixtureData
{
	ContactListener* listener;
	FixtureDataType type;

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