#pragma once

#include "Entity.h"
#include "EventManager.h"

struct AttackResult
{
    bool success = false;

    int requestedDamage = 0;
    int appliedDamage = 0;

    int previousHealth = 0;
    int currentHealth = 0;

    bool targetDefeated = false;
};

class CombatSystem {
public:
    CombatSystem() = default;
    explicit CombatSystem(EventManager* eventManager);

    void SetEventManager(EventManager* eventManager);

    AttackResult Attack(
        Entity& attacker,
        Entity& target
    );

    int CalculateDamage(
        const Entity& attacker,
        const Entity& target
    ) const;

    bool CanAttack(
        const Entity& attacker,
        const Entity& target
    ) const;

private:
    EventManager* eventManager = nullptr;

    void EmitDamageEvent(
        const Entity& attacker,
        const Entity& target,
        int damage
    );

    void EmitDefeatedEvent(
        const Entity& attacker,
        const Entity& target
    );
};