#pragma once

#include <glm/glm.hpp>

struct Transform
{
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;

	Transform(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
	{
		this->position = position;
		this->scale = scale;
		this->rotation = rotation;
	}
};