#include "NPC.h"

NPC::NPC(){
    name = "NPC";

    entity.x = 0;
    entity.y = 0;
    entity.width = 40;
    entity.height = 40;
    entity.type = NPC_TYPE;
}

void NPC::SetName(const std::string& name) {
    this->name = name;
}

void NPC::SetPosition(int x, int y){
    entity.x = x;
    entity.y = y;
};

void NPC::SetSize(int width, int height) {
    entity.width = width;
    entity.height = height;
}

void NPC::SetTexture(SDL_Texture* texture) {
    entity.texture = texture;
}

void NPC::SetDialogue(const std::vector<std::string>& lines) {
    dialogueLines = lines;
}

void NPC::Update(Entity* player) {
    int oldX = entity.x;
    int oldY = entity.y;

    if (behavior == NPCBehavior::Idle) {
        
    }
    else if (behavior == NPCBehavior::Patrol) {
        if (!patrolPoints.empty()) {
            SDL_Point target = patrolPoints[currentPatrolIndex];

            if (entity.x < target.x) {
                entity.direction = Direction::Right;
                entity.x += moveSpeed;
            }
            else if (entity.x > target.x) {
                entity.direction = Direction::Left;
                entity.x -= moveSpeed;
            }

            if (entity.y < target.y) {
                entity.y += moveSpeed;
            }
            else if (entity.y > target.y) {
                entity.y -= moveSpeed;
            }

            int dx = entity.x - target.x;
            int dy = entity.y - target.y;

            if (dx * dx + dy * dy < moveSpeed * moveSpeed * 4) {
                currentPatrolIndex++;

                if (currentPatrolIndex >= patrolPoints.size()) {
                    currentPatrolIndex = 0;
                }
            }
        }
    }
    else if (behavior == NPCBehavior::FollowPlayer) {
        if (player != nullptr) {
            float dx = (float)(player->x - entity.x);
            float dy = (float)(player->y - entity.y);

            float distance = std::sqrt(dx * dx + dy * dy);
            float stopDistance = 60.0f;

            if (distance > stopDistance) {

                float dirX = dx / distance;
                float dirY = dy / distance;

                float moveAmount = moveSpeed;

                if (distance - stopDistance < moveAmount) {
                    moveAmount = distance - stopDistance;
                }

                entity.x += (int)(dirX * moveAmount);
                entity.y += (int)(dirY * moveAmount);

                if (dx > 0) {
                    entity.direction = Direction::Right;
                }
                else if (dx < 0) {
                    entity.direction = Direction::Left;
                }
            }
        }
    }

    
    bool isMoving = (oldX != entity.x || oldY != entity.y);

    if (isMoving) {
        entity.animator.Play("Walk");
    }
    else {
        entity.animator.Play("Idle");
    }

    entity.animator.Update();
}

void NPC::Render(SDL_Renderer* renderer, Camera camera) {
    entity.Render(renderer, camera);
}

Entity& NPC::GetEntity() {
    return entity;
}

std::string NPC::GetName() {
    return name;
}

std::vector<std::string> NPC::GetDialogue() {
    return dialogueLines;
}

void NPC::SetBehavior(NPCBehavior behavior) {
    this->behavior = behavior;
}

void NPC::SetPatrolPoints(const std::vector<SDL_Point>& points) {
    patrolPoints = points;
    currentPatrolIndex = 0;
}

void NPC::SetMoveSpeed(int speed) {
    moveSpeed = speed;
}