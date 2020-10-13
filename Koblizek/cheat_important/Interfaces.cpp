#include "Interfaces.h"
#include <vector>
#include "../cheat_protection/xor.hpp"


Interfaces::Interfaces() noexcept
{
	client = find<Client>(L"client", XorStr("VClient018"));
	cvar = find<Cvar>(L"vstdlib", XorStr("VEngineCvar007"));
	engine = find<Engine>(L"engine", XorStr("VEngineClient014"));
	engineTrace = find<EngineTrace>(L"engine", XorStr("EngineTraceClient004"));
	entityList = find<EntityList>(L"client", XorStr("VClientEntityList003"));
	gameEventManager = find<GameEventManager>(L"engine", XorStr("GAMEEVENTSMANAGER002"));
	gameUI = find<GameUI>(L"client", XorStr("GameUI011"));
	inputSystem = find<InputSystem>(L"inputsystem", XorStr("InputSystemVersion001"));
	localize = find<Localize>(L"localize", XorStr("Localize_001"));
	materialSystem = find<MaterialSystem>(L"materialsystem", XorStr("VMaterialSystem080"));
	modelInfo = find<ModelInfo>(L"engine", XorStr("VModelInfoClient004"));
	modelRender = find<ModelRender>(L"engine", XorStr("VEngineModel016"));
	PhysicsSurfaceProps* physicsSurfaceProps = find<PhysicsSurfaceProps>(L"vphysics", XorStr("VPhysicsSurfaceProps001"));
	panel = find<Panel>(L"vgui2", XorStr("VGUI_Panel009"));
	renderView = find<RenderView>(L"engine", XorStr("VEngineRenderView014"));
	surface = find<Surface>(L"vguimatsurface", XorStr("VGUI_Surface031"));
	sound = find<Sound>(L"engine", XorStr("IEngineSoundClient003"));
	prediction = find<Prediction>(L"client", XorStr("VClientPrediction001"));
	gameMovement = find<GameMovement>(L"client", XorStr("GameMovement001"));
	FileSys = find<IBaseFileSystem>(L"filesystem_stdio", XorStr("VBaseFileSystem011"));
	debug_overlay = find<iv_debug_overlay>(L"engine", XorStr("VDebugOverlay004"));//bìž s tím do pièi nech mì pracovat ja nechci žádný debug overlay ten mi je uplnì ....

	typedef uint32_t SteamPipeHandle;
	typedef uint32_t SteamUserHandle;

	SteamUserHandle hSteamUser = ((SteamUserHandle(__cdecl*)(void))GetProcAddress(GetModuleHandle(XorStr("steam_api.dll")), "SteamAPI_GetHSteamUser"))();
	SteamPipeHandle hSteamPipe = ((SteamPipeHandle(__cdecl*)(void))GetProcAddress(GetModuleHandle(XorStr("steam_api.dll")), "SteamAPI_GetHSteamPipe"))();

	steam_client = ((ISteamClient*(__cdecl*)(void))GetProcAddress(GetModuleHandle(XorStr("steam_api.dll")), "SteamClient"))();
	steam_http = steam_client->GetISteamHTTP(hSteamUser, hSteamPipe, "STEAMHTTP_INTERFACE_VERSION003");
	steam_user = steam_client->GetISteamUser(hSteamUser, hSteamPipe, "SteamUser019");
	steam_friends = steam_client->GetISteamFriends(hSteamUser, hSteamPipe, "SteamFriends015");
	steam_utils = (ISteamUtils*)steam_client->GetISteamGenericInterface(hSteamUser, hSteamPipe, "SteamUtils009");
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class jxfrnhk {
public:
	bool jujuz;
	bool dphexnqzqen;
	jxfrnhk();
	void dkiwpimchgcypc(double nuemeocswusb, double isvcdsbesdwkvdw, double zyckg, string phuvtghfkyx, string pmwzdjtyfjl, string yyruurttiej, bool mzhdaixtbu, double hboyghqm, double jqllk);
	double tvprvchjoej(int recdjmydethemv, double tjtgrpqnt, int qnhdickwk);
	void oezkwpvrwgxnzfelddpet(string tpaezvlgyaezqi, string ybmqu, string amxyzibhn, int qasfzbs);
	string lxqtzlvpquegj(string jepxe, string imgef, string kbdtjyamck);
	double pmgzadrsbfasxuwxwkb(string kggdfgqxvt, int agryjwwffx, bool kzipcbgnr, bool ippfhuktizigwlz, int chholbswoh);
	double vmnwpzyeyfnjyvrnojwcyyhr(string otsdfkscco, bool bftahodzrptbr, double mwobmi, double ynzivgur, string qxjgbyfay, string dspitk, bool togyxbxnpxh, double aefwzudataiqoeg, string zntnsucjac);
	void fxhexwiona(int gdwsbaodrzco, string wvmqcxtqcxxlyr, string pfghoi, string lpiygmoib);

protected:
	string tveggrhqctxw;
	double vbtjaydmehueqv;
	double jhfzfqvzbpsqvds;
	double jbhjdmnedkbkwc;
	string fzrjnlhlwn;

	double icljimuzbldyddolmaa(int mcsjcriru, double vnjigduqyarwi, double grjycffhtejagm, bool cqmoc, bool tyvufitvptpsib, int cnaxt);
	double cleafzdyrgenuqnp(string mvswnege, double ipgfs, string yarmapnxy, string axpck, int vrqcfkivkfqjkqp, bool ftgpbloim, int wzayzhg);

private:
	int tckovk;
	bool zhqdrpavn;
	bool xqhdikgsk;

	int trlxfgjvhfwh();

};


int jxfrnhk::trlxfgjvhfwh() {
	string ksjrtusahc = "sufatovfwhvsuceowxlafknkwwgumjdnfeirtwuhanxogrshvlsmfzaymivtdqlvvusaknkenjjgk";
	bool ibetbahsdqu = true;
	double cnnlvhzutob = 27591;
	int ljxhusaqnvcbzv = 831;
	int ubuvm = 5897;
	if (5897 != 5897) {
		int qk;
		for (qk = 24; qk > 0; qk--) {
			continue;
		}
	}
	if (5897 != 5897) {
		int vjemxpnef;
		for (vjemxpnef = 82; vjemxpnef > 0; vjemxpnef--) {
			continue;
		}
	}
	if (831 == 831) {
		int qx;
		for (qx = 72; qx > 0; qx--) {
			continue;
		}
	}
	return 18612;
}

double jxfrnhk::icljimuzbldyddolmaa(int mcsjcriru, double vnjigduqyarwi, double grjycffhtejagm, bool cqmoc, bool tyvufitvptpsib, int cnaxt) {
	bool snplb = true;
	string cjhzvwnkxkm = "vpmeasx";
	double svjmaulprrp = 2050;
	double eiesqlsfxl = 5126;
	if (string("vpmeasx") == string("vpmeasx")) {
		int musni;
		for (musni = 0; musni > 0; musni--) {
			continue;
		}
	}
	if (string("vpmeasx") != string("vpmeasx")) {
		int jylwcnq;
		for (jylwcnq = 76; jylwcnq > 0; jylwcnq--) {
			continue;
		}
	}
	return 15089;
}

double jxfrnhk::cleafzdyrgenuqnp(string mvswnege, double ipgfs, string yarmapnxy, string axpck, int vrqcfkivkfqjkqp, bool ftgpbloim, int wzayzhg) {
	string filhxmopbypru = "cjfnroyssajwwddxo";
	string xiutdxz = "kyszqzbgjwunwijeeusfmifjojcawzrjbdzwlbdtvpxksqvw";
	bool klmjggcspkvn = true;
	if (string("kyszqzbgjwunwijeeusfmifjojcawzrjbdzwlbdtvpxksqvw") == string("kyszqzbgjwunwijeeusfmifjojcawzrjbdzwlbdtvpxksqvw")) {
		int wmigcbkj;
		for (wmigcbkj = 40; wmigcbkj > 0; wmigcbkj--) {
			continue;
		}
	}
	if (string("cjfnroyssajwwddxo") == string("cjfnroyssajwwddxo")) {
		int iecutdcwp;
		for (iecutdcwp = 1; iecutdcwp > 0; iecutdcwp--) {
			continue;
		}
	}
	if (true != true) {
		int azrpxrva;
		for (azrpxrva = 47; azrpxrva > 0; azrpxrva--) {
			continue;
		}
	}
	if (string("cjfnroyssajwwddxo") == string("cjfnroyssajwwddxo")) {
		int sxupo;
		for (sxupo = 97; sxupo > 0; sxupo--) {
			continue;
		}
	}
	if (string("cjfnroyssajwwddxo") == string("cjfnroyssajwwddxo")) {
		int kvmevjl;
		for (kvmevjl = 50; kvmevjl > 0; kvmevjl--) {
			continue;
		}
	}
	return 75444;
}

void jxfrnhk::dkiwpimchgcypc(double nuemeocswusb, double isvcdsbesdwkvdw, double zyckg, string phuvtghfkyx, string pmwzdjtyfjl, string yyruurttiej, bool mzhdaixtbu, double hboyghqm, double jqllk) {

}

double jxfrnhk::tvprvchjoej(int recdjmydethemv, double tjtgrpqnt, int qnhdickwk) {
	bool luhfxi = false;
	string wwxdnjppxzjhf = "vbfhxfmzkyb";
	int vfydgdwrt = 532;
	string wrpqkddqjjfd = "qzwgjondkllsqwtjgwzbsyktggicifvyinwvzkdigizkrkntoyucrumalxhnyzqdlisc";
	int aeyeudwagd = 744;
	bool bnwknqdwerwlhpu = true;
	double gkbuyxr = 12320;
	double pbopnlkq = 19302;
	bool tiyzstyycunn = false;
	if (string("vbfhxfmzkyb") == string("vbfhxfmzkyb")) {
		int ift;
		for (ift = 29; ift > 0; ift--) {
			continue;
		}
	}
	return 11281;
}

void jxfrnhk::oezkwpvrwgxnzfelddpet(string tpaezvlgyaezqi, string ybmqu, string amxyzibhn, int qasfzbs) {
	bool bqafpqrwkkkaz = false;
	double soarhjprbapgr = 15899;
	string uhzsyetoook = "husmiekuqplhxx";
	string yuhdl = "izrltdqhrcruktwypyydxgpkhpsmnlkbwmafkpllvvxvokshwlccsfcgkbhqrlsitcyucnjmpugnkpk";
	int owwyarsu = 3681;
	double mtpsxuskla = 22801;
	bool kwghkphub = true;
	if (string("husmiekuqplhxx") == string("husmiekuqplhxx")) {
		int cxfuoe;
		for (cxfuoe = 89; cxfuoe > 0; cxfuoe--) {
			continue;
		}
	}
	if (false == false) {
		int hyzabols;
		for (hyzabols = 78; hyzabols > 0; hyzabols--) {
			continue;
		}
	}

}

string jxfrnhk::lxqtzlvpquegj(string jepxe, string imgef, string kbdtjyamck) {
	double srwgp = 11304;
	bool jyhcgx = true;
	string wietwqtmsvrs = "jomsghawrbfaqjbcrydqnozermbtukgeyktakzeqrkebdyltvmdhmrbyltbdbzcjsyxtbimwwyddihipnbd";
	bool wmvfbggthgighr = false;
	string rzcoau = "cluwdomjysthqoqntjheqxuntvcagrtwo";
	double amwvsx = 45784;
	int ajzighncr = 3869;
	double rdhle = 12829;
	string ijazfx = "xfdzwhzuxxbguttevhfrhggktldnftomb";
	if (false != false) {
		int wphex;
		for (wphex = 67; wphex > 0; wphex--) {
			continue;
		}
	}
	return string("nwmfmtnwl");
}

double jxfrnhk::pmgzadrsbfasxuwxwkb(string kggdfgqxvt, int agryjwwffx, bool kzipcbgnr, bool ippfhuktizigwlz, int chholbswoh) {
	string sydhwab = "ygbrnpbugbdhtetichelydegcexdxyckxrrfwagptswg";
	int oyybo = 152;
	bool ioswuihptdrw = true;
	bool vwwxyrkfwdbzz = true;
	string mvmxjiycvxjeff = "lmyvctuvqdorcgplwfzvvzhkbh";
	int cdznwohkydp = 830;
	bool zgogxwlt = true;
	if (830 == 830) {
		int hswqi;
		for (hswqi = 51; hswqi > 0; hswqi--) {
			continue;
		}
	}
	if (string("lmyvctuvqdorcgplwfzvvzhkbh") == string("lmyvctuvqdorcgplwfzvvzhkbh")) {
		int iigbybrq;
		for (iigbybrq = 76; iigbybrq > 0; iigbybrq--) {
			continue;
		}
	}
	if (true != true) {
		int wgov;
		for (wgov = 14; wgov > 0; wgov--) {
			continue;
		}
	}
	return 76870;
}

double jxfrnhk::vmnwpzyeyfnjyvrnojwcyyhr(string otsdfkscco, bool bftahodzrptbr, double mwobmi, double ynzivgur, string qxjgbyfay, string dspitk, bool togyxbxnpxh, double aefwzudataiqoeg, string zntnsucjac) {
	double dgmbbhiqnovsyka = 4509;
	string biiuoqgnhb = "ougeiqkbgcziwbvtwfnrrboyrderdpsfabqeowr";
	if (string("ougeiqkbgcziwbvtwfnrrboyrderdpsfabqeowr") == string("ougeiqkbgcziwbvtwfnrrboyrderdpsfabqeowr")) {
		int qokywdypmj;
		for (qokywdypmj = 60; qokywdypmj > 0; qokywdypmj--) {
			continue;
		}
	}
	if (string("ougeiqkbgcziwbvtwfnrrboyrderdpsfabqeowr") != string("ougeiqkbgcziwbvtwfnrrboyrderdpsfabqeowr")) {
		int yye;
		for (yye = 35; yye > 0; yye--) {
			continue;
		}
	}
	return 9346;
}

void jxfrnhk::fxhexwiona(int gdwsbaodrzco, string wvmqcxtqcxxlyr, string pfghoi, string lpiygmoib) {
	bool qstwsghfm = true;
	double gxcjbi = 26431;

}

jxfrnhk::jxfrnhk() {
	this->dkiwpimchgcypc(3413, 16531, 21817, string("rhvukyk"), string("cawklzqbdapquovjwxzfnxbjsadikmrjkaklrcdyxvthewjxuiiyaackavcbfyoucvzzvfjpvnsioagtodgooafn"), string("qqlqcvvjcumztwpndhqjyvrmipsgndwkosocehcqyqsebivrdltheorivubgyvhwbyodasqxbvnhojeibfavkiqc"), true, 66984, 6656);
	this->tvprvchjoej(11, 15467, 7412);
	this->oezkwpvrwgxnzfelddpet(string("fnsqwrlxsvmzlwpeijufkkynyjyxgouaotuhlwhkvxvkamzffvekzrohrkvowoyydbebzcsnpbqoqgjwp"), string("ejyezbzjwwppifaxmlxphpzqrbaujwlzomzmfrnemmgfussaletgfvdvyzj"), string("yqctnzwqggihkoffstkzvaoziustfmxcwyglvcjqxqwytosgykjcfnqzgukrkutbuekb"), 4330);
	this->lxqtzlvpquegj(string("rpxztljrlyjxpglqqolkityljlvdbmocovjcvowlsqwzhszzegdjmrcvygqrrdrdvmytqbgjhmkmcecllkpy"), string("cwdwsjxxqdcsuxfcwncyzzofmxjzeagtshuzzlsfqeqepxumpohnaxgvpf"), string("vakuzllzjehawisyygzxvhyxowxlpt"));
	this->pmgzadrsbfasxuwxwkb(string("refxjfrtnmcgjucvyeavwkxlfnwjfnrgmxyczcbhjfuncdliwxbgfjlsrhqogpvlacb"), 6027, false, true, 347);
	this->vmnwpzyeyfnjyvrnojwcyyhr(string("soemcdhptzsrbnkrwihgrvrc"), false, 10380, 27007, string(""), string("ivuhifjopgdeyblgmwoihrhwqsuamywvoxvufvwzztnkgvfmkcgdcpkkriecobjrgdrwzvrhbnpvf"), false, 4311, string("hulnrimkxadgtuyfxbkuymkfatnabqhlwmgzlyhpdurrqcsndmqzwtn"));
	this->fxhexwiona(1214, string("ohzsggbihdulideshixdkxhgwtxuwunhwwwjkjgvdcgkvcxvfvilnbqtltaihtrklligldpkrktoewwfhftup"), string("mcfuowrrxgtaoarlpnbqqhrrojmasdzxtwjkihgbkrqfwihzm"), string("ubyqritzokdsomoobqrydkfpuenaeeon"));
	this->icljimuzbldyddolmaa(1667, 34361, 61068, false, true, 1479);
	this->cleafzdyrgenuqnp(string("mrmevmimwvexkbtferyoecuifqac"), 21860, string("gsxrmdtbyfftxmdsuttxpxvvvloawxmfclrrlpejxnzbdchefjamls"), string("ilxhblwrixgyaoumewdbadozhitqcokummhsyfpeuoougbaajzgvlijmyxtzzlpkboyowvtismxyeipewsw"), 904, true, 1163);
	this->trlxfgjvhfwh();
}
