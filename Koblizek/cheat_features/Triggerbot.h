#pragma once

#include "../cheat_sdk/UserCmd.h"

namespace Triggerbot {
	int UpdateActiveWeapon();
	void run(UserCmd*) noexcept;
}
