#pragma once

enum class SceneRequest {
    None,

    GoToMainMenu,
    GoToPlay,
    GoToPause,
    GoToSampleCombat,

    OpenShop,
    OpenInventory,
    CloseOverlay,
    OpenQuestLog,
    
    ExitGame
};