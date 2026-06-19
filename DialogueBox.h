#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

class DialogueBox{
public:
    void Show(const std::string& speaker, const std::vector<std::string>& lines);
    void Hide();
    void Next();
    bool IsVisible();

    void Render(SDL_Renderer* renderer, TTF_Font* font);

private:
    bool visible = false;
    std::string speakerName;
    std::vector<std::string> dialogueLines;
    int currentLine = 0;
};