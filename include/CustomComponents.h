#pragma once

class CPlanet {
public:
	float distanceFromSun;
	bool isRotatingClockwise;

  CPlanet(const float d, const bool r) : distanceFromSun(d), isRotatingClockwise(r) {}
};
