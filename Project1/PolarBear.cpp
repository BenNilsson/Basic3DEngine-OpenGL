#include "PolarBear.h"
#include "Camera.h"

#include <glm/gtx/compatibility.hpp>

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

	mMovementSpeed = 2.0f;
	mRotationAmount = 0.35f;

	// Setup patrol waypoints
	mWaypointIndex = 1;
	mWaypoints = std::vector<glm::vec3>
	{
		transform.position,
		glm::vec3(15.0f, 0.0f, 10.0f),
		glm::vec3(-3.0f, 0.0f, -16.0f)
	};
	mWayPointToMoveTo = mWaypoints[mWaypointIndex];

	if (PhysicsEngine::GetInstance() != 0)
		PhysicsEngine::GetInstance()->AddRigidbody(&mRigidbody);
}

PolarBear::~PolarBear()
{
	delete gfx;
	gfx = nullptr;
}

void PolarBear::Render(glm::mat4 _model)
{
	if (mShader == nullptr) return;

	// Render Model
	mShader->use();

	// View/Projection Transforms
	glm::mat4 projection = glm::perspective(glm::radians(Camera::GetInstance()->Zoom), (float)Camera::GetInstance()->SCREEN_WIDTH / (float)Camera::GetInstance()->SCREEN_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = Camera::GetInstance()->GetViewMatrix();
	mShader->setMat4("projection", projection);
	mShader->setMat4("view", view);

	glm::mat4 model = glm::mat4(1.0f);
	// Translate
	model = glm::translate(model, mTransform.position);
	// Rotation - x, y, z
	model = glm::rotate(model, glm::radians(mTransform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(mTransform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(mTransform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	// Scale
	model = glm::scale(model, mTransform.scale);

	// Set forward vector
	mTransform.forward = normalize(glm::vec3(view[2]) * glm::vec3(1, 1, -1));
	
	model = _model * model;
	mShader->setMat4("model", model);

	transformationMatrix = model;

	// Render Model
	if (mModel != nullptr)
		mModel->Render(*mShader);

	// Render Child Objects
	for (unsigned int i = 0; i < childObjects.size(); i++)
	{
		childObjects[i]->Render(model);
	}

	// Render components
	for (unsigned int i = 0; i < gameComponents.size(); i++)
	{
		gameComponents[i]->Render();
	}
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
	
	// Face next waypoint
	

	// Move bear towards waypoint if not within distance
	if (glm::distance(mRigidbody.mPosition, mWayPointToMoveTo) > 0.5f && !mHasReachedWaypoint)
	{
		// Move towards point
		glm::vec3 interp = glm::mix(mRigidbody.mPosition, mWayPointToMoveTo, deltaTime * mMovementSpeed);
		glm::vec3 move = glm::vec3(interp.x, interp.y, interp.z);
		mRigidbody.mPosition = move;
		if (glm::distance(mRigidbody.mPosition, mWayPointToMoveTo) < 0.5f)
			mHasReachedWaypoint = true;
	}

	// Move next checkpoint
	if (mHasReachedWaypoint)
	{
		// Reset list if needed
		mWaypointIndex++;
		if (mWaypointIndex == mWaypoints.size()) mWaypointIndex = 0;
		mWayPointToMoveTo = mWaypoints[mWaypointIndex];

		// Reset progression
		mHasReachedWaypoint = false;
	}

	std::cout << mRigidbody.mPosition.x << " | " << mRigidbody.mPosition.y << " | " << mRigidbody.mPosition.z << "  -  " << mWayPointToMoveTo.x << " | " << mWayPointToMoveTo.y << " | " << mWayPointToMoveTo.z << std::endl;
	/*
	mRigidbody.mRotation.y += 0.3f;
	if (mRigidbody.mRotation.y >= 360)
		mRigidbody.mRotation.y = 0;
		*/

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
