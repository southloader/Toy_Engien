#include "CharacterData.h"

CharacterData::CharacterData(const std::string& id): id(id) {}

void CharacterData::AddAnimation(
    const std::string& name,
    const std::vector<std::string>& frames,
    int frameDelay
) {
    AnimationData animation;

    animation.textureIds = frames;
    animation.frameDelay = frameDelay;

    animations[name] = animation;
}

void CharacterData::SetDefaultAnimation(const std::string& name) {
    defaultAnimation = name;
}