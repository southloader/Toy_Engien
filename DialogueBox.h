#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>

#include "DialogueAction.h"
#include "TextRenderer.h"

struct DialogueChoice {
    std::string text;
    std::string result;
    DialogueAction action = DialogueAction::None;
    std::string questId = "";
};

class DialogueBox{
public:
    void Show(const std::string& speaker, const std::vector<std::string>& lines);
    void Hide();
    void Next();
    bool IsVisible();

    const std::string& GetSpeakerName() const;

    void ShowChoices(const std::string& speaker, const std::string& line, const std::vector<DialogueChoice>& choices);
    void SelectChoice(int index);
    std::string GetRequestedQuestId();
    DialogueAction GetRequest();
    void ClearRequest();

    void Render(SDL_Renderer* renderer, TTF_Font* font);

private:
    bool visible = false;
    std::string speakerName;
    std::vector<std::string> dialogueLines;
    DialogueAction request = DialogueAction::None;
    int currentLine = 0;
    std::vector<DialogueChoice> choices;
    bool hasChoices = false;
    std::string requestedQuestId;

};