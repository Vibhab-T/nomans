#pragma once

#include <cstdint>
#include <vector>
#include "Icosphere.h"

class StarSystem {

public:
	std::vector<Icosphere> planets;

	StarSystem(uint8_t planetNums);
	~StarSystem();

	void update();
};
