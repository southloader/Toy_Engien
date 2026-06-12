#pragma once

#include <string>
#include <unordered_map>

#include "Scene.h"

class SceneManager
{
public:
    void AddScene(
        const std::string& name,
        Scene* scene
    );
    void ChangeScene(const std::string& name);
    void PushOverlay(const std::string& name);
    void PopOverlay();
    void Update();
    void Render(SDL_Renderer* renderer);
    void HandleEvents(SDL_Event& event);
    void ProcessRequest(bool& running);
    Scene* GetCurrentScene();

    void Clean();

private:
    std::unordered_map<std::string,Scene*> scenes;

    Scene* currentScene = nullptr;
    Scene* overlayScene = nullptr;
};