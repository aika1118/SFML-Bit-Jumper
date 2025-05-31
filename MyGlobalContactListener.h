// File: MyGlobalContactListener.h
// Purpose: �� ��ü�� ������ �����ϴ� Ŭ����

#pragma once

#include <box2d/box2d.h>
#include <iostream>
#include "FixtureData.h"
#include "ContactListener.h"

using namespace std;

class MyGlobalContactListener : public b2ContactListener
{
	/**
	* �� ��ü�� ������ ���۵� �� ȣ��Ǵ� �Լ��Դϴ�.
	* @param contact ���� ���� ��ü
	*/
	virtual void BeginContact(b2Contact* contact) override;
	/**
	* �� ��ü�� ������ ���� �� ȣ��Ǵ� �Լ��Դϴ�.
	* @param contact ���� ���� ��ü
	*/
	virtual void EndContact(b2Contact* contact) override;
};