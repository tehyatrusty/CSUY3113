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
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.h"

#define PLATFORM_COUNT 17
#define GOAL_COUNT 2

struct GameState {
    Entity* player;
    Entity* platforms;
    Entity* goals;
};

GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);

    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(image);
    return textureID;
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Project 3!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0.0f, 0.3f, 0.5f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Initialize Game Objects

    GLuint textureSheetTextureID = LoadTexture("sheet.png");

    // Initialize Player
    state.player = new Entity();
    state.player->position = glm::vec3(-2, 5, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0,-0.9f, 0);
    state.player->speed = 1.5f;
    state.player->textureID = textureSheetTextureID;

    state.player->textureIndex = 24;

    state.player->height = 0.8;
    state.player->width = 0.8;

    // Initialize Platforms
    state.platforms = new Entity[PLATFORM_COUNT];

    state.platforms[0].textureID = textureSheetTextureID;
    state.platforms[0].position = glm::vec3(-5, -3.25, 0);
    state.platforms[0].textureIndex = 3;

    state.platforms[1].textureID = textureSheetTextureID;
    state.platforms[1].position = glm::vec3(-4, -3.25, 0);
    state.platforms[1].textureIndex = 3;

    state.platforms[2].textureID = textureSheetTextureID;
    state.platforms[2].position = glm::vec3(-3, -3.25, 0);
    state.platforms[2].textureIndex = 3;

    state.platforms[3].textureID = textureSheetTextureID;
    state.platforms[3].position = glm::vec3(-2, -3.25, 0);
    state.platforms[3].textureIndex = 3;

    state.platforms[4].textureID = textureSheetTextureID;
    state.platforms[4].position = glm::vec3(-1, -3.25, 0);
    state.platforms[4].textureIndex = 3;

    state.platforms[5].textureID = textureSheetTextureID;
    state.platforms[5].position = glm::vec3(0, -3.25, 0);
    state.platforms[5].textureIndex = 3;

    state.platforms[6].textureID = textureSheetTextureID;
    state.platforms[6].position = glm::vec3(1, -3.25, 0);
    state.platforms[6].textureIndex = 3;

    state.platforms[7].textureID = textureSheetTextureID;
    state.platforms[7].position = glm::vec3(4, -3.25, 0);
    state.platforms[7].textureIndex = 3;

    state.platforms[8].textureID = textureSheetTextureID;
    state.platforms[8].position = glm::vec3(5, -3.25, 0);
    state.platforms[8].textureIndex = 3;

    state.platforms[9].textureID = textureSheetTextureID;
    state.platforms[9].position = glm::vec3(5, 0, 0);
    state.platforms[9].textureIndex = 22;

    state.platforms[10].textureID = textureSheetTextureID;
    state.platforms[10].position = glm::vec3(4, 0, 0);
    state.platforms[10].textureIndex = 22;

    state.platforms[11].textureID = textureSheetTextureID;
    state.platforms[11].position = glm::vec3(3, 0, 0);
    state.platforms[11].textureIndex = 22;

    state.platforms[12].textureID = textureSheetTextureID;
    state.platforms[12].position = glm::vec3(2, 0, 0);
    state.platforms[12].textureIndex = 22;

    state.platforms[13].textureID = textureSheetTextureID;
    state.platforms[13].position = glm::vec3(-2, 1.0, 0);
    state.platforms[13].textureIndex = 22;

    state.platforms[14].textureID = textureSheetTextureID;
    state.platforms[14].position = glm::vec3(-3, 1.0, 0);
    state.platforms[14].textureIndex = 22;

    state.platforms[15].textureID = textureSheetTextureID;
    state.platforms[15].position = glm::vec3(-4, 1.0, 0);
    state.platforms[15].textureIndex = 22;

    state.platforms[16].textureID = textureSheetTextureID;
    state.platforms[16].position = glm::vec3(-5, 1.0, 0);
    state.platforms[16].textureIndex = 22;


    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Update(0, NULL, 0, NULL, NULL);
    }

    // Initialize Goal Platforms
    state.goals = new Entity[GOAL_COUNT];

    state.goals[0].textureID = textureSheetTextureID;
    state.goals[0].textureIndex = 6;
    state.goals[0].position = glm::vec3(2, -3.25, 0);
    state.goals[0].height = 0.3;
    state.goals[0].Update(0, NULL, 0, NULL, NULL);

    state.goals[1].textureID = textureSheetTextureID;
    state.goals[1].textureIndex = 7;
    state.goals[1].position = glm::vec3(3, -3.25, 0);
    state.goals[1].height = 0.3;
    state.goals[1].Update(0, NULL, 0, NULL, NULL);
    
}

void ProcessInput() {

    state.player->movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;
        }
        
    }
        const Uint8* keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_LEFT]) {
            state.player->movement.x = -1.0f;
            
        }

        else if (keys[SDL_SCANCODE_RIGHT]) {
            state.player->movement.x = 1.0f;
         
        }

        if (glm::length(state.player->movement) > 1.0f) {
            state.player->movement = glm::normalize(state.player->movement);
        }
    

}


#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) {
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
        state.player->Update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT, state.goals, GOAL_COUNT);

        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;
}

void DrawText(ShaderProgram* program, GLuint fontTextureID, std::string text,
    float size, float spacing, glm::vec3 position) {

    float width = 1.0f / 16.0f;
    float height = 1.0f / 16.0f;

    std::vector<float> vertices;
    std::vector<float> texCoords;

    for (int i = 0; i < text.size(); i++) {

        int index = (int)text[i];
        float offset = (size + spacing) * i;
        float u = (float)(index % 16) / 16.0f;
        float v = (float)(index / 16) / 16.0f;

        vertices.insert(vertices.end(), {
             offset + (-0.5f * size), 0.5f * size,
             offset + (-0.5f * size), -0.5f * size,
             offset + (0.5f * size), 0.5f * size,
             offset + (0.5f * size), -0.5f * size,
             offset + (0.5f * size), 0.5f * size,
             offset + (-0.5f * size), -0.5f * size,
            });
        
        texCoords.insert(texCoords.end(), {
        u, v,
        u, v + height,
        u + width, v,
        u + width, v + height,
        u + width, v,
        u, v + height,
            });

    }

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    program->SetModelMatrix(modelMatrix);

    glUseProgram(program->programID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
    glEnableVertexAttribArray(program->texCoordAttribute);

    glBindTexture(GL_TEXTURE_2D, fontTextureID);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int i = 0; i < PLATFORM_COUNT; i++) {
        state.platforms[i].Render(&program);
    }

    for (int i = 0; i < GOAL_COUNT; i++) {
        state.goals[i].Render(&program);
    }

    state.player->Render(&program);

    if (state.player->collided) {
        GLuint fontTextureID = LoadTexture("font2.png");
        DrawText(&program, fontTextureID, "Mission Failed", 0.5f, 0.05f, glm::vec3(-3.5f, -0.5f, 0.0f));
    }

    else if (state.player->goalHit) {
        GLuint fontTextureID = LoadTexture("font2.png");
        DrawText(&program, fontTextureID, "Mission Successful", 0.5f, 0.05f, glm::vec3(-4.7f, -0.5f, 0.0f));
    }

    SDL_GL_SwapWindow(displayWindow);
}


void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }


    Shutdown();
    return 0;
}
