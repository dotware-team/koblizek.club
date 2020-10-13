#pragma once

#include <cstddef>

enum class WeaponType {
	Knife = 0,
	Pistol,
	SubMachinegun,
	Rifle,
	Shotgun,
	SniperRifle,
	Machinegun,
	C4,
	Placeholder,
	Grenade,
	Unknown,
	StackableItem,
	Fists,
	BreachCharge,
	BumpMine,
	Tablet,
	Melee
};

struct WeaponData {
	std::byte pad[20];
	int maxClip;
	std::byte pad1[112];
	char* name;
	std::byte pad1_[60];
	int type;
	std::byte pad2[32];
	bool fullAuto;
	std::byte pad3[3];
	int damage;
	float armorRatio;
	int bullets;
	float penetration;
	std::byte pad4[8];
	float range;
	float rangeModifier;
	std::byte pad5[16];
	bool hasSilencer;
};