#pragma once

class IntersectData
{
public:
	IntersectData(const bool doesIntersect, const float distance) : mDoesIntersect(doesIntersect), mDistance(distance) {}

	inline bool GetDoesIntersect() const { return mDoesIntersect; }
	inline float GetDistance() const { return mDistance; }

private:
	const bool mDoesIntersect;
	const float mDistance;
};