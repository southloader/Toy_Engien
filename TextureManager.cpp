#include "TextureManager.h"
#include <cstdio>

TextureManager::TextureManager(SDL_Renderer* renderer) {
    this->renderer = renderer;
}

bool TextureManager::LoadTexture(const std::string& id, const std::string& path) {
    SDL_Texture* texture= IMG_LoadTexture(renderer, path.c_str());

    if(texture == nullptr) {
        printf("Failed to load texture: %s\n", IMG_GetError());
        return false;
    }

    textures[id] = texture;
    return true;
}

SDL_Texture* TextureManager::GetTexture(const std::string& id) {
    if (textures.find(id) == textures.end()) {
        printf("Texture not found: %s\n", id.c_str());
        return nullptr;
    }

    return textures[id];
}

void TextureManager::Clean() {
    for (auto& pair : textures) {
        SDL_DestroyTexture(pair.second);
    }

    textures.clear();
}