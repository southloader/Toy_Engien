#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

void RenderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y);