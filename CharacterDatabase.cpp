#include "CharacterDatabase.h"
#include <cstdio>

std::unordered_map<std::string, CharacterData> CharacterDatabase::characters;

void CharacterDatabase::Init(TextureManager* textureManager) {
    textureManager->LoadTexture("player_idle", "assets/player.png");
    textureManager->LoadTexture("player_walk_1", "assets/player_walk1.png");
    textureManager->LoadTexture("player_walk_2", "assets/player_walk2.png");
    textureManager->LoadTexture("player_walk_3", "assets/player_walk3.png");

    textureManager->LoadTexture("player_run_1", "assets/player_run1.png");
    textureManager->LoadTexture("player_run_2", "assets/player_run2.png");
    textureManager->LoadTexture("player_run_3", "assets/player_run3.png");

    CharacterData player("player");
    player.SetCombatStats(CombatStats(100, 15, 5, 10));
    player.AddAnimation("Idle",{"player_idle"}, 1000);
    player.AddAnimation("Walk",{"player_walk_1","player_walk_2","player_walk_3"}, 120);
    player.AddAnimation("Run" ,{"player_run_1","player_run_2","player_run_3"}, 70);
    player.SetDefaultAnimation("Idle");
    Register(player);

    textureManager->LoadTexture("npc_idle", "assets/npc.png");
    textureManager->LoadTexture("npc_walk_1", "assets/player_walk1.png");
    textureManager->LoadTexture("npc_walk_2", "assets/player_walk2.png");
    textureManager->LoadTexture("npc_walk_3", "assets/player_walk3.png");

    CharacterData npc("npc");
    npc.SetCombatStats(CombatStats(30,5,2,5));
    npc.AddAnimation("Idle",{"npc_idle"}, 1000);
    npc.AddAnimation("Walk",{"npc_walk_1","npc_walk_2","npc_walk_3"}, 150);
    npc.SetDefaultAnimation("Idle");
    Register(npc);

    textureManager->LoadTexture("slime","assets/slime.png");
    CharacterData slime("slime");
    slime.SetCombatStats(CombatStats(30,8,2,4));
    slime.AddAnimation("Idle", {"slime"}, 1000);
    slime.SetDefaultAnimation("Idle");
    Register(slime);
}

void CharacterDatabase::Register(const CharacterData& character) {
    characters [character.id] = character;
}

CharacterData CharacterDatabase::Get(const std::string& id) {
    auto found = characters.find(id);
    if (found == characters.end()) {
        printf("Character not found: %s\n", id.c_str());

        CharacterData empty;
        empty.id = "none";
        return empty;
    }

    return found->second;
}