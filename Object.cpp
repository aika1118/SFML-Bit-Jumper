#include "Object.h"
#include "Physics.h" // ��ȯ���� ����

void Object::destroyBody()
{	
	Physics::bodiesToDestroy.push_back(_body);
}
