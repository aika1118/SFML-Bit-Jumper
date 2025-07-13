// File: Object.h
// Purpose: 게임 내 Object 관리를 위한 부모 class

#pragma once
#include "Renderer.h" // 상속받은 class에서 Render()를 구현하며 Renderer.h 선언이 필요할 것 (ex. renderer.Draw())

class b2Body;

using namespace std;

class Object
{
public:
	/**
	* 오브젝트 초기화 함수입니다.
	* 오브젝트의 초기 상태를 설정합니다.
	* 
	*/
	virtual void Begin() = 0;
	/**
	* 오브젝트 업데이트 함수입니다.
	* 오브젝트의 상태를 업데이트 합니다.
	* @param deltaTime 시간 차이
	*/
	virtual void Update(float deltaTime) = 0;
	/**
	* 오브젝트 렌더링 함수입니다.
	* 오브젝트를 화면에 그리기 위해 호출됩니다.
	* @param renderer 렌더러 객체
	*/
	virtual void Render(Renderer& renderer) = 0;
	/**
	* 현재 object의 body를 world에서 제거
	* 오브젝트가 더 이상 화면에 표시되지 않을 때 호출됩니다.
	*/
	virtual void destroyBody(); 
	virtual ~Object() = default; // 소멸자

	string _tag; // object 구분을 위한 tag
	float _angle;
	b2Body* _body;
	Vector2f _position;
};