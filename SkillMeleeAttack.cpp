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
	if (Game::getInstance().getPlayerFacingLeft()) attackX -= MAP_CELL_SIZE; // ���� ���ݹ����� ���� attack box
	else attackX += MAP_CELL_SIZE; // ������ ���ݹ����� ���� attack box
	bodyDef.position.Set(attackX, Game::getInstance().getPlayerPosition().y); // ĳ���� �������� ��ġ ����
	_attackBody = Physics::world->CreateBody(&bodyDef); // bodyDef �����͸� �����ؼ� ����ϴ� bodyDef�� ���� scope �������� ��ȿ�ص� ���� ����

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

	// unique_ptr�� ���� �� Physics�� ������ �̵� (Physics���� ��� �����ϵ��� �Ͽ� �������� ���� �� �����ϰ� �޸� �����ǵ��� ��)
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
