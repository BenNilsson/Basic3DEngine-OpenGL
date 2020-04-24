#include "PolarBear.h"
#include "Camera.h"

PolarBear::PolarBear(std::string modelPath, Transform transform, Shader* shader, Scene* scene) : GameObject(modelPath, transform, shader, scene), mRigidbody(RigidBody(glm::vec3(0.0f, 0.0f, 0.0f)))
{
	mRigidbody.mEnableGravity = false;
	mRigidbody.mPosition = transform.position;
	mSphereCollider = new SphereCollider(mRigidbody.mPosition + glm::vec3(0.0f), 0.35f);

	mRigidbody.mPosition = mSphereCollider->GetCenter();
	mIsMoving = false;

	mRigidbody.mCollider = mSphereCollider;
	AddComponent((GameComponent*)&mRigidbody);

	gfx = new GameObject((char*)"Objects/PolarBear/polarbear.obj", Transform(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.50f), glm::vec3(0.0f, 90.0f, 0.0f)), mShader, scene);
	AddChild(gfx);

	GameObject* fish2 = new GameObject((char*)"Objects/Fish/fish.obj", Transform(glm::vec3(0.3f, 0.8f, -0.65f), glm::vec3(1.50f), glm::vec3(0.0f)), mShader, scene);

	mMovementSpeed = 3.0f;
	mRotationAmount = 0.35f;
	

	if (PhysicsEngine::GetInstance() != 0)
		PhysicsEngine::GetInstance()->AddRigidbody(&mRigidbody);
}

PolarBear::~PolarBear()
{
	delete gfx;
	gfx = nullptr;
}

void PolarBear::Update(float deltaTime)
{
	// Update Collider's position
	mSphereCollider->mCenter = mRigidbody.GetPosition();

	GameObject::Update(deltaTime);
}

void PolarBear::FixedUpdate(float deltaTime)
{
	// Move forward and rotate, this will result in PolarBears moving around in a circle
	// Move forward
	mRigidbody.mVelocity = mTransform.forward * mMovementSpeed;

	mRigidbody.mRotation.y += 0.3f;
	if (mRigidbody.mRotation.y >= 360)
		mRigidbody.mRotation.y = 0;

	// Rotate
	if (GetIsMoving())
	{
		mRigidbody.mRotation.z += mRotationAmount;
		if (mRigidbody.mRotation.z >= 5.0f || mRigidbody.mRotation.z <= -5.0f)
			mRotationAmount *= -1;
	}

	// Set transform to be the rigidbody's transform
	// Note, this will make any chances to the game object's position/rotation invalid
	mTransform.position = mRigidbody.GetPosition();
	mTransform.rotation = mRigidbody.GetRotation();


}

void PolarBear::HandleInput(GLFWwindow* window, float deltaTime)
{

}

void PolarBear::Destroy()
{
	if (this != nullptr)
		GameObject::Destroy();
}
