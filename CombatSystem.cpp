#include "CombatSystem.h"

#include <algorithm>
#include <cstdio>

CombatSystem::CombatSystem(EventManager* eventManager)
    : eventManager(eventManager){}

void CombatSystem::SetEventManager(EventManager* eventManager){
    this->eventManager = eventManager;
}

AttackResult CombatSystem::Attack(Entity& attacker, Entity& target){
    AttackResult result;

    if (!CanAttack(attacker, target)){
        return result;
    }
    result.success = true;
    result.previousHealth = target.combatStats.GetCurrentHealth();
    result.requestedDamage = CalculateDamage(attacker, target);
    result.appliedDamage = target.combatStats.TakeDamage(result.requestedDamage);
    result.currentHealth = target.combatStats.GetCurrentHealth();
    result.targetDefeated = target.combatStats.IsDead();

    std::printf(
        "[Combat] %s attacked %s: %d damage, HP %d -> %d\n",
        attacker.characterId.c_str(),
        target.characterId.c_str(),
        result.appliedDamage,
        result.previousHealth,
        result.currentHealth
    );

    EmitDamageEvent(attacker, target, result.appliedDamage);

    if (result.targetDefeated){
        EmitDefeatedEvent(attacker, target);
    }
    return result;
}

int CombatSystem::CalculateDamage(const Entity& attacker, const Entity& target) const {
    const int attack = attacker.combatStats.GetAttack();
    const int defense = target.combatStats.GetDefense();

    // 방어력이 공격력보다 높아도 최소 1 피해를 준다.
    return std::max(1, attack - defense);
}

bool CombatSystem::CanAttack(const Entity& attacker, const Entity& target) const {
    if (&attacker == &target){
        return false;
    }
    if (attacker.combatStats.IsDead()){
        return false;
    }
    if (target.combatStats.IsDead()){
        return false;
    }
    return true;
}

void CombatSystem::EmitDamageEvent(const Entity& attacker, const Entity& target, int damage){
    if (eventManager == nullptr){
        return;
    }
    if (damage <= 0){
        return;
    }
    if (target.type == PLAYER){
        Event event;

        event.type = EventType::PlayerDamaged;
        event.subjectId = target.characterId;
        event.amount = damage;

        eventManager->Emit(event);
    }
}

void CombatSystem::EmitDefeatedEvent(const Entity& attacker, const Entity& target){
    if (eventManager == nullptr){
        return;
    }

    if (target.type == ENEMY){
        Event event;

        event.type = EventType::EnemyKilled;
        event.subjectId = target.characterId;
        event.amount = 1;

        eventManager->Emit(event);

        return;
    }

    if (target.type == PLAYER){
        Event event;

        event.type = EventType::PlayerDead;
        event.subjectId = target.characterId;
        event.amount = 1;

        eventManager->Emit(event);
    }
}