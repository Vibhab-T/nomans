#pragma once

class CPlayerInput {
public:
	bool forward{false};
	bool backward{false};
	bool left{false};
	bool right{false};
	bool up{false};
	bool down{false};
	bool interact{false};

	CPlayerInput() = default;
};
