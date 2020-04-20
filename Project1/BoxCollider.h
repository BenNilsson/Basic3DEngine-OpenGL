#pragma once

#include "GameComponent.h"

class BoxCollider : GameComponent
{
public:
	BoxCollider()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		width = 0.0f;
		height = 0.0f;
		depth = 0.0f;
	}

	BoxCollider(float x, float y, float z, float width, float height, float depth)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->width = width;
		this->height = height;
		this->depth = depth;
	}

	float x;
	float y;
	float z;

	float width;
	float height;
	float depth;

};