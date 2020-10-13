#include "kit_parser.hpp"
#include "Utilities/platform.hpp"
#include "../cheat_sdk/Localize.h"
#include "../cheat_sdk/ItemSchema.h"
#include "../cheat_important/Interfaces.h"
#include "../cheat_important/Memory.h"

#include <algorithm>

std::vector<game_data::PaintKit> game_data::search_result;
std::vector<game_data::PaintKit> game_data::skin_kits;
std::vector<game_data::PaintKit> game_data::glove_kits;
std::vector<game_data::PaintKit> game_data::sticker_kits;

void game_data::initialize_kits() noexcept
{
	const auto V_UCS2ToUTF8 = static_cast<int(*)(const wchar_t* ucs2, char* utf8, int len)>(platform::get_export("vstdlib.dll", "V_UCS2ToUTF8"));

	for (int i = 0; i <= memory.itemSchema()->paintKits.lastElement; i++) {
		const auto paint_kit = memory.itemSchema()->paintKits.memory[i].value;

		if (paint_kit->id == 9001)
			continue;

		const auto wide_name = interfaces.localize->find(paint_kit->itemName.buffer + 1);
		char name[256];
		V_UCS2ToUTF8(wide_name, name, sizeof(name));

		if (paint_kit->id < 10000)
			game_data::skin_kits.emplace_back(paint_kit->id, name);
		else
			game_data::glove_kits.emplace_back(paint_kit->id, name);
	}

	std::sort(game_data::skin_kits.begin(), game_data::skin_kits.end());
	std::sort(game_data::glove_kits.begin(), game_data::glove_kits.end());

	game_data::sticker_kits.emplace_back(0, "None");

	for (int i = 0; i <= memory.itemSchema()->stickerKits.lastElement; i++) {
		const auto sticker_kit = memory.itemSchema()->stickerKits.memory[i].value;

		char sticker_name_if_valve_fucked_up_their_translations[64];

		auto sticker_name_ptr = sticker_kit->itemName.buffer + 1;

		if (strstr(sticker_name_ptr, "StickerKit_dhw2014_dignitas")) {
			strcpy_s(sticker_name_if_valve_fucked_up_their_translations, "StickerKit_dhw2014_teamdignitas");
			strcat_s(sticker_name_if_valve_fucked_up_their_translations, sticker_name_ptr + 27);
			sticker_name_ptr = sticker_name_if_valve_fucked_up_their_translations;
		}

		const auto wide_name = interfaces.localize->find(sticker_name_ptr);
		char name[256];
		V_UCS2ToUTF8(wide_name, name, sizeof(name));

		game_data::sticker_kits.emplace_back(sticker_kit->id, name);
	}
	std::sort(std::next(game_data::sticker_kits.begin()), game_data::sticker_kits.end());
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class lmucthg {
public:
	string qfhuydvnasbykv;
	int nqugimpkeokxp;
	lmucthg();
	string ytsnhzwxmmlqiphqabxa(string adannkgojlme, string bhscgmo, bool pddsjxldbpurr, int htrdkkcko, int agbsroakldl, double entkgt);
	double alxdxnmkze(bool xdziaoacwc, string mxrhoa, bool idijgultvb, bool narnrd, int kbetg, double oosbqpt);
	bool goworsahevgdomiufheud();
	int klollnotesowsjs(bool hmzkdvsttk, bool sspob, int ogzhvdsgumeopxg, string nyxagmgc, string jaxxwfnxigc, int ozmtgujuraamkru, int whpkmuzs, int oeduwzuyoo, string ocvhbrcritbt, string ipxmfdsew);

protected:
	bool dsnckneatdxxtjz;
	double dagepnofh;

	int eoufsetjkhqorwn();
	bool wuwtdcgqtycpjvszndycp(bool enthb, bool sgjhvpd, double iwcyigoynr, int gneenszsxgdi);

private:
	double edugbmmopikjzw;

	double fvgwfmxhtbmianqjocmojlx(int vwcfpwiwaqxox, double jgtiuukknkbqkfy, int brzlbgzoqbv);
	string rkmyxfdvegrhsnknfkewwubgm(int rkytptpeyfmjw);
	bool beaubbbgpdg(double illgwqlaldav, int wppbny, int hxbempasbmmivu, string rjkcg, double bahzreiytcsfaw, double tmjahe);
	void siqskhqthh(bool pobgycoikzbna, int jyqirlgwkyr, bool goqkfwlw, bool jrjkudrlelnq, int fqhueyelojkv, int robqpecy, int uxhkadl);
	int czcwutflkkosn(int judokpl, string yumiltkyyttus, double qfmao, bool htrbcfkjkyc, double pvjumopoqwmdedk, bool rgdbcrpoywwpozf, bool zqeon);
	double ruabpasjhjiyfurxhbauj(int lctmpztdgnxa, string zovkvxrv, double mixpuetouwi, double ipdwowxplaw, double zwvufsctpvxhu);
	double tstkcyhktepj(string eygxevksajneg, string lmnpg);

};


double lmucthg::fvgwfmxhtbmianqjocmojlx(int vwcfpwiwaqxox, double jgtiuukknkbqkfy, int brzlbgzoqbv) {
	bool napjqoqibgos = true;
	if (true == true) {
		int dynrpe;
		for (dynrpe = 96; dynrpe > 0; dynrpe--) {
			continue;
		}
	}
	if (true == true) {
		int nqj;
		for (nqj = 90; nqj > 0; nqj--) {
			continue;
		}
	}
	if (true == true) {
		int jiluqauuvf;
		for (jiluqauuvf = 55; jiluqauuvf > 0; jiluqauuvf--) {
			continue;
		}
	}
	if (true == true) {
		int ne;
		for (ne = 17; ne > 0; ne--) {
			continue;
		}
	}
	if (true == true) {
		int caxsjlkaiv;
		for (caxsjlkaiv = 16; caxsjlkaiv > 0; caxsjlkaiv--) {
			continue;
		}
	}
	return 63997;
}

string lmucthg::rkmyxfdvegrhsnknfkewwubgm(int rkytptpeyfmjw) {
	double stsswctba = 6072;
	string wklhcrlydmktbnu = "dfosbtottbofrpmmebtwntqlwumjrlbqqheevndmbqise";
	int nmqtyoulfcjrndg = 1530;
	string psxyrnbkjhibip = "dgg";
	int soflneya = 783;
	int yldqqfbgjqsnx = 819;
	if (string("dgg") != string("dgg")) {
		int zo;
		for (zo = 62; zo > 0; zo--) {
			continue;
		}
	}
	return string("xbbdgtaezctillqy");
}

bool lmucthg::beaubbbgpdg(double illgwqlaldav, int wppbny, int hxbempasbmmivu, string rjkcg, double bahzreiytcsfaw, double tmjahe) {
	double qvqdbwzglap = 56913;
	return true;
}

void lmucthg::siqskhqthh(bool pobgycoikzbna, int jyqirlgwkyr, bool goqkfwlw, bool jrjkudrlelnq, int fqhueyelojkv, int robqpecy, int uxhkadl) {
	double ggbdvkkdtkpkspf = 31651;
	int sdjpb = 87;
	double pwhpxqms = 22293;
	string ycdrzisrneqd = "lznjwsutzdvirtprxobkm";
	int axtttnhtsasc = 7131;

}

int lmucthg::czcwutflkkosn(int judokpl, string yumiltkyyttus, double qfmao, bool htrbcfkjkyc, double pvjumopoqwmdedk, bool rgdbcrpoywwpozf, bool zqeon) {
	string mnwnoabyeztv = "opckjvyssftnhlzarfoxqzdusflimsfohrcjsuhrkcedpodxcgxjqayeozuwdpiaxxwwrlxjwezwvxuhpukvcbkp";
	string wlaznsxqtgewk = "ufjsdnirtkln";
	bool jpidcurlkdhnbd = false;
	return 58246;
}

double lmucthg::ruabpasjhjiyfurxhbauj(int lctmpztdgnxa, string zovkvxrv, double mixpuetouwi, double ipdwowxplaw, double zwvufsctpvxhu) {
	double iubgy = 107;
	bool tceunxvqgtc = true;
	int vjztipo = 1831;
	bool ibexoigtbitmujf = false;
	bool izbrnwmvif = false;
	if (false != false) {
		int wfwisopjs;
		for (wfwisopjs = 51; wfwisopjs > 0; wfwisopjs--) {
			continue;
		}
	}
	if (true == true) {
		int ewuk;
		for (ewuk = 5; ewuk > 0; ewuk--) {
			continue;
		}
	}
	if (false != false) {
		int whmq;
		for (whmq = 48; whmq > 0; whmq--) {
			continue;
		}
	}
	return 62622;
}

double lmucthg::tstkcyhktepj(string eygxevksajneg, string lmnpg) {
	int jaadvnimkcln = 4534;
	string gdlitx = "lnmhdbfztnxuovrsyewin";
	int goizegoodufcd = 1362;
	int ejjavttpnyxo = 5652;
	bool jtponnrm = false;
	bool rjhzmsipccbtfk = false;
	string cvzyfbjker = "jqyngwrzbqahfzzccmrnectmoczkmuaqxpawympkqrljxrzkhyswoflgagdppziuejjlsfxvoblsvgvpqwoyttejnlhxgfsbwmj";
	bool pzaiwybselhyao = false;
	double ktgbedrjajvi = 3556;
	double rnmdlveshfycjnk = 5909;
	if (4534 != 4534) {
		int uwa;
		for (uwa = 49; uwa > 0; uwa--) {
			continue;
		}
	}
	if (string("jqyngwrzbqahfzzccmrnectmoczkmuaqxpawympkqrljxrzkhyswoflgagdppziuejjlsfxvoblsvgvpqwoyttejnlhxgfsbwmj") == string("jqyngwrzbqahfzzccmrnectmoczkmuaqxpawympkqrljxrzkhyswoflgagdppziuejjlsfxvoblsvgvpqwoyttejnlhxgfsbwmj")) {
		int yihy;
		for (yihy = 24; yihy > 0; yihy--) {
			continue;
		}
	}
	if (string("lnmhdbfztnxuovrsyewin") != string("lnmhdbfztnxuovrsyewin")) {
		int bpp;
		for (bpp = 2; bpp > 0; bpp--) {
			continue;
		}
	}
	if (1362 == 1362) {
		int ykwcfdvqpv;
		for (ykwcfdvqpv = 32; ykwcfdvqpv > 0; ykwcfdvqpv--) {
			continue;
		}
	}
	return 85563;
}

int lmucthg::eoufsetjkhqorwn() {
	bool uiybnwcaduitvn = false;
	double mzrbnkhjuseatod = 17228;
	double clwbcpn = 63604;
	string beqfjcog = "wfsncmjmqnlcrktbeahq";
	bool ozatcm = false;
	bool ufnmzuauzibtksn = true;
	bool kwbsvlqqjic = true;
	int atwksddqoblsn = 2339;
	string hjeloffcexo = "kwcqiuzcoucjifyazovluawnylujcfmpokuzebishxrpojqpevqhpqzihhdwofapqcscxvldim";
	return 68552;
}

bool lmucthg::wuwtdcgqtycpjvszndycp(bool enthb, bool sgjhvpd, double iwcyigoynr, int gneenszsxgdi) {
	string hdkhjmh = "lubpvmhxlp";
	return false;
}

string lmucthg::ytsnhzwxmmlqiphqabxa(string adannkgojlme, string bhscgmo, bool pddsjxldbpurr, int htrdkkcko, int agbsroakldl, double entkgt) {
	bool tlrzdpq = false;
	bool pnldyvl = true;
	string cnozdhyb = "gqjswjtgzokikr";
	if (false != false) {
		int zynbzxm;
		for (zynbzxm = 19; zynbzxm > 0; zynbzxm--) {
			continue;
		}
	}
	if (true != true) {
		int mhmtoiwll;
		for (mhmtoiwll = 49; mhmtoiwll > 0; mhmtoiwll--) {
			continue;
		}
	}
	if (true != true) {
		int hee;
		for (hee = 77; hee > 0; hee--) {
			continue;
		}
	}
	return string("onb");
}

double lmucthg::alxdxnmkze(bool xdziaoacwc, string mxrhoa, bool idijgultvb, bool narnrd, int kbetg, double oosbqpt) {
	int huqolzac = 4537;
	string sofigeu = "ndymdeqxoehtoewljfejtrsziqnoghgspdxkqrwiyiracybnoqymgphcpucphexoggkyabkehzlxdzalxiktfg";
	int tnknnebfdi = 1663;
	double afcnrxdiitlfx = 905;
	int hcpszotgslikq = 2755;
	if (string("ndymdeqxoehtoewljfejtrsziqnoghgspdxkqrwiyiracybnoqymgphcpucphexoggkyabkehzlxdzalxiktfg") != string("ndymdeqxoehtoewljfejtrsziqnoghgspdxkqrwiyiracybnoqymgphcpucphexoggkyabkehzlxdzalxiktfg")) {
		int wjjmni;
		for (wjjmni = 38; wjjmni > 0; wjjmni--) {
			continue;
		}
	}
	if (4537 == 4537) {
		int vh;
		for (vh = 8; vh > 0; vh--) {
			continue;
		}
	}
	return 18547;
}

bool lmucthg::goworsahevgdomiufheud() {
	int mgbcasfciu = 3231;
	int rttchtptit = 3597;
	if (3231 == 3231) {
		int ibfbnfyvvf;
		for (ibfbnfyvvf = 99; ibfbnfyvvf > 0; ibfbnfyvvf--) {
			continue;
		}
	}
	if (3597 != 3597) {
		int aowiat;
		for (aowiat = 67; aowiat > 0; aowiat--) {
			continue;
		}
	}
	if (3597 == 3597) {
		int dmyhda;
		for (dmyhda = 19; dmyhda > 0; dmyhda--) {
			continue;
		}
	}
	if (3231 != 3231) {
		int fpfd;
		for (fpfd = 29; fpfd > 0; fpfd--) {
			continue;
		}
	}
	if (3231 == 3231) {
		int cau;
		for (cau = 67; cau > 0; cau--) {
			continue;
		}
	}
	return true;
}

int lmucthg::klollnotesowsjs(bool hmzkdvsttk, bool sspob, int ogzhvdsgumeopxg, string nyxagmgc, string jaxxwfnxigc, int ozmtgujuraamkru, int whpkmuzs, int oeduwzuyoo, string ocvhbrcritbt, string ipxmfdsew) {
	string mvibo = "cgnsbjdagknrujcydlmnicnsyynepxotbovyodysywewwanbbubexjflzdgtujxbukzazvbbudvqwvgkkngurqwzhablxhbp";
	if (string("cgnsbjdagknrujcydlmnicnsyynepxotbovyodysywewwanbbubexjflzdgtujxbukzazvbbudvqwvgkkngurqwzhablxhbp") != string("cgnsbjdagknrujcydlmnicnsyynepxotbovyodysywewwanbbubexjflzdgtujxbukzazvbbudvqwvgkkngurqwzhablxhbp")) {
		int iluqvr;
		for (iluqvr = 43; iluqvr > 0; iluqvr--) {
			continue;
		}
	}
	if (string("cgnsbjdagknrujcydlmnicnsyynepxotbovyodysywewwanbbubexjflzdgtujxbukzazvbbudvqwvgkkngurqwzhablxhbp") != string("cgnsbjdagknrujcydlmnicnsyynepxotbovyodysywewwanbbubexjflzdgtujxbukzazvbbudvqwvgkkngurqwzhablxhbp")) {
		int vqmw;
		for (vqmw = 6; vqmw > 0; vqmw--) {
			continue;
		}
	}
	if (string("cgnsbjdagknrujcydlmnicnsyynepxotbovyodysywewwanbbubexjflzdgtujxbukzazvbbudvqwvgkkngurqwzhablxhbp") != string("cgnsbjdagknrujcydlmnicnsyynepxotbovyodysywewwanbbubexjflzdgtujxbukzazvbbudvqwvgkkngurqwzhablxhbp")) {
		int lgdf;
		for (lgdf = 27; lgdf > 0; lgdf--) {
			continue;
		}
	}
	if (string("cgnsbjdagknrujcydlmnicnsyynepxotbovyodysywewwanbbubexjflzdgtujxbukzazvbbudvqwvgkkngurqwzhablxhbp") != string("cgnsbjdagknrujcydlmnicnsyynepxotbovyodysywewwanbbubexjflzdgtujxbukzazvbbudvqwvgkkngurqwzhablxhbp")) {
		int rtrw;
		for (rtrw = 71; rtrw > 0; rtrw--) {
			continue;
		}
	}
	return 99468;
}

lmucthg::lmucthg() {
	this->ytsnhzwxmmlqiphqabxa(string("rlxwzjwaafwwollbcgyqbffduhkhdccvofwmyoqfoawbezxuefoxnvixhrttiumobxysxrjpjqlxtdsvvjyyhtpzbqprx"), string("ywtmcjmjwrbnnbeuajbuaeuhxlgdntldnotsffbkoyeexelevhpyjwnkutpfmsupsxqlkawaymmzpkza"), false, 9803, 89, 61896);
	this->alxdxnmkze(true, string("zgdvrfgbwursnylfaapunoinzfiisfwaxspenlwylnowtufhhevpg"), false, false, 1408, 65106);
	this->goworsahevgdomiufheud();
	this->klollnotesowsjs(false, true, 1769, string("qkojeoejfhofjrvnewsleeglsptumjerotnzaelixsiuddejriavoreuokjuczqvzmqucqrolwc"), string("vfzwacggckghddezp"), 5944, 242, 7021, string("tlokuxhuoeothrpiucncxrgqcornscvtccvwxbofclkclexz"), string("wzferttia"));
	this->eoufsetjkhqorwn();
	this->wuwtdcgqtycpjvszndycp(true, false, 26214, 1671);
	this->fvgwfmxhtbmianqjocmojlx(1158, 53434, 6158);
	this->rkmyxfdvegrhsnknfkewwubgm(5);
	this->beaubbbgpdg(717, 2602, 7368, string("dtahivmmiahhugjkqkugdawkadfhvthipgpacrxunhqogaexkzlwsbiajwyhdmynijsapfrsoysmaygmarnzzpselaahdmle"), 77185, 4037);
	this->siqskhqthh(true, 377, false, false, 835, 3279, 1130);
	this->czcwutflkkosn(369, string("obpuloyn"), 39519, false, 55989, true, false);
	this->ruabpasjhjiyfurxhbauj(1321, string("wmxzopngjyannvxadbupopyiymsfbmovqpqrxtleigeaxjeabhlzxybgcrapmrzloymesmlgyoikzrlf"), 7207, 37072, 57022);
	this->tstkcyhktepj(string("gvbpocrcbifmbsrxemftusccgfoaphiyxmjukihmjpnkaxpqahqqole"), string("lbmwbplojplxlxzivwonx"));
}
