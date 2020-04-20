#pragma once

#include "Model.h"
#include "Camera.h"
#include "Scene.h"
#include "Shader.h"
#include "GameObject.h"
#include "Penguin.h"

class SceneLevel1 : Scene
{
public:
	SceneLevel1(GLFWwindow* window);
	~SceneLevel1();

	void Render();
	void Update(float deltaTime);
private:
	
	void HandleInputs(GLFWwindow* window, float deltaTime);
	void ConfigureLightShader();

	Shader* lightingShader;
	Shader* modelShader;

	GLFWwindow* mWindow;
	Penguin* penguin;
};