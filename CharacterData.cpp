#include "CharacterData.h"

CharacterData::CharacterData(const std::string& id) {
    this->id = id;
}

void CharacterData::AddAnimation(const std::string& name, const std::vector<std::string>& frames, int frameDelay) {
    AnimationData anim;
    anim.textureIds = frames;
    anim.frameDelay = frameDelay;
    animations[name] = anim;
}

void CharacterData::SetDefaultAnimation(const std::string& name) {
    defaultAnimation = name;
}

void CharacterData::SetCombatStats(const CombatStats& stats) {
    combatStats = stats;
}