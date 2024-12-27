// File: ContactListner.h
// Purpose: 충돌을 감지할 객체가 상속하여 사용

#pragma once

class b2Fixture;

class ContactListener
{
public:
	virtual void OnBeginContact(b2Fixture* self, b2Fixture* other) = 0;
	virtual void OnEndContact(b2Fixture* self, b2Fixture* other) = 0;

	virtual ~ContactListener() = 0; // 순수 가상 소멸자여도 객체 소멸 시 반드시 호출되어야하기 때문에 때문에 정의가 필요함
};