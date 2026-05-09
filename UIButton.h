#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class UIButton {
public :
//좌표 값
int x, y;
//버튼의 크기
int width, height;

//밝기
int brightness = 255;

//호버체크 플래그
bool isHovered = false;

std::string text;

void Update();

//렌더러
void Render(SDL_Renderer* renderer, TTF_Font* font);

bool IsClicked(int mouseX, int mouseY);

};