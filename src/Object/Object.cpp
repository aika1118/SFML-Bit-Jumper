#include "Object.h"
#include "Physics.h" // 순환참조 방지

void Object::destroyBody()
{	
	Physics::bodiesToDestroy.insert(_body);
}
