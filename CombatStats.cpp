#include "CombatStats.h"

#include <algorithm>

CombatStats::CombatStats(
    int maxHealth,
    int attack,
    int defense,
    int speed
) {
    this->maxHealth = std::max(1, maxHealth);
    currentHealth = this->maxHealth;

    this->attack = ClampToNonNegative(attack);
    this->defense = ClampToNonNegative(defense);
    this->speed = ClampToNonNegative(speed);
}

int CombatStats::GetCurrentHealth() const{
    return currentHealth;
}

int CombatStats::GetMaxHealth() const{
    return maxHealth;
}

int CombatStats::GetAttack() const{
    return attack;
}

int CombatStats::GetDefense() const{
    return defense;
}

int CombatStats::GetSpeed() const{
    return speed;
}

int CombatStats::TakeDamage(int amount){
    amount = ClampToNonNegative(amount);

    const int previousHealth = currentHealth;

    currentHealth = std::max(0, currentHealth - amount);

    // 요청된 피해량이 아니라 실제로 감소한 체력을 반환한다.
    return previousHealth - currentHealth;
}

int CombatStats::Heal(int amount){
    amount = ClampToNonNegative(amount);

    const int previousHealth = currentHealth;

    currentHealth = std::min(maxHealth, currentHealth + amount);

    // 요청된 회복량이 아니라 실제로 회복된 체력을 반환한다.
    return currentHealth - previousHealth;
}

void CombatStats::RestoreHealth(){
    currentHealth = maxHealth;
}

void CombatStats::SetMaxHealth(int value, bool restoreHealth){
    maxHealth = std::max(1, value);

    if (restoreHealth){
        currentHealth = maxHealth;
        return;
    }

    // 최대 체력이 줄었을 때 현재 체력이 최대치를 넘지 않게 한다.
    currentHealth = std::min(currentHealth, maxHealth);
}

void CombatStats::SetAttack(int value){
    attack = ClampToNonNegative(value);
}

void CombatStats::SetDefense(int value){
    defense = ClampToNonNegative(value);
}

void CombatStats::SetSpeed(int value){
    speed = ClampToNonNegative(value);
}

bool CombatStats::IsDead() const{
    return currentHealth <= 0;
}

bool CombatStats::IsFullHealth() const{
    return currentHealth >= maxHealth;
}

int CombatStats::ClampToNonNegative(int value){
    return std::max(0, value);
}