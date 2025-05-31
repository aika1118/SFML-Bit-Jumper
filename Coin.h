// File: Coin.h
// Purpose: 코인 오브젝트 관리

#pragma once
#include "Object.h"
#include "Animation.h"
#include "Resources.h"
#include "box2d/box2d.h"
#include "Physics.h"

class Coin : public Object
{
public:
	/**
	 * 코인 객체를 초기화합니다.
	 *
	 * @return 없음
	 *
	 * @throws 없음
	 */
	void Begin() override;
	/**
	 * 코인 객체를 업데이트합니다.
	 *
	 * @param deltaTime 업데이트에 사용되는 시간 간격입니다.
	 *
	 * @return 없음
	 *
	 * @throws 없음
	 */
	void Update(float deltaTime) override;
	/**
	 * 코인 객체를 렌더링합니다.
	 *
	 * @param renderer 렌더링에 사용되는 렌더러입니다.
	 *
	 * @return 없음
	 *
	 * @throws 없음
	 */
	void Render(Renderer& renderer) override;

private:
	Animation _animation;
};