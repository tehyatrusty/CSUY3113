#include "entity.h"

Mix_Chunk* asteroidHit;
Mix_Chunk* gameWon;

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;

    modelMatrix = glm::mat4(1.0f);
}

bool Entity::CheckCollision(Entity* other) {
    if (isActive == false || other->isActive == false) return false;

    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);

    if (xdist < 0 && ydist < 0) return true;
    return false;
}

void Entity::CheckCollisionsY(Entity* objects, int objectCount) {
    for (int i = 0; i < objectCount; i++) {
        Entity* object = &objects[i];
        
        if (this == object) continue;

        if (CheckCollision(object)) {
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
            if (velocity.y > 0) {
                position.y -= penetrationY;
                velocity.y = 0;
                collidedObj = true;
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
                collidedObj = true;
            }

            if (object->aiType == ASTEROID && aiType == PLAYER) object->isActive = false;
        }
    }
}

void Entity::CheckCollisionsY(Map* map)
{
    // Probes for tiles
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
    glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);

    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);

    float penetration_x = 0;
    float penetration_y = 0;

    if (map->IsSolid(top, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        
        int tile = map->ReturnTile(top, &penetration_x, &penetration_y);
        if (tile == 1) reachedRestArea = true;
        else {
            for (size_t i = 0; i < destinations.size(); i++) {
                if (destinations[i] == tile) alreadyVisited = true;
            }
            if (!alreadyVisited) {
                reachedDestination = true;
                destinations.push_back(tile);
            }
        }
    }
    else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        
        int tile = map->ReturnTile(top_left, &penetration_x, &penetration_y);
        if (tile == 1) reachedRestArea = true;
        else {
            for (size_t i = 0; i < destinations.size(); i++) {
                if (destinations[i] == tile) alreadyVisited = true;
            }
            if (!alreadyVisited) {
                reachedDestination = true;
                destinations.push_back(tile);
            }
        }
    }
    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        
        int tile = map->ReturnTile(top_right, &penetration_x, &penetration_y);
        if (tile == 1) reachedRestArea = true;
        else {
            reachedDestination = true;
            for (size_t i = 0; i < destinations.size(); i++) {
                if (destinations[i] == tile) alreadyVisited = true;
            }
            if (!alreadyVisited) {
                reachedDestination = true;
                destinations.push_back(tile);
            }
        }
    }

    if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        
        int tile = map->ReturnTile(bottom, &penetration_x, &penetration_y);
        if (tile == 1) reachedRestArea = true;
        else {
            for (size_t i = 0; i < destinations.size(); i++) {
                if (destinations[i] == tile) alreadyVisited = true;
            }
            if (!alreadyVisited) {
                reachedDestination = true;
                destinations.push_back(tile);
            }
        }
    }
    else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        
        int tile = map->ReturnTile(bottom_left, &penetration_x, &penetration_y);
        if (tile == 1) reachedRestArea = true;
        else {
            for (size_t i = 0; i < destinations.size(); i++) {
                if (destinations[i] == tile) alreadyVisited = true;
            }
            if (!alreadyVisited) {
                reachedDestination = true;
                destinations.push_back(tile);
            }
        }
    }
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        
        int tile = map->ReturnTile(bottom_right, &penetration_x, &penetration_y);
        if (tile == 1) reachedRestArea = true;
        else {
            for (size_t i = 0; i < destinations.size(); i++) {
                if (destinations[i] == tile) alreadyVisited = true;
            }
            if (!alreadyVisited) {
                reachedDestination = true;
                destinations.push_back(tile);
            }
        }
    }

}

void Entity::CheckCollisionsX(Entity* objects, int objectCount) {
    for (int i = 0; i < objectCount; i++) {
        Entity* object = &objects[i];

        if (this == object) continue;

        if (CheckCollision(object)) {
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            if (velocity.x > 0) {
                position.x -= penetrationX;
                velocity.x = 0;
                collidedObj = true;
            }
            else if (velocity.x < 0) {
                position.x += penetrationX;
                velocity.x = 0;
                collidedObj = true;
            }

            if (object->aiType == ASTEROID && aiType == PLAYER) object->isActive = false;
        }
    }
}

void Entity::CheckCollisionsX(Map* map) {
    // Probes for tiles
    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);

    float penetration_x = 0;
    float penetration_y = 0;

    if (map->IsSolid(left, &penetration_x, &penetration_y) && velocity.x < 0) {
        position.x += penetration_x;
        velocity.x = 0;
        
        int tile = map->ReturnTile(left, &penetration_x, &penetration_y);
        if (tile == 1) reachedRestArea = true;
        else {
            for (size_t i = 0; i < destinations.size(); i++) {
                if (destinations[i] == tile) alreadyVisited = true;
            }
            if (!alreadyVisited) {
                reachedDestination = true;
                destinations.push_back(tile);
            }
        }
    }

    if (map->IsSolid(right, &penetration_x, &penetration_y) && velocity.x > 0) {
        position.x -= penetration_x;
        velocity.x = 0;
        
        int tile = map->ReturnTile(right, &penetration_x, &penetration_y);
        if (tile == 1) reachedRestArea = true;
        else {
            for (size_t i = 0; i < destinations.size(); i++) {
                if (destinations[i] == tile) alreadyVisited = true;
            }
            if (!alreadyVisited) {
                reachedDestination = true;
                destinations.push_back(tile);
            }
               
        }
    }
}

void Entity::AIAsteroid() {
    if (position.x > 13) {
        movement.x = -1;
    }
    
    else if (position.x < -13) {
        movement.x = 1;
    }

    if (position.y > 1) {
        movement.y = -1;
    }

    else if (position.y < -14) {
        movement.y = 1;
    }
    
    
}

void Entity::AIAlien(Entity* player) {
    switch (aiState) {
        case FLYING:
            if (glm::distance(position, player->position) < 3.0f) {
                aiState = CHASING;
            }
            break;
        
        case CHASING:
            if (player->position.x < position.x) {
                //movement = glm::vec3(-1, 0, 0);
            }
            
            else {
                //movement = glm::vec3(1, 0, 0);
            }
            
            break;
        
        case WAITING:
            if (glm::distance(position, player->position) < 3.0f) {
                aiState = CHASING;
            }
            break;
    }
}

void Entity::AI(Entity* player) {

    switch (aiType) {
    case ASTEROID:
        AIAsteroid();
        break;

    case ALIEN:
        AIAlien(player);
        break;
    }

}


void Entity::Update(float deltaTime, Entity* player, Entity* objects, int objectCount, Map* map) {
    if (!isActive) return;

    asteroidHit = Mix_LoadWAV("Hull-Breach-4.wav");
    
    reachedRestArea = false;
    reachedDestination = false;
    alreadyVisited = false;
    collidedObj = false;

    if (entityType == ENEMY) {
        AI(player);
    }
    
    if (animIndices != NULL) {

        if (health == 0) animIndex = 0;
        else if (movement.x == 0 && movement.y == 0) {
            animIndex = 1;
        }
        else animIndex = 2;
    }

    /* 
    if (jump) {
        jump = false;
        velocity.y += jumpPower;
    }
    */

    velocity.x = movement.x * speed;
    velocity.y = movement.y * speed;
    //velocity += acceleration * deltaTime;

    position.y += velocity.y * deltaTime; 
    CheckCollisionsY(map);
    CheckCollisionsY(objects, objectCount); 

    position.x += velocity.x * deltaTime; 
    CheckCollisionsX(map);
    CheckCollisionsX(objects, objectCount); 
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);

    if (entityType == PLAYER) {
        if (reachedDestination) {
            while (health < 3) {
                health++;
                switch (health) {
                case 3:
                    animIndices = animFull;
                    break;
                case 2:
                    animIndices = animDamaged1;
                    break;
                case 1:
                    animIndices = animDamaged2;
                    break;
                }
            }
            planetsLeft--;
        }

        else if (reachedRestArea) {
            switch (health) {
            case 2:
                health++;
                animIndices = animFull;
                break;
            case 1:
                health++;
                animIndices = animDamaged1;
                break;
            }
        }

        else if (collidedObj) {
            Mix_PlayChannel(-1, asteroidHit, 0);
            health -= 1;
            switch (health) {
            case 2:
                animIndices = animDamaged1;
                break;
            case 1:
                animIndices = animDamaged2;
                break;
            case 0:
                animIndices = animDamaged3;
                isActive = false;
                break;
            }
        }

        // updates the rotation of the spaceship texture
        if (movement.x > 0 && movement.y > 0) player_rotate = 315.0f;
        else if (movement.x < 0 && movement.y < 0) player_rotate = 135.0f;
        else if (movement.x > 0 && movement.y < 0) player_rotate = 225.0f;
        else if (movement.x < 0 && movement.y > 0) player_rotate = 45.0f;
        else if (movement.x > 0 && movement.y == 0) player_rotate = -90.0f;
        else if (movement.x < 0 && movement.y == 0) player_rotate = 90.0f;
        else if (movement.x == 0 && movement.y > 0) player_rotate = 0.0f;
        else if (movement.x == 0 && movement.y < 0) player_rotate = 180.0f;
        
        modelMatrix = glm::rotate(modelMatrix, glm::radians(player_rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    }
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index)
{
    float u = (float)(index % animCols) / (float)animCols;
    float v = (float)(index / animCols) / (float)animRows;

    float width = 1.0f / (float)animCols;
    float height = 1.0f / (float)animRows;

    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v };

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram* program) {

    if (!isActive) return;

    program->SetModelMatrix(modelMatrix);

    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
        return;
    }

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}
