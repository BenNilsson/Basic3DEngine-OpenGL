#pragma once
#include "GameObject.h"

class Penguin : GameObject
{
public:

	Penguin(std::string modelPath, Transform transform, Shader* shader, Scene* scene);
	~Penguin();

	void Update(float deltaTime);
	void HandleInput(GLFWwindow* window, float deltaTime);

	void Destroy();

private:

	float mMovementSpeed = 3.0f;

	GameObject* penguinGFX;
	GameObject* ak47;
	glm::vec3 startPos;

};

