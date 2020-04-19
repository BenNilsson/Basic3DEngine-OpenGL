#include "GameObject.h"
#include "Camera.h"

GameObject::GameObject(std::string modelPath, Transform transform, Shader* shader) : mShader(shader), mTransform(transform)
{
	mModel = new Model((char*)modelPath.c_str());
}

GameObject::~GameObject()
{
	delete mModel;
	mModel = nullptr;
}

void GameObject::Update(float deltaTime)
{
	for (unsigned int i = 0; i < childObjects.size(); i++)
	{
		childObjects[i]->Update(deltaTime);
	}
}

void GameObject::Render()
{
	// Render Model
	mShader->use();

	// Set Shader's Colors
	mShader->setVec3("viewPos", Camera::GetInstance()->Position);
	mShader->setFloat("material.shininess", 32.0f);

	// View/Projection Transforms
	glm::mat4 projection = glm::perspective(glm::radians(Camera::GetInstance()->Zoom), (float)1920 / (float)1080, 0.1f, 100.0f);
	glm::mat4 view = Camera::GetInstance()->GetViewMatrix();
	mShader->setMat4("projection", projection);
	mShader->setMat4("view", view);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::mat4(1.0f);
	model = glm::scale(model, mTransform.scale);
	model = glm::translate(model, mTransform.position);
	mShader->setMat4("model", model);
	mModel->Render(*mShader);

	// Render Child Objects
	for (unsigned int i = 0; i < childObjects.size(); i++)
	{
		model = glm::scale(model, childObjects[i]->mTransform.scale);
		model = glm::translate(model, childObjects[i]->mTransform.position);
		childObjects[i]->mShader->setMat4("model", model);
		childObjects[i]->Render();
	}
}

void GameObject::AddChild(GameObject* child)
{
	childObjects.push_back(child);
}

void GameObject::RemoveChild(GameObject* child)
{
}
