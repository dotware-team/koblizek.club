#pragma once

struct UserCmd;

namespace EnginePrediction
{
	void run(UserCmd* cmd) noexcept;
	void end() noexcept;
	int getFlags() noexcept;
}