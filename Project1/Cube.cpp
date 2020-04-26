#include "Cube.h"

Cube::Cube(std::string modelPath, Transform transform, Shader* shader, Scene* scene) : GameObject(modelPath, transform, shader, scene), mRigidbody(RigidBody(glm::vec3(0.0f, 0.0f, 0.0f)))
{
	mRigidbody.mEnableGravity = false;
	mRigidbody.mPosition = transform.position;
	mSphereCollider = new SphereCollider(mRigidbody.mPosition + glm::vec3(0.0f), 0.4f);

	mRigidbody.mPosition = mSphereCollider->GetCenter();

	mRigidbody.mMass = 5.0f;

	mRigidbody.mCollider = mSphereCollider;
	AddComponent((GameComponent*)&mRigidbody);

	if (PhysicsEngine::GetInstance() != 0)
		PhysicsEngine::GetInstance()->AddRigidbody(&mRigidbody);
}

Cube::~Cube()
{
}

void Cube::Update(float deltaTime)
{
	// Update Collider's position
	mSphereCollider->mCenter = mRigidbody.GetPosition();

	GameObject::Update(deltaTime);
}

void Cube::FixedUpdate(float deltaTime)
{
	mTransform.position = mRigidbody.GetPosition();
	mTransform.rotation = mRigidbody.GetRotation();
}
