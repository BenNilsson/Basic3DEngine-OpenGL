#pragma once
#include <string>
#include "Shader.h"
#include "Structs.h"
#include "Model.h"

class GameObject
{
public:
	GameObject(std::string modelPath, Transform transform, Shader* shader);
	~GameObject();

	virtual void Update(float deltaTime);
	virtual void Render();
protected:

	Model* mModel;
	Shader* mShader;
	Transform mTransform;
};

