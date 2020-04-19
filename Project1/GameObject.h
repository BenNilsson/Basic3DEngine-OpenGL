#pragma once
#include <string>
#include "Shader.h"
#include "Structs.h"
#include "Model.h"

#include <vector>

class GameObject
{
public:
	GameObject(std::string modelPath, Transform transform, Shader* shader);
	~GameObject();

	virtual void Update(float deltaTime);
	virtual void Render();

	void AddChild(GameObject* child);
	void RemoveChild(GameObject* child);

protected:

	Model* mModel;
	Shader* mShader;
	Transform mTransform;

	std::vector<GameObject*> childObjects;
};

