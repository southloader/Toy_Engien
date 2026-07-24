#include "SampleCombatScene.h"

#include <algorithm>
#include <cstdio>

#include "SampleCombatDatabase.h"
#include "TextRenderer.h"

SampleCombatScene::SampleCombatScene(
    TextureManager* textureManager,
    TTF_Font* font,
    SampleCombatSession* session
)
    : textureManager(textureManager),
      font(font),
      session(session)
{
}

void SampleCombatScene::Update() {
    // SceneManager는 Scene 객체를 한 번 생성한 뒤
    // 다시 전환해서 사용하므로, 전투 요청이 들어올 때마다
    // 새로운 전투를 초기화해야 한다.
    if (
        !initialized &&
        session != nullptr &&
        session->IsActive()
    ) {
        InitializeFromSession();
    }
}

void SampleCombatScene::Render(SDL_Renderer* renderer) {
    if (renderer == nullptr){
        return;
    }

    // 샘플 전투 전용 배경
    SDL_SetRenderDrawColor(
        renderer,
        25,
        30,
        45,
        255
    );

    SDL_Rect background = {
        0,
        0,
        800,
        600
    };

    SDL_RenderFillRect(renderer, &background);

    if (state == SampleCombatState::WaitingForRequest) {
        RenderText(
            renderer,
            font,
            "Waiting for combat request...",
            220,
            280
        );

        return;
    }

    if (state == SampleCombatState::Error) {
        RenderText(
            renderer,
            font,
            "Sample Combat Error",
            270,
            220
        );

        RenderText(
            renderer,
            font,
            battleMessage,
            120,
            280
        );

        RenderText(
            renderer,
            font,
            "Press Enter to return.",
            250,
            360
        );

        return;
    }

    RenderCombatant(
        renderer,
        player,
        120,
        250,
        true
    );

    RenderCombatant(
        renderer,
        enemy,
        580,
        250,
        false
    );

    RenderBattleText(renderer);
}

void SampleCombatScene::HandleEvents(SDL_Event& event) {
    if (event.type != SDL_KEYDOWN || event.key.repeat != 0) {
        return;
    }

    if (state == SampleCombatState::PlayerTurn && event.key.keysym.sym == SDLK_SPACE) {
        ExecutePlayerTurn();
        return;
    }

    if (
        state == SampleCombatState::Victory ||
        state == SampleCombatState::Defeat ||
        state == SampleCombatState::Error
    ) {
        if (event.key.keysym.sym == SDLK_RETURN) {
            FinishAndReturn();
        }
    }
}

SceneRequest SampleCombatScene::GetRequest() {
    return request;
}

void SampleCombatScene::ClearRequest() {
    request = SceneRequest::None;
}

void SampleCombatScene::InitializeFromSession() {
    initialized = true;

    if (session == nullptr) {
        SetError(
            "Combat session is null."
        );

        return;
    }

    if (!session->IsActive()) {
        SetError(
            "No active combat request."
        );

        return;
    }

    const SampleCombatRequest& combatRequest = session->GetRequest();

    if (!combatRequest.IsValid()) {
        SetError(
            "Combat request is invalid."
        );

        return;
    }

    if (!SampleCombatDatabase::Contains(combatRequest.playerCharacterId)) {
        SetError(
            "Player combat data not found: " +
            combatRequest.playerCharacterId
        );

        return;
    }

    if (!SampleCombatDatabase::Contains(combatRequest.enemyCharacterId)) {
        SetError(
            "Enemy combat data not found: " +
            combatRequest.enemyCharacterId
        );

        return;
    }

    player = SampleCombatant(
        combatRequest.playerCharacterId,
        SampleCombatTeam::Player,
        SampleCombatDatabase::GetStats(
            combatRequest.playerCharacterId
        )
    );

    enemy = SampleCombatant(
        combatRequest.enemyCharacterId,
        SampleCombatTeam::Enemy,
        SampleCombatDatabase::GetStats(
            combatRequest.enemyCharacterId
        )
    );

    state = SampleCombatState::PlayerTurn;

    battleMessage =
        "A " +
        combatRequest.enemyCharacterId +
        " appeared!";

    std::printf(
        "[SampleCombatScene] Started: %s vs %s\n",
        player.GetCharacterId().c_str(),
        enemy.GetCharacterId().c_str()
    );
}

void SampleCombatScene::ExecutePlayerTurn() {
    AttackResult result =
        combatSystem.Attack(
            player,
            enemy
        );

    if (!result.success) {
        battleMessage =
            "The player could not attack.";

        return;
    }

    battleMessage =
        player.GetCharacterId() +
        " dealt " +
        std::to_string(
            result.appliedDamage
        ) +
        " damage.";

    if (result.targetDefeated) {
        SetVictory();
        return;
    }

    ExecuteEnemyTurn();
}

void SampleCombatScene::ExecuteEnemyTurn() {
    AttackResult result =
        combatSystem.Attack(
            enemy,
            player
        );

    if (!result.success) {
        battleMessage =
            "The enemy could not attack.";

        return;
    }

    battleMessage +=
        " " +
        enemy.GetCharacterId() +
        " dealt " +
        std::to_string(
            result.appliedDamage
        ) +
        " damage.";

    if (result.targetDefeated) {
        SetDefeat();
    }
}

void SampleCombatScene::SetVictory() {
    state = SampleCombatState::Victory;

    battleMessage =
        "Victory! " +
        enemy.GetCharacterId() +
        " was defeated.";

    std::printf(
        "[SampleCombatScene] Victory: %s defeated.\n",
        enemy.GetCharacterId().c_str()
    );
}

void SampleCombatScene::SetDefeat() {
    state = SampleCombatState::Defeat;

    battleMessage =
        "Defeat... The player was defeated.";

    std::printf(
        "[SampleCombatScene] Player defeated.\n"
    );
}

void SampleCombatScene::SetError(const std::string& message) {
    state = SampleCombatState::Error;
    battleMessage = message;

    std::printf(
        "[SampleCombatScene] Error: %s\n",
        message.c_str()
    );
}

void SampleCombatScene::FinishAndReturn() {
    if (session == nullptr) {
        initialized = false;
        state = SampleCombatState::WaitingForRequest;
        request = SceneRequest::GoToPlay;
        return;
    }

    SampleCombatResult result;

    result.enemyInstanceId =
        session
            ->GetRequest()
            .enemyInstanceId;

    result.returnSceneName =
        session
            ->GetRequest()
            .returnSceneName;

    if (state == SampleCombatState::Victory) {
        result.outcome = SampleCombatOutcome::Victory;
        result.defeatedCharacterId = enemy.GetCharacterId();
    }
    else if (state == SampleCombatState::Defeat) {
        result.outcome = SampleCombatOutcome::Defeat;
    }
    else {
        result.outcome = SampleCombatOutcome::Cancelled;
    }

    session->Finish(result);

    // 다음 전투에서 다시 Request를 읽도록 초기화한다.
    initialized = false;
    state = SampleCombatState::WaitingForRequest;
    battleMessage.clear();

    // 현재 샘플에서는 Play로만 돌아간다.
    // 이후 SceneManager의 동적 Scene 이름 요청을 만들면
    // returnSceneName을 직접 사용할 수 있다.
    request = SceneRequest::GoToPlay;
}

void SampleCombatScene::RenderCombatant(
    SDL_Renderer* renderer,
    const SampleCombatant& combatant,
    int x,
    int y,
    bool isPlayer
)
{
    const std::string textureId =
        combatant.GetCharacterId() ==
        "player"
            ? "player_idle"
            : combatant.GetCharacterId();

    SDL_Texture* texture = nullptr;

    if (textureManager != nullptr)
    {
        texture =
            textureManager->GetTexture(
                textureId
            );
    }

    SDL_Rect destination =
    {
        x,
        y,
        100,
        100
    };

    if (texture != nullptr)
    {
        SDL_RendererFlip flip =
            isPlayer
                ? SDL_FLIP_NONE
                : SDL_FLIP_HORIZONTAL;

        SDL_RenderCopyEx(
            renderer,
            texture,
            nullptr,
            &destination,
            0.0,
            nullptr,
            flip
        );
    }
    else
    {
        if (isPlayer)
        {
            SDL_SetRenderDrawColor(
                renderer,
                60,
                180,
                100,
                255
            );
        }
        else
        {
            SDL_SetRenderDrawColor(
                renderer,
                200,
                80,
                80,
                255
            );
        }

        SDL_RenderFillRect(
            renderer,
            &destination
        );
    }

    const CombatStats& stats =
        combatant.GetStats();

    RenderHealthBar(
        renderer,
        stats,
        x - 20,
        y + 125,
        140,
        20
    );

    RenderText(
        renderer,
        font,
        combatant.GetCharacterId(),
        x,
        y - 40
    );

    const std::string healthText =
        "HP " +
        std::to_string(
            stats.GetCurrentHealth()
        ) +
        " / " +
        std::to_string(
            stats.GetMaxHealth()
        );

    RenderText(
        renderer,
        font,
        healthText,
        x - 5,
        y + 155
    );
}

void SampleCombatScene::RenderHealthBar(
    SDL_Renderer* renderer,
    const CombatStats& stats,
    int x,
    int y,
    int width,
    int height
)
{
    SDL_Rect background =
    {
        x,
        y,
        width,
        height
    };

    SDL_SetRenderDrawColor(
        renderer,
        65,
        65,
        65,
        255
    );

    SDL_RenderFillRect(
        renderer,
        &background
    );

    const int maxHealth =
        std::max(
            1,
            stats.GetMaxHealth()
        );

    const float ratio =
        static_cast<float>(
            stats.GetCurrentHealth()
        ) /
        static_cast<float>(
            maxHealth
        );

    SDL_Rect health =
    {
        x,
        y,
        static_cast<int>(
            width * ratio
        ),
        height
    };

    SDL_SetRenderDrawColor(
        renderer,
        40,
        190,
        70,
        255
    );

    SDL_RenderFillRect(
        renderer,
        &health
    );

    SDL_SetRenderDrawColor(
        renderer,
        230,
        230,
        230,
        255
    );

    SDL_RenderDrawRect(
        renderer,
        &background
    );
}

void SampleCombatScene::RenderBattleText(
    SDL_Renderer* renderer
)
{
    RenderText(
        renderer,
        font,
        "Basic Combat Sample",
        270,
        40
    );

    RenderText(
        renderer,
        font,
        battleMessage,
        100,
        450
    );

    if (
        state == SampleCombatState::PlayerTurn
    )
    {
        RenderText(
            renderer,
            font,
            "SPACE: Attack",
            300,
            520
        );
    }
    else if (
        state == SampleCombatState::Victory
    )
    {
        RenderText(
            renderer,
            font,
            "Victory - Press Enter",
            260,
            520
        );
    }
    else if (
        state == SampleCombatState::Defeat
    )
    {
        RenderText(
            renderer,
            font,
            "Defeat - Press Enter",
            265,
            520
        );
    }
}