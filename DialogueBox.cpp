#include "DialogueBox.h"

void DialogueBox::Show(const std::string& speaker, const std::vector<std::string>& lines) {
    speakerName = speaker;
    dialogueLines = lines;
    currentLine = 0;
    visible = true;
}

void DialogueBox::Hide() {
    visible = false;
}

void DialogueBox::Next() {
    if (!visible) return;

    if(hasChoices) return;

    currentLine++;

    if (currentLine >= dialogueLines.size()) {
        Hide();
    }
}

bool DialogueBox::IsVisible() {
    return visible;
}

DialogueAction DialogueBox::GetRequest() {
    return request;
}

std::string DialogueBox::GetRequestedQuestId() {
    return requestedQuestId;
}

void DialogueBox::ClearRequest() {
    request = DialogueAction::None;
    requestedQuestId = "";
}

void DialogueBox::ShowChoices(const std::string& speaker, const std::string& line, const std::vector<DialogueChoice>& choices){
    speakerName = speaker;

    dialogueLines.clear();
    dialogueLines.push_back(line);

    this->choices = choices;

    currentLine = 0;
    visible = true;
    hasChoices = true;
}

void DialogueBox::SelectChoice(int index){
    if(!visible) return;
    if(!hasChoices) return;
    if(index <0 || index >= choices.size()) return;

    dialogueLines.clear();
    dialogueLines.push_back(choices[index].result);
    
    request = choices[index].action;
    requestedQuestId = choices[index].questId;

    choices.clear();
    hasChoices = false;
    currentLine = 0;
}

const std::string& DialogueBox::GetSpeakerName() const {
    return speakerName;
}

void DialogueBox::Render(SDL_Renderer* renderer, TTF_Font* font) {
    if (!visible) return;
    if (font == nullptr) return;
    if (dialogueLines.empty()) return;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    int boxX = 50;
    int boxWidth = 700;
    int padding = 20;
    int lineHeight = 30;
    int boxHeight = 100;

    if (hasChoices) {
        boxHeight += choices.size() * lineHeight;
    }

    int boxY = 600 - boxHeight - 40;

    SDL_Rect box = { boxX, boxY, boxWidth, boxHeight};

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 210);
    SDL_RenderFillRect(renderer, &box);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &box);

    RenderText(renderer, font, speakerName, boxX + padding, boxY + padding);
    RenderText(renderer, font, dialogueLines[currentLine], boxX + padding, boxY + padding + 40);

    if (hasChoices) {
        int choiceStartY = boxY + padding + 80;

        for (int i = 0; i < choices.size(); i++) {
            std::string choiceText = std::to_string(i + 1) + ". " + choices[i].text;

            RenderText(renderer, font, choiceText, boxX + padding + 10, choiceStartY + i * lineHeight);
        }
    }
}