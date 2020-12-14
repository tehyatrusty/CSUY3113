#include "menu.h"

Mix_Music* gameMusic;

void Menu::Initialize() {
    state.nextScene = -1;
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    gameMusic = Mix_LoadMUS("Retro-Sci-Fi-Planet_Looping.mp3");
    Mix_PlayMusic(gameMusic, -1);
}

void Menu::Update(float deltaTime) {

}

void Menu::Render(ShaderProgram* program) {

    GLuint fontTextureID = Util::LoadTexture("font.png");
    Util::DrawText(program, fontTextureID, "Space Transport!", 0.4f, 0.05f, glm::vec3(1.5f, -1.0f, 0.0f));
    Util::DrawText(program, fontTextureID, "Drop Off Your Passengers", 0.3f, 0.05f, glm::vec3(1.0f, -3.0f, 0.0f));
    Util::DrawText(program, fontTextureID, "Avoid Asteroids", 0.3f, 0.05f, glm::vec3(1.0f, -4.0f, 0.0f));
    Util::DrawText(program, fontTextureID, "Press Enter To Start! ", 0.3f, 0.05f, glm::vec3(1.0f, -5.0f, 0.0f));

}