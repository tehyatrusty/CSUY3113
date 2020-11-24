#pragma once
#include "scene.h"

class Level3 : public Scene {

public:
	void Initialize() override;
	void Update(float deltaTime) override;
	void Render(ShaderProgram* program) override;
};
