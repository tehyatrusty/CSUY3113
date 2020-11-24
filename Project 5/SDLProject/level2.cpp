#include "level2.h"

#define LEVEL2_WIDTH 16
#define LEVEL2_HEIGHT 8

#define LEVEL2_ENEMY_COUNT 1
unsigned int level2_data[] =
{
16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 44, 0,
 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 41, 41, 41, 41, 0,
 16, 0, 0, 0, 0, 0, 41, 41, 41, 0, 0, 0, 0, 0, 0, 0,
 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 16, 0, 41, 41, 41, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 16, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16
};

Mix_Chunk* successL2;
Mix_Chunk* gameoverL2;

void Level2::Initialize() {
    state.nextScene = -1;

    GLuint mapTextureID = Util::LoadTexture("platformPack_tilesheet@2.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 13, 7);
    // Move over all of the player and enemy code from initialization.

    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(2, 0, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("george_0.png");

    state.player->animRight = new int[4]{ 3, 7, 11, 15 };
    state.player->animLeft = new int[4]{ 1, 5, 9, 13 };
    state.player->animUp = new int[4]{ 2, 6, 10, 14 };
    state.player->animDown = new int[4]{ 0, 4, 8, 12 };

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;

    state.player->height = 0.8f;
    state.player->width = 0.8f;

    state.player->jumpPower = 6.0f;

    state.enemies = new Entity[LEVEL2_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("ghost-Sheet.png");

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].speed = 1.0;
    state.enemies[0].isActive = true;
    state.enemies[0].position = glm::vec3(6, -5, 0);

    state.enemies[0].animIndices = new int[4]{ 0, 1, 2, 3 };
    state.enemies[0].animFrames = 4;
    state.enemies[0].animIndex = 0;
    state.enemies[0].animTime = 0;
    state.enemies[0].animCols = 4;
    state.enemies[0].animRows = 1;

    successL2 = Mix_LoadWAV("success.wav");
    gameoverL2 = Mix_LoadWAV("gameover.wav");
}

void Level2::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.map);
    state.enemies[0].Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.map);

    if (state.player->position.x >= 13 && state.player->position.y >= -2) {
        Mix_PlayChannel(-1, successL2, 0);
        state.nextScene = 3;
    }

    if (state.player->position.y <= -7) {
        state.player->playerLives -= 1;
        state.player->position = glm::vec3(5, 0, 0);
    }

    if (state.player->playerLives == 0) {
        state.player->isActive = false;
        Mix_PlayChannel(-1, gameoverL2, 0);
    }
}

void Level2::Render(ShaderProgram* program) {
    GLuint fontTextureID = Util::LoadTexture("font2.png");
    
    state.map->Render(program);

    switch (state.player->playerLives) {
    case 3:
        Util::DrawText(program, fontTextureID, "Lives Left: 3", 0.2f, 0.05f, glm::vec3(6.0f, -1.0f, 0.0f));
        break;
    case 2:
        Util::DrawText(program, fontTextureID, "Lives Left: 2", 0.2f, 0.05f, glm::vec3(6.0f, -1.0f, 0.0f));
        break;
    case 1:
        Util::DrawText(program, fontTextureID, "Lives Left: 1", 0.2f, 0.05f, glm::vec3(6.0f, -1.0f, 0.0f));
        break;
}
   
    
    if (state.player->isActive) state.player->Render(program);
    else {
        Util::DrawText(program, fontTextureID, "You Lose!", 0.5f, 0.05f, glm::vec3(2.0f, -4.0f, 0.0f));
    }

    state.enemies[0].Render(program);
}