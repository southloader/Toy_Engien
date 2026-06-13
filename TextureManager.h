#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <unordered_map>

class TextureManager {
public:
    TextureManager(SDL_Renderer* renderer);

    bool LoadTexture(const std::string& id, const std::string& path);
    SDL_Texture* GetTexture(const std::string& id);

    void Clean();

private:
    SDL_Renderer* renderer;
    std::unordered_map<std::string, SDL_Texture*> textures;
};