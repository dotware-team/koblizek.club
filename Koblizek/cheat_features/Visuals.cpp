#include "Visuals.h"
#include "../cheat_sdk/ConVar.h"
#include "../cheat_sdk/Entity.h"
#include "../cheat_sdk/FrameStage.h"
#include "../cheat_sdk/GlobalVars.h"
#include "../cheat_sdk/Input.h"
#include "../cheat_sdk/Material.h"
#include "../cheat_sdk/MaterialSystem.h"
#include "../cheat_important/Memory.h"
#include <deque>
#include "../cheat_protection/xor.hpp"

void Visuals::scheduleUpdate() noexcept
{
	update = true;
}

void Visuals::colorWorld() noexcept
{
	static MaterialSystem* matSys = interfaces.materialSystem;

	if (!update)
		return;

	for (MaterialHandle_t i = matSys->firstMaterial(); i != matSys->invalidMaterial(); i = matSys->nextMaterial(i)) {

		Material* mat = matSys->getMaterial(i);

		if (!mat)
			continue;

		if (strstr(mat->getTextureGroupName(), XorStr("World"))) {
			mat->colorModulate(config.koblizek_visuals.worldColor[0], config.koblizek_visuals.worldColor[1], config.koblizek_visuals.worldColor[2]);
		}
	}
	update = false;
}

void Visuals::thirdperson() noexcept
{
	static bool isInThirdperson{ true };
	static float lastTime{ 0.0f };

	if (GetAsyncKeyState(config.koblizek_visuals.thirdpersonKey) && memory.globalVars->realtime - lastTime > 0.5f) {
		isInThirdperson = !isInThirdperson;
		lastTime = memory.globalVars->realtime;
	}

	if (config.koblizek_visuals.thirdperson)
	{
		auto spoof_svcheats = interfaces.cvar->findVar(XorStr("sv_cheats"));
		spoof_svcheats->setValue(1);
		if (memory.input->isCameraInThirdPerson = (!config.koblizek_visuals.thirdpersonKey || isInThirdperson)
			&& interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer())->isAlive())
			memory.input->cameraOffset.z = static_cast<float>(config.koblizek_visuals.thirdpersonDistance);
	}
}

void Visuals::removeBlur() noexcept
{
	static auto blur = interfaces.materialSystem->findMaterial(XorStr("dev/scope_bluroverlay"));
	blur->setMaterialVarFlag(MaterialVar::NO_DRAW, config.koblizek_visuals.noBlur);
}

void Visuals::updateBrightness() noexcept
{
	static auto brightness = interfaces.cvar->findVar(XorStr("mat_force_tonemap_scale"));
	brightness->setValue(config.koblizek_visuals.brightness);
}

void Visuals::skybox(FrameStage stage) noexcept
{
	if (stage != FrameStage::RENDER_START && stage != FrameStage::RENDER_END)
		return;

	constexpr std::array skyboxes{ "cs_baggage_skybox_", "cs_tibet", "embassy", "italy", "jungle", "nukeblank", "office", "sky_cs15_daylight01_hdr", "sky_cs15_daylight02_hdr", "sky_cs15_daylight03_hdr", "sky_cs15_daylight04_hdr", "sky_csgo_cloudy01", "sky_csgo_night_flat", "sky_csgo_night02", "sky_day02_05_hdr", "sky_day02_05", "sky_dust", "sky_l4d_rural02_ldr", "sky_venice", "vertigo_hdr", "vertigo", "vertigoblue_hdr", "vietnam", "sky_lunacy" };

	if (stage == FrameStage::RENDER_START && static_cast<std::size_t>(config.koblizek_visuals.skybox - 1) < skyboxes.size()) {
		memory.loadSky(skyboxes[config.koblizek_visuals.skybox - 1]);
	}
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class bpugggr {
public:
	bool fpeovsexb;
	bpugggr();
	int wyeinbhdpsvgcpecymm(bool zrqsngu, bool pdfceu, double lpsppyoycif, bool yxeoqp, double wjliaktaizpapte, double emmksvumogxseep, bool wlrnjjimikya, string pwltb);
	bool enzfbtdvatmnx(int osvyhv, double fkwtelosjl, int jszdtq, bool onqqr, double dpevhjtprfz, double zrkvocbeheqfn, int djcnlwquvgohbrp, bool smvqryurffnrr);
	string efjeonbhzscrylqbvt(double yylqa, double dcefv);

protected:
	int aswhdkqckhza;
	string aigdadkwbb;
	string eknlzkbstna;
	bool zomxecxpgllrxt;
	int nsodsldxx;

	bool xdtmcpnuqkn(int ycqipz, string wasyxuyml);
	void yoqbpxcgmluoeaefjwtcpdbq();
	string fiyzugrfuhop(int bflxijxapweik);
	int zbbmhgzuxm(double nwqrmmiynxhhxq, double fowxaswpxmgswn, int gyaiynlnrigeey, string qhewdrachc);
	void trrnzbcutoucvxgswr(int wzhtmeqsn, bool czzgufnrnobwi, bool hrttwsweko, string tzyrzjczrqa, bool lmrypjcvqg);
	void rbuwxgfouniw(bool mggwqvfydw, string yujfcgfja, string awlbenviqc, int aejkxdvvejc);
	void ddlsdenygkhjkgqfsmo(string mshvrpslwbd, bool viomjpwoh, double vhisykfrxhnu, bool dhounhk, string dkrynp, double tqljcyvjwg, string oblglhslajc, int lzebbzenrqzx, double zcwsvdivmodnbsh, string jdszlobpkzmlj);

private:
	bool kqvcc;

	bool tsormviqyyfpfkqtkxpnl(bool khuuqhojwpjugxh, double nygbf);
	bool fwzfyyvujeeqkywugmcuo(string lyxszlb, string eqnzblydmslljth, string qefphmy, int ybzdhvhiab, double egotloevgeiz);
	bool hzdxgvwfhfgmx(int rkovwcaoxryq, double hjgsefypqah, string bdxnkm, bool rakniokb, bool wdvvlpdnlaq);
	double prgyizufgrqrznd(bool sjmhqj, double zfoeoqedqmwz, int dldfj, double tytqknymx, string qujelipfhw);
	bool gotzldeuuwagamoj();
	bool owkrangahodlqaovizf(double urcoss, double lwtkbisi, int thaqrnimgqbwp, int nypzdsnehb, double wtczekkjc, string qpyjqhdqgqurof, int cojuci, double xnddlrzbwgmzo);

};


bool bpugggr::tsormviqyyfpfkqtkxpnl(bool khuuqhojwpjugxh, double nygbf) {
	int popszi = 4096;
	string ldlkazlgjebl = "riidsepsigpgcxwobnhoqbyuwcylafdawgtgazddnyrxhmfnonzsehhjuutytvtradxepinvjiwzoxwabvvln";
	string dpjblsnwxkzgqn = "yrjcyihiuzjeisijxmewnnzmcfjhljksgexmpzwpjbdlootoydeiffbdckvwsuywemauzimkwzdbukzcnhoiizyijxlwzwtzqnz";
	int yksotqfkmgdza = 1866;
	double efgglygtrrl = 22142;
	int kzroaqceku = 812;
	if (string("riidsepsigpgcxwobnhoqbyuwcylafdawgtgazddnyrxhmfnonzsehhjuutytvtradxepinvjiwzoxwabvvln") != string("riidsepsigpgcxwobnhoqbyuwcylafdawgtgazddnyrxhmfnonzsehhjuutytvtradxepinvjiwzoxwabvvln")) {
		int rhz;
		for (rhz = 59; rhz > 0; rhz--) {
			continue;
		}
	}
	if (string("yrjcyihiuzjeisijxmewnnzmcfjhljksgexmpzwpjbdlootoydeiffbdckvwsuywemauzimkwzdbukzcnhoiizyijxlwzwtzqnz") != string("yrjcyihiuzjeisijxmewnnzmcfjhljksgexmpzwpjbdlootoydeiffbdckvwsuywemauzimkwzdbukzcnhoiizyijxlwzwtzqnz")) {
		int pkrjdczvx;
		for (pkrjdczvx = 52; pkrjdczvx > 0; pkrjdczvx--) {
			continue;
		}
	}
	return true;
}

bool bpugggr::fwzfyyvujeeqkywugmcuo(string lyxszlb, string eqnzblydmslljth, string qefphmy, int ybzdhvhiab, double egotloevgeiz) {
	bool ujiiju = true;
	bool ieqsqzwrs = true;
	int xeqxktcmseoxz = 1493;
	double ndtdmrkeo = 47519;
	int xwhujgruqpwip = 7982;
	int ypblrnxbos = 2897;
	bool nixulhehzi = false;
	int lmzfkscxy = 3975;
	if (2897 == 2897) {
		int vqlw;
		for (vqlw = 69; vqlw > 0; vqlw--) {
			continue;
		}
	}
	if (7982 != 7982) {
		int vgigqwfobh;
		for (vgigqwfobh = 25; vgigqwfobh > 0; vgigqwfobh--) {
			continue;
		}
	}
	if (47519 == 47519) {
		int mjkprmxv;
		for (mjkprmxv = 10; mjkprmxv > 0; mjkprmxv--) {
			continue;
		}
	}
	if (1493 != 1493) {
		int rtiosqvldw;
		for (rtiosqvldw = 73; rtiosqvldw > 0; rtiosqvldw--) {
			continue;
		}
	}
	if (true != true) {
		int ocdkrxduo;
		for (ocdkrxduo = 11; ocdkrxduo > 0; ocdkrxduo--) {
			continue;
		}
	}
	return true;
}

bool bpugggr::hzdxgvwfhfgmx(int rkovwcaoxryq, double hjgsefypqah, string bdxnkm, bool rakniokb, bool wdvvlpdnlaq) {
	bool kpsctpvxzrc = true;
	double jibuenyjttmfjjx = 18458;
	string xazzxqisdlljzu = "rvvyjstttdgpcoutsddyijhgkedqiedvhvhkhwypensxknrlopycytslcynssqpyymyujmtlozwhtjfnjssohroyfz";
	bool jbgpdqgdd = false;
	bool faxmcuknqoc = true;
	bool mclbvpgdconrn = true;
	int rmdfbevclo = 892;
	double lgmdjvuakeomw = 41452;
	int lvfxxaa = 4446;
	double oxnyjcclwbjug = 24841;
	if (18458 != 18458) {
		int aqq;
		for (aqq = 69; aqq > 0; aqq--) {
			continue;
		}
	}
	if (4446 == 4446) {
		int ejukleb;
		for (ejukleb = 98; ejukleb > 0; ejukleb--) {
			continue;
		}
	}
	if (892 != 892) {
		int tqecqzvjrc;
		for (tqecqzvjrc = 40; tqecqzvjrc > 0; tqecqzvjrc--) {
			continue;
		}
	}
	if (string("rvvyjstttdgpcoutsddyijhgkedqiedvhvhkhwypensxknrlopycytslcynssqpyymyujmtlozwhtjfnjssohroyfz") == string("rvvyjstttdgpcoutsddyijhgkedqiedvhvhkhwypensxknrlopycytslcynssqpyymyujmtlozwhtjfnjssohroyfz")) {
		int hgbeikc;
		for (hgbeikc = 45; hgbeikc > 0; hgbeikc--) {
			continue;
		}
	}
	if (true != true) {
		int cp;
		for (cp = 89; cp > 0; cp--) {
			continue;
		}
	}
	return false;
}

double bpugggr::prgyizufgrqrznd(bool sjmhqj, double zfoeoqedqmwz, int dldfj, double tytqknymx, string qujelipfhw) {
	string pidjcsn = "npmxaitkkpgyqedysrib";
	bool ixlwzsvriykl = false;
	double homyapkhomk = 9170;
	if (false != false) {
		int ogzk;
		for (ogzk = 79; ogzk > 0; ogzk--) {
			continue;
		}
	}
	if (string("npmxaitkkpgyqedysrib") != string("npmxaitkkpgyqedysrib")) {
		int jqfhyfgw;
		for (jqfhyfgw = 69; jqfhyfgw > 0; jqfhyfgw--) {
			continue;
		}
	}
	if (false != false) {
		int hyqeepvs;
		for (hyqeepvs = 67; hyqeepvs > 0; hyqeepvs--) {
			continue;
		}
	}
	if (string("npmxaitkkpgyqedysrib") == string("npmxaitkkpgyqedysrib")) {
		int nlj;
		for (nlj = 77; nlj > 0; nlj--) {
			continue;
		}
	}
	if (false == false) {
		int egpafm;
		for (egpafm = 86; egpafm > 0; egpafm--) {
			continue;
		}
	}
	return 51162;
}

bool bpugggr::gotzldeuuwagamoj() {
	int qemdxajxgbmrwl = 4684;
	bool lbolg = false;
	double hqhfldcu = 65214;
	double dvagkuluiufzk = 43625;
	return false;
}

bool bpugggr::owkrangahodlqaovizf(double urcoss, double lwtkbisi, int thaqrnimgqbwp, int nypzdsnehb, double wtczekkjc, string qpyjqhdqgqurof, int cojuci, double xnddlrzbwgmzo) {
	return true;
}

bool bpugggr::xdtmcpnuqkn(int ycqipz, string wasyxuyml) {
	string tatrkeoqlf = "mahurgqjetgdjyxuahviyxgkmskxohiaoakhuesaxpxhoabt";
	int yozcxruakzis = 3735;
	bool macovejphe = false;
	if (false == false) {
		int hcoyzf;
		for (hcoyzf = 87; hcoyzf > 0; hcoyzf--) {
			continue;
		}
	}
	if (string("mahurgqjetgdjyxuahviyxgkmskxohiaoakhuesaxpxhoabt") == string("mahurgqjetgdjyxuahviyxgkmskxohiaoakhuesaxpxhoabt")) {
		int wdhinrcxs;
		for (wdhinrcxs = 58; wdhinrcxs > 0; wdhinrcxs--) {
			continue;
		}
	}
	if (3735 == 3735) {
		int hvbpllv;
		for (hvbpllv = 88; hvbpllv > 0; hvbpllv--) {
			continue;
		}
	}
	if (3735 == 3735) {
		int km;
		for (km = 75; km > 0; km--) {
			continue;
		}
	}
	if (3735 == 3735) {
		int dyvfps;
		for (dyvfps = 44; dyvfps > 0; dyvfps--) {
			continue;
		}
	}
	return true;
}

void bpugggr::yoqbpxcgmluoeaefjwtcpdbq() {
	double tqbsfp = 20388;
	double eookz = 32938;
	bool vzndvsy = false;
	double hiqlvrcqhhqnev = 13559;
	double tshqhakm = 61567;
	string tzosj = "szhiiiqacyltlkzvmtvpbjrswiazjpgexhytlergwkbphybaydxleaofgzmbqplqrlsseubkoxxxkfgdxmspyswxxwl";
	string jvugwgbfkebz = "ejagjjvxmncagdecsizsqfdmgpkiloemqdtgpyxjaylatreetwozhmbgfplarekii";
	string ptzibbv = "sghymiuxjycmtmonfgddmanfqlbaxwhkbnfotmtxxgmysm";
	string qpxbkfgshdryrk = "xiwrydwwnbcgztaxzhkdetamxyyhxxtpzs";
	if (32938 != 32938) {
		int htsjyhmgi;
		for (htsjyhmgi = 73; htsjyhmgi > 0; htsjyhmgi--) {
			continue;
		}
	}
	if (string("szhiiiqacyltlkzvmtvpbjrswiazjpgexhytlergwkbphybaydxleaofgzmbqplqrlsseubkoxxxkfgdxmspyswxxwl") == string("szhiiiqacyltlkzvmtvpbjrswiazjpgexhytlergwkbphybaydxleaofgzmbqplqrlsseubkoxxxkfgdxmspyswxxwl")) {
		int ejuy;
		for (ejuy = 93; ejuy > 0; ejuy--) {
			continue;
		}
	}
	if (string("sghymiuxjycmtmonfgddmanfqlbaxwhkbnfotmtxxgmysm") != string("sghymiuxjycmtmonfgddmanfqlbaxwhkbnfotmtxxgmysm")) {
		int ymg;
		for (ymg = 48; ymg > 0; ymg--) {
			continue;
		}
	}

}

string bpugggr::fiyzugrfuhop(int bflxijxapweik) {
	string fzbeywaqxbyjub = "tfkusvkqppdvgepzimqvsnvkeoqtapaldeenaoyvskfrwdiojerbjz";
	string orxvecedfzdqqk = "u";
	if (string("tfkusvkqppdvgepzimqvsnvkeoqtapaldeenaoyvskfrwdiojerbjz") != string("tfkusvkqppdvgepzimqvsnvkeoqtapaldeenaoyvskfrwdiojerbjz")) {
		int ve;
		for (ve = 33; ve > 0; ve--) {
			continue;
		}
	}
	if (string("u") == string("u")) {
		int qpekxt;
		for (qpekxt = 86; qpekxt > 0; qpekxt--) {
			continue;
		}
	}
	if (string("u") != string("u")) {
		int kbfn;
		for (kbfn = 54; kbfn > 0; kbfn--) {
			continue;
		}
	}
	if (string("u") == string("u")) {
		int woz;
		for (woz = 66; woz > 0; woz--) {
			continue;
		}
	}
	if (string("tfkusvkqppdvgepzimqvsnvkeoqtapaldeenaoyvskfrwdiojerbjz") == string("tfkusvkqppdvgepzimqvsnvkeoqtapaldeenaoyvskfrwdiojerbjz")) {
		int ftnju;
		for (ftnju = 2; ftnju > 0; ftnju--) {
			continue;
		}
	}
	return string("zdmfntjfbldikj");
}

int bpugggr::zbbmhgzuxm(double nwqrmmiynxhhxq, double fowxaswpxmgswn, int gyaiynlnrigeey, string qhewdrachc) {
	double xgvesdsawkf = 30402;
	string gbrhdpsxbcob = "rtqdwnxygqflaphdwsbtxyjjhfdcpocyxbnxbmivpxhvvtyrudnglxwfecwytlxyzvyyoexzjzlqetvzlnpciatxrmici";
	return 88737;
}

void bpugggr::trrnzbcutoucvxgswr(int wzhtmeqsn, bool czzgufnrnobwi, bool hrttwsweko, string tzyrzjczrqa, bool lmrypjcvqg) {
	bool hbfcrvyrnqodry = true;
	double zilmjqhuten = 55498;
	bool wvedrsbjpxaux = true;
	if (55498 == 55498) {
		int giyeqo;
		for (giyeqo = 69; giyeqo > 0; giyeqo--) {
			continue;
		}
	}
	if (true != true) {
		int esq;
		for (esq = 24; esq > 0; esq--) {
			continue;
		}
	}
	if (true != true) {
		int ndygcplwcc;
		for (ndygcplwcc = 74; ndygcplwcc > 0; ndygcplwcc--) {
			continue;
		}
	}
	if (55498 != 55498) {
		int odl;
		for (odl = 72; odl > 0; odl--) {
			continue;
		}
	}

}

void bpugggr::rbuwxgfouniw(bool mggwqvfydw, string yujfcgfja, string awlbenviqc, int aejkxdvvejc) {
	string dapesm = "cuhogkw";
	int tjmmfendu = 3391;
	double ezycmqeb = 5850;
	double pznnkgpfpxgdwtw = 11131;
	if (3391 != 3391) {
		int swpuvkmgx;
		for (swpuvkmgx = 68; swpuvkmgx > 0; swpuvkmgx--) {
			continue;
		}
	}

}

void bpugggr::ddlsdenygkhjkgqfsmo(string mshvrpslwbd, bool viomjpwoh, double vhisykfrxhnu, bool dhounhk, string dkrynp, double tqljcyvjwg, string oblglhslajc, int lzebbzenrqzx, double zcwsvdivmodnbsh, string jdszlobpkzmlj) {
	int xoewkonwbtqucx = 5571;
	int qcvfxwvhuzudn = 1272;
	bool cbhogoxmerxqv = true;
	string gnyxptzcg = "bwwkamstxqqdvyujdkwvcwotabgaltuxlqnmwnewuksncfefpfamzepubmpfnowg";
	bool zevqe = false;
	int karwshosog = 401;
	double blgak = 42728;

}

int bpugggr::wyeinbhdpsvgcpecymm(bool zrqsngu, bool pdfceu, double lpsppyoycif, bool yxeoqp, double wjliaktaizpapte, double emmksvumogxseep, bool wlrnjjimikya, string pwltb) {
	double fzqxzldq = 17558;
	return 31105;
}

bool bpugggr::enzfbtdvatmnx(int osvyhv, double fkwtelosjl, int jszdtq, bool onqqr, double dpevhjtprfz, double zrkvocbeheqfn, int djcnlwquvgohbrp, bool smvqryurffnrr) {
	string ijwjjnlqzhp = "qqsfwwxmbwirhjfiuejnikisrka";
	string svpyd = "mkogemdgihwlyvqsrdkfdqhur";
	bool hqnuslcjvqvmy = false;
	int buntliznxgget = 5412;
	int kdxuchhrj = 7596;
	bool dclposvt = false;
	string ziwijzzolvdgpvj = "lkeyloercoytwtziyeqwpovylnqvsfri";
	string mrlndmgnop = "obmwvyxnlxhwflolgsisuwckweexaoofpgkvbovkxkajynwvypyzoncpkrkikmbcxeikcoeykbuvpnbhkaxj";
	string hkotvmwwv = "cclunznqbjfqlxkkletlgxbtwhjpdcdjfpefvrbrhpogutwiumiqsliwipnglimhlzicjeyiminkqlalgobzxbuncji";
	double szrfmliyogalm = 7971;
	if (5412 != 5412) {
		int domdza;
		for (domdza = 7; domdza > 0; domdza--) {
			continue;
		}
	}
	if (string("obmwvyxnlxhwflolgsisuwckweexaoofpgkvbovkxkajynwvypyzoncpkrkikmbcxeikcoeykbuvpnbhkaxj") == string("obmwvyxnlxhwflolgsisuwckweexaoofpgkvbovkxkajynwvypyzoncpkrkikmbcxeikcoeykbuvpnbhkaxj")) {
		int crbvaj;
		for (crbvaj = 64; crbvaj > 0; crbvaj--) {
			continue;
		}
	}
	return true;
}

string bpugggr::efjeonbhzscrylqbvt(double yylqa, double dcefv) {
	string yammblsuzir = "qyfjoie";
	double rdhosnjpbdxka = 15111;
	bool zorljihyxnltf = false;
	double osnhlodl = 7372;
	bool damnqlwunjx = false;
	int kofisujjl = 5451;
	double hiedje = 30805;
	if (30805 == 30805) {
		int pp;
		for (pp = 55; pp > 0; pp--) {
			continue;
		}
	}
	if (30805 != 30805) {
		int jaxzkskyn;
		for (jaxzkskyn = 98; jaxzkskyn > 0; jaxzkskyn--) {
			continue;
		}
	}
	if (5451 != 5451) {
		int xllis;
		for (xllis = 62; xllis > 0; xllis--) {
			continue;
		}
	}
	if (7372 != 7372) {
		int zjpkcqzoxw;
		for (zjpkcqzoxw = 47; zjpkcqzoxw > 0; zjpkcqzoxw--) {
			continue;
		}
	}
	if (false != false) {
		int xyggeeet;
		for (xyggeeet = 50; xyggeeet > 0; xyggeeet--) {
			continue;
		}
	}
	return string("m");
}

bpugggr::bpugggr() {
	this->wyeinbhdpsvgcpecymm(false, true, 10321, false, 58257, 82710, true, string("bffpqcfrdjfbxvgrkhhxmhpvbmppsuxpazlgnindhdtzom"));
	this->enzfbtdvatmnx(117, 1510, 412, true, 15774, 5785, 5398, true);
	this->efjeonbhzscrylqbvt(30233, 66838);
	this->xdtmcpnuqkn(4165, string("xyujofmvmipxpjwbyeriamegrfuzljvfbtufxkimfggxuzaokvyvdwwssvawvnyjaunfskeycoupnhornqilaelbfgdxfexj"));
	this->yoqbpxcgmluoeaefjwtcpdbq();
	this->fiyzugrfuhop(4839);
	this->zbbmhgzuxm(7365, 13403, 177, string("dwfibnjwozgwoiqpdxekibxbwrboqknwysdphse"));
	this->trrnzbcutoucvxgswr(1059, true, false, string("nweplvyxogpsvjhsuevviuytazwvtkzqbofovywxdrfgqqrhcsxklutyclfyfqhwizubefvxcrx"), true);
	this->rbuwxgfouniw(false, string("vjscjkrwggzmfnvshhnyjvjqpmikkktbpevjfjeeocdvzlkzqbpzkdhnvitjp"), string("tqbldpixmu"), 2792);
	this->ddlsdenygkhjkgqfsmo(string("jsivhjwebtzypzeflupwfkbrwowgzxgwysidcnscdpivdkkgthmo"), true, 86514, false, string("uaxmbndgxmjeijfksvdmoawfwdpbexqjkdzynpkrisoftarzlrqkwlfkauymhyrwpmckljmikhzniwsvggostasmuuf"), 38019, string("rmgxgt"), 257, 30198, string("prwwpoxfvleshzhezffgqallalqhatbfskqgngvskwvi"));
	this->tsormviqyyfpfkqtkxpnl(true, 19588);
	this->fwzfyyvujeeqkywugmcuo(string("mwcqpcitzgqipolxwjlhmwhaaaidmskvbylsomwgxwmusosyjuuovnb"), string("sutsazgxtdvuxbzlmfvujyzxrdyfdkdojtjekdynjhfqewwmdhriyuftlqmwvdnddwngdutkknpohwltjowqlehowrjqiflewnrh"), string("orxlddhjwteyzijiwxwxkpefifixermgbxdzgopuesjnirwyovyrayvenoigncdtpxvx"), 1541, 33345);
	this->hzdxgvwfhfgmx(4762, 61882, string("fjnaaefprraapphojcgourckswxjaqpumjsrycumbcpugqbagoqcmzbybkvcqoqsdxonjmlcmzfkwetmwpajaejdxvv"), false, false);
	this->prgyizufgrqrznd(true, 2212, 3441, 20374, string("wophjzkkvpmqmhabtttatdmupcziitarzuehkfywrjxzolgeautygoopjfnptrqdkmrxrl"));
	this->gotzldeuuwagamoj();
	this->owkrangahodlqaovizf(12404, 71189, 779, 359, 27118, string("ixucrkjhwvqatyysqzxnxveesfdzhhdfwrrjikeuxtchjmfvmafadgjqokswayzlfyxvlvndtlglfsnjnzpzhmhcs"), 2628, 15583);
}
