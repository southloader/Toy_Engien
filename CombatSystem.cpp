#include "CombatSystem.h"

#include <algorithm>
#include <cstdio>

AttackResult CombatSystem::Attack(SampleCombatant& attacker, SampleCombatant& target) const {
    AttackResult result;

    if (!CanAttack(attacker, target)) {
        return result;
    }

    CombatStats& targetStats = target.GetStats();

    result.success = true;
    result.previousHealth = targetStats.GetCurrentHealth();
    result.requestedDamage = CalculateDamage(attacker, target);
    result.appliedDamage = targetStats.TakeDamage(result.requestedDamage);
    result.currentHealth = targetStats.GetCurrentHealth();
    result.targetDefeated = targetStats.IsDead();

    std::printf(
        "[SampleCombat] %s attacked %s: "
        "%d damage, HP %d -> %d\n",
        attacker.GetCharacterId().c_str(),
        target.GetCharacterId().c_str(),
        result.appliedDamage,
        result.previousHealth,
        result.currentHealth
    );

    return result;
}

int CombatSystem::CalculateDamage(const SampleCombatant& attacker, const SampleCombatant& target) const {
    const int attack = attacker.GetStats().GetAttack();
    const int defense = target.GetStats().GetDefense();
    return std::max(1, attack - defense);
}

bool CombatSystem::CanAttack(const SampleCombatant& attacker, const SampleCombatant& target) const {
    if (&attacker == &target) {
        return false;
    }
    if (attacker.GetTeam() == target.GetTeam()) {
        return false;
    }
    if (attacker.GetStats().IsDead()) {
        return false;
    }
    if (target.GetStats().IsDead()) {
        return false;
    }

    return true;
}