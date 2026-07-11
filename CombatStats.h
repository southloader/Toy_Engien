#pragma once

class CombatStats
{
public:
    CombatStats() = default;

    CombatStats(
        int maxHealth,
        int attack,
        int defense,
        int speed
    );

    // 현재 체력 상태
    int GetCurrentHealth() const;
    int GetMaxHealth() const;

    // 기본 전투 능력치
    int GetAttack() const;
    int GetDefense() const;
    int GetSpeed() const;

    // 체력 조작
    int TakeDamage(int amount);
    int Heal(int amount);
    void RestoreHealth();

    // 능력치 설정
    void SetMaxHealth(int value, bool restoreHealth = false);
    void SetAttack(int value);
    void SetDefense(int value);
    void SetSpeed(int value);

    // 상태 확인
    bool IsDead() const;
    bool IsFullHealth() const;

private:
    int maxHealth = 1;
    int currentHealth = 1;

    int attack = 0;
    int defense = 0;
    int speed = 0;

    static int ClampToNonNegative(int value);
};