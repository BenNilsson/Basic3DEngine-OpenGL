#include "Penguin.h"

Penguin::Penguin() : GameObject((char*)"Objects/Penguinv2/penguin.obj", Transform(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.0f), glm::vec3(1.0f)), new Shader("Shaders/multiple_lights.vs", "Shaders/multiple_lights.fs"))
{
}

Penguin::~Penguin()
{
}

void Penguin::Render()
{
}

void Penguin::Update()
{
}
