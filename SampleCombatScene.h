#pragma once

#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

#include "Scene.h"
#include "TextureManager.h"
#include "SampleCombatSession.h"
#include "SampleCombatant.h"
#include "CombatSystem.h"

enum class SampleCombatState
{
    WaitingForRequest,
    PlayerTurn,
    Victory,
    Defeat,
    Error
};

class SampleCombatScene : public Scene {
public:
    SampleCombatScene(
        TextureManager* textureManager,
        TTF_Font* font,
        SampleCombatSession* session
    );

    void Update() override;
    void Render(SDL_Renderer* renderer) override;
    void HandleEvents(SDL_Event& event) override;

    SceneRequest GetRequest() override;
    void ClearRequest() override;

private:
    TextureManager* textureManager = nullptr;
    TTF_Font* font = nullptr;

    SampleCombatSession* session = nullptr;

    SceneRequest request = SceneRequest::None;
    SampleCombatState state = SampleCombatState::WaitingForRequest;
    CombatSystem combatSystem;

    SampleCombatant player;
    SampleCombatant enemy;

    bool initialized = false;

    std::string battleMessage;

    void InitializeFromSession();

    void ExecutePlayerTurn();
    void ExecuteEnemyTurn();

    void SetVictory();
    void SetDefeat();
    void SetError(const std::string& message);

    void FinishAndReturn();

    void RenderCombatant(
        SDL_Renderer* renderer,
        const SampleCombatant& combatant,
        int x,
        int y,
        bool isPlayer
    );

    void RenderHealthBar(
        SDL_Renderer* renderer,
        const CombatStats& stats,
        int x,
        int y,
        int width,
        int height
    );

    void RenderBattleText(SDL_Renderer* renderer);
};