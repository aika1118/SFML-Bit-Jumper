// File: Object.h
// Purpose: ���� �� Object ������ ���� �θ� class

#pragma once
#include "Renderer.h" // ��ӹ��� class���� Render()�� �����ϸ� Renderer.h ������ �ʿ��� �� (ex. renderer.Draw())

class b2Body;

using namespace std;

class Object
{
public:
	/**
	* ������Ʈ �ʱ�ȭ �Լ��Դϴ�.
	* ������Ʈ�� �ʱ� ���¸� �����մϴ�.
	* 
	*/
	virtual void Begin() = 0;
	/**
	* ������Ʈ ������Ʈ �Լ��Դϴ�.
	* ������Ʈ�� ���¸� ������Ʈ �մϴ�.
	* @param deltaTime �ð� ����
	*/
	virtual void Update(float deltaTime) = 0;
	/**
	* ������Ʈ ������ �Լ��Դϴ�.
	* ������Ʈ�� ȭ�鿡 �׸��� ���� ȣ��˴ϴ�.
	* @param renderer ������ ��ü
	*/
	virtual void Render(Renderer& renderer) = 0;
	/**
	* ���� object�� body�� world���� ����
	* ������Ʈ�� �� �̻� ȭ�鿡 ǥ�õ��� ���� �� ȣ��˴ϴ�.
	*/
	virtual void destroyBody(); 
	virtual ~Object() = default; // �Ҹ���

	string _tag; // object ������ ���� tag
	float _angle;
	b2Body* _body;
	Vector2f _position;
};