// File: Physics.h
// Purpose: ���ӿ��� �������� ��� ��ȣ�ۿ��� ����

#pragma once
#include <box2d/b2_world.h>
#include <vector>
#include "Renderer.h"
#include "Config.h"
#include "MyDebugDraw.h"
#include "MyGlobalContactListener.h"
#include "set"

using namespace std;

class b2Draw;

// ��ü ���ӿ��� �������� ��� ��ȣ�ۿ��� �����ϱ� ������ static ����
class Physics
{
public:
	/**
	* ���� ���� �ʱ�ȭ �Լ��Դϴ�.
	* ���� ������ �ʱ�ȭ�մϴ�.
	*/
	static void Init();
	/**
	* ���� ���� ������Ʈ �Լ��Դϴ�.
	* ���� ������ ���¸� ������Ʈ �մϴ�.
	* @param deltaTime �ð� ����
	*/
	static void Update(float deltaTime);
	/**
	* ���� ���� ����� ����� �Լ��Դϴ�.
	* ���� ������ ����� ������ ȭ�鿡 �׸��� ���� ȣ��˴ϴ�.
	* @param renderer ������ ��ü
	*/
	static void DebugDraw(Renderer& renderer);
	/**
	* world�� �Ҹ��ϱ� ���� �Լ��Դϴ�.
	* 
	*/
	static void Shutdown();
	/**
	* fixtureData �����͸� Physics���� �����ϱ� ���� �Լ�
	* @param fixtureData world�� ������ fixtureData ��ü
	*/
	static void AddFixtureData(unique_ptr<FixtureData> fixtureData); 

	static inline b2World* world; // ������ ��ü���� �����ϴ� world (inline�� ���� ������Ͽ��� static ���� ���� ���� ����)
	static inline b2Draw* debugDraw; // ����� ����� ��

	static inline set<b2Body*> bodiesToDestroy; // ���� ��� ����Ʈ (�ߺ��ڷᰡ ������ �ȵǴ� set ���)
	static inline set<pair<b2Body*, bool>> bodiesSetEnabled; // body�� ���� SetEnable ó���� ��� (�������� �ʰ� ��Ȱ��ȭ ���·� ����, pool���� Ȱ��)
	

private:
	static inline vector<unique_ptr<FixtureData>> fixtureDataList; // fixtureData ������ ������ (���� world �ı� �� physics.h ������ destroy ó��)
};