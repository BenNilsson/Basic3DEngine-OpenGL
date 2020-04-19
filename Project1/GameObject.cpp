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
	model = glm::translate(model, mTransform.position);
	model = glm::scale(model, mTransform.scale);
	mShader->setMat4("model", model);
	mModel->Render(*mShader);
}
