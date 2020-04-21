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

	// Callbacks
	virtual void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	virtual void scroll_callback(GLFWwindow* window, double offset, double yoffset);
	virtual void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	void AddChild(GameObject* child);
	void RemoveChild(GameObject* child);

	void AddComponent(GameComponent* component);
	GameComponent* GetComponent(GameComponent* component);
	GameComponent* GetComponentInChild(GameComponent* component, int childIndex);


	Shader* mShader;
	Model* mModel;
	Transform mTransform;

protected:

	std::vector<GameObject*> childObjects;
	Scene* mScene;
	std::vector<GameComponent*> gameComponents;

};

