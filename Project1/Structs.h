#pragma once

#include <glm/glm.hpp>

struct Transform
{
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;

	glm::vec3 forward;

	Transform(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
	{
		this->position = position;
		this->scale = scale;
		this->rotation = rotation;
		this->forward = glm::vec3(0.0f);
	}
};