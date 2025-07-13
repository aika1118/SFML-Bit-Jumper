#include "MyGlobalContactListener.h"

// world에서 임의의 두 물체 충돌 시 함수가 불림
// 충돌한 두 물체는 각각 contact에서 FixtureA와 FixtureB에 저장
// 어떤 것이 Player일지 모르기 때문에 A, B 둘다 비교
// 현재 Player가 ContactListener를 상속함, 만약 Player 충돌 감지 시 부모 class로 Player를 접근하는 형태
void MyGlobalContactListener::BeginContact(b2Contact* contact)
{
	FixtureData* data = (FixtureData*)contact->GetFixtureA()->GetUserData().pointer;
	if (data && data->listener) // fixture에 listener가 존재하는 경우 (ex. player)
		data->listener->OnBeginContact(contact->GetFixtureA(), contact->GetFixtureB());

	data = (FixtureData*)contact->GetFixtureB()->GetUserData().pointer;
	if (data && data->listener)
		data->listener->OnBeginContact(contact->GetFixtureB(), contact->GetFixtureA());
}

void MyGlobalContactListener::EndContact(b2Contact* contact)
{
	FixtureData* data = (FixtureData*)contact->GetFixtureA()->GetUserData().pointer;
	if (data && data->listener) // fixture에 listener가 존재하는 경우 (ex. player)
		data->listener->OnEndContact(contact->GetFixtureA(), contact->GetFixtureB());

	data = (FixtureData*)contact->GetFixtureB()->GetUserData().pointer;
	if (data && data->listener)
		data->listener->OnEndContact(contact->GetFixtureB(), contact->GetFixtureA());
}
