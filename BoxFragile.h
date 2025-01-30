#pragma once

#include "Object.h"
#include "Resources.h"
#include "box2d/box2d.h"
#include "Physics.h"

class BoxFragile : public Object
{
public:
	void Begin() override;
	void Update(float deltaTime) override;
	void Render(Renderer& renderer) override;
	void TakeDamage();
	int getHP();

private:
	Texture GetTextureBasedOnHP() const;
	int _hp = 2;
};