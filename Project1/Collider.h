#pragma once

#include "GameComponent.h"
#include "IntersectData.h"
#include <glm/glm.hpp>

class Collider : GameComponent
{
public:
	
	enum
	{
		COLLIDER_SPHERE,
		COLLIDER_BOX,
		COLLIDER_SIZE
	};

	Collider(int type) : mType(type) {}

	IntersectData Intersect(const Collider& other) const;

	virtual void Update(float deltaTime) {}
	virtual void Render() {}
	virtual void HandleInput(GLFWwindow* window, float deltaTime) {}

	virtual void Transform(const glm::vec3 translation) {}
	virtual glm::vec3 GetCenter() const { return glm::vec3(0.0f); }
	 
	inline int GetType() const { return mType; }
private:
	int mType;
};