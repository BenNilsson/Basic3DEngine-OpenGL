#include "Penguin.h"
#include "Camera.h"

Penguin::Penguin(std::string modelPath, Transform transform, Shader* shader) : GameObject(modelPath, transform, shader)
{
}

Penguin::~Penguin()
{
}

void Penguin::Render()
{
	GameObject::Render();
}

void Penguin::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}
