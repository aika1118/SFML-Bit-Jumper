#pragma once

#include "Object.h"
#include "Resources.h"
#include "box2d/box2d.h"
#include "Physics.h"

class Mushroom : public Object
{
public:
	void Begin() override;
	void Update(float deltaTime) override;
	void Render(Renderer& renderer) override;

private:
	float _jumpImproveTimeLength = 5.f; // jump improve time when player eats a mushroom 
	float _timer = 0.f;
};