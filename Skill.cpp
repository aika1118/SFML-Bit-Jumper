#include "Skill.h"

Skill::Skill(int skillId)
    : _cooldown(0.f), _damage(0.f), _timer(0.f), _skillTime(0.f)
{
    // skillId�� ���� �ʱ�ȭ ����
    switch (skillId)
    {
        case (int)SkillList::MELEE_ATTACK:
            _cooldown = SKILL_MELEE_ATTACK_COOLDOWN;
            _damage = SKILL_MELEE_ATTACK_DAMAGE;
            _skillTime = SKILL_MELEE_ATTACK_SKILLTIME;
            _animation = Animation(_skillTime,
                {
                    AnimFrame(_skillTime / 3.f, Resources::_textures["melee1.png"]),
                    AnimFrame(2 * _skillTime / 3.f, Resources::_textures["melee2.png"]),
                });
            break;

        case (int)SkillList::MELEE_ATTACK2:
            break;

        default:
            cerr << "Invalid skillId while initializing skill." << endl;
            break;
    }
}

bool Skill::isSkillEnd()
{
    if (_timer >= _skillTime) // ������ ��ų�ð��� ��� �Ҹ��� ��� ��ų�� ����� ������ ����
    {
        Physics::bodiesToDestroy.insert(_attackBody); // attack box ����
        return true;
    }

    return false;
}
