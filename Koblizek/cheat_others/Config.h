#pragma once
#include <string.h>
#include <filesystem>

#include "../cheat_skinchanger/config_.hpp"
#include "../Color.hpp"

class Config final {
public:
    explicit Config(const char*) noexcept;
    void load(size_t) noexcept;
    void save(size_t) const noexcept;
    void add(const char*) noexcept;
    void remove(size_t) noexcept;
    void rename(size_t, const char*) noexcept;
    void reset() noexcept;
	void listConfigs() noexcept;

    constexpr auto& getConfigs() noexcept
    {
        return configs;
    }

	struct Color {
		float color[3]{ 1.0f, 1.0f, 1.0f };
	};

	struct ColorToggle : public Color {
		bool enabled{ false };
	};

    struct {
        constexpr auto& operator[](size_t index) noexcept
        {
            return weapons[index];
        }
    private:
        struct {
            bool enabled{ false };
            bool onKey{ true };
            int key{ 0 };
            bool silent{ false };
            bool friendlyFire{ false };
            bool visibleOnly{ false };
            bool scopedOnly{ false };
            bool ignoreFlash{ false };
            bool ignoreSmoke{ false };
			bool jumpCheck{ false };
            bool autoshoot{ false };
            bool recoilbasedFov{ false };
            float fov{ 0.0f };
            float maxAngleDelta{ 0.0f };
            float smooth{ 1.0f };
            int bone{ 0 };
            float recoilControlX{ 0.0f };
            float recoilControlY{ 0.0f };
			int hitchance{ 20 };
        } weapons[35];
    } koblizek_aimbot;

    struct {
        bool enabled{ false };
        bool ignoreSmoke{ true };
        int timeLimit{ 200 };
    } koblizek_backtrack;

    struct {
        constexpr auto& operator[](size_t index) noexcept
        {
            return glow[index];
        }
    private:
        struct {
            bool enabled{ false };
            bool healthBased{ false };
            bool rainbow{ false };
            float thickness{ 1.0f };
            float alpha{ 1.0f };
            int style{ 0 };
            float color[3]{ 1.0f, 1.0f, 1.0f };
        } glow[13];
    } koblizek_glow;

    struct {
        constexpr auto& operator[](size_t index) noexcept
        {
            return chams[index];
        }
    private:
        struct {
            bool enabled{ false };
            bool healthBased{ false };
            bool rainbow{ false };
            int material{ 0 };
            bool wireframe{ false };
            float color[3]{ 1.0f, 1.0f, 1.0f };
            float alpha{ 1.0f };
        } chams[12];
    } koblizek_chams;

	struct {
			bool disablePostProcessing{ false };
			bool noFog{ false };
			bool no3dSky{ true };
			bool noVisualRecoil{ false };
			bool noSleeves{ false };
			bool noSmoke{ false };
			bool noBlur{ false };
			bool noScopeOverlay{ false };
			bool wireframeSmoke{ false };
			bool thirdperson{ false };
			int thirdpersonKey{ 0 };
			int thirdpersonDistance{ 350 };
			int viewmodelFov{ 18 };
			int fov{ 0 };
			int changefov{ 0 };
			int flashReduction{ 0 };
			float brightness{ 1.0f };
			int skybox{ 0 };
			float worldColor[3]{ 1.0f, 1.0f, 1.0f };
			bool overwatch{ false };
	} koblizek_visuals;

    struct {
        int menuKey{ 0x2D }; 
        bool bunnyHop{ false };
        bool Clantag{ false };
        bool autoAccept{ false };
        bool radarHack{ false };
		bool GrenadePrediction{ false };
		bool removeblurpan{ true };
        bool revealRanks{ false };
        bool fixAnimationLOD{ true };
		bool bombTimer{ false };
        bool hitSound{ false };
		bool hitmarker{ false };
		float hitMarkerTime{ 0.7f };
        int chokedPackets{ 0 };
		float maxAngleDelta{ 255.0f };
		bool fixMovement{ true };
		bool EdgeJump{ false };
		int EdgeJumpKey{ 0 };
		bool EdgeJumpAutostrafer{ false };
		bool VelocityIndicator{ false };
		bool resolver{ false };
		bool SpectatorList{ false };
		bool JumpBug{ false };
		int JumpBugKey{ 0 };
		float JumpBugHeight{ 2.26f };
		bool JumpBugScroll{ false };
		bool JumpBugAutostrafe{ false };
		bool EdgeBug{ false };
		int EdgeBugKey{ 0 };
		bool forceCrosshair{ false };
		int spam{ 0 };
		bool FullBright{ false };
		bool ShowImpacts{ false };
		bool fpsboost{ false };
		int auto_strafe{ 0 };
		bool fakePrime{ false };
		bool AntiAfkKick{ false };
		int aspectratio{ 0 };
		bool BlockBot{ false };
		int blockbotkey{ 0 };
		bool noSmoke{ false };
		bool wireframeSmoke{ false };
		bool removeShadows{ false };
		bool removeBlood{ false };
		bool nosky{ false };
		float menu_color[3]{ 0.54f, 0.09f, 0.09f };
		bool Indicators{ false };
		bool DamageLog{ false };
		bool InfiniteCrouch{ false };
		float viewmodel_offset_x = 2.5f;
		float viewmodel_offset_y = 2.0f;
		float viewmodel_offset_z = -1.5f;
		bool watermark{ true };
		bool AutoPistol{ false };
		float velocity_graph_color[3]{ 1.f, 1.f, 1.f };
		bool velocity_graph_enabled{ false };
		float trail_color[3]{ 1.f, 1.f, 1.f};
		bool trail_enabled{ false };
		float trail_duration{ 2.f };
		bool bullet_beams_enabled{ false };
		float bullet_beams_color[3]{ 1.f, 1.f, 1.f };
		float bullet_beams_duration{ 2.f };
		bool sound_esp_enabled{ false };
		float sound_esp_color[3]{ 1.f, 1.f, 1.f };
		bool legit_antiaim{ false };
		int legit_antiaim_switch{ 0 };
		float legit_antiaim_maxping{ 50.f };
		bool legit_antiaim_resolver{ false };
		bool fast_stop{ false };
		bool purchase_log{ false };
		bool unlock_inventory{ false };
		bool fixbonematrix{ true };
		bool bypass_svpure{ false };
    } koblizek_misc;

	struct {
		item_setting items[200];
	} skinChanger;
	
	struct Esp {
		struct Shared {
			bool enabled{ false };
			int font{ 0xc1 };
			ColorToggle snaplines;
			ColorToggle box;
			bool WeaponWorld{ false };
			int boxType{ 0 };
			bool name{ false };
			ColorToggle outline{ 0.0f, 0.0f, 0.0f };
			ColorToggle distance;
			float maxDistance{ 5000.f };
		};

		struct Player : public Shared {
			ColorToggle eyeTraces;
			bool health{ false };
			bool healthBar{ false };
			float healthBar_color[3]{ 1.f, 1.f, 1.f };
			ColorToggle armor;
			ColorToggle armorBar;
			ColorToggle money;
			ColorToggle headDot;
			bool activeWeapon{ false };
			float weapon_icon_color[3]{ 1.f, 1.f, 1.f };
		};

		struct Weapon : public Shared { } weapon;

		std::array<Player, 6> players;
	} koblizek_esp;

	struct Triggerbot {
		bool enabled{ false };
		bool onKey{ true };
		int key{ 0 };
		bool friendlyFire{ false };
		bool scopedOnly{ false };
		bool ignoreFlash{ false };
		bool ignoreSmoke{ false };
		int hitgroup{ 0 };
		int shotDelay{ 7 };
		int minDamage{ 50 };
		bool hitChance{ true };
		int hitChanceAmmount{ 50 };
		bool killshot{ true };
		bool magnet{ false };
		int magnetkey{ 0 };
	};
	std::array<Triggerbot, 40> koblizek_triggerbot;

private:
    std::filesystem::path path;
    std::vector<std::string> configs;
};

extern Config config;