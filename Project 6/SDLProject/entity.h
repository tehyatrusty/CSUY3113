#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include <SDL_mixer.h>
#include "map.h"
#include <vector>

enum EntityType {PLAYER, PLATFORM, ENEMY};

enum AIType { ALIEN, ASTEROID};
enum AIState { FLYING, CHASING, WAITING};

class Entity {
public:
    EntityType entityType;
    AIType aiType;
    AIState aiState;

    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;

    float width = 1;
    float height = 1;

    bool jump = false;
    float jumpPower = 8;

    float speed;

    GLuint textureID;

    glm::mat4 modelMatrix;

    int* animFull = NULL;
    int* animDamaged1 = NULL;
    int* animDamaged2 = NULL;
    int* animDamaged3 = NULL;

    int* animIndices = NULL;
    int animFrames = 0;
    int animIndex = 0;
    float animTime = 0;
    int animCols = 0;
    int animRows = 0;

    int health = 3;
    float player_rotate = 0.0f;

    bool isActive = true;

    bool reachedRestArea = false;
    bool reachedDestination = false;
    bool alreadyVisited = false;
    std::vector<int> destinations;

    int planetsLeft = 6;

    bool collidedObj = false;

    bool enteredGalaxySpace = false;


    Entity();
    
    bool CheckCollision(Entity* other);
    void CheckCollisionsY(Entity* objects, int objectCount);
    void CheckCollisionsX(Entity* objects, int objectCount);
    void CheckCollisionsX(Map* map);
    void CheckCollisionsY(Map* map);

    void Update(float deltaTime, Entity* player, Entity* objects, int objectCount, Map* map);
    void Render(ShaderProgram* program);
    void DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index);

    void AI(Entity* player);
    void AIAsteroid();
    void AIAlien(Entity* player);
};
