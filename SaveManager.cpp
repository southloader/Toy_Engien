#include "SaveManager.h"
#include "GameData.h"
#include "ItemDatabase.h"
#include "QuestDatabase.h"
#include <string>

SaveManager::SaveManager(GameData* gameData){
    this->gameData = gameData;
}

#include <fstream>

bool SaveManager::Save(const std::string& filename){
    printf("Save Start\n");
    if(gameData == nullptr) {
        printf("Save ERROR: gameData is nullptr\n");
        return false;
    }

    std::ofstream file(filename);

    if(!file.is_open()){
        printf("Save Error: cannot open file\n");
        return false;
    }

    printf("Saving Gold\n");
    file << "Gold" << gameData->gold << "\n";

    printf("Saving Inventory\n");
    file << "Inventory" << "\n";
    for(const auto& slot:gameData->inventory.GetInventory()) {
        file << slot.item.id << " " << slot.count << "\n";
    }
    file << "EndInventory" << "\n";

    printf("Saving Quest\n");
    file << "Quest" << "\n";

    for(const auto& quest:gameData->questLog.GetQuests()) {
        file << quest.id << " " << (int) quest.state << "\n";
    }

    file << "EndQuest" << "\n";

    file.close();

    printf("Save Complete\n");

    return true;
}

#include <sstream>

bool SaveManager::Load(const std::string& filename){
    std::ifstream file(filename);

    if(!file.is_open()) return false;

    gameData->inventory.Clear();
    gameData->questLog.Clear();

    std::string type;

    while(file >> type) {
        if(type == "Gold") {
            file >> gameData->gold;
        }
        else if (type == "Inventory") {
            while(true) {
                std::string id;
                file >> id;

                if(id == "EndInventory") {
                    break;
                }

                int count; 

                file >> count;

                Item item = ItemDatabase::Get(id);

                gameData->inventory.RestoreItem(item, count);
            }
        }
        else if(type == "Quest") {
            while(true) {
                std::string id;
                file >> id;

                if(id == "EndQuest") {
                    break;
                }

                int state;
                file >> state;

                Quest quest = QuestDatabase::Get(id);
                quest.state = static_cast<QuestState>(state);

                gameData->questLog.AddQuest(quest);
            }
        }
    }

    file.close();
    return true;
}