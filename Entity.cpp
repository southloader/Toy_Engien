#include "Entity.h"

void Entity::Render(SDL_Renderer* renderer, Camera camera){
    SDL_Rect dst = {
        x - camera.x,
        y - camera.y,
        width,
        height
    };

    SDL_RendererFlip flip = SDL_FLIP_NONE;

    if (direction == Direction::Left) {
        flip = SDL_FLIP_HORIZONTAL;
    }

    if (animator != nullptr) {
        SDL_Texture* frame = animator->GetCurrentFrame();

        if (frame != nullptr) {
            SDL_RenderCopyEx(renderer, frame, nullptr, &dst, 0, nullptr, flip);
            return;
        }
    }

    if (texture != nullptr) {
        SDL_RenderCopyEx(renderer, texture, nullptr, &dst, 0, nullptr, flip);
        return;
    }

    if (type == PLAYER)
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    else
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    SDL_RenderFillRect(renderer, &dst);
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