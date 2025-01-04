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
	void Begin() override;
	void Update(float deltaTime) override;
	void Render(Renderer& renderer) override;

private:
	Animation _animation;
};