#include "MyGlobalContactListener.h"

// world���� ������ �� ��ü �浹 �� �Լ��� �Ҹ�
// �浹�� �� ��ü�� ���� contact���� FixtureA�� FixtureB�� ����
// � ���� Player���� �𸣱� ������ A, B �Ѵ� ��
// ���� Player�� ContactListener�� �����, ���� Player �浹 ���� �� �θ� class�� Player�� �����ϴ� ����
void MyGlobalContactListener::BeginContact(b2Contact* contact)
{
	FixtureData* data = (FixtureData*)contact->GetFixtureA()->GetUserData().pointer;
	if (data && data->listener) // fixture�� listener�� �����ϴ� ��� (ex. player)
		data->listener->OnBeginContact(contact->GetFixtureA(), contact->GetFixtureB());

	data = (FixtureData*)contact->GetFixtureB()->GetUserData().pointer;
	if (data && data->listener)
		data->listener->OnBeginContact(contact->GetFixtureB(), contact->GetFixtureA());
}

void MyGlobalContactListener::EndContact(b2Contact* contact)
{
	FixtureData* data = (FixtureData*)contact->GetFixtureA()->GetUserData().pointer;
	if (data && data->listener) // fixture�� listener�� �����ϴ� ��� (ex. player)
		data->listener->OnEndContact(contact->GetFixtureA(), contact->GetFixtureB());

	data = (FixtureData*)contact->GetFixtureB()->GetUserData().pointer;
	if (data && data->listener)
		data->listener->OnEndContact(contact->GetFixtureB(), contact->GetFixtureA());
}
