// File: Physics.h
// Purpose: ���ӿ��� �������� ��� ��ȣ�ۿ��� ����

#pragma once
#include <box2d/b2_world.h>
#include <vector>
#include "Renderer.h"
#include "Config.h"
#include "MyDebugDraw.h"
#include "MyGlobalContactListener.h"

using namespace std;

class b2Draw;

// ��ü ���ӿ��� �������� ��� ��ȣ�ۿ��� �����ϱ� ������ static ����
class Physics
{
public:
	static void Init();
	static void Update(float deltaTime);
	static void DebugDraw(Renderer& renderer);

	static b2World* world; // ������ ��ü���� �����ϴ� ����
	static b2Draw* debugDraw; // ����� ����� ��

	static vector<b2Body*> bodiesToDestroy; // ���� ��� ����Ʈ
};