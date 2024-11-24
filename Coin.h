// File: Coin.h
// Purpose: ���� ������Ʈ ����

#pragma once
#include "Object.h"
#include "Animation.h"
#include "Resources.h"
#include "box2d/box2d.h"
#include "Physics.h"

class Coin : public Object
{
public:
	virtual void Begin() override;
	virtual void Update(float deltaTime) override;
	virtual void Render(Renderer& renderer) override;

private:
	Animation _animation;
};