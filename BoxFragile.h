#pragma once

#include "Object.h"
#include "Resources.h"
#include "box2d/box2d.h"
#include "Physics.h"

class BoxFragile : public Object
{
public:
	/**
	 * 객체를 초기화합니다.
	 *
	 * @return 없음
	 *
	 * @throws 없음
	 */
	void Begin() override;
	/**
	 * 객체를 업데이트 합니다.
	 *
	 * @param deltaTime 업데이트에 사용되는 시간 간격입니다.
	 *
	 * @return 없음
	 *
	 * @throws 없음
	 */
	void Update(float deltaTime) override;
	/**
	 * 객체를 렌더링합니다.
	 *
	 * @param renderer 렌더링에 사용되는 렌더러입니다.
	 *
	 * @return 없음
	 *
	 * @throws 없음
	 */
	void Render(Renderer& renderer) override;
	/**
	 * 객체에 피해를 입힙니다.
	 *
	 * @return 없음
	 *
	 * @throws 없음
	 */
	void TakeDamage();
	/**
	 * 객체의 현재 HP를 반환합니다.
	 *
	 * @return 객체의 현재 HP입니다.
	 *
	 * @throws 없음
	 */
	int getHP();

private:
	/**
	 * 객체의 현재 HP에 기반한 텍스처를 반환합니다.
	 *
	 * @return 객체의 현재 HP에 기반한 텍스처입니다.
	 *
	 * @throws 없음
	 */
	Texture GetTextureBasedOnHP() const;
	int _hp = 2;
};