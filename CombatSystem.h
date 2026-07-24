#pragma once

#include "SampleCombatant.h"

struct AttackResult {
    bool success = false;

    int requestedDamage = 0;
    int appliedDamage = 0;

    int previousHealth = 0;
    int currentHealth = 0;

    bool targetDefeated = false;
};

class CombatSystem {
public:
    AttackResult Attack(
        SampleCombatant& attacker,
        SampleCombatant& target
    ) const;

    int CalculateDamage(
        const SampleCombatant& attacker,
        const SampleCombatant& target
    ) const;

    bool CanAttack(
        const SampleCombatant& attacker,
        const SampleCombatant& target
    ) const;
};