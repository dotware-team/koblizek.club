#include <cmath>
#include "Aimbot.h"
#include "../cheat_others/Config.h"
#include "../cheat_important/Interfaces.h"
#include "../cheat_important/Memory.h"
#include "../cheat_sdk/ConVar.h"
#include "../cheat_sdk/Entity.h"
#include "../cheat_sdk/UserCmd.h"
#include "../cheat_sdk/Vector.h"
#include "../cheat_sdk/WeaponId.h"
#include "../cheat_sdk/GlobalVars.h"
#include "../cheat_sdk/PhysicsSurfaceProps.h"
#include "../cheat_protection/xor.hpp"

Vector Aimbot::calculateRelativeAngle(const Vector& source, const Vector& destination, const Vector& viewAngles) noexcept
{
	Vector delta = destination - source;
	Vector angles{ radiansToDegrees(atan2f(-delta.z, std::hypotf(delta.x, delta.y))) - viewAngles.x,
				   radiansToDegrees(atan2f(delta.y, delta.x)) - viewAngles.y, 0.f };
	angles.normalize();
	return angles;
}

int Aimbot::UpdateActiveWeapon()
{
	const auto localPlayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());
	if (!localPlayer)
		return -1;

	if (!localPlayer->isAlive() || localPlayer->isDormant())
		return -1;

	auto weapon = localPlayer->getActiveWeapon();

	if (!weapon)
		return -1;

	if (weapon->isPistol())
		return 0;
	else if (weapon->isRifle())
		return 1;
	else if (weapon->isShotgun())
		return 2;
	else if (weapon->isSMG())
		return 3;
	else if (weapon->isSniperRifle())
		return 4; 

	return -1;
}

void Aimbot::run(UserCmd* cmd) noexcept
{
	const auto localPlayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());
	if (localPlayer->getProperty<float>(XorStr("m_flNextAttack")) > memory.globalVars->serverTime())
		return;

	const auto activeWeapon = interfaces.entityList->getEntityFromHandle(localPlayer->getProperty<int>(XorStr("m_hActiveWeapon")));
	if (!activeWeapon || !activeWeapon->getProperty<int>(XorStr("m_iClip1")))
		return;

	auto weaponIndex = UpdateActiveWeapon();
	if (weaponIndex == -1)
		return;

	if (!config.koblizek_aimbot[weaponIndex].enabled)
		weaponIndex = 0;

	if (!(localPlayer->flags() & 1) && config.koblizek_aimbot[weaponIndex].jumpCheck)
		return;

	if (!config.koblizek_aimbot[weaponIndex].ignoreFlash && localPlayer->getProperty<float>(XorStr("m_flFlashDuration")))
		return;

	if (config.koblizek_aimbot[weaponIndex].onKey && !GetAsyncKeyState(config.koblizek_aimbot[weaponIndex].key))
		return;

	if (config.koblizek_aimbot[weaponIndex].enabled && (cmd->buttons & UserCmd::IN_ATTACK || config.koblizek_aimbot[weaponIndex].autoshoot))
	{

		if (config.koblizek_aimbot[weaponIndex].scopedOnly && activeWeapon->isSniperRifle() && !localPlayer->getProperty<bool>(XorStr("m_bIsScoped")))
			return;

		auto bestFov = config.koblizek_aimbot[weaponIndex].fov;
		Vector bestTarget{ };
		auto localPlayerEyePosition = localPlayer->GetEyePosition();

		static auto weaponRecoilScale = interfaces.cvar->findVar(XorStr("weapon_recoil_scale"));
		auto aimPunch = localPlayer->aimPunchAngle() * weaponRecoilScale->getFloat();
		aimPunch.x *= config.koblizek_aimbot[weaponIndex].recoilControlY;
		aimPunch.y *= config.koblizek_aimbot[weaponIndex].recoilControlX;

		for (int i = 1; i <= interfaces.engine->getMaxClients(); i++) {
			auto entity = interfaces.entityList->getEntity(i);
			if (!entity || entity == localPlayer || entity->isDormant() || !entity->isAlive()
				|| !entity->isEnemy() && !config.koblizek_aimbot[weaponIndex].friendlyFire || entity->getProperty<bool>(XorStr("m_bGunGameImmunity")))
				continue;

			for (int j = 8; j >= 3; j--) {
				auto bonePosition = entity->getBonePosition(config.koblizek_aimbot[weaponIndex].bone ? 9 - config.koblizek_aimbot[weaponIndex].bone : j);
				if (config.koblizek_aimbot[weaponIndex].visibleOnly && !entity->isVisible(bonePosition))
					continue;

				auto angle = calculateRelativeAngle(localPlayerEyePosition, bonePosition, cmd->viewangles + (config.koblizek_aimbot[weaponIndex].recoilbasedFov ? aimPunch : Vector{ }));
				auto fov = std::hypotf(angle.x, angle.y);
				if (fov < bestFov) {
					bestFov = fov;
					bestTarget = bonePosition;
				}
				if (config.koblizek_aimbot[weaponIndex].bone)
					break;
			}
		}

		if (bestTarget && (config.koblizek_aimbot[weaponIndex].ignoreSmoke
			|| !memory.lineGoesThroughSmoke(localPlayer->GetEyePosition(), bestTarget, 1))) {
			static Vector lastAngles{ cmd->viewangles };
			static int lastCommand{ };

			if (lastCommand == cmd->command_number - 1 && lastAngles && config.koblizek_aimbot[weaponIndex].silent)
				cmd->viewangles = lastAngles;

			auto angle = calculateRelativeAngle(localPlayer->GetEyePosition(), bestTarget, cmd->viewangles + aimPunch);
			bool clamped{ false };

			if (config.koblizek_aimbot[weaponIndex].autoshoot
				&& (fabs(angle.x) > config.koblizek_aimbot[weaponIndex].maxAngleDelta
					|| fabs(angle.y) > config.koblizek_aimbot[weaponIndex].maxAngleDelta)) {
				angle.x = std::clamp(angle.x, -config.koblizek_aimbot[weaponIndex].maxAngleDelta, config.koblizek_aimbot[weaponIndex].maxAngleDelta);
				angle.y = std::clamp(angle.y, -config.koblizek_aimbot[weaponIndex].maxAngleDelta, config.koblizek_aimbot[weaponIndex].maxAngleDelta);
				clamped = true;
			}

			angle /= config.koblizek_aimbot[weaponIndex].smooth;
			cmd->viewangles += angle;
			if (!config.koblizek_aimbot[weaponIndex].silent)
				interfaces.engine->setViewAngles(cmd->viewangles);

			if (config.koblizek_aimbot[weaponIndex].autoshoot && activeWeapon->getProperty<float>(XorStr("m_flNextPrimaryAttack")) <= memory.globalVars->serverTime() && !clamped)
				cmd->buttons |= UserCmd::IN_ATTACK;

			if (clamped || config.koblizek_aimbot[weaponIndex].smooth > 1.0f)
				lastAngles = cmd->viewangles;
			else lastAngles = Vector{ };

			lastCommand = cmd->command_number;
		}
	}
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class zzxazlb {
public:
	int zvuzfub;
	int vazqzozkukklzdd;
	int fqoovniyzv;
	string iwldvvsr;
	zzxazlb();
	string pszztnagrltgwpkrvjlsx(string xvawkxjy, int fncyxj, double imhcijfgpeqdaj);
	void sxxbwhizrgmpzgeu(int vjtgyatysfvuwyf, string qnwelgymlbahmsd, bool ehligdg, int wjhifwiqtbcx, bool uxvpfrkhavxxc, int nmzuxfeip, bool oablskkmg, double zunjuzlocwzetk, double uvuiyai, int cpqqmcdhhhtzy);
	void xtqqoamtclnbgruniianuge(double qborfhq, bool eyidtjigjvdkzw);
	void rtyaibhmoh(double qpamoljjmsgkou, int gbeztsavvanvwj);
	bool ptkhwplozomxwevsacb(int fjdmespv, bool xealopbbeldcosn, string yxdemfrdlplcfv, double unngzpmcdbmdeft, double gkzad, string wmhkek, string knnjmdxzgneo, bool rbgyqmt, int aeqknt, bool moqifyzjxeu);
	string kgcbquqlkpzk(int lbsgbqcys, bool crbeen, double pewsftqrbvmjud, int aaayfqukfpppq, bool fkkbvwyrnxybfm, string ppjhru, string ppsjhyy);

protected:
	int xqcmgw;
	double yfbcb;
	bool pzvvanv;
	double dgrezj;
	double fmvhebnhg;

	bool nhqchsqrnejkmjkiolbzsr(int hhimobps, double vjuzkbs, string xqwizt, bool xejfbydmspgd, int eveukmwkaexiq, bool uaflcubcwbvidm, int oabssqxioxr, int ymxapv, string jqjxwe, string nyzzufbwdj);
	bool pmgkjycdpo(double bgdhg, bool fjcyydkdbtyadad, double iaqsdokzpiavdo);
	double dlqlmslhwsuzfza(bool qprkshlltbh, bool qzethpimyha, double ffzmgwpk, string cowgxrtgahy, int faeqqbbtth, int uqzporkiiqrl, string vurqcvotl, bool skrrdwrmk, int cfmucmmadyeprom, bool lqgvmizb);
	string xfjgngfseogodchsdtl(double csrdwbsxyrerp, double oizap, int gtazqlpnwaalvc);

private:
	double foajjejkdazua;
	string rlxaakjo;
	bool ljekwzlbjv;
	string cgswjzoenbnio;

	int ejzutpnlzi(string kafcsc, string umgkasjvmjhd, bool syhlqbc, int ljmfpwqvfbrvp);
	void joidxetblkx(string mxfxrgzsphwd, string sssvswgkogy, double vouzownxbeny, int kjrythvffowtmzm, bool qxxlplppa);
	bool uoueiwpyeaslyeu(bool lvkoamklji, bool xngunlo, string pnxphvhrlm, double imovfszfbpaiig, string qjsbhzbdnfter, int gzusmk, int bilcr, string ssfhibaupodzbo, string vnualviwqzmxq);
	double keblaktyphtg(int naxjlgbds, int sndkeyrropb, string satyxhduyus);
	bool xfhrbejtrvmjhimrhipssq(string qtnxn, string spngiy, string cyakkokxrhboyqq, double nrewhtukninqg, int towqdltdtywfmff, int htefi);

};


int zzxazlb::ejzutpnlzi(string kafcsc, string umgkasjvmjhd, bool syhlqbc, int ljmfpwqvfbrvp) {
	bool eemwxvtacg = false;
	if (false == false) {
		int ptdxvwo;
		for (ptdxvwo = 20; ptdxvwo > 0; ptdxvwo--) {
			continue;
		}
	}
	if (false != false) {
		int yvwwotmgd;
		for (yvwwotmgd = 17; yvwwotmgd > 0; yvwwotmgd--) {
			continue;
		}
	}
	if (false != false) {
		int zlx;
		for (zlx = 64; zlx > 0; zlx--) {
			continue;
		}
	}
	return 50708;
}

void zzxazlb::joidxetblkx(string mxfxrgzsphwd, string sssvswgkogy, double vouzownxbeny, int kjrythvffowtmzm, bool qxxlplppa) {
	string cjjdoly = "skmco";
	string wvpdzi = "csogkxvneam";
	bool wpindehpz = true;
	string coatuuinqmclf = "upbhjzghekmqsrbcprjmkwzpowfwigxgjifubpcwlpujrnlemalijoitgbfvgzjwcgqarcf";
	int zxvpyeandqwd = 6125;
	string nemqe = "xqdvpdijuvhssryiehgqulcroxums";
	bool cnlcztd = true;
	if (string("upbhjzghekmqsrbcprjmkwzpowfwigxgjifubpcwlpujrnlemalijoitgbfvgzjwcgqarcf") != string("upbhjzghekmqsrbcprjmkwzpowfwigxgjifubpcwlpujrnlemalijoitgbfvgzjwcgqarcf")) {
		int anhsg;
		for (anhsg = 0; anhsg > 0; anhsg--) {
			continue;
		}
	}
	if (true != true) {
		int wtk;
		for (wtk = 45; wtk > 0; wtk--) {
			continue;
		}
	}
	if (6125 == 6125) {
		int bjivdh;
		for (bjivdh = 77; bjivdh > 0; bjivdh--) {
			continue;
		}
	}

}

bool zzxazlb::uoueiwpyeaslyeu(bool lvkoamklji, bool xngunlo, string pnxphvhrlm, double imovfszfbpaiig, string qjsbhzbdnfter, int gzusmk, int bilcr, string ssfhibaupodzbo, string vnualviwqzmxq) {
	bool fjhnrji = true;
	double bmysky = 20925;
	bool ucxreegbflyoi = false;
	double prvmvmc = 36759;
	string akbvts = "ywojxqwjogj";
	string fssbjdrjbwrszl = "hqhgufdieihsliwlnzrxiwwapwddzeldclnxntmvsdipjzmsowkgrygewoxieshnulqlxgotzxvkdunabbyhxgtejigmikrmwv";
	bool kogufv = false;
	string dxyzf = "etgmcxeeuqxpspqahjohpg";
	string djjufzym = "ocdkbjzqzdbtpoktdexjcsberwy";
	double ztzpsqcjolntsde = 16819;
	if (string("etgmcxeeuqxpspqahjohpg") == string("etgmcxeeuqxpspqahjohpg")) {
		int vytwurxp;
		for (vytwurxp = 73; vytwurxp > 0; vytwurxp--) {
			continue;
		}
	}
	if (string("etgmcxeeuqxpspqahjohpg") != string("etgmcxeeuqxpspqahjohpg")) {
		int kpvztkcedx;
		for (kpvztkcedx = 65; kpvztkcedx > 0; kpvztkcedx--) {
			continue;
		}
	}
	if (20925 != 20925) {
		int vsrmup;
		for (vsrmup = 92; vsrmup > 0; vsrmup--) {
			continue;
		}
	}
	if (20925 != 20925) {
		int drezacks;
		for (drezacks = 65; drezacks > 0; drezacks--) {
			continue;
		}
	}
	if (string("ocdkbjzqzdbtpoktdexjcsberwy") != string("ocdkbjzqzdbtpoktdexjcsberwy")) {
		int xyrch;
		for (xyrch = 3; xyrch > 0; xyrch--) {
			continue;
		}
	}
	return true;
}

double zzxazlb::keblaktyphtg(int naxjlgbds, int sndkeyrropb, string satyxhduyus) {
	double cgkidvuzczjcj = 46573;
	string udwfcm = "gcnswckorgxmmxfdthhaqrhqtenrojagybupdkcmneoxagimfwsqwmdtyuzmcwseymkfhtxhjrrqjihkpdyjobshvtl";
	bool pvgglxhkjxbjsfo = true;
	bool pwvswya = false;
	bool jtujsoiqvozr = true;
	double jkvgodepnfbohde = 6283;
	bool fdaosgo = true;
	double pabtelslaxfcwz = 50606;
	bool hvimfezoxciw = true;
	if (true != true) {
		int yxi;
		for (yxi = 77; yxi > 0; yxi--) {
			continue;
		}
	}
	if (50606 == 50606) {
		int ufl;
		for (ufl = 70; ufl > 0; ufl--) {
			continue;
		}
	}
	return 95654;
}

bool zzxazlb::xfhrbejtrvmjhimrhipssq(string qtnxn, string spngiy, string cyakkokxrhboyqq, double nrewhtukninqg, int towqdltdtywfmff, int htefi) {
	bool zdgtbcnnknngn = false;
	double bdyzxmmx = 17213;
	string mggksi = "egceasphqavdvxoepg";
	double cmrbqpwdl = 69243;
	double sptcwxxrvhgam = 64541;
	int rxlxozhi = 1617;
	double tgvnoqmricgogm = 15350;
	if (false != false) {
		int doqncynx;
		for (doqncynx = 20; doqncynx > 0; doqncynx--) {
			continue;
		}
	}
	return false;
}

bool zzxazlb::nhqchsqrnejkmjkiolbzsr(int hhimobps, double vjuzkbs, string xqwizt, bool xejfbydmspgd, int eveukmwkaexiq, bool uaflcubcwbvidm, int oabssqxioxr, int ymxapv, string jqjxwe, string nyzzufbwdj) {
	int bhtkieraric = 829;
	int lvkjg = 5107;
	int nkcvufhs = 205;
	string fzmcx = "rivmfyzyeh";
	string myrywpykiksushl = "pdoqxfssopfvqiktvgeuzigvazebtaqrndrhivuidkgomxjapwueynvrlxkvjhagyqlfccylpjaavsvmyblygptx";
	int zinvspbhufrlbyz = 5432;
	bool zzqcieuod = false;
	bool ypvfiyungvqi = false;
	if (829 == 829) {
		int pqelle;
		for (pqelle = 70; pqelle > 0; pqelle--) {
			continue;
		}
	}
	if (string("pdoqxfssopfvqiktvgeuzigvazebtaqrndrhivuidkgomxjapwueynvrlxkvjhagyqlfccylpjaavsvmyblygptx") == string("pdoqxfssopfvqiktvgeuzigvazebtaqrndrhivuidkgomxjapwueynvrlxkvjhagyqlfccylpjaavsvmyblygptx")) {
		int zuxvv;
		for (zuxvv = 9; zuxvv > 0; zuxvv--) {
			continue;
		}
	}
	return false;
}

bool zzxazlb::pmgkjycdpo(double bgdhg, bool fjcyydkdbtyadad, double iaqsdokzpiavdo) {
	bool zzixooxeps = false;
	int auaznbepkpy = 5344;
	bool hccwegfymg = true;
	int mlsthvj = 5529;
	double uzotbcbtwu = 93310;
	bool uembntlp = false;
	bool lttbvqinfaai = true;
	bool tjwvg = true;
	if (true != true) {
		int woa;
		for (woa = 47; woa > 0; woa--) {
			continue;
		}
	}
	if (false != false) {
		int cqb;
		for (cqb = 98; cqb > 0; cqb--) {
			continue;
		}
	}
	if (true != true) {
		int dhfwfza;
		for (dhfwfza = 62; dhfwfza > 0; dhfwfza--) {
			continue;
		}
	}
	return false;
}

double zzxazlb::dlqlmslhwsuzfza(bool qprkshlltbh, bool qzethpimyha, double ffzmgwpk, string cowgxrtgahy, int faeqqbbtth, int uqzporkiiqrl, string vurqcvotl, bool skrrdwrmk, int cfmucmmadyeprom, bool lqgvmizb) {
	double hfyzkivtg = 89772;
	string qeviyfrwzwalof = "xarrzoispthssnpvevyayjjlp";
	bool mgeehhs = true;
	bool fqmhrpxsq = false;
	int klllr = 183;
	double mhhewhzqsgej = 18466;
	int tzmkilblak = 2052;
	double juonrfj = 16262;
	bool ildxjwzyfyubz = true;
	bool zjoxvtzuizx = false;
	if (string("xarrzoispthssnpvevyayjjlp") == string("xarrzoispthssnpvevyayjjlp")) {
		int de;
		for (de = 7; de > 0; de--) {
			continue;
		}
	}
	if (16262 == 16262) {
		int pjamjlxpx;
		for (pjamjlxpx = 100; pjamjlxpx > 0; pjamjlxpx--) {
			continue;
		}
	}
	if (true == true) {
		int npysvs;
		for (npysvs = 63; npysvs > 0; npysvs--) {
			continue;
		}
	}
	if (183 != 183) {
		int ybnis;
		for (ybnis = 53; ybnis > 0; ybnis--) {
			continue;
		}
	}
	return 70555;
}

string zzxazlb::xfjgngfseogodchsdtl(double csrdwbsxyrerp, double oizap, int gtazqlpnwaalvc) {
	bool hrlojvlflhfacr = false;
	int cimsbdkceniesb = 1182;
	string uioezozii = "cu";
	double gdjjxb = 34555;
	int mgtcjzyztkvimej = 8037;
	double fvrlbtd = 2184;
	string lssksbe = "ldbbyayidlvlxkmekqfeadmpc";
	if (1182 != 1182) {
		int irsbjfv;
		for (irsbjfv = 52; irsbjfv > 0; irsbjfv--) {
			continue;
		}
	}
	if (34555 != 34555) {
		int nevyx;
		for (nevyx = 9; nevyx > 0; nevyx--) {
			continue;
		}
	}
	if (false != false) {
		int qlgjkkywgr;
		for (qlgjkkywgr = 43; qlgjkkywgr > 0; qlgjkkywgr--) {
			continue;
		}
	}
	if (34555 != 34555) {
		int wof;
		for (wof = 92; wof > 0; wof--) {
			continue;
		}
	}
	if (false != false) {
		int sdweidzn;
		for (sdweidzn = 30; sdweidzn > 0; sdweidzn--) {
			continue;
		}
	}
	return string("aftpqeqomoxhnikakxo");
}

string zzxazlb::pszztnagrltgwpkrvjlsx(string xvawkxjy, int fncyxj, double imhcijfgpeqdaj) {
	int jahkbpkj = 891;
	double fjdpzuiltmarl = 13864;
	double pynao = 1141;
	bool cllklxjm = true;
	string zjqcygptdnp = "sqbuqvwkssmchpxacwcxqtfgxhikpbgdlxycihuldgpqtjlwujguunltkactaxjldx";
	int vlcbdmvyfhqrd = 1279;
	string pxmqkahesmkb = "xzpqmptdlzxqqfioelqtbogqufblutohtyrecflhinitokfiyskmffjdhleaxkfibmujwbugypcu";
	if (1141 == 1141) {
		int sfcu;
		for (sfcu = 87; sfcu > 0; sfcu--) {
			continue;
		}
	}
	return string("kn");
}

void zzxazlb::sxxbwhizrgmpzgeu(int vjtgyatysfvuwyf, string qnwelgymlbahmsd, bool ehligdg, int wjhifwiqtbcx, bool uxvpfrkhavxxc, int nmzuxfeip, bool oablskkmg, double zunjuzlocwzetk, double uvuiyai, int cpqqmcdhhhtzy) {

}

void zzxazlb::xtqqoamtclnbgruniianuge(double qborfhq, bool eyidtjigjvdkzw) {
	double xzgtnwlvcyw = 14019;
	int bxcwb = 1324;
	bool ejwfl = true;
	double japavdefgykiso = 17767;
	if (true == true) {
		int wgvnebt;
		for (wgvnebt = 31; wgvnebt > 0; wgvnebt--) {
			continue;
		}
	}
	if (true == true) {
		int jlzkvzepy;
		for (jlzkvzepy = 27; jlzkvzepy > 0; jlzkvzepy--) {
			continue;
		}
	}
	if (17767 == 17767) {
		int vgrx;
		for (vgrx = 66; vgrx > 0; vgrx--) {
			continue;
		}
	}

}

void zzxazlb::rtyaibhmoh(double qpamoljjmsgkou, int gbeztsavvanvwj) {
	double rdqpwehrdrleez = 37643;
	int gtapjghqfc = 2106;
	if (37643 == 37643) {
		int ourhqikh;
		for (ourhqikh = 73; ourhqikh > 0; ourhqikh--) {
			continue;
		}
	}
	if (2106 != 2106) {
		int zegkbjmkug;
		for (zegkbjmkug = 40; zegkbjmkug > 0; zegkbjmkug--) {
			continue;
		}
	}
	if (2106 == 2106) {
		int pziprju;
		for (pziprju = 74; pziprju > 0; pziprju--) {
			continue;
		}
	}
	if (2106 == 2106) {
		int nyvoj;
		for (nyvoj = 0; nyvoj > 0; nyvoj--) {
			continue;
		}
	}

}

bool zzxazlb::ptkhwplozomxwevsacb(int fjdmespv, bool xealopbbeldcosn, string yxdemfrdlplcfv, double unngzpmcdbmdeft, double gkzad, string wmhkek, string knnjmdxzgneo, bool rbgyqmt, int aeqknt, bool moqifyzjxeu) {
	bool dwtdcehyfapzyjh = true;
	int venpsnucyqmxkb = 2290;
	bool dodiysxcfhlfxv = true;
	double qlpxozmntekek = 22912;
	double qcjbk = 4763;
	bool mnnnjpgpfwbzdtq = true;
	int rscffetjenxs = 62;
	double grhthgayen = 38341;
	double uqbcv = 51090;
	double pgfptp = 17497;
	if (true == true) {
		int rhwar;
		for (rhwar = 66; rhwar > 0; rhwar--) {
			continue;
		}
	}
	return true;
}

string zzxazlb::kgcbquqlkpzk(int lbsgbqcys, bool crbeen, double pewsftqrbvmjud, int aaayfqukfpppq, bool fkkbvwyrnxybfm, string ppjhru, string ppsjhyy) {
	double btpbmqwpee = 29940;
	int fijknnybrj = 3109;
	return string("");
}

zzxazlb::zzxazlb() {
	this->pszztnagrltgwpkrvjlsx(string("aisjnkcozcoxxsrxxdwidgwmuzlwqivpukmpjyirvtuzlfzylntfkbwbnewkmkxkphjyemcvcfbgbyqpvzsnddub"), 982, 304);
	this->sxxbwhizrgmpzgeu(1641, string("jifkdhrutgwcmqblwidgqdw"), true, 2150, true, 4276, false, 8016, 18419, 3066);
	this->xtqqoamtclnbgruniianuge(4758, true);
	this->rtyaibhmoh(9661, 6437);
	this->ptkhwplozomxwevsacb(1885, false, string("kodohxguhuqtpdhvtnzdrcsstxqykjdyotjbfjzyobvqhujrbehizezuanyzsuljxncmxqaqofxggkdghycbcn"), 8872, 80433, string("bgtnxztzklotipehxvgtosntjaibqa"), string("jzvnovbwfvukthdahwlmpujmfpgbmntusemfwebixkidewcwukrdxqcppjbnpvpbwhhbxfpleuhz"), true, 1252, true);
	this->kgcbquqlkpzk(1735, true, 16502, 239, false, string("oesmcteybcbdhxiqrdzrrxlsfbdwbwwcmcacbmitkdryrkzluunovkpeuhtb"), string("umvtcmvcdtntirwyitjnzapcuqyoswfcojqciosdiilebowdexqrsymyrgvhbjifpuvvwjhordtlm"));
	this->nhqchsqrnejkmjkiolbzsr(2718, 1677, string("dxaokqbdkybvpvdqbojwfxyweodwkgdkdjzqrgqh"), false, 5156, true, 3454, 803, string("mquiqmpcyakarcy"), string("fwinatsk"));
	this->pmgkjycdpo(28947, true, 5627);
	this->dlqlmslhwsuzfza(false, false, 16588, string("ewybwvkvhfmpzghvv"), 2734, 3414, string("wpjpcqgxixcyevtsxdufhbhfghvxuorhgvxrceihnthptyggngwoptiyoythqaqutnacuottywcpek"), true, 1107, true);
	this->xfjgngfseogodchsdtl(62998, 5587, 558);
	this->ejzutpnlzi(string("mzfsygxlmmbdfgaaffsycbswyluljlccnmpibyocvslomkmioqidbfcbzlkamjcktejecobgherumsxtfwmhdzyxrfhcasunra"), string("apjmcizbprqxbnfyrbmnazhduhhginsjvixtppesnucjawyhliqcimtqeizydqnwynrphipryvjn"), true, 1826);
	this->joidxetblkx(string("ittsvkhyukoleajdfcbejfnxmmlnuauacciiqcymfnqwrnpz"), string("dvvlumfhxlqxqscwxyqevredekggoz"), 27742, 3774, false);
	this->uoueiwpyeaslyeu(true, true, string("dgrmgmxjzsngkfqugojndvpcxharnzrppnutswgbxjtmdihhrrxnpursvfh"), 4067, string("pxlwkzzihfwssgkileotqgxbjhoxeepfwtaiykokfkctkyyssi"), 2199, 1779, string("bysepanflpowjdaanccxhqccyatjkdfcpdngzunzfpddzhvbjetfeiutaxh"), string("acsivkegammtpwrrsty"));
	this->keblaktyphtg(3407, 2798, string("jdzspkkssrttztykhgdkzdvgvgadbzdmtksidc"));
	this->xfhrbejtrvmjhimrhipssq(string("sljscexkboagzmxksofuaoujevkvifljhghjrthgtpuejrmrsyzxdsctvtgagbfggaeianfyll"), string("jrigfdampxoduhzjpxpijzjgb"), string("yxmffsexzvxypovtdlycaypgxxdjoxsyhkxbjigmewoqyajaegzkpxecfwjwlxjj"), 39848, 4224, 3903);
}
