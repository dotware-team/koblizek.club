#pragma once

#include "../cheat_sdk/UserCmd.h"

namespace Aimbot {
	Vector calculateRelativeAngle(const Vector& source, const Vector& destination, const Vector& viewAngles) noexcept;
	int UpdateActiveWeapon();
	void run(UserCmd*) noexcept;
}
