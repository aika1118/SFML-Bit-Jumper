// 현재 미사용 class

#pragma once

#include <unordered_map>
#include "Skill.h"
#include <iostream>


using namespace std;

class SkillManager 
{
public:
	static SkillManager& getInstance(); // 싱글톤

	void AddSkill(int skillId, Skill* skill); // 스킬 추가
	void Begin(int skillId);
	void Update(float deltaTime, int skillId); 
	void Render(Renderer& renderer, int skillId);
	bool isSkillEnd(int skillId); // 스킬 사용이 끝났는지 체크
	bool isValidSkill(int skillId); // 스킬 유효성 검사
	
private:
	SkillManager(); // 외부에서 생성자 호출 불가하도록 설정
	SkillManager(const SkillManager&) = delete; // 복사생성자 삭제
	SkillManager& operator=(const SkillManager&) = delete; // 대입연산자 삭제
	
	unordered_map<int, Skill*> _skills; // 스킬 ID와 skill 객체 저장
	Skill* _currentSkill;
};