// File: Object.h
// Purpose: 게임 내 Object 관리를 위한 부모 class

#pragma once
#include "Renderer.h" // 상속받은 class에서 Render()를 구현하며 Renderer.h 선언이 필요할 것 (ex. renderer.Draw())

class b2Body;

using namespace std;

class Object
{
public:
	virtual void Begin() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(Renderer& renderer) = 0;
	virtual void destroyBody(); // 현재 object의 body를 world에서 제거
	virtual ~Object() = default;

	string _tag; // object 구분을 위한 tag
	float _angle;
	b2Body* _body;
	Vector2f _position;
};