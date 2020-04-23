#include "Lamp.h"
#include "Camera.h"
#include "SceneManager.h"


Lamp::Lamp(std::string modelPath, Transform transform, Shader* shader, Scene* scene) : GameObject(modelPath, transform, shader, scene)
{
	glm::vec3 lightPos = mTransform.position + glm::vec3(0.0f, 3.25f, 2.0f);
	
	// Setup point light
	shader->setVec3("light.position", lightPos);
	shader->setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
	shader->setVec3("light.diffuse", 0.875f, 0.769f, 0.353f);
	//shader->setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
	shader->setVec3("light.specular", 0.25f, 0.25f, 0.25f);
	shader->setFloat("light.constant", 1.0f);
	shader->setFloat("light.linear", 0.09);
	shader->setFloat("light.quadratic", 0.032);

}

Lamp::~Lamp()
{
}


void Lamp::Update(float deltaTime)
{
	GameObject::Update(deltaTime);
}
