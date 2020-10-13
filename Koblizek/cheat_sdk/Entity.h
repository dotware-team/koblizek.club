#pragma once

#include "../cheat_important/Animstate.h"
#include "ClientClass.h"
#include "../cheat_important/Interfaces.h"
#include "../cheat_important/Netvars.h"
#include "Cvar.h"
#include "Engine.h"
#include "EngineTrace.h"
#include "EntityList.h"
#include "Utils.h"
#include "Vector.h"
#include "WeaponId.h"
#include "VarMapping.h"
#include "../cheat_important/Memory.h"
#include "WeaponData.h"
#include "../VirtualMethod.h"
#include <algorithm>
#include <cstdint>
#include <chrono>
#include "UserCmd.h"
#include "../cheat_others/Config.h"

struct AnimState;

struct AnimationLayer
{
public:
	std::byte pad[20];
	unsigned int order;
	unsigned int sequence;
	std::byte pad2[4];
	float weight;
	std::byte pad3[8];
	float cycle;
};

enum ObserverMode {
	OBS_MODE_NONE = 0,
	OBS_MODE_DEATHCAM = 1,
	OBS_MODE_FREEZECAM = 2,
	OBS_MODE_FIXED = 3,
	OBS_MODE_IN_EYE = 4,
	OBS_MODE_CHASE = 5,
	OBS_MODE_ROAMING = 6
};

enum class MoveType {
	NOCLIP = 8,
	LADDER = 9
};

class Collideable {
public:
	virtual void* pad() = 0;
	virtual const Vector& obbMins() = 0;
	virtual const Vector& obbMaxs() = 0;
};

class Entity {
public:
	template <typename T>
	constexpr auto getProperty(const char* name, const std::size_t offset = 0) const noexcept
	{
		return *reinterpret_cast<const T*>(this + netvars[name] + offset);
	}

	template <typename T>
	constexpr void setProperty(const char* name, const T& value) noexcept
	{
		*reinterpret_cast<T*>(this + netvars[name]) = value;
	}

	constexpr auto getCollideable() noexcept
	{
		return callVirtualMethod<Collideable*>(this, 3);
	}

	constexpr auto getWeaponType() noexcept
	{
		const auto weaponData = getWeaponData();
		if (!weaponData) 
			return WeaponType::Unknown;

		return (WeaponType)weaponData->type;
	}

	constexpr auto isPistol() noexcept
	{
		return getWeaponType() == WeaponType::Pistol;
	}

	constexpr auto isSniperRifle() noexcept
	{
		return getWeaponType() == WeaponType::SniperRifle;
	}

	constexpr auto isRifle() noexcept
	{
		return getWeaponType() == WeaponType::Rifle;
	}

	constexpr auto isSMG() noexcept
	{
		return getWeaponType() == WeaponType::SubMachinegun;
	}

	constexpr auto isShotgun() noexcept
	{
		return getWeaponType() == WeaponType::Shotgun;
	}

	constexpr bool isKnife() noexcept
	{
		switch (getClientClass()->classId) {
		case ClassId::Knife:
			return true;
		default:
			return false;
		}
	}

	constexpr bool setupBones(matrix3x4* out, int maxBones, int boneMask, float currentTime) noexcept
	{
		if (config.koblizek_misc.fixbonematrix) {
			int* render = reinterpret_cast<int*>(this + 0x274);
			int backup = *render;
			Vector absOrigin = getAbsOrigin();
			*render = 0;
			memory.setAbsOrigin(this, origin());
			auto result = VirtualMethod::call<bool, 13>(this + 4, out, maxBones, boneMask, currentTime);
			memory.setAbsOrigin(this, absOrigin);
			*render = backup;
			return result;
		}
		return VirtualMethod::call<bool, 13>(this + 4, out, maxBones, boneMask, currentTime);
	}

	Vector getBonePosition(int bone) noexcept
	{
		if (matrix3x4 boneMatrices[256]; setupBones(boneMatrices, 256, 256, 0.0f))
			return Vector{ boneMatrices[bone][0][3], boneMatrices[bone][1][3], boneMatrices[bone][2][3] };
		else
			return Vector{ };
	}

	auto GetEyePosition() noexcept
	{
		Vector vec;
		VirtualMethod::call<void, 284>(this, std::ref(vec));
		return vec;
	}

	bool isVisible(const Vector& position = { }) noexcept
	{
		auto localPlayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());
		static Trace trace;
		interfaces.engineTrace->traceRay({ localPlayer->GetEyePosition(), position ? position : getBonePosition(8) }, 0x46004009, { localPlayer }, trace);
		return trace.entity == this || trace.fraction > 0.97f;
	}

	bool isEnemy() noexcept
	{
		return memory.isOtherEnemy(this, interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer()));
	}

	constexpr WeaponData* getWeaponData() noexcept
	{
		return callVirtualMethod<WeaponData*>(this, 460);
	}

	enum item_definition_indexes {
		WEAPON_NONE = 0,
		WEAPON_DEAGLE,
		WEAPON_ELITE,
		WEAPON_FIVESEVEN,
		WEAPON_GLOCK,
		WEAPON_AK47 = 7,
		WEAPON_AUG,
		WEAPON_AWP,
		WEAPON_FAMAS,
		WEAPON_G3SG1,
		WEAPON_GALILAR = 13,
		WEAPON_M249,
		WEAPON_M4A1 = 16,
		WEAPON_MAC10,
		WEAPON_P90 = 19,
		WEAPON_MP5SD = 23,
		WEAPON_UMP45,
		WEAPON_XM1014,
		WEAPON_BIZON,
		WEAPON_MAG7,
		WEAPON_NEGEV,
		WEAPON_SAWEDOFF,
		WEAPON_TEC9,
		WEAPON_TASER,
		WEAPON_HKP2000,
		WEAPON_MP7,
		WEAPON_MP9,
		WEAPON_NOVA,
		WEAPON_P250,
		WEAPON_SHIELD,
		WEAPON_SCAR20,
		WEAPON_SG556,
		WEAPON_SSG08,
		WEAPON_KNIFEGG,
		WEAPON_KNIFE,
		WEAPON_FLASHBANG,
		WEAPON_HEGRENADE,
		WEAPON_SMOKEGRENADE,
		WEAPON_MOLOTOV,
		WEAPON_DECOY,
		WEAPON_INCGRENADE,
		WEAPON_C4,
		WEAPON_HEALTHSHOT = 57,
		WEAPON_KNIFE_T = 59,
		WEAPON_M4A1_SILENCER,
		WEAPON_USP_SILENCER,
		WEAPON_CZ75A = 63,
		WEAPON_REVOLVER,
		WEAPON_TAGRENADE = 68,
		WEAPON_FISTS,
		WEAPON_BREACHCHARGE,
		WEAPON_TABLET = 72,
		WEAPON_MELEE = 74,
		WEAPON_AXE,
		WEAPON_HAMMER,
		WEAPON_SPANNER = 78,
		WEAPON_KNIFE_GHOST = 80,
		WEAPON_FIREBOMB,
		WEAPON_DIVERSION,
		WEAPON_FRAG_GRENADE,
		WEAPON_SNOWBALL,
		WEAPON_BUMPMINE,
		WEAPON_BAYONET = 500,
		WEAPON_KNIFE_CSS = 503,
		WEAPON_KNIFE_FLIP = 505,
		WEAPON_KNIFE_GUT,
		WEAPON_KNIFE_KARAMBIT,
		WEAPON_KNIFE_M9_BAYONET,
		WEAPON_KNIFE_TACTICAL,
		WEAPON_KNIFE_FALCHION = 512,
		WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
		WEAPON_KNIFE_BUTTERFLY,
		WEAPON_KNIFE_PUSH,
		WEAPON_KNIFE_CORD,
		WEAPON_KNIFE_SURV,
		WEAPON_KNIFE_URSUS = 519,
		WEAPON_KNIFE_GYPSY_JACKKNIFE,
		WEAPON_KNIFE_GYPSY_NOMAD,
		WEAPON_KNIFE_STILETTO = 522,
		WEAPON_KNIFE_WIDOWMAKER,
		WEAPON_KNIFE_SKELETON = 525,
		GLOVE_STUDDED_BLOODHOUND = 5027,
		GLOVE_T_SIDE = 5028,
		GLOVE_CT_SIDE = 5029,
		GLOVE_SPORTY = 5030,
		GLOVE_SLICK = 5031,
		GLOVE_LEATHER_WRAP = 5032,
		GLOVE_MOTORCYCLE = 5033,
		GLOVE_SPECIALIST = 5034,
		GLOVE_HYDRA = 5035
	};

	constexpr Entity* getActiveWeapon() noexcept
	{
		return callVirtualMethod<Entity*>(this, 267);
	}

	constexpr bool isDormant() noexcept
	{
		return callVirtualMethod<bool>(this + 8, 9);
	}

	constexpr void preDataUpdate(int updateType) noexcept
	{
		callVirtualMethod<void, int>(this + 8, 6, updateType);
	}

	constexpr void release() noexcept
	{
		return callVirtualMethod<void>(this + 8, 1);
	}

	constexpr void setDestroyedOnRecreateEntities() noexcept
	{
		return callVirtualMethod<void>(this + 8, 13);
	}

	constexpr bool isWeapon() noexcept
	{
		return callVirtualMethod<bool>(this, 165);
	}

	constexpr ClientClass* getClientClass() noexcept
	{
		return callVirtualMethod<ClientClass*>(this + 8, 2);
	}

	PlayerInfo GetPlayerInfo()
	{
		Entity* entity;
		PlayerInfo info;
		if (this == nullptr)
			return info;

		interfaces.engine->getPlayerInfo(entity->index(), info);
		return info;
	}

	constexpr bool isAlive() noexcept
	{
		return callVirtualMethod<bool>(this, 155);
	}

	constexpr float getInaccuracy() noexcept
	{
		return callVirtualMethod<float>(this, 482);
	}

	constexpr void updateAccuracyPenalty() noexcept
	{
		return callVirtualMethod<void>(this, 483);
	}

	float normalize_yaw(float& yaw) noexcept
	{
		while (yaw < 180.0f) yaw += 360.0f;
		while (yaw > 180.0f) yaw -= 360.0f;
		return yaw;
	}

	AnimState* getAnimstate() noexcept
	{
		return *reinterpret_cast<AnimState**>(this + 0x3914);
	}

	VarMap* getVarMap() noexcept
	{
		return reinterpret_cast<VarMap*>(this + 0x24);
	}

	constexpr Vector getAbsOrigin() noexcept
	{
		return callVirtualMethod<Vector&>(this, 10);
	}

	Vector GetVecOrigin() {
		return *reinterpret_cast<Vector*>(uintptr_t(this) + 0x138);
	}

	constexpr void setModelIndex(int index) noexcept
	{
		callVirtualMethod<void, int>(this, 75, index);
	}
	 
	constexpr Entity* getObserverTarget() noexcept
	{
		return callVirtualMethod<Entity*>(this, 294);
	}

	ObserverMode* GetObserverMode()
	{
		return (ObserverMode*)((uintptr_t)this + 0x3378);
	}

	matrix3x4& coordinateFrame() noexcept
	{
		return *reinterpret_cast<matrix3x4*>(this + 0x444);
	}

	float getMaxDesyncAngle() noexcept
	{
		const auto animState = getAnimstate();

		if (!animState)
			return 0.0f;

		float yawModifier = (animState->stopToFullRunningFraction * -0.3f - 0.2f) * std::clamp(animState->footSpeed, 0.0f, 1.0f) + 1.0f;

		if (animState->duckAmount > 0.0f)
			yawModifier += (animState->duckAmount * std::clamp(animState->footSpeed2, 0.0f, 1.0f) * (0.5f - yawModifier));

		return animState->velocitySubtractY * yawModifier;
	}

	bool throwing(UserCmd* cmd)
	{
		auto local_player = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());
		auto weapon = local_player->getActiveWeapon();
		auto weaponClass = getWeaponClass(weapon->itemDefinitionIndex2());
		if (weaponClass == 40) {
			if (!weapon->pinPulled())
				if (weapon->throwTime() > 0.f)
					return true;

			if ((cmd->buttons & (UserCmd::IN_ATTACK | UserCmd::IN_ATTACK2)))
				if (weapon->throwTime() > 0.f)
					return true;
		}
		return false;
	}

	auto getUserId() noexcept
	{
		if (PlayerInfo playerInfo; interfaces.engine->getPlayerInfo(index(), playerInfo))
			return playerInfo.userId;

		return -1;
	}

	NETVAR_OFFSET(index, "CBaseEntity", "m_bIsAutoaimTarget", 4, int);
	NETVAR(modelIndex, "CBaseEntity", "m_nModelIndex", unsigned);
	NETVAR(origin, "CBaseEntity", "m_vecOrigin", Vector)
	NETVAR(simulationTime, "CBaseEntity", "m_flSimulationTime", float)
	NETVAR_OFFSET(moveType, "CBaseEntity", "m_nRenderMode", 1, MoveType);
	NETVAR(team, "CBaseEntity", "m_iTeamNum", int);

	NETVAR(weapons, "CBaseCombatCharacter", "m_hMyWeapons", std::array<int, 48>);
	PNETVAR(wearables, "CBaseCombatCharacter", "m_hMyWearables", int);
	NETVAR(health, "CBasePlayer", "m_iHealth", int);
	NETVAR(m_flLowerBodyYawTarget, "CCSPlayer", "m_flLowerBodyYawTarget", float);
	NETVAR(stamina, "CCSPlayer", "m_flStamina", float);
	NETVAR(flash_duration, "CCSPlayer", "m_flFlashDuration", float);
	NETVAR(m_angEyeAngles, "CCSPlayer", "m_angEyeAngles", Vector);
	NETVAR(viewModel, "CBasePlayer", "m_hViewModel[0]", int);
	NETVAR(flags, "CBasePlayer", "m_fFlags", int);
	NETVAR(tickBase, "CBasePlayer", "m_nTickBase", int)

	NETVAR(pinPulled, "CBaseCSGrenade", "m_bPinPulled", bool);
	NETVAR(throwTime, "CBaseCSGrenade", "m_fThrowTime", float);

	NETVAR(hasDefuser, "CCSPlayer", "m_bHasDefuser", bool)

	NETVAR(viewModelIndex, "CBaseCombatWeapon", "m_iViewModelIndex", int);
	NETVAR(worldModelIndex, "CBaseCombatWeapon", "m_iWorldModelIndex", int);
	NETVAR(worldDroppedModelIndex, "CBaseCombatWeapon", "m_iWorldDroppedModelIndex", int);
	NETVAR(weaponWorldModel, "CBaseCombatWeapon", "m_hWeaponWorldModel", int);
	NETVAR(velocity, "CBasePlayer", "m_vecVelocity[0]", Vector);
	NETVAR(aimPunchAngle, "CBasePlayer", "m_aimPunchAngle", Vector);

	NETVAR(accountID, "CBaseAttributableItem", "m_iAccountID", int);
	NETVAR(itemDefinitionIndex, "CBaseAttributableItem", "m_iItemDefinitionIndex", short);
	NETVAR(itemIDHigh, "CBaseAttributableItem", "m_iItemIDHigh", int);
	NETVAR(entityQuality, "CBaseAttributableItem", "m_iEntityQuality", int);
	NETVAR(customName, "CBaseAttributableItem", "m_szCustomName", char[32]);
	NETVAR(fallbackPaintKit, "CBaseAttributableItem", "m_nFallbackPaintKit", unsigned);
	NETVAR(fallbackSeed, "CBaseAttributableItem", "m_nFallbackSeed", unsigned);
	NETVAR(fallbackWear, "CBaseAttributableItem", "m_flFallbackWear", float);
	NETVAR(fallbackStatTrak, "CBaseAttributableItem", "m_nFallbackStatTrak", unsigned);
	NETVAR(nextAttack, "CBaseCombatCharacter", "m_flNextAttack", float);
	NETVAR(nextPrimaryAttack, "CBaseCombatWeapon", "m_flNextPrimaryAttack", float);
	NETVAR(itemDefinitionIndex2, "CBaseAttributableItem", "m_iItemDefinitionIndex", WeaponId);
	NETVAR(clip, "CBaseCombatWeapon", "m_iClip1", int);
	NETVAR(reserveAmmoCount, "CBaseCombatWeapon", "m_iPrimaryReserveAmmoCount", int)
	NETVAR(gunGameImmunity, "CCSPlayer", "m_bGunGameImmunity", bool);
	NETVAR(flashDuration, "CCSPlayer", "m_flFlashDuration", float);
	NETVAR(isScoped, "CCSPlayer", "m_bIsScoped", bool);
	NETVAR(hasHelmet, "CCSPlayer", "m_bHasHelmet", bool);
	NETVAR(getShotsFired, "CCSPlayer", "m_iShotsFired", int);
	NETVAR(armor, "CCSPlayer", "m_ArmorValue", int);
	NETVAR(money, "CCSPlayer", "m_iAccount", int);
	NETVAR(is_defusing, "CCSPlayer", "m_bIsDefusing", bool);
	NETVAR(GetGround, "CCSPlayer", "m_hGroundEntity", int);

	NETVAR(owner, "CBaseViewModel", "m_hOwner", int);
	NETVAR(weapon, "CBaseViewModel", "m_hWeapon", int);

	NETVAR(c4StartedArming, "CC4", "m_bStartedArming", bool);

	NETVAR(c4BlowTime, "CPlantedC4", "m_flC4Blow", float);
	NETVAR(c4BombSite, "CPlantedC4", "m_nBombSite", int);
	NETVAR(c4Ticking, "CPlantedC4", "m_bBombTicking", bool);
	NETVAR(c4DefuseCountDown, "CPlantedC4", "m_flDefuseCountDown", float);
	NETVAR(c4Defuser, "CPlantedC4", "m_hBombDefuser", int);

	std::string get_icon()
	{
		switch (itemDefinitionIndex())
		{
		case WEAPON_BAYONET:
			return "1";
		case WEAPON_KNIFE_SURVIVAL_BOWIE:
			return "7";
		case WEAPON_KNIFE_BUTTERFLY:
			return "8";
		case WEAPON_KNIFE:
			return "]";
		case WEAPON_KNIFE_FALCHION:
			return "0";
		case WEAPON_KNIFE_FLIP:
			return "2";
		case WEAPON_KNIFE_GUT:
			return "3";
		case WEAPON_KNIFE_KARAMBIT:
			return "4";
		case WEAPON_KNIFE_M9_BAYONET:
			return "5";
		case WEAPON_KNIFE_T:
			return "[";
		case WEAPON_KNIFE_TACTICAL:
			return "6";
		case WEAPON_KNIFE_PUSH:
			return "]";
		case WEAPON_DEAGLE:
			return "A";
		case WEAPON_ELITE:
			return "B";
		case WEAPON_FIVESEVEN:
			return "C";
		case WEAPON_GLOCK:
			return "D";
		case WEAPON_HKP2000:
			return "E";
		case WEAPON_P250:
			return "F";
		case WEAPON_USP_SILENCER:
			return "G";
		case WEAPON_TEC9:
			return "H";
		case WEAPON_REVOLVER:
			return "J";
		case WEAPON_MAC10:
			return "K";
		case WEAPON_UMP45:
			return "L";
		case WEAPON_BIZON:
			return "M";
		case WEAPON_MP7:
			return "N";
		case WEAPON_MP9:
			return "O";
		case WEAPON_P90:
			return "P";
		case WEAPON_GALILAR:
			return "Q";
		case WEAPON_FAMAS:
			return "R";
		case WEAPON_M4A1_SILENCER:
			return "S";
		case WEAPON_M4A1:
			return "T";
		case WEAPON_AUG:
			return "U";
		case WEAPON_SG556:
			return "V";
		case WEAPON_AK47:
			return "W";
		case WEAPON_G3SG1:
			return "X";
		case WEAPON_SCAR20:
			return "Y";
		case WEAPON_AWP:
			return "Z";
		case WEAPON_SSG08:
			return "a";
		case WEAPON_XM1014:
			return "b";
		case WEAPON_SAWEDOFF:
			return "c";
		case WEAPON_MAG7:
			return "d";
		case WEAPON_NOVA:
			return "e";
		case WEAPON_NEGEV:
			return "f";
		case WEAPON_M249:
			return "g";
		case WEAPON_TASER:
			return "h";
		case WEAPON_FLASHBANG:
			return "i";
		case WEAPON_HEGRENADE:
			return "j";
		case WEAPON_SMOKEGRENADE:
			return "k";
		case WEAPON_MOLOTOV:
			return "l";
		case WEAPON_DECOY:
			return "m";
		case WEAPON_INCGRENADE:
			return "n";
		case WEAPON_C4:
			return "o";
		case WEAPON_CZ75A:
			return "I";
		case WEAPON_KNIFE_URSUS:
			return "]";
		case WEAPON_KNIFE_GYPSY_JACKKNIFE:
			return "]";
		case WEAPON_KNIFE_STILETTO:
			return "]";
		case WEAPON_KNIFE_WIDOWMAKER:
			return "]";
		default:
			return "  ";
		}
	}
};

struct Smoke_t : public Entity {
	float GetSpawnTime() {
		return *(float*)((uintptr_t)this + 0x20);
	}
	static float GetExpireTime() {
		return 18.06f;
	}
};

struct Inferno_t : public Entity {
	float GetSpawnTime() {
		return *(float*)((uintptr_t)this + 0x20);
	}
	static float GetExpireTime() {
		return 7.03125f;
	}
};