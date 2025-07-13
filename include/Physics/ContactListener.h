// File: ContactListner.h
// Purpose: 충돌을 감지할 객체가 상속하여 사용

#pragma once

class b2Fixture;

class ContactListener
{
public:
	/**
	 * 두 개의 fixture가 처음으로 접촉했을 때 호출되는 함수입니다.
	 *
	 * 이 함수는 ContactListener 클래스의 파생 클래스에서 구현되어야 합니다.
	 *
	 * @param self  접촉한 fixture 중 하나입니다.
	 * @param other 접촉한 fixture 중 다른 하나입니다.
	 *
	 * @return 없음
	 */
	virtual void OnBeginContact(b2Fixture* self, b2Fixture* other) = 0;
	/**
	 * 두 개의 fixture가 접촉을 끝내고 분리되었을 때 호출되는 함수입니다.
	 *
	 * 이 함수는 ContactListener 클래스의 파생 클래스에서 구현되어야 합니다.
	 *
	 * @param self  접촉을 끝낸 fixture 중 하나입니다.
	 * @param other 접촉을 끝낸 fixture 중 다른 하나입니다.
	 *
	 * @return 없음
	 */
	virtual void OnEndContact(b2Fixture* self, b2Fixture* other) = 0;
	/**
	 * ContactListener 클래스의 순수 가상 소멸자입니다.
	 *
	 *
	 * @return 없음
	 */
	virtual ~ContactListener() = 0; // 순수 가상 소멸자여도 객체 소멸 시 반드시 호출되어야하기 때문에 때문에 정의가 필요함
};