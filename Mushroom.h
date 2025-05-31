#pragma once

#include "Object.h"
#include "Resources.h"
#include "box2d/box2d.h"
#include "Physics.h"

class Mushroom : public Object
{
public:
	/**
	 * @brief mushroom 초기화시 호출
	 */
	void Begin() override;
	/**
	 * @brief mushroom 상태 업데이트
	 * @param deltaTime 두 프레임 차이 시간
	 */
	void Update(float deltaTime) override;
	/**
	 * @brief mushroom 렌더링
	 * @param renderer 렌더링을 위한 object
	 */
	void Render(Renderer& renderer) override;

private:
	float _jumpImproveTimeLength = 5.f; // jump improve time when player eats a mushroom 
	float _timer = 0.f;
};