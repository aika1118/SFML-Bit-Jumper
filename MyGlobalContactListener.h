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
	/**
	* 두 물체의 접촉이 시작될 때 호출되는 함수입니다.
	* @param contact 접촉 정보 객체
	*/
	virtual void BeginContact(b2Contact* contact) override;
	/**
	* 두 물체의 접촉이 끝날 때 호출되는 함수입니다.
	* @param contact 접촉 정보 객체
	*/
	virtual void EndContact(b2Contact* contact) override;
};