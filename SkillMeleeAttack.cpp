#include "SkillMeleeAttack.h"
#include "Game.h"


SkillMeleeAttack::SkillMeleeAttack(int skillId)
	:Skill(skillId)
{
}

void SkillMeleeAttack::Begin()
{
	_timer = 0.f;
	
	// body for attack box
	b2BodyDef bodyDef;
	float attackX = Game::getInstance().getPlayerPosition().x;
	if (Game::getInstance().getPlayerFacingLeft()) attackX -= MAP_CELL_SIZE; // 왼쪽 공격방향을 위한 attack box
	else attackX += MAP_CELL_SIZE; // 오른쪽 공격방향을 위한 attack box
	bodyDef.position.Set(attackX, Game::getInstance().getPlayerPosition().y); // 캐릭터 기준으로 위치 선정
	_attackBody = Physics::world->CreateBody(&bodyDef); // bodyDef 데이터를 복사해서 사용하니 bodyDef가 현재 scope 내에서만 유효해도 문제 없음

	// sensor shape for attack box
	b2PolygonShape sensorShape;
	sensorShape.SetAsBox(MAP_CELL_SIZE / 2.f, MAP_CELL_SIZE / 2.f);

	FixtureData* fixtureData = new FixtureData();

	// fixture for attack box
	fixtureData->listener = &(Game::getInstance().getPlayer());
	fixtureData->player = &(Game::getInstance().getPlayer());
	fixtureData->type = FixtureDataType::Attack;
	fixtureData->isSkillAttacked = false;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &sensorShape;
	fixtureDef.isSensor = true;
	fixtureDef.userData.pointer = (uintptr_t)fixtureData;

	_attackBody->CreateFixture(&fixtureDef);

	// unique_ptr로 래핑 후 Physics로 소유권 이동 (Physics에서 모두 관리하도록 하여 스테이지 종료 후 안전하게 메모리 해제되도록 함)
	unique_ptr<FixtureData> fixtureDataPtr(fixtureData);
	Physics::AddFixtureData(move(fixtureDataPtr));
}

void SkillMeleeAttack::Update(float deltaTime)
{
	_timer += deltaTime;
	_animation.Update(deltaTime);
}

void SkillMeleeAttack::Render(Renderer& renderer)
{
	renderer.Draw(_animation.GetTexture(), Game::getInstance().getPlayerPosition(), Vector2f(Game::getInstance().getPlayerFacingLeft() ? -PLAYER_SIZE_WIDTH : PLAYER_SIZE_WIDTH, PLAYER_SIZE_HEIGHT), Game::getInstance().getPlayerAngle());
}
