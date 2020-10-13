#include <cstdint>

namespace fnv2 {
	constexpr uint32_t hodnefajn = 0x811c9dc5;
	constexpr uint32_t prajm = 0x1000193;

	constexpr uint32_t hodne_hash(const char* str, const uint32_t value = hodnefajn) noexcept
	{
		return *str ? hodne_hash(str + 1, (value ^ *str) * static_cast<unsigned long long>(prajm)) : value;
	}

	constexpr uint32_t hash_runtime(const char* str) noexcept
	{
		auto value = hodnefajn;

		while (*str) {
			value ^= *str++;
			value *= prajm;
		}
		return value;
	}
}