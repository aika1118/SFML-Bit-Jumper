// File: Object.h
// Purpose: ���� �� Object ������ ���� �θ� class

#pragma once
#include "Renderer.h" // ��ӹ��� class���� Render()�� �����ϸ� Renderer.h ������ �ʿ��� �� (ex. renderer.Draw())

class b2Body;

using namespace std;

class Object
{
public:
	virtual void Begin() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(Renderer& renderer) = 0;
	virtual void destroyBody(); // ���� object�� body�� world���� ����
	virtual ~Object() = default;

	string _tag; // object ������ ���� tag
	float _angle;
	b2Body* _body;
	Vector2f _position;
};