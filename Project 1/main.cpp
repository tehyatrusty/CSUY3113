#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

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
glm::mat4 viewMatrix, cloudMatrix, heartMatrix, projectionMatrix;

float cloud_x = -4;
float cloud_y = 3;
float heart_x = -4;
float heart_y = 3;
float heart_rotate = 0;
GLuint cloudTextureID, heartTextureID;


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
	displayWindow = SDL_CreateWindow("Project 1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif
	program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
	
	viewMatrix = glm::mat4(1.0f);
	cloudMatrix = glm::mat4(1.0f);
	heartMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);

	glUseProgram(program.programID);
	
	glClearColor(0.0f, 0.7f, 1.0f, 0.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	cloudTextureID = LoadTexture("cloud.png");
	heartTextureID = LoadTexture("heart.png");

}

void ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			gameIsRunning = false;
		}
	}
}

float lastTicks = 0.9f;

void Update() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float deltaTime = ticks - lastTicks;
	lastTicks = ticks;
	
	cloud_x += 1.0f * deltaTime;
	
	if (cloud_x > 4.0f) {
		cloud_x = -4.0f;
		cloud_y -= 0.5f;
	}
	
	if (cloud_y == 1.0f) {
		cloud_y = 3.0f;
	}

	heart_y -= 1.0f * deltaTime;
	heart_rotate += -90.0f * deltaTime;
	
	if (heart_y < -3.0f) {
		heart_x = cloud_x;
		heart_y = cloud_y;
	}

	cloudMatrix = glm::mat4(1.0f);
	cloudMatrix = glm::translate(cloudMatrix, glm::vec3(cloud_x, cloud_y, 0.0f));
	cloudMatrix = glm::scale(cloudMatrix, glm::vec3(3.0f, 2.0f, 2.0f));

	heartMatrix = glm::mat4(1.0f);
	heartMatrix = glm::translate(heartMatrix, glm::vec3(heart_x, heart_y, 0.0f));
	heartMatrix = glm::rotate(heartMatrix, glm::radians(heart_rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	
}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	program.SetModelMatrix(cloudMatrix);

	float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
	float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program.positionAttribute);
	glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program.texCoordAttribute);

	program.SetModelMatrix(heartMatrix);
	glBindTexture(GL_TEXTURE_2D, heartTextureID);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	
	program.SetModelMatrix(cloudMatrix);
	glBindTexture(GL_TEXTURE_2D, cloudTextureID);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program.positionAttribute);
	glDisableVertexAttribArray(program.texCoordAttribute);

	SDL_GL_SwapWindow(displayWindow);
}

int main(int argc, char* argv[]) {
	Initialize();

	while (gameIsRunning) {
		ProcessInput();
		Update();
		Render();
	}

	SDL_Quit();
	return 0;
}