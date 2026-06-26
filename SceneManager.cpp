#include "SceneManager.h"

void SceneManager::AddScene(const std::string& name, Scene* scene)
{
    scenes[name] = scene;
}

void SceneManager::Update(){
    if (overlayScene) {
        overlayScene->Update();
    }
    else if(currentScene)
    {
        currentScene->Update();
    }
}

void SceneManager::Render(SDL_Renderer* renderer) {
    if(currentScene){
        currentScene->Render(renderer);
    }
    if (overlayScene) {
        overlayScene->Render(renderer);
    }
}

void SceneManager::ProcessRequest(bool& running) {
    Scene* activeScene = overlayScene ? overlayScene : currentScene;

    if (activeScene == nullptr) return;

    SceneRequest request = activeScene->GetRequest();

    if (request == SceneRequest::None) return;

    activeScene->ClearRequest();

    if (request == SceneRequest::GoToMainMenu) {
        ChangeScene("MainMenu");
    }
    else if (request == SceneRequest::GoToPlay) {
        PopOverlay();
        ChangeScene("Play");
    }
    else if (request == SceneRequest::GoToPause) {
        PushOverlay("Pause");
    }
    else if (request == SceneRequest::OpenShop) {
        PushOverlay("Shop");
    }
    else if (request == SceneRequest::OpenInventory) {
        PushOverlay("Inventory");
    }
    else if (request == SceneRequest::CloseOverlay) {
        PopOverlay();
    }
    else if (request == SceneRequest::ExitGame) {
        running = false;
    }
}

Scene* SceneManager::GetCurrentScene()
{
    return currentScene;
}

void SceneManager::HandleEvents(SDL_Event& event){
    if (overlayScene) {
        overlayScene->HandleEvents(event);
    }
    else if(currentScene != nullptr){
        currentScene->HandleEvents(event);
    }
}

void SceneManager::Clean() {
    for (auto& pair : scenes) {
        delete pair.second;
    }

    scenes.clear();
    currentScene = nullptr;
}

void SceneManager::ChangeScene(const std::string& name) {
    currentScene = scenes[name];
    overlayScene = nullptr;
}

void SceneManager::PushOverlay(const std::string& name) {
    overlayScene = scenes[name];
}

void SceneManager::PopOverlay() {
    overlayScene = nullptr;
}