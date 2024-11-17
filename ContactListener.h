// File: ContactListner.h
// Purpose: �浹�� ������ ��ü�� ����Ͽ� ���

#pragma once

class b2Fixture;

class ContactListener
{
public:
	virtual void OnBeginContact(b2Fixture* self, b2Fixture* other) = 0;
	virtual void OnEndContact(b2Fixture* self, b2Fixture* other) = 0;
};