// File: Player.h
// Purpose: �÷��̾� ����

#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>
#include "Renderer.h"
#include "Physics.h"
#include "Resources.h"
#include "ContactListener.h"
#include "FixtureData.h"
#include "Animation.h"
#include "Coin.h"
#include "Enemy.h"
#include "SkillManager.h"

using namespace sf;
using namespace std;

enum class PlayerStatus { // PlayerStatus �����ڷθ� ���� ����
	Idle,
	Moving,
	Jumping,
	Attacking
};

class Player : public ContactListener
{
public:
	/**
	 * @brief �÷��̾� �ʱ�ȭ�� �����մϴ�.
	 * �÷��̾� �ʱ�ȭ�� �÷��̾��� ���¸� �ʱⰪ���� �����մϴ�.
	 */
	void Begin();
	/**
	 * @brief �÷��̾ ������Ʈ�մϴ�.
	 * ������Ʈ�� �÷��̾��� ���¸� �����մϴ�.
	 *
	 * @param deltaTime ���� �����Ӱ� ���� ������ ������ �ð� �����Դϴ�.
	 */
	void Update(float deltaTime);
	/**
	 * @brief �÷��̾ �������մϴ�.
	 * �������� �÷��̾��� �׷����� ȭ�鿡 ����մϴ�.
	 *
	 * @param renderer �������� ������ �������Դϴ�.
	 */
	void Draw(Renderer& renderer);
	/**
	 * @brief �÷��̾ �ٸ� ��ü�� ������ �� ȣ��˴ϴ�.
	 * �� �Լ��� �÷��̾�� �ٸ� ��ü�� ������ ó���մϴ�.
	 *
	 * @param self �÷��̾� �ڽ��Դϴ�.
	 * @param other �ٸ� ��ü�Դϴ�.
	 */
	virtual void OnBeginContact(b2Fixture* self, b2Fixture* other) override;
	/**
	 * @brief �÷��̾ �ٸ� ��ü���� ������ ������ �� ȣ��˴ϴ�.
	 * �� �Լ��� �÷��̾�� �ٸ� ��ü�� ������ ������ �� ó���մϴ�.
	 *
	 * @param self �÷��̾� �ڽ��Դϴ�.
	 * @param other �ٸ� ��ü�Դϴ�.
	 */
	virtual void OnEndContact(b2Fixture* self, b2Fixture* other) override;
	/**
	 * @brief �÷��̾��� ���� ������ �����ɴϴ�.
	 * ���� ������ �÷��̾��� ��Ȯ���� ���ϴ� �� ���˴ϴ�.
	 *
	 * @return �÷��̾��� ���� �����Դϴ�.
	 */
	float getJudgementPercentage();
	/**
	 * @brief �÷��̾��� ���� ������ �����ɴϴ�.
	 *
	 * @return �÷��̾��� ���� �����Դϴ�.
	 */
	string getJudgementCurrent();
	/**
	 * @brief �÷��̾ Ŭ������ ���������� �����ɴϴ�.
	 *
	 * @return �÷��̾ Ŭ������ ���������Դϴ�.
	 */
	int getCurrentClearStage();
	/**
	 * @brief �÷��̾ ������ �ִ� ������ �����ɴϴ�.
	 *
	 * @return �÷��̾ ������ �ִ� �����Դϴ�.
	 */
	int getCoin();

	Vector2f _position; // �÷��̾��� ��ġ
	float _angle; // �÷��̾��� ����
	int _hp; // �÷��̾��� HP
	bool _isDead = false; // �÷��̾� ���� ����
	bool _facingLeft = false; // ĳ���Ͱ� ������ �ٶ󺸰� �ִ��� üũ (x�� �ӵ��� ��ȣ�� �Ǻ�)
	bool bIsJumpImproved = false; // ������ �Ծ� �÷��̾ ���� ������ �� �ִ� ��������

private:
	/**
	 * @brief �÷��̾��� �̵��� ó���մϴ�.
	 *
	 * @param deltaTime ���� �����Ӱ� ���� ������ ������ �ð� �����Դϴ�.
	 * @param velocity �÷��̾��� �ӵ��Դϴ�.
	 */
	void HandleMove(float deltaTime, b2Vec2& velocity);
	/**
	 * @brief �÷��̾��� ������ ó���մϴ�.
	 *
	 * @param velocity �÷��̾��� �ӵ��Դϴ�.
	 */
	void HandleJump(b2Vec2& velocity);
	/**
	 * @brief �÷��̾��� ��ų�� ó���մϴ�.
	 *
	 * @param deltaTime ���� �����Ӱ� ���� ������ ������ �ð� �����Դϴ�.
	 * @param velocity �÷��̾��� �ӵ��Դϴ�.
	 */
	void HandleSkill(float deltaTime, b2Vec2& velocity);

	Animation _runAnimation; // �÷��̾� �޸��� animation ����
	Texture _textureToDraw; // ���� �׸� texture

	b2Body* body;
	int _groundContactCount = 0; // ���� mapTile�� ��ģ ���¿��� �Ϻ� Ÿ�ϸ� ���� �������� ���� ���� �پ������� �Ǻ��ϱ� ���� int�� ����
	int _jumpCount = 0; // ��������Ƚ��
	bool _isJumping = false; // �÷��̾ �������ΰ�
	bool _previousSpaceState = false; // ������ ���� keyDown, keyReleased �Ǻ��� ���� ���º���

	int _coins = 0; // �������� coin ����

	bool _isFirstGrounded = false; // ���� �ѹ� ���� ������ ���� ����ϱ� ���� boolean
	float _timeWhenJumpKeydown = 0.f; // ���� ���� ���
	float _timeWhenGrounded = 0.f; // ���� ���� ���� ���
	float _timeInOneCycle = 0.f; // �� ���� ����Ŭ���� ��������, ���� ���� ������ ����ϱ� ���� time ��
	
	float _judgementPerfectCnt = 0; // perfect ���� ��
	float _judgementGreatCnt = 0; // great ���� ��
	float _judgementMissCnt = 0; // miss ���� ��
	float _judgementTotalCnt = 0; // �� ���� Ƚ��
	float _judgementPercentage = 0; // �� ���� Ƚ���� ���� ������ ������ ȥ���Ͽ� ����� ��������
	string _judgementCurrent = "-"; // �÷��̾��� ���� ����

	PlayerStatus _playerStatus; // player�� ���� ���ϰ� �ִ� ���°�
	int _currentSkillId; // skill id ����

	int _currentClearStage = -1; // ���� Ŭ������ ��������
	unordered_map<int, float> _stageScores; // �������� Ŭ���� ���� ���

	float _clearTime = 0.f; // �������� Ŭ���� �ð�
};