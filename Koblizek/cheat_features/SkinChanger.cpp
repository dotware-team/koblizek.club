#include "../cheat_important/Interfaces.h"
#include "../cheat_sdk/Entity.h"
#include "SkinChanger.h"
#include "../cheat_others/Config.h"

#include "../cheat_sdk/Client.h"
#include "../cheat_sdk/ClientClass.h"
#include "../cheat_sdk/Engine.h"
#include "../cheat_sdk/FrameStage.h"
#include "../cheat_sdk/ModelInfo.h"
#include "../cheat_sdk/EntityList.h"
#include "../cheat_sdk/Entity.h"
#include "../cheat_skinchanger/Utilities/vmt_smart_hook.hpp"
#include "../cheat_sdk/GameEvent.h"
#include "../cheat_sdk/Cvar.h"
#include "../cheat_skinchanger/config_.hpp"
#include "../cheat_sdk/ConVar.h"
#include "../cheat_protection/xor.hpp"

static std::unordered_map<std::string_view, const char*> iconOverrides;

enum class StickerAttribute {
	Index,
	Wear,
	Scale,
	Rotation
};

static auto s_econ_item_interface_wrapper_offset = std::uint16_t(0);

struct GetStickerAttributeBySlotIndexFloat {
	static auto __fastcall hooked(void* thisptr, void*, const int slot,
		const StickerAttribute attribute, const float unknown) -> float
	{
		auto item = reinterpret_cast<Entity*>(std::uintptr_t(thisptr) - s_econ_item_interface_wrapper_offset);

		const auto defindex = item->itemDefinitionIndex();

		auto config = g_config.get_by_definition_index(defindex);

		if (config) {
			switch (attribute) {
			case StickerAttribute::Wear:
				return config->stickers[slot].wear;
			case StickerAttribute::Scale:
				return config->stickers[slot].scale;
			case StickerAttribute::Rotation:
				return config->stickers[slot].rotation;
			default:
				break;
			}
		}
		return m_original(thisptr, nullptr, slot, attribute, unknown);
	}
	static decltype(&hooked) m_original;
};

decltype(GetStickerAttributeBySlotIndexFloat::m_original) GetStickerAttributeBySlotIndexFloat::m_original;

struct GetStickerAttributeBySlotIndexInt {
	static int __fastcall hooked(void* thisptr, void*, const int slot,
		const StickerAttribute attribute, const int unknown)
	{
		auto item = reinterpret_cast<Entity*>(std::uintptr_t(thisptr) - s_econ_item_interface_wrapper_offset);

		if (attribute == StickerAttribute::Index)
			if (auto config = g_config.get_by_definition_index(item->itemDefinitionIndex()))
				return config->stickers[slot].kit;
		return m_original(thisptr, nullptr, slot, attribute, unknown);
	}

	static decltype(&hooked) m_original;
};

decltype(GetStickerAttributeBySlotIndexInt::m_original) GetStickerAttributeBySlotIndexInt::m_original;

void apply_sticker_changer(Entity* item) noexcept
{
	if (!s_econ_item_interface_wrapper_offset)
		s_econ_item_interface_wrapper_offset = netvars.get_offset(FNV("CBaseAttributableItem->m_Item")) + 0xC;

	static vmt_multi_hook hook;

	const auto econ_item_interface_wrapper = std::uintptr_t(item) + s_econ_item_interface_wrapper_offset;

	if (hook.initialize_and_hook_instance(reinterpret_cast<void*>(econ_item_interface_wrapper))) {
		hook.apply_hook<GetStickerAttributeBySlotIndexFloat>(4);
		hook.apply_hook<GetStickerAttributeBySlotIndexInt>(5);
	}
}

static void erase_override_if_exists_by_index(const int definition_index) noexcept
{
	// We have info about the item not needed to be overridden
	if (const auto original_item = game_data::get_weapon_info(definition_index)) {
		if (!original_item->icon)
			return;

		// We are overriding its icon when not needed
		if (const auto override_entry = iconOverrides.find(original_item->icon); override_entry != end(iconOverrides))
			iconOverrides.erase(override_entry); // Remove the leftover override
	}
}

static void apply_config_on_attributable_item(Entity* item, const item_setting* config,
	const unsigned xuid_low) noexcept
{
	// Force fallback values to be used.
	item->itemIDHigh() = -1;

	// Set the owner of the weapon to our lower XUID. (fixes StatTrak)
	item->accountID() = xuid_low;

	if (config->entity_quality_index)
		item->entityQuality() = config->entity_quality_index;

	if (config->custom_name[0])
		strcpy_s(item->customName(), config->custom_name);

	if (config->paint_kit_index)
		item->fallbackPaintKit() = config->paint_kit_index;

	if (config->seed)
		item->fallbackSeed() = config->seed;

	if (config->stat_trak)
		item->fallbackStatTrak() = config->stat_trak;

	item->fallbackWear() = config->wear;

	auto& definition_index = item->itemDefinitionIndex();

	if (config->definition_override_index // We need to override defindex
		&& config->definition_override_index != definition_index) // It is not yet overridden
	{
		// We have info about what we gonna override it to
		if (const auto replacement_item = game_data::get_weapon_info(config->definition_override_index)) {
			const auto old_definition_index = definition_index;

			definition_index = config->definition_override_index;

			// Set the weapon model index -- required for paint kits to work on replacement items after the 29/11/2016 update.
			//item->GetModelIndex() = g_model_info->GetModelIndex(k_weapon_info.at(config->definition_override_index).model);
			item->setModelIndex(interfaces.modelInfo->getModelIndex(replacement_item->model));
			item->preDataUpdate(0);

			// We didn't override 0, but some actual weapon, that we have data for
			if (old_definition_index)
				if (const auto original_item = game_data::get_weapon_info(old_definition_index); original_item && original_item->icon && replacement_item->icon)
					iconOverrides[original_item->icon] = replacement_item->icon;
		}
	}
	else
	{
		erase_override_if_exists_by_index(definition_index);
	}

	apply_sticker_changer(item);
}

static auto get_wearable_create_fn() noexcept
{
	auto clazz = interfaces.client->getAllClasses();
	while (fnv::hash_runtime(clazz->networkName) != FNV("CEconWearable"))
		clazz = clazz->next;

	return clazz->createFunction;
}

static auto make_glove(int entry, int serial) noexcept
{
	static auto create_wearable_fn = get_wearable_create_fn();

	create_wearable_fn(entry, serial);

	const auto glove = interfaces.entityList->getEntity(entry);

	// He he
	{
		//  static auto set_abs_origin_addr = platform::find_pattern("client", "\x55\x8B\xEC\x83\xE4\xF8\x51\x53\x56\x57\x8B\xF1", "xxxxxxxxxxxx");

		  //const auto set_abs_origin_fn = reinterpret_cast<void(__thiscall*)(void*, const Vector&)>(set_abs_origin_addr);

		static constexpr Vector new_pos = { 10000.f, 10000.f, 10000.f };

		memory.setAbsOrigin(glove, new_pos);
	}

	return glove;
}

static void post_data_update_start(Entity* local) noexcept
{
	const auto local_index = local->index();

	PlayerInfo player_info;
	if (!interfaces.engine->getPlayerInfo(local_index, player_info))
		return;

	// Handle glove config
	{
		const auto wearables = local->wearables();

		const auto glove_config = g_config.get_by_definition_index(GLOVE_T_SIDE);

		static int glove_handle;

		auto glove = interfaces.entityList->getEntityFromHandle(wearables[0]);

		if (!glove) // There is no glove
		{
			// Try to get our last created glove
			const auto our_glove = interfaces.entityList->getEntityFromHandle(glove_handle);

			if (our_glove) // Our glove still exists
			{
				wearables[0] = glove_handle;
				glove = our_glove;
			}
		}

		if (!local->isAlive()) {
			if (glove) {
				glove->setDestroyedOnRecreateEntities();
				glove->release();
			}
			return;
		}

		if (glove_config && glove_config->definition_override_index)
		{
			// We don't have a glove, but we should
			if (!glove)
			{
				const auto entry = interfaces.entityList->getHighestEntityIndex() + 1;
				const auto serial = rand() % 0x1000;

				glove = make_glove(entry, serial);

				wearables[0] = entry | serial << 16;

				// Let's store it in case we somehow lose it.
				glove_handle = wearables[0];
			}

			// Thanks, Beakers
			glove->index() = -1;

			apply_config_on_attributable_item(glove, glove_config, player_info.xuidLow);
		}
	}

	// Handle weapon configs
	{
		auto& weapons = local->weapons();

		for (auto weapon_handle : weapons) {
			if (weapon_handle == -1)
				break;

			auto weapon = interfaces.entityList->getEntityFromHandle(weapon_handle);

			if (!weapon)
				continue;

			auto& definition_index = weapon->itemDefinitionIndex();

			// All knives are terrorist knives.
			if (const auto active_conf = g_config.get_by_definition_index(is_knife(definition_index) ? WEAPON_KNIFE : definition_index))
				apply_config_on_attributable_item(weapon, active_conf, player_info.xuidLow);
			else
				erase_override_if_exists_by_index(definition_index);
		}
	}

	const auto view_model = interfaces.entityList->getEntityFromHandle(local->viewModel());

	if (!view_model)
		return;

	const auto view_model_weapon = interfaces.entityList->getEntityFromHandle(view_model->weapon());

	if (!view_model_weapon)
		return;

	const auto override_info = game_data::get_weapon_info(view_model_weapon->itemDefinitionIndex());

	if (!override_info)
		return;

	const auto override_model_index = interfaces.modelInfo->getModelIndex(override_info->model);
	view_model->modelIndex() = override_model_index;

	const auto world_model = interfaces.entityList->getEntityFromHandle(view_model_weapon->weaponWorldModel());

	if (!world_model)
		return;

	world_model->modelIndex() = override_model_index + 1;
}

static bool hudUpdateRequired{ false };

static constexpr void updateHud() noexcept
{
	if (auto hud_weapons = memory.findHudElement(memory.hud, "CCSGO_HudWeaponSelection") - 0x28) {
		for (int i = 0; i < *(hud_weapons + 0x20); i++)
			i = memory.clearHudWeapon(hud_weapons, i);
	}
	hudUpdateRequired = false;
}

void SkinChanger::run(FrameStage stage) noexcept
{
	if (stage == FrameStage::NET_UPDATE_POSTDATAUPDATE_START) {
		if (const auto localPlayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer())) {
			post_data_update_start(localPlayer);
			if (hudUpdateRequired && !localPlayer->isDormant())
				updateHud();
		}
	}
}

void SkinChanger::scheduleHudUpdate() noexcept
{
	interfaces.cvar->findVar(XorStr("cl_fullupdate"))->callBack();
	hudUpdateRequired = true;
}

void SkinChanger::overrideHudIcon(GameEvent* event) noexcept
{
	if (!strcmp(event->getName(), "player_death") && interfaces.engine->getPlayerForUserID(event->getInt("attacker")) == interfaces.engine->getLocalPlayer()) {
		if (const auto iconOverride = iconOverrides[event->getString("weapon")])
			event->setString("weapon", iconOverride);
	}
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class etswayk {
public:
	double tnebw;
	string ubkip;
	int mjiyrirs;
	etswayk();
	int eqwvswzszuhadlqlccapfkjb(bool nfjigrujswqxwt, string uicxfqbciqg);
	string mnoejaftwsnujwlznh(bool vtyxtnukdzo, double wuvppauzm, double qjxlv, double xcyfgajrtr, double qgmhpdoe, int gdoavfesprk, int femudncjiohdnx, double visebnwdkvtq, string nxwrovtbwbyeyd);
	string ufhsqugztfdzstfer(double eqioeid, double bupeinvec, double letpzod, double tlaytlmnnvk, bool oktbayhrpd, double pynmhsxdknmd, double qgyrctgre);
	bool acpdgykpdwsuztbiffpd(double xoelclm, int hgjeykwmpovf, int kpzmrnsumpefh, bool fqnvhvhedpldev, double qnetvgpsa, string cjqlayieqcfq, bool sxfqjkkbccy);
	string gywiflabktfrnmp();
	string xuiethpzdieytuwk(string fvvvcpgwiimlrs, bool cvnhskxftwawql, double bihhekx, int glsihsoxxakfud, int ayevampsbbv, int jivssvhsw, int pyxbenwytckn, bool rysdderv, int rshhswe, string dhdcwfyiqwafeq);

protected:
	string fjlwem;
	double grhlkqbbu;
	int mjateey;
	string iggurkgpdaxruci;

	string ijjopkpkulc();
	void svvrjlafxquowyxjheeojzjxu(double wxtfodiqkidlfbs, int rwsydzyeh);
	double sespoiyqxc();
	bool imyvzkeiyslyzczmvc(string nlrzcdycpadp, bool oubndh, bool vuvrewgyrjcc, string aisrmzmuvdjw, double kyrdlcc, int ahusdtq);
	string biddupzigntcrqtbc(int njzsbqccvvadzm);
	double sqfrktoscpizfwdilwbihitzk(string sjwvkqljdvqf, double gqyjj, int yjxwfgutmbeev, string snhdvnpviqds, string ghtkysgwvbmi, int wbctahhoaaxv, int togipmbfsxgktgl);
	string xwhtkcxgfeo(bool hmvug, bool bcdtuidjbtvc, string wiiuphaya, string qefbuja);
	int elirmvglfwbq(double xcfcwpf, double lyazuubtxvzxzk, int guyki, string uyirqkzobhp, double uebgjcxwfcxny);
	string mrsctuhjbrprchctdwvtpbkr(double ocwzplxwdh, string lxzquyr, string ohymyw, string ybnsiczgkhqlin, double vnlqkdqfv, double gzfjnbifonblg, double djzco, double qdbcrmjmrfvak);

private:
	double igqrhvyeqqzkl;
	string sufts;
	int dhlvbc;
	bool pjlzvcrujth;
	int umscd;

	void wzsdrvjfkkytnhtzbsc(bool cprsaluh, bool bwvkuhjfotid, double ddvljkdi, string xlxuzhwh, double kyorid, string sdftgm, bool ealfurzbwgpv, int lnaghj, double mhqlui);
	string ljqjfqlmilckvwmdfsggqi();
	string utywnusrndmfignbfwjaih(bool huimjldzn, bool ovchxzzwrpr, double bekpi, string gmdpngwrpnwnoyf, int gaqqzwblph, string laoukyd, double haapdqtwvkmoblr, string bblensg, string avjdahfzli, bool vwdakxhwiekacnj);
	bool upwrrbpjnmk(int murys, int npgunlgltukxep, int mpayygwmdbukxen, int wzhfnggotqf);
	int khzwcatzkcemnqrvgmmnyqhji(bool xitoqkdlhjnw, double yqjweqylpneq, string zjkmfrb, string qxohczlpbtk, string udylbjtjbsdgswk, bool stfcxoicojg, double lxawzenp, string rnspnxjkeyke, double ymaykvlalkpwj);
	void gzlhecgfgzv(string ocqijihremire, int ufmaicomafbdld, double ksxyviakoxsoitn, bool ckhzcptnxcij, double vlonlucpxj, int bkxstyqinmwvo);
	string thyhvirspsquvkuyzkpf();
	bool lwmrreflyyj(bool ejhcdy, int zcgppthdxshm, int fqymkqanhzueh, int whdsudthnlh, string kzxogtejhwith, string ykjoskszimcazlh, bool iasizxqvt, double titkgbq, string tounf);
	double mdxcvuqnwhv(double bdidkrdx, string gfwcxrgolmzdtc, string nqpixdozribwodo, bool bmguxjcmdmarb, bool ikixffzebl, double jtrwpcxakbfdvg, bool peljcxwrb);

};


void etswayk::wzsdrvjfkkytnhtzbsc(bool cprsaluh, bool bwvkuhjfotid, double ddvljkdi, string xlxuzhwh, double kyorid, string sdftgm, bool ealfurzbwgpv, int lnaghj, double mhqlui) {
	int inlywbvr = 4656;
	string qmzfqlfxaqgu = "tkohmxudooasushfhzxpkyddjyoqabzkuihppukqzxfzgq";
	double qkhuupgy = 25346;
	bool hgovrjnqcq = true;
	if (25346 != 25346) {
		int cupvmrt;
		for (cupvmrt = 17; cupvmrt > 0; cupvmrt--) {
			continue;
		}
	}
	if (true != true) {
		int uyekjvp;
		for (uyekjvp = 87; uyekjvp > 0; uyekjvp--) {
			continue;
		}
	}
	if (25346 != 25346) {
		int fzoxufyemn;
		for (fzoxufyemn = 82; fzoxufyemn > 0; fzoxufyemn--) {
			continue;
		}
	}
	if (25346 != 25346) {
		int tewvtw;
		for (tewvtw = 70; tewvtw > 0; tewvtw--) {
			continue;
		}
	}
	if (string("tkohmxudooasushfhzxpkyddjyoqabzkuihppukqzxfzgq") == string("tkohmxudooasushfhzxpkyddjyoqabzkuihppukqzxfzgq")) {
		int cxljj;
		for (cxljj = 75; cxljj > 0; cxljj--) {
			continue;
		}
	}

}

string etswayk::ljqjfqlmilckvwmdfsggqi() {
	double uxrlstlq = 16819;
	double pwcqqfu = 72937;
	bool sjiitwznisveh = true;
	if (16819 == 16819) {
		int npiht;
		for (npiht = 63; npiht > 0; npiht--) {
			continue;
		}
	}
	if (16819 == 16819) {
		int ps;
		for (ps = 68; ps > 0; ps--) {
			continue;
		}
	}
	if (16819 != 16819) {
		int zzdagybq;
		for (zzdagybq = 93; zzdagybq > 0; zzdagybq--) {
			continue;
		}
	}
	if (true != true) {
		int smpbyt;
		for (smpbyt = 43; smpbyt > 0; smpbyt--) {
			continue;
		}
	}
	if (true == true) {
		int nevlhf;
		for (nevlhf = 66; nevlhf > 0; nevlhf--) {
			continue;
		}
	}
	return string("fsxbwgtkutntdyoiegq");
}

string etswayk::utywnusrndmfignbfwjaih(bool huimjldzn, bool ovchxzzwrpr, double bekpi, string gmdpngwrpnwnoyf, int gaqqzwblph, string laoukyd, double haapdqtwvkmoblr, string bblensg, string avjdahfzli, bool vwdakxhwiekacnj) {
	string aqoljlrtgkd = "fgdgbyljfkccuvzuqckjeqbgwwglrqlupcvxuabmwmpnnpdlpibjzlujqpccrsbetyjdhmvru";
	bool yneewijh = false;
	string vqjwt = "egifnkvqwmfntkamnzpxhxsuldiytucrhxiruwpnabunqsyxizvsdazepyzarnsqloqnulr";
	string wefgizud = "wokmjixptniqcsghuzkuagybiuobneixifme";
	double gphfk = 7407;
	string hfcvim = "xtcbaagkdhoubjgtdpuftqsykrznflibxmipvsakfaunbcd";
	bool wgteecc = false;
	if (string("fgdgbyljfkccuvzuqckjeqbgwwglrqlupcvxuabmwmpnnpdlpibjzlujqpccrsbetyjdhmvru") == string("fgdgbyljfkccuvzuqckjeqbgwwglrqlupcvxuabmwmpnnpdlpibjzlujqpccrsbetyjdhmvru")) {
		int ajm;
		for (ajm = 45; ajm > 0; ajm--) {
			continue;
		}
	}
	if (string("xtcbaagkdhoubjgtdpuftqsykrznflibxmipvsakfaunbcd") == string("xtcbaagkdhoubjgtdpuftqsykrznflibxmipvsakfaunbcd")) {
		int nsejh;
		for (nsejh = 5; nsejh > 0; nsejh--) {
			continue;
		}
	}
	if (false == false) {
		int ttfdxua;
		for (ttfdxua = 26; ttfdxua > 0; ttfdxua--) {
			continue;
		}
	}
	if (false == false) {
		int kkwjoszkgs;
		for (kkwjoszkgs = 75; kkwjoszkgs > 0; kkwjoszkgs--) {
			continue;
		}
	}
	if (false == false) {
		int kegka;
		for (kegka = 37; kegka > 0; kegka--) {
			continue;
		}
	}
	return string("omlil");
}

bool etswayk::upwrrbpjnmk(int murys, int npgunlgltukxep, int mpayygwmdbukxen, int wzhfnggotqf) {
	bool rnijmftvm = true;
	bool xzqdnzpqlgzz = true;
	string urptk = "cbflqtqdxjkguuysomxys";
	double tfayjkqoulgcvp = 31591;
	if (string("cbflqtqdxjkguuysomxys") == string("cbflqtqdxjkguuysomxys")) {
		int uqfoblxs;
		for (uqfoblxs = 71; uqfoblxs > 0; uqfoblxs--) {
			continue;
		}
	}
	if (true != true) {
		int wikhhyt;
		for (wikhhyt = 66; wikhhyt > 0; wikhhyt--) {
			continue;
		}
	}
	if (true == true) {
		int pjgdrtg;
		for (pjgdrtg = 68; pjgdrtg > 0; pjgdrtg--) {
			continue;
		}
	}
	if (true == true) {
		int enmifxpcb;
		for (enmifxpcb = 81; enmifxpcb > 0; enmifxpcb--) {
			continue;
		}
	}
	return true;
}

int etswayk::khzwcatzkcemnqrvgmmnyqhji(bool xitoqkdlhjnw, double yqjweqylpneq, string zjkmfrb, string qxohczlpbtk, string udylbjtjbsdgswk, bool stfcxoicojg, double lxawzenp, string rnspnxjkeyke, double ymaykvlalkpwj) {
	string wpqrblfh = "nlnfxdxubpfxkbetpjly";
	if (string("nlnfxdxubpfxkbetpjly") != string("nlnfxdxubpfxkbetpjly")) {
		int gftrid;
		for (gftrid = 14; gftrid > 0; gftrid--) {
			continue;
		}
	}
	if (string("nlnfxdxubpfxkbetpjly") == string("nlnfxdxubpfxkbetpjly")) {
		int uxgde;
		for (uxgde = 99; uxgde > 0; uxgde--) {
			continue;
		}
	}
	if (string("nlnfxdxubpfxkbetpjly") != string("nlnfxdxubpfxkbetpjly")) {
		int cwe;
		for (cwe = 69; cwe > 0; cwe--) {
			continue;
		}
	}
	if (string("nlnfxdxubpfxkbetpjly") != string("nlnfxdxubpfxkbetpjly")) {
		int dnacx;
		for (dnacx = 44; dnacx > 0; dnacx--) {
			continue;
		}
	}
	if (string("nlnfxdxubpfxkbetpjly") != string("nlnfxdxubpfxkbetpjly")) {
		int mih;
		for (mih = 9; mih > 0; mih--) {
			continue;
		}
	}
	return 65683;
}

void etswayk::gzlhecgfgzv(string ocqijihremire, int ufmaicomafbdld, double ksxyviakoxsoitn, bool ckhzcptnxcij, double vlonlucpxj, int bkxstyqinmwvo) {
	string gcdfkv = "kipqnjzdurgxrzqkxlqxguhrifuvjwunuaenlkpqsme";
	bool jqwdukvszwjpxah = false;
	bool kywxnzoffsac = true;
	bool knwksbxrpeqmdsm = true;
	double wrukrojfzzoavk = 2716;
	bool nenaonjtfpqvk = true;
	if (true != true) {
		int rsj;
		for (rsj = 86; rsj > 0; rsj--) {
			continue;
		}
	}
	if (2716 == 2716) {
		int vvxbg;
		for (vvxbg = 63; vvxbg > 0; vvxbg--) {
			continue;
		}
	}
	if (true != true) {
		int mmrsgcxsls;
		for (mmrsgcxsls = 12; mmrsgcxsls > 0; mmrsgcxsls--) {
			continue;
		}
	}
	if (true != true) {
		int eocqhyf;
		for (eocqhyf = 59; eocqhyf > 0; eocqhyf--) {
			continue;
		}
	}
	if (true != true) {
		int zizqzdhp;
		for (zizqzdhp = 67; zizqzdhp > 0; zizqzdhp--) {
			continue;
		}
	}

}

string etswayk::thyhvirspsquvkuyzkpf() {
	return string("mmlaqfeniurw");
}

bool etswayk::lwmrreflyyj(bool ejhcdy, int zcgppthdxshm, int fqymkqanhzueh, int whdsudthnlh, string kzxogtejhwith, string ykjoskszimcazlh, bool iasizxqvt, double titkgbq, string tounf) {
	bool qnxeabys = true;
	int ynsiunltavjjhh = 2260;
	double whruwbvvoi = 26140;
	bool kdftj = true;
	string hoqxgegekdtm = "u";
	double lbolwag = 13967;
	double mhxdxtuzbgcw = 38789;
	string cdsizuhdixz = "buxxzlnrndtzfrc";
	bool byrozeixm = true;
	double dpxggzrjcu = 23943;
	return true;
}

double etswayk::mdxcvuqnwhv(double bdidkrdx, string gfwcxrgolmzdtc, string nqpixdozribwodo, bool bmguxjcmdmarb, bool ikixffzebl, double jtrwpcxakbfdvg, bool peljcxwrb) {
	string acgwevwawxqsui = "zlllzsninmwhhhsthyhxwlxznzojcwntiemzpcdwzxoiqfhftbhawduveayqnhaxrkwhgqdklxuitonfiasticxzytoyqujm";
	int hajhhmtjeti = 9538;
	double lwwewojtrl = 11683;
	if (string("zlllzsninmwhhhsthyhxwlxznzojcwntiemzpcdwzxoiqfhftbhawduveayqnhaxrkwhgqdklxuitonfiasticxzytoyqujm") == string("zlllzsninmwhhhsthyhxwlxznzojcwntiemzpcdwzxoiqfhftbhawduveayqnhaxrkwhgqdklxuitonfiasticxzytoyqujm")) {
		int ycytcjb;
		for (ycytcjb = 31; ycytcjb > 0; ycytcjb--) {
			continue;
		}
	}
	if (9538 != 9538) {
		int ezwxjq;
		for (ezwxjq = 37; ezwxjq > 0; ezwxjq--) {
			continue;
		}
	}
	if (string("zlllzsninmwhhhsthyhxwlxznzojcwntiemzpcdwzxoiqfhftbhawduveayqnhaxrkwhgqdklxuitonfiasticxzytoyqujm") != string("zlllzsninmwhhhsthyhxwlxznzojcwntiemzpcdwzxoiqfhftbhawduveayqnhaxrkwhgqdklxuitonfiasticxzytoyqujm")) {
		int hxta;
		for (hxta = 86; hxta > 0; hxta--) {
			continue;
		}
	}
	if (11683 == 11683) {
		int whe;
		for (whe = 97; whe > 0; whe--) {
			continue;
		}
	}
	if (9538 == 9538) {
		int lfabtrozd;
		for (lfabtrozd = 0; lfabtrozd > 0; lfabtrozd--) {
			continue;
		}
	}
	return 24169;
}

string etswayk::ijjopkpkulc() {
	double wjoozjeytt = 52980;
	int lvyewxlv = 979;
	double gruqe = 16740;
	bool bwjsjtpcumi = true;
	string ipyqpuomyymut = "fcsjml";
	bool rcuqfov = true;
	int tqmsxkgrudoinn = 5135;
	bool fdwktqehm = false;
	bool burvwse = true;
	return string("xf");
}

void etswayk::svvrjlafxquowyxjheeojzjxu(double wxtfodiqkidlfbs, int rwsydzyeh) {
	int hzxoaoregzaf = 3638;
	bool bxjpjkszbhyqvun = true;
	bool johhgvgurfzxis = true;
	string oaquouduz = "stwuawjelbpqegojbczemgmwenruwisnlmelkdjqtpaknajwpvd";
	if (true != true) {
		int pvi;
		for (pvi = 52; pvi > 0; pvi--) {
			continue;
		}
	}
	if (3638 == 3638) {
		int kd;
		for (kd = 54; kd > 0; kd--) {
			continue;
		}
	}
	if (true == true) {
		int wvu;
		for (wvu = 86; wvu > 0; wvu--) {
			continue;
		}
	}
	if (true != true) {
		int bqebxsjc;
		for (bqebxsjc = 25; bqebxsjc > 0; bqebxsjc--) {
			continue;
		}
	}
	if (string("stwuawjelbpqegojbczemgmwenruwisnlmelkdjqtpaknajwpvd") != string("stwuawjelbpqegojbczemgmwenruwisnlmelkdjqtpaknajwpvd")) {
		int tyt;
		for (tyt = 21; tyt > 0; tyt--) {
			continue;
		}
	}

}

double etswayk::sespoiyqxc() {
	int btkcylggs = 2541;
	bool lvlrkeyfy = false;
	return 87360;
}

bool etswayk::imyvzkeiyslyzczmvc(string nlrzcdycpadp, bool oubndh, bool vuvrewgyrjcc, string aisrmzmuvdjw, double kyrdlcc, int ahusdtq) {
	double uhohguyjri = 11547;
	if (11547 == 11547) {
		int awg;
		for (awg = 14; awg > 0; awg--) {
			continue;
		}
	}
	if (11547 != 11547) {
		int kd;
		for (kd = 9; kd > 0; kd--) {
			continue;
		}
	}
	if (11547 == 11547) {
		int dyavtdry;
		for (dyavtdry = 92; dyavtdry > 0; dyavtdry--) {
			continue;
		}
	}
	if (11547 != 11547) {
		int kktojwy;
		for (kktojwy = 13; kktojwy > 0; kktojwy--) {
			continue;
		}
	}
	return false;
}

string etswayk::biddupzigntcrqtbc(int njzsbqccvvadzm) {
	bool srofybxzranpkpr = false;
	string okeji = "ras";
	string moiasfknv = "axkwzgfkygb";
	string mujmzmgvxlumkcj = "fsqrnvzjocytrmnzqepmbujulayhgakzt";
	bool ukezg = false;
	bool htqwfzklwc = false;
	string ymnisrpukwacfmk = "ssoazlcmbrwom";
	string fzfwvgrmvkpjja = "djcvroswconcnwfjbvltnsucmtrriecglaavncapunrmhyqhljhryypmihshbgdxbwygydzjvk";
	bool mgsgd = false;
	if (string("ras") == string("ras")) {
		int ueqcpk;
		for (ueqcpk = 85; ueqcpk > 0; ueqcpk--) {
			continue;
		}
	}
	if (string("djcvroswconcnwfjbvltnsucmtrriecglaavncapunrmhyqhljhryypmihshbgdxbwygydzjvk") == string("djcvroswconcnwfjbvltnsucmtrriecglaavncapunrmhyqhljhryypmihshbgdxbwygydzjvk")) {
		int cliyhm;
		for (cliyhm = 66; cliyhm > 0; cliyhm--) {
			continue;
		}
	}
	if (false != false) {
		int uuf;
		for (uuf = 96; uuf > 0; uuf--) {
			continue;
		}
	}
	if (string("ssoazlcmbrwom") != string("ssoazlcmbrwom")) {
		int ibtczeioyg;
		for (ibtczeioyg = 7; ibtczeioyg > 0; ibtczeioyg--) {
			continue;
		}
	}
	if (false != false) {
		int onjc;
		for (onjc = 2; onjc > 0; onjc--) {
			continue;
		}
	}
	return string("pea");
}

double etswayk::sqfrktoscpizfwdilwbihitzk(string sjwvkqljdvqf, double gqyjj, int yjxwfgutmbeev, string snhdvnpviqds, string ghtkysgwvbmi, int wbctahhoaaxv, int togipmbfsxgktgl) {
	int hefbtjqyctnx = 2353;
	if (2353 != 2353) {
		int yl;
		for (yl = 31; yl > 0; yl--) {
			continue;
		}
	}
	return 41971;
}

string etswayk::xwhtkcxgfeo(bool hmvug, bool bcdtuidjbtvc, string wiiuphaya, string qefbuja) {
	return string("gnnxfnapnrrg");
}

int etswayk::elirmvglfwbq(double xcfcwpf, double lyazuubtxvzxzk, int guyki, string uyirqkzobhp, double uebgjcxwfcxny) {
	int qztfwc = 3754;
	int wouezxqgbsplly = 2077;
	double wzuavytqfy = 35560;
	if (35560 == 35560) {
		int kquyow;
		for (kquyow = 57; kquyow > 0; kquyow--) {
			continue;
		}
	}
	if (3754 != 3754) {
		int qb;
		for (qb = 27; qb > 0; qb--) {
			continue;
		}
	}
	if (3754 != 3754) {
		int sz;
		for (sz = 20; sz > 0; sz--) {
			continue;
		}
	}
	return 34472;
}

string etswayk::mrsctuhjbrprchctdwvtpbkr(double ocwzplxwdh, string lxzquyr, string ohymyw, string ybnsiczgkhqlin, double vnlqkdqfv, double gzfjnbifonblg, double djzco, double qdbcrmjmrfvak) {
	int bxbgzaiyueqzz = 347;
	string uqpbs = "bsrpn";
	double sqkfgyzukopq = 24795;
	bool bnpvbudz = false;
	double qshwwzenx = 31628;
	string bsipcxafe = "pfrqovazqiecydyhouadmblqakydzsfrwzqyywoaxexbxkjhosdoqbmzqykrxowcxcbyizyofbxrnlqcdarqdcuwbehmy";
	int tpeisesemcbvc = 7275;
	bool mpczmrwogvirzk = false;
	bool tgrtqrfyv = false;
	if (false == false) {
		int pnz;
		for (pnz = 10; pnz > 0; pnz--) {
			continue;
		}
	}
	if (string("pfrqovazqiecydyhouadmblqakydzsfrwzqyywoaxexbxkjhosdoqbmzqykrxowcxcbyizyofbxrnlqcdarqdcuwbehmy") != string("pfrqovazqiecydyhouadmblqakydzsfrwzqyywoaxexbxkjhosdoqbmzqykrxowcxcbyizyofbxrnlqcdarqdcuwbehmy")) {
		int qziwidw;
		for (qziwidw = 90; qziwidw > 0; qziwidw--) {
			continue;
		}
	}
	return string("gyjjqqeeu");
}

int etswayk::eqwvswzszuhadlqlccapfkjb(bool nfjigrujswqxwt, string uicxfqbciqg) {
	bool vhhnfcxo = true;
	string qxkkw = "mbxvhohszyynweqoszgktkzmktcwvczxuvujyoibtcvdhwwwimadjxboimhwpwy";
	string vaefkax = "znxhejtkgjdtexplxacojoyovtejwfylfjcjxiuiybkicgfyfwychqaeltpwpesch";
	bool hmyvb = false;
	if (string("znxhejtkgjdtexplxacojoyovtejwfylfjcjxiuiybkicgfyfwychqaeltpwpesch") == string("znxhejtkgjdtexplxacojoyovtejwfylfjcjxiuiybkicgfyfwychqaeltpwpesch")) {
		int mfdjmgv;
		for (mfdjmgv = 52; mfdjmgv > 0; mfdjmgv--) {
			continue;
		}
	}
	return 76850;
}

string etswayk::mnoejaftwsnujwlznh(bool vtyxtnukdzo, double wuvppauzm, double qjxlv, double xcyfgajrtr, double qgmhpdoe, int gdoavfesprk, int femudncjiohdnx, double visebnwdkvtq, string nxwrovtbwbyeyd) {
	double wkkkw = 29953;
	string jzakrewrmorbh = "xjjoprunpgrvjlmsoicrkqlmsvdvfnhdqsbfgccmphtroooevcdvjsquec";
	bool hfnvyceavvwgzb = true;
	int gcuntgfffl = 1855;
	int vysratkhqj = 1629;
	int aksgpiat = 1737;
	string ejvwfnenpaxarxg = "ijblzukhnrtfpfhdbjfzcjyxslzdqnyigzo";
	double carwuzg = 97767;
	int dtxhzz = 3677;
	return string("mlwchvoddimg");
}

string etswayk::ufhsqugztfdzstfer(double eqioeid, double bupeinvec, double letpzod, double tlaytlmnnvk, bool oktbayhrpd, double pynmhsxdknmd, double qgyrctgre) {
	int vqdrry = 340;
	if (340 == 340) {
		int yl;
		for (yl = 83; yl > 0; yl--) {
			continue;
		}
	}
	if (340 != 340) {
		int duq;
		for (duq = 5; duq > 0; duq--) {
			continue;
		}
	}
	return string("vfnoyqvttpg");
}

bool etswayk::acpdgykpdwsuztbiffpd(double xoelclm, int hgjeykwmpovf, int kpzmrnsumpefh, bool fqnvhvhedpldev, double qnetvgpsa, string cjqlayieqcfq, bool sxfqjkkbccy) {
	double pcagmbfv = 654;
	string kikomv = "hudykmuugiqgeiehdxzogjnvoqucdojwtoxtgeophkqjbv";
	int igdxzv = 2939;
	int yyljzvovovfbpoo = 6857;
	int dfncniqfpizhiv = 3036;
	double dxirbakkpstmxj = 80935;
	int nuuxcqvbouoi = 3090;
	bool vnkbzercfoowuhh = true;
	if (80935 != 80935) {
		int qlwa;
		for (qlwa = 9; qlwa > 0; qlwa--) {
			continue;
		}
	}
	if (654 != 654) {
		int yicu;
		for (yicu = 14; yicu > 0; yicu--) {
			continue;
		}
	}
	return false;
}

string etswayk::gywiflabktfrnmp() {
	double bjjne = 62272;
	bool ygdllcm = false;
	int pvqecher = 429;
	double boomhb = 11144;
	int yhetaxcgvzdk = 4554;
	double ptonwo = 3806;
	return string("pcmjgtfjovwsxi");
}

string etswayk::xuiethpzdieytuwk(string fvvvcpgwiimlrs, bool cvnhskxftwawql, double bihhekx, int glsihsoxxakfud, int ayevampsbbv, int jivssvhsw, int pyxbenwytckn, bool rysdderv, int rshhswe, string dhdcwfyiqwafeq) {
	int qupbanfxduxkn = 6004;
	int vmmjypqqdnfyt = 710;
	bool evglgnodjgc = false;
	double xklttx = 15299;
	int yaqcthre = 2659;
	string zobdbbpkcovsx = "puattfitwhaxodihpifjljxxeswcxbfvq";
	int mmzxzv = 3998;
	string ruors = "vlxgavknddhvxfbutseczffwfsrlzpilzsrqpgqhjihylczcgadmkithsiyt";
	int dpritpneyasbq = 2737;
	string xdefwgbk = "xloazkgmuilfltdjahanrxuhmbwpdiwvldiprnztyhozrbsafrexjjqupslvadymxgwklhqi";
	return string("pcc");
}

etswayk::etswayk() {
	this->eqwvswzszuhadlqlccapfkjb(false, string("memquirtylerxfwixplxdkvlxzicuanjxajczcvspirqazcqmstcgyjzcgwh"));
	this->mnoejaftwsnujwlznh(true, 10155, 12202, 45001, 78156, 5644, 576, 18392, string("eloxstnsbqudzrebtaqceeymsxvaltoeuvpfcvernnufosmqejsyfgfvhdcvoudczddkmjyidhafvbxfbii"));
	this->ufhsqugztfdzstfer(22874, 11050, 8057, 32176, true, 29148, 10661);
	this->acpdgykpdwsuztbiffpd(44313, 5771, 3825, true, 37808, string("qtpvkkmhnssyyxntdzkpmtbuaamniqnduoqcjqvfbpfdwmqjcotlsxpvsnbeseadmcffmvskitgkfreay"), true);
	this->gywiflabktfrnmp();
	this->xuiethpzdieytuwk(string("vsynibncfvavytjdeyxesisyywbtmzqugwkrmksreudrb"), false, 28733, 6698, 6050, 810, 854, false, 1364, string("dwuimvdp"));
	this->ijjopkpkulc();
	this->svvrjlafxquowyxjheeojzjxu(38917, 511);
	this->sespoiyqxc();
	this->imyvzkeiyslyzczmvc(string("gatoajcphyjvcycmez"), true, true, string("weaqpcuuayatubydoqhwtfbzbnnjliiozcokfihkmwprhexnyedrtrrdotfdfbxhztbtbdutvdeqqlg"), 20237, 2845);
	this->biddupzigntcrqtbc(4749);
	this->sqfrktoscpizfwdilwbihitzk(string(""), 18305, 230, string("dhojpbtvnwgvwtnkyovserlwqwsdznpnltzhj"), string("fnkuqhefgudmwheknnqqflolhrcmexkmvdepmxglfzrmgicjhrewpgzw"), 3989, 3345);
	this->xwhtkcxgfeo(true, false, string("flrcgqtezdnrcncqdmzdubnuksid"), string("exylsquhbzjwddqxbcsmnsvbervnyrtcoulrvx"));
	this->elirmvglfwbq(24983, 18097, 2954, string("qprexbxlrxxrsxw"), 28403);
	this->mrsctuhjbrprchctdwvtpbkr(16136, string("dhbevopovdotqjrxldswbakotlljowvijyworfcofwrqelhpopjoovjxmoewnooikjfitkjjpxm"), string("lohjldljtaupivfkgcsaseiuiduyzkqeaybqkjrxcpzzowaqxgkv"), string("xcpspumutgmvgrmwqwvzgufqguzigxwclshhwdcfemmywrxbvty"), 10014, 5662, 12710, 57638);
	this->wzsdrvjfkkytnhtzbsc(false, false, 2668, string("jtztvxtnorzelnidekjuoyrujzwkocxcfwhgdzwgipwzkkmuiltbmyhwszbhuqmtxp"), 93016, string("hjmoczphgyrooepwvpcsxxuceyhcwtahurvuefdslyudmeovjxzlrjnydjpfmzoiqwxdgbgfgfpuszgzxtldjdwixbz"), false, 4220, 45104);
	this->ljqjfqlmilckvwmdfsggqi();
	this->utywnusrndmfignbfwjaih(false, true, 19935, string("ftzjuitdacaacvlgcswbbmsrefpnrlgwfhgwobagertcpgvendntuxlqvmplewnmrtiaxqzdxofeptsmndc"), 2065, string("lkkzduvjxfkkbofbfpigsgshnrkaumgjkumkgswrrvtksfufataavwkxzchoyvnexebkojt"), 1618, string("wxljzgqsdlprpyvkyufbrkwqmeq"), string("bsgbrfjkiivtbqvviaycttnlzleugjwmdugtimvfzdqf"), true);
	this->upwrrbpjnmk(362, 1043, 585, 9200);
	this->khzwcatzkcemnqrvgmmnyqhji(false, 15016, string("igreotladdeqqpiiyqlpgcakjjcoazkwuddvwaqlmwpseywigdmzegkuiumwdbsdamzkqewmd"), string("ulfypltlctzjsmaxlrppyaabqarpklbauqqpt"), string("cepsdsqgaawduxxrcjszzqbskygynjyrxkdrargzqoocaczsmldlnihjfvyerbiorjsfnpulwshhsgsohocnicjzkqhvgjiz"), true, 39912, string("exsdvzvlaboawtbdmhxzltchszpsjkgngsmfavrmgcoaanmfwickqwzcsbbtllgf"), 14745);
	this->gzlhecgfgzv(string("zqethhaozkkyvtvhxmnioeprdzepaaflktduqgmukaqwywpljbdfherdmyybddisuxxzvouleyquecfkybnnpxopwvxrmylfwc"), 9415, 28972, true, 10721, 1155);
	this->thyhvirspsquvkuyzkpf();
	this->lwmrreflyyj(false, 2721, 7337, 723, string("feaagadmqaxjspshvcfgucjdkfpnnsuzizzsjdhiowidtygkgmuxxkngrtzhzzhhpttihrucynukmaenjjvhepobpmbsoihhleh"), string("lbhbsjcfq"), false, 41663, string("omttnzpnvdxmilcpingtm"));
	this->mdxcvuqnwhv(73866, string("gjxzgigumdjxkllyblzppuiepmdwtdjnlyvqnhwesqflbevwosvmadliflwgdehvic"), string("axuzfevua"), false, false, 20775, false);
}
