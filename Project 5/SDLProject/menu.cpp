#include "menu.h"

Mix_Music* menuMusic;

void Menu::Initialize() {
    state.nextScene = -1;
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    menuMusic = Mix_LoadMUS("dooblydoo.mp3");
    Mix_PlayMusic(menuMusic, -1);
}

void Menu::Update(float deltaTime) {

}

void Menu::Render(ShaderProgram* program) {
    
    GLuint fontTextureID = Util::LoadTexture("font2.png");
    Util::DrawText(program, fontTextureID, "Escape The Ghosts!", 0.4f, 0.05f, glm::vec3(-4.0f, 1.0f, 0.0f));
    Util::DrawText(program, fontTextureID, "Press Enter To Start! ", 0.4f, 0.05f, glm::vec3(-4.5f, -1.0f, 0.0f));
    
}