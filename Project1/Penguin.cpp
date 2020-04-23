#include "Penguin.h"
#include "Camera.h"

Penguin::Penguin(std::string modelPath, Transform transform, Shader* shader, Scene* scene) : GameObject(modelPath, transform, shader, scene), mRigidbody(RigidBody(glm::vec3(1.0f, 0.0f, 0.5f)))
{
	mRigidbody.mEnableGravity = false;
	mRigidbody.mPosition = transform.position;
	mSphereCollider = new SphereCollider(mRigidbody.mPosition + glm::vec3(0.0f), 0.35f);
	
	mRigidbody.mPosition = mSphereCollider->GetCenter();

	mRigidbody.mVelocity = glm::vec3(1.5f, 0.0f, 0.0f);

	mRigidbody.mCollider = mSphereCollider;
	AddComponent((GameComponent*)&mRigidbody);

	penguinGFX = new GameObject((char*)"Objects/Penguinv2/Penguin.obj", Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.50f), glm::vec3(0.0f, -90.0f, 0.0f)), mShader, scene);
	AddChild(penguinGFX);

	ak47 = new GameObject((char*)"Objects/ak47/ak_47.obj", Transform(glm::vec3(0.3f, 0.8f, 0.5f), glm::vec3(0.50f), glm::vec3(0.0f)), mShader, scene);
	penguinGFX->AddChild(ak47);

	mMovementSpeed = 5.0f;
	//mRigidbody.mVelocity *= mMovementSpeed;

	mRotationAmount = 1.5f;
	//mRigidbody.mVelocity = mTransform.forward * mMovementSpeed;

	if (PhysicsEngine::GetInstance() != 0)
		PhysicsEngine::GetInstance()->AddRigidbody(&mRigidbody);
}

Penguin::~Penguin()
{
	delete penguinGFX;
	penguinGFX = nullptr;

	delete ak47;
	ak47 = nullptr;
}

void Penguin::Update(float deltaTime)
{
	// Update Collider's position
	mSphereCollider->mCenter = mRigidbody.GetPosition();

	GameObject::Update(deltaTime);

	

	//std::cout << mRigidbody.mPosition.x << " | " << mRigidbody.mPosition.y << " | " << mRigidbody.mPosition.z << " | " << std::endl;
}

void Penguin::FixedUpdate(float deltaTime)
{
	// Move forward and rotate, this will result in penguins moving around in a circle
	// Move forward
	
	// Rotate
	mRigidbody.mRotation.y += mRotationAmount;
	if (mRigidbody.mRotation.y >= 360)
		mRigidbody.mRotation.y = 0;
	

	
	// Set transform to be the rigidbody's transform
	// Note, this will make any chances to the game object's position/rotation invalid
	mTransform.position = mRigidbody.GetPosition();
	mTransform.rotation = mRigidbody.GetRotation();
	

}

void Penguin::HandleInput(GLFWwindow* window, float deltaTime)
{
	
}

void Penguin::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		// Invert rotation amount
		mRigidbody.mVelocity *= -1;
	} 

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		Destroy();
	}
}

void Penguin::Destroy()
{
	if(this != nullptr)
		GameObject::Destroy();
}
