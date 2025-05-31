#pragma once

// Purpose: 가시 오브젝트 관리

#pragma once
#include "Object.h"
#include "Resources.h"
#include "box2d/box2d.h"
#include "Physics.h"

class Spike : public Object
{
public:
	/**
	* 오브젝트 초기화 함수입니다.
	* 오브젝트의 초기 상태를 설정합니다.
	*
	*/
	void Begin() override;
	/**
	* 오브젝트 업데이트 함수입니다.
	* 오브젝트의 상태를 업데이트 합니다.
	* @param deltaTime 시간 차이
	*/
	void Update(float deltaTime) override;
	/**
	* 오브젝트 렌더링 함수입니다.
	* 오브젝트를 화면에 그리기 위해 호출됩니다.
	* @param renderer 렌더러 객체
	*/
	void Render(Renderer& renderer) override;
};