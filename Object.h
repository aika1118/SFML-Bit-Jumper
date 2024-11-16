// File: Object.h
// Purpose: ���� �� Object ������ ���� �θ� class

#pragma once
#include <string>

class b2Body;
class Renderer;

using namespace std;

class Object
{
public:
	virtual void Begin() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(Renderer& renderer) = 0;
	virtual void destroyBody(); // ���� object�� body�� world���� ����

	string _tag; // object ������ ���� tag
	float _angle;
	b2Body* _body;
};