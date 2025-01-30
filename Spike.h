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
	void Begin() override;
	void Update(float deltaTime) override;
	void Render(Renderer& renderer) override;
};