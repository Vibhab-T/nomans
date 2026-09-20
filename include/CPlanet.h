#pragma once

class CPlanet {
public:
	float distanceFromSun;
	bool isRotatingClockwise;
	float revolutionSpeed;
	float rotationSpeed;
	float angleFromCenter;

  CPlanet(const float d, const bool r, const float rev, const float rot, const float a) : distanceFromSun(d), isRotatingClockwise(r), revolutionSpeed(rev), rotationSpeed(rot), angleFromCenter(a) {}
};
