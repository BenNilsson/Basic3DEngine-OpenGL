#pragma once

#include <glm/glm.hpp>

class IntersectData
{
public:
	IntersectData(const bool doesIntersect, const glm::vec3 direction) :
		mDoesIntersect(doesIntersect),
		mDirection(direction) {}

	inline bool GetDoesIntersect() const { return mDoesIntersect; }
	inline float GetDistance() const { return mDirection.length(); }
	inline const glm::vec3& GetDirection() const { return mDirection; }

private:
	const bool mDoesIntersect;
	
	const glm::vec3 mDirection;

};