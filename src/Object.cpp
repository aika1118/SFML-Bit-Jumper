#include "Object.h"
#include "Physics.h" // ��ȯ���� ����

void Object::destroyBody()
{	
	Physics::bodiesToDestroy.insert(_body);
}
