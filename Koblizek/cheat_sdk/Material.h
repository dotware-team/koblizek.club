#pragma once

#include "Utils.h"

enum class MaterialVar {
	NO_DRAW = 1 << 2,
	IGNOREZ = 1 << 15,
	WIREFRAME = 1 << 28
};

class Material final {
public:
	constexpr int incrementReferenceCount() noexcept
	{
		callVirtualMethod<void>(this, 12);
		return 0;
	}

	constexpr void alphaModulate(float alpha) noexcept
	{
		callVirtualMethod<void, float>(this, 27, alpha);
	}

	constexpr void colorModulate(float color[3]) noexcept
	{
		callVirtualMethod<void, float, float, float>(this, 28, color[0], color[1], color[2]);
	}

	constexpr void colorModulate(float r, float g, float b) noexcept
	{
		callVirtualMethod<void, float, float, float>(this, 28, r, g, b);
	}

	constexpr void setMaterialVarFlag(MaterialVar flag, bool on) noexcept
	{
		callVirtualMethod<void, MaterialVar, bool>(this, 29, flag, on);
	}
	constexpr auto getTextureGroupName() noexcept
	{
		return callVirtualMethod<const char*>(this, 1);
	}

	VIRTUAL_METHOD(void, setValue, 4, (float value), (this, value))
	VIRTUAL_METHOD(void, setVectorValue, 11, (float x, float y, float z), (this, x, y, z))
	VIRTUAL_METHOD(void, setVecComponentValue, 26, (float value, int component), (this, value, component))
	VIRTUAL_METHOD(Material*, findVar, 11, (const char* name, bool* found = nullptr, bool complain = true), (this, name, found, complain))
};
