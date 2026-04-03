#include <vector>
#include "Entity.h"

class Game {
public:
    void Init();
    void HandleEvents();
    void Update();
    void Render();
    void Clean();
    std::vector<std::vector<int>> map;
    int tileSize;

    bool running;

private:
    std::vector<Entity> entities;
    SDL_Window* window;
    SDL_Renderer* renderer;
};