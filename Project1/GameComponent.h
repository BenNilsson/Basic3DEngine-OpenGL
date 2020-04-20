#pragma once

#include <GLFW/glfw3.h>

class GameComponent
{
public:
	GameComponent() {}
	~GameComponent() {}

	virtual void Update(float deltaTime) {}
	virtual void Render() {}
	virtual void HandleInput(GLFWwindow* window, float deltaTime) {}
};

