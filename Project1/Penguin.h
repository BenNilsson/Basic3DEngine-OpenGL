#pragma once
#include "GameObject.h"

class Penguin : GameObject
{
public:

	Penguin(std::string modelPath, Transform transform, Shader* shader);
	~Penguin();

	void Render();
	void Update(float deltaTime);

private:

};

