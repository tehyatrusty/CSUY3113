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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, ballMatrix, paddle1_matrix, paddle2_matrix, projectionMatrix;

glm::vec3 ball_position = glm::vec3(0, 0, 0);
glm::vec3 paddle1_position = glm::vec3(-4.8f, 0, 0);
glm::vec3 paddle1_movement = glm::vec3(0, 0, 0);
glm::vec3 paddle2_position = glm::vec3(4.8f, 0, 0);
glm::vec3 paddle2_movement = glm::vec3(0, 0, 0);

float paddle_speed = 2.0f;

float ball_width = 0.5;
float ball_height = 0.5;
float paddle_width = 0.4;
float paddle_height = 1.5;

bool move_right = true;
bool move_up = true;
bool wall_hit = true;

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Pong! - Press Enter To Start!", 
              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");

    viewMatrix = glm::mat4(1.0f);
    ballMatrix = glm::mat4(1.0f);
    paddle1_matrix = glm::mat4(1.0f);
    paddle2_matrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);

    glUseProgram(program.programID);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void ProcessInput() {
    paddle1_movement = glm::vec3(0);
    paddle2_movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_RETURN:
                wall_hit = false;
                break;
            }
            break;
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_W] && paddle1_position.y < 3) {
        paddle1_movement.y = 1.0f;
    }

    else if (keys[SDL_SCANCODE_S] && paddle1_position.y > -3) {
        paddle1_movement.y = -1.0f;
    }

    if (glm::length(paddle1_movement) > 1.0f) {
        paddle1_movement = glm::normalize(paddle1_movement);
    }

    if (keys[SDL_SCANCODE_UP] && paddle2_position.y < 3) {
        paddle2_movement.y = 1.0f;
    }

    else if (keys[SDL_SCANCODE_DOWN] && paddle2_position.y > -3) {
        paddle2_movement.y = -1.0f;
    }

    if (glm::length(paddle2_movement) > 1.0f) {
        paddle2_movement = glm::normalize(paddle2_movement);
    }

}

void BallCollision(float deltaTime) {
    float p1AndBallX = fabs(paddle1_position.x - ball_position.x) - 
        ((paddle_width + ball_width) / 2.0f);

    float p1AndBallY = fabs(paddle1_position.y - ball_position.y) - 
        ((paddle_height + ball_height) / 2.0f);

    float p2AndBallX = fabs(paddle2_position.x - ball_position.x) - 
        ((paddle_width + ball_width) / 2.0f);

    float p2AndBallY = fabs(paddle2_position.y - ball_position.y) - 
        ((paddle_height + ball_height) / 2.0f);

    if (p1AndBallX < 0 && p1AndBallY < 0) {
        move_right = true;
    }

    else if (p2AndBallX < 0 && p2AndBallY < 0) {
        move_right = false;
    }

    if (ball_position.x > 4.5 || ball_position.x < -4.5 ) {
        ball_position = glm::vec3(0, 0, 0);
        paddle1_position = glm::vec3(-4.8f, 0, 0);
        paddle2_position = glm::vec3(4.8f, 0, 0);
        move_right = true;
        move_up = true;
        wall_hit = true;
    }

    else if (ball_position.y > 3.50) {
        move_up = false;
    }
    else if (ball_position.y < -3.50) {
        move_up = true;
    }

}

float lastTicks = 0.9f;

void Update() {

    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;

    lastTicks = ticks;

    paddle1_position += paddle1_movement * paddle_speed * deltaTime;
    paddle2_position += paddle2_movement * paddle_speed * deltaTime;
    
    paddle1_matrix = glm::mat4(1.0f);
    paddle1_matrix = glm::translate(paddle1_matrix, paddle1_position);

    paddle2_matrix = glm::mat4(1.0f);
    paddle2_matrix = glm::translate(paddle2_matrix, paddle2_position);

    BallCollision(deltaTime);

    if (!wall_hit) {

        if (move_right) ball_position.x += 1.0f * deltaTime;
        else ball_position.x -= 1.0f * deltaTime;

        
        if (move_up) ball_position.y += 1.0f * deltaTime;
        else ball_position.y -= 1.0f * deltaTime;
        
    }

   ballMatrix = glm::mat4(1.0f);
   ballMatrix = glm::translate(ballMatrix, ball_position);

}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    float ballVertices[] = 
    { -0.25, -0.25, 0.25, -0.25, 0.25, 0.25, -0.25, -0.25, 0.25, 0.25, -0.25, 0.25 };

    float paddle1Vertices[] = 
    { -0.20, -0.75, 0.20, -0.75, 0.20, 0.75, -0.20, -0.75, 0.20, 0.75, -0.20, 0.75 };

    float paddle2Vertices[] = 
    { -0.20, -0.75, 0.20, -0.75, 0.20, 0.75, -0.20, -0.75, 0.20, 0.75, -0.20, 0.75 };

    program.SetModelMatrix(ballMatrix);
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, ballVertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
   
    
    program.SetModelMatrix(paddle1_matrix);
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, paddle1Vertices);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    

    program.SetModelMatrix(paddle2_matrix);
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, paddle2Vertices);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program.positionAttribute);

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
