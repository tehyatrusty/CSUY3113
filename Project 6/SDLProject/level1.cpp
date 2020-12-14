#include "level1.h"
#define LEVEL1_WIDTH 18
#define LEVEL1_HEIGHT 16

#define LEVEL1_ENEMY_COUNT 10
unsigned int level1_data[] =
{                                              
 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void Level1::Initialize() {
    state.nextScene = -1;

    GLuint mapTextureID = Util::LoadTexture("planet_sheet.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 4);

    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(4, -3, 0);
    state.player->movement = glm::vec3(0);
    state.player->speed = 1.5f;
    state.player->textureID = Util::LoadTexture("Spaceship_Asset.png");

    state.player->animFull = new int[3]{ 12, 13, 14 };
    state.player->animDamaged1 = new int[3]{ 8, 9, 10 };
    state.player->animDamaged2 = new int[3]{ 4, 5, 6 };
    state.player->animDamaged3 = new int[3]{ 0, 1, 2 };

    state.player->animIndices = state.player->animFull;
    state.player->animFrames = 4;
    state.player->animIndex = 2;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;

    state.player->height = 0.8f;
    state.player->width = 0.8f;

    state.player->jumpPower = 6.0f;

    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("asteroid.png");

    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].aiType = ASTEROID;
        state.enemies[i].speed = 1.5;
        state.enemies[i].isActive = true;
    }

    state.enemies[0].position = glm::vec3(4, -3, 0);
    state.enemies[0].movement = glm::vec3(1, 1, 0);

    state.enemies[1].position = glm::vec3(2, -7, 0);
    state.enemies[1].movement = glm::vec3(-1, -1, 0);

    state.enemies[2].position = glm::vec3(1, -8, 0);
    state.enemies[2].movement = glm::vec3(1, -1, 0);

    state.enemies[3].position = glm::vec3(8, -4, 0);
    state.enemies[3].movement = glm::vec3(-1, 1, 0);

    state.enemies[4].position = glm::vec3(-1, -6, 0);
    state.enemies[4].movement = glm::vec3(1, 1, 0);

    state.enemies[5].position = glm::vec3(14, -5, 0);
    state.enemies[5].movement = glm::vec3(-1, -1, 0);

    state.enemies[6].position = glm::vec3(6, -11, 0);
    state.enemies[6].movement = glm::vec3(-1, 1, 0);

    state.enemies[7].position = glm::vec3(10, -13, 0);
    state.enemies[7].movement = glm::vec3(1, -1, 0);

    state.enemies[8].position = glm::vec3(0, -1, 0);
    state.enemies[8].movement = glm::vec3(1, 1, 0);

    state.enemies[9].position = glm::vec3(10, -9, 0);
    state.enemies[9].movement = glm::vec3(-1, 1, 0);

    state.player->destinations = {};

    Mix_VolumeMusic(MIX_MAX_VOLUME / 3);
}

void Level1::Update(float deltaTime) {
	state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
    
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
    }

}
void Level1::Render(ShaderProgram* program) {
	state.map->Render(program);
	state.player->Render(program);
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }

    float playerX = state.player->position.x;
    float playerY = state.player->position.y;
    GLuint fontTextureID = Util::LoadTexture("font.png");

    switch (state.player->health) {
    case 3:
        Util::DrawText(program, fontTextureID, "Ship Health: 3", 0.2f, 0.02f, glm::vec3(playerX + 1.0f, playerY + 3.0f, 0.0f));
        break;
    case 2:
        Util::DrawText(program, fontTextureID, "Ship Health: 2", 0.2f, 0.02f, glm::vec3(playerX + 1.0f, playerY + 3.0f, 0.0f));
        break;
    case 1:
        Util::DrawText(program, fontTextureID, "Ship Health: 1", 0.2f, 0.02f, glm::vec3(playerX + 1.0f, playerY + 3.0f, 0.0f));
        break;
    case 0:
        Util::DrawText(program, fontTextureID, "Ship Health: 0", 0.2f, 0.02f, glm::vec3(playerX + 1.0f, playerY + 3.0f, 0.0f));
        Util::DrawText(program, fontTextureID, "Your Ship Died - You Lose", 0.3f, 0.02f, glm::vec3(playerX - 4.0f, playerY + 2.0f, 0.0f));
        break;
    }

    switch (state.player->planetsLeft) {
    case 6:
        Util::DrawText(program, fontTextureID, "Planets Left: 6", 0.2f, 0.02f, glm::vec3(playerX - 4.0f, playerY + 3.0f, 0.0f));
        break;
    case 5:
        Util::DrawText(program, fontTextureID, "Planets Left: 5", 0.2f, 0.02f, glm::vec3(playerX - 4.0f, playerY + 3.0f, 0.0f));
        break;
    case 4:
        Util::DrawText(program, fontTextureID, "Planets Left: 4", 0.2f, 0.02f, glm::vec3(playerX - 4.0f, playerY + 3.0f, 0.0f));
        break;
    case 3:
        Util::DrawText(program, fontTextureID, "Planets Left: 3", 0.2f, 0.02f, glm::vec3(playerX - 4.0f, playerY + 3.0f, 0.0f));
        break;
    case 2:
        Util::DrawText(program, fontTextureID, "Planets Left: 2", 0.2f, 0.02f, glm::vec3(playerX - 4.0f, playerY + 3.0f, 0.0f));
        break;
    case 1:
        Util::DrawText(program, fontTextureID, "Planets Left: 1", 0.2f, 0.02f, glm::vec3(playerX - 4.0f, playerY + 3.0f, 0.0f));
        break;
    case 0: 
        Util::DrawText(program, fontTextureID, "Planets Left : 0", 0.2f, 0.02f, glm::vec3(playerX - 4.0f, playerY + 3.0f, 0.0f));
        Util::DrawText(program, fontTextureID, "You Dropped Off All Of", 0.3f, 0.02f, glm::vec3(playerX - 4.0f, playerY + 2.0f, 0.0f));
        Util::DrawText(program, fontTextureID, "Your Passengers! You Win", 0.3f, 0.02f, glm::vec3(playerX - 4.0f, playerY + 1.0f, 0.0f));
        break;
    }

    Util::DrawText(program, fontTextureID, "Open for Repairs", 0.2f, 0.02f, glm::vec3(-1.5f, -2.0f, 0.0f));
    
}