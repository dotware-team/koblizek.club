#pragma once

#include <cstddef>

#include "Utils.h"
#include "../VirtualMethod.h"
#define DECLARE_POINTER_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__ *name

typedef unsigned short MaterialHandle_t;
DECLARE_POINTER_HANDLE(MaterialLock_t);

class Material;

class MaterialSystem {
public:
	constexpr auto findMaterial(const char* materialName) noexcept
	{
		return callVirtualMethod<Material*, const char*, const char*, bool, const char*>(this, 84, materialName, nullptr, true, nullptr);
	}
	constexpr auto firstMaterial() noexcept
	{
		return callVirtualMethod<MaterialHandle_t>(this, 86);
	}

	constexpr auto nextMaterial(MaterialHandle_t h) noexcept
	{
		return callVirtualMethod<MaterialHandle_t, MaterialHandle_t>(this, 87, h);
	}

	constexpr auto invalidMaterial() noexcept
	{
		return callVirtualMethod<MaterialHandle_t>(this, 88);
	}

	constexpr auto getMaterial(MaterialHandle_t h) noexcept
	{
		return callVirtualMethod<Material*, MaterialHandle_t>(this, 89, h);
	}

	VIRTUAL_METHOD(Material*, createMaterial, 83, (const char* materialName, KeyValues* keyValues), (this, materialName, keyValues))
};
