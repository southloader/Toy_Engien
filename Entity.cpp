#include "Entity.h"

void Entity::Render(SDL_Renderer* renderer){
    SDL_Rect rect = { x, y, width, height };
    if (type == PLAYER)
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // 초록
    else
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // 빨강
    SDL_RenderFillRect(renderer, &rect);
}

//단순 길이 비교에 불과하다. 3차원에서 '벡터거리'재는 물건이 아니다.
bool Entity::CheckCollision(Entity& other){
    return(
        x < other.x + other.width &&
        x + width > other.x &&
        y < other.y + other.height &&
        y + height > other.y
    );
}