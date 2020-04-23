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
	void FixedUpdate(float deltaTime);
	void HandleInputs(GLFWwindow* window, float deltaTime);

	// Callbacks
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double offset, double yoffset);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
	
	void ConfigureLightShader();

	Shader* lightingShader;
	Shader* modelShader;
	Shader* pointLightShader;

	GLFWwindow* mWindow;
};