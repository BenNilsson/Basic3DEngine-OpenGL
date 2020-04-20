#pragma once
#include <string>
#include "Shader.h"
#include "Structs.h"
#include "Model.h"
#include "GameComponent.h"

#include <vector>

class Scene;

class GameObject
{
public:
	GameObject(std::string modelPath, Transform transform, Shader* shader, Scene* scene);
	~GameObject();

	virtual void Update(float deltaTime);
	virtual void Render(glm::mat4 _model);
	virtual void HandleInput(GLFWwindow* window, float deltaTime);
	virtual void Destroy();

	void AddChild(GameObject* child);
	void RemoveChild(GameObject* child);

	void AddComponent(GameComponent* component);
	GameComponent* GetComponent(GameComponent* component);
	GameComponent* GetComponentInChild(GameComponent* component, int childIndex);


	Shader* mShader;
	Model* mModel;
	Transform mTransform;

protected:

	Scene* mScene;
	std::vector<GameObject*> childObjects;
	std::vector<GameComponent*> gameComponents;

};

