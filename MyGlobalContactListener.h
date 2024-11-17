// File: MyGlobalContactListener.h
// Purpose: 두 물체의 접촉을 감지하는 클래스

#pragma once

#include <box2d/box2d.h>
#include <iostream>
#include "FixtureData.h"
#include "ContactListener.h"

using namespace std;

class MyGlobalContactListener : public b2ContactListener
{
	virtual void BeginContact(b2Contact* contact) override;
	virtual void EndContact(b2Contact* contact) override;
};