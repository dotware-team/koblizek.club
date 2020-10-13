#pragma once
#include <chrono>
#include <string>

long long get_epoch_time()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}