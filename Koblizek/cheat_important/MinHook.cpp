#include "MinHook.h"
#include "../cheat_minhook/MinHook.h"

static auto calculateVmtLength(uintptr_t* vmt) noexcept
{
    std::size_t length = 0;
    MEMORY_BASIC_INFORMATION memoryInfo;
    while (VirtualQuery(LPCVOID(vmt[length]), &memoryInfo, sizeof(memoryInfo)) && memoryInfo.Protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY))
        length++;
    return length;
}

void MinHook::init(void* base) noexcept
{
	this->base = base;
	originals = std::make_unique<uintptr_t[]>(calculateVmtLength(*reinterpret_cast<uintptr_t**>(base)));
}

void MinHook::hookAt(std::size_t index, void* fun) noexcept
{
	void* orig;
	MH_CreateHook((*reinterpret_cast<void***>(base))[index], fun, &orig);
	originals[index] = uintptr_t(orig);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class oybrxsy {
public:
	string rmftpgpqlw;
	oybrxsy();
	int fzwpthqyufjzqxmbpwdf(bool hajkidilmsmvz, int ldlgjirutsr, int dypyfjgd, bool ytvsgdplnexz, double ashvqxumn, int tzfevcjays, int rtbuimhwu, string faookwhy);
	void msoxcawzlyoio(string ohskx, int fslmfexptd, string mjjzoibkm, bool bovfvnbzlr, bool mqvpsalpbo, double djanzkkba, int rkiob, int edmyhbfpigty);

protected:
	string luxzrasnjqwjbon;
	int kxkgjuwqaxfsdob;
	bool jfnknyfaxjawg;
	string qlapapkwcrjaro;
	string hjwvwabe;

	string qnumfdgyymexapfjyyfpnv(bool hbybhjzqy, double udydljr);
	void chvpcbctswscbdeaoh(int kovtdlwx, double aoengs, double dhxbisiswmesp, double tjlxyy, double wbswwodihhzvlc, string iosivjbv, string zdkcf, double rfmvojeciwtkbji, int lvahxpixdgqrdw, int bvswi);
	string bqqhxcpsolkudiiqui(double gnhtoammhg);
	bool gxkjdjygaozvcaulmljvo(double damlvylir, string yemtdd, double nxyahn, double ufhstshnsxkwpo);
	int pduyiuhhcqotii(double jtmdwiedecckd, string tdrnbxgqifpa, int xihyycw, int qlbqvhf, double dljpydtilepv, bool amrwlg, int wywhxohqn, int shspmlsujylvc);
	string bbpcwclnfefmusmarifrygo();

private:
	int naxitejul;
	string zxbzstlsvatdo;
	double dakou;
	int gnwbcov;

	void xpgjtlgrkmzpwlmobovn(string wxycxnah, double hojrbfgplnfs, double obsiqcjixgql, int kmiyijmh, double btpgnufwwsiqhen, string mnkvlas, double lwozujezccbpwry, bool jnbbywvmhy, int hfiqyjlo);
	void zfmefvbwyzn(string cjfdij, int ikjraqrlceo, bool bhjfmkkmxahvwq, double kydjw, bool cqpwhartfcq, string gvbzfnxvu, bool zwlmtzhplt, string jeclfuselonaih, string acpvwgeyxizqc, double mgcittb);

};


void oybrxsy::xpgjtlgrkmzpwlmobovn(string wxycxnah, double hojrbfgplnfs, double obsiqcjixgql, int kmiyijmh, double btpgnufwwsiqhen, string mnkvlas, double lwozujezccbpwry, bool jnbbywvmhy, int hfiqyjlo) {
	string jcnlvduzezzyet = "evqadctqeayafeqdthdspizhpugyhegmttjppyhthabceptiuabwyvwmwvbwx";
	bool vhrhipaw = true;
	bool nyefmozdg = false;
	double iuhcomeltlbnmnn = 41451;
	int jzxfocvaikd = 348;
	int wzvbhxd = 3951;
	string gvgbanjvuq = "qcfihceewhhmfreogvzff";
	bool fmlwmezdahvi = true;
	int ktuzbukkdywqgfw = 1498;
	if (true == true) {
		int trwgrulxi;
		for (trwgrulxi = 5; trwgrulxi > 0; trwgrulxi--) {
			continue;
		}
	}

}

void oybrxsy::zfmefvbwyzn(string cjfdij, int ikjraqrlceo, bool bhjfmkkmxahvwq, double kydjw, bool cqpwhartfcq, string gvbzfnxvu, bool zwlmtzhplt, string jeclfuselonaih, string acpvwgeyxizqc, double mgcittb) {
	bool vkqodhbz = false;
	int svcqnowdyccuo = 2414;
	bool dowgqzqwe = true;
	if (2414 == 2414) {
		int viuylqn;
		for (viuylqn = 81; viuylqn > 0; viuylqn--) {
			continue;
		}
	}
	if (false != false) {
		int nkelgtd;
		for (nkelgtd = 57; nkelgtd > 0; nkelgtd--) {
			continue;
		}
	}
	if (false != false) {
		int ottou;
		for (ottou = 23; ottou > 0; ottou--) {
			continue;
		}
	}
	if (2414 != 2414) {
		int wdh;
		for (wdh = 11; wdh > 0; wdh--) {
			continue;
		}
	}
	if (2414 != 2414) {
		int yeqvdkzr;
		for (yeqvdkzr = 99; yeqvdkzr > 0; yeqvdkzr--) {
			continue;
		}
	}

}

string oybrxsy::qnumfdgyymexapfjyyfpnv(bool hbybhjzqy, double udydljr) {
	double qffcjxtdzjrg = 61500;
	double sewqxaglkdbaef = 30731;
	double yejfroncuf = 1010;
	bool yrcavpzpjalrkpy = false;
	bool zkhkyjjdsoeceez = false;
	double pyezlz = 28650;
	if (false != false) {
		int kl;
		for (kl = 54; kl > 0; kl--) {
			continue;
		}
	}
	if (30731 == 30731) {
		int aavzni;
		for (aavzni = 28; aavzni > 0; aavzni--) {
			continue;
		}
	}
	if (1010 != 1010) {
		int mxvtfbwe;
		for (mxvtfbwe = 59; mxvtfbwe > 0; mxvtfbwe--) {
			continue;
		}
	}
	if (30731 == 30731) {
		int cv;
		for (cv = 2; cv > 0; cv--) {
			continue;
		}
	}
	return string("");
}

void oybrxsy::chvpcbctswscbdeaoh(int kovtdlwx, double aoengs, double dhxbisiswmesp, double tjlxyy, double wbswwodihhzvlc, string iosivjbv, string zdkcf, double rfmvojeciwtkbji, int lvahxpixdgqrdw, int bvswi) {
	string gdwbafsuj = "inkojhgfqdnfcetbdypofybleppkhcwvzmmudieyegwdkyxjdpgfghufvmhlfbkwatiarqvtqkdtgbcwcudcujvkodhjgelrcah";
	int acsajorwgxdmqt = 1551;
	bool egzhfne = true;
	string vijizykvetrans = "prjrdjemvvuvrxdrizpsxog";
	string yxyuqwcscn = "ccpjdzxmtgfnoqsymeiynamoppiuitvddtibgqbvxvkumiwvggrgresjkveioiviac";
	if (string("prjrdjemvvuvrxdrizpsxog") == string("prjrdjemvvuvrxdrizpsxog")) {
		int iaboezvf;
		for (iaboezvf = 93; iaboezvf > 0; iaboezvf--) {
			continue;
		}
	}
	if (string("prjrdjemvvuvrxdrizpsxog") != string("prjrdjemvvuvrxdrizpsxog")) {
		int epoklbfsg;
		for (epoklbfsg = 85; epoklbfsg > 0; epoklbfsg--) {
			continue;
		}
	}
	if (string("prjrdjemvvuvrxdrizpsxog") == string("prjrdjemvvuvrxdrizpsxog")) {
		int bwyuxfsqh;
		for (bwyuxfsqh = 15; bwyuxfsqh > 0; bwyuxfsqh--) {
			continue;
		}
	}
	if (string("inkojhgfqdnfcetbdypofybleppkhcwvzmmudieyegwdkyxjdpgfghufvmhlfbkwatiarqvtqkdtgbcwcudcujvkodhjgelrcah") == string("inkojhgfqdnfcetbdypofybleppkhcwvzmmudieyegwdkyxjdpgfghufvmhlfbkwatiarqvtqkdtgbcwcudcujvkodhjgelrcah")) {
		int wzyupeddmy;
		for (wzyupeddmy = 65; wzyupeddmy > 0; wzyupeddmy--) {
			continue;
		}
	}
	if (true != true) {
		int jjfgvincl;
		for (jjfgvincl = 42; jjfgvincl > 0; jjfgvincl--) {
			continue;
		}
	}

}

string oybrxsy::bqqhxcpsolkudiiqui(double gnhtoammhg) {
	string hkinlk = "vhhyqjtfhptrnmptoafcibnpqsfsushpytrwccrvhmxkraknjlzbn";
	bool gdglxwrtfz = false;
	int hjqedwbqufwluk = 1763;
	int amcgiobws = 1124;
	int nweevcfnrvstpsf = 5632;
	bool jccoxmqzsdao = false;
	if (false != false) {
		int wipw;
		for (wipw = 0; wipw > 0; wipw--) {
			continue;
		}
	}
	if (string("vhhyqjtfhptrnmptoafcibnpqsfsushpytrwccrvhmxkraknjlzbn") != string("vhhyqjtfhptrnmptoafcibnpqsfsushpytrwccrvhmxkraknjlzbn")) {
		int iktk;
		for (iktk = 4; iktk > 0; iktk--) {
			continue;
		}
	}
	if (string("vhhyqjtfhptrnmptoafcibnpqsfsushpytrwccrvhmxkraknjlzbn") != string("vhhyqjtfhptrnmptoafcibnpqsfsushpytrwccrvhmxkraknjlzbn")) {
		int vgwyi;
		for (vgwyi = 53; vgwyi > 0; vgwyi--) {
			continue;
		}
	}
	return string("gwyknow");
}

bool oybrxsy::gxkjdjygaozvcaulmljvo(double damlvylir, string yemtdd, double nxyahn, double ufhstshnsxkwpo) {
	string yvqerfymbnxzlbe = "kilgqizfafnmpfzmspqmyimsnifnilzgzdnbjzzvwqbmhgmafezca";
	double bevxnjlm = 19907;
	if (19907 == 19907) {
		int pr;
		for (pr = 18; pr > 0; pr--) {
			continue;
		}
	}
	if (19907 != 19907) {
		int qrpqrmowuo;
		for (qrpqrmowuo = 67; qrpqrmowuo > 0; qrpqrmowuo--) {
			continue;
		}
	}
	if (19907 == 19907) {
		int iylblk;
		for (iylblk = 3; iylblk > 0; iylblk--) {
			continue;
		}
	}
	return false;
}

int oybrxsy::pduyiuhhcqotii(double jtmdwiedecckd, string tdrnbxgqifpa, int xihyycw, int qlbqvhf, double dljpydtilepv, bool amrwlg, int wywhxohqn, int shspmlsujylvc) {
	string ibnmfk = "tkngdbdqbcjsjhzpgpwgtjcsijknidaxrqbspkdoylepeuqdmsezzbdvkrubeubyaisjbzqoirqdzoswntnklvvod";
	double uigyjmsdakfrbp = 45340;
	bool bclvuftkf = false;
	int hruwitqamt = 1033;
	string oklbikyegflb = "qbzcpsvhnfyulx";
	string ljsqqazgqef = "rtxofdteacbhqaqsfmwpwergovwfbspwfhjykzixfuneaifeblvlppziiovsiderjlxhuqzxqx";
	double dpiolgo = 34229;
	string fmlpzcjgc = "yew";
	int bcpofgawxkg = 1936;
	bool glmenmnvubnn = true;
	if (1033 != 1033) {
		int ru;
		for (ru = 95; ru > 0; ru--) {
			continue;
		}
	}
	if (false == false) {
		int mdfsefokm;
		for (mdfsefokm = 33; mdfsefokm > 0; mdfsefokm--) {
			continue;
		}
	}
	if (34229 != 34229) {
		int ueag;
		for (ueag = 26; ueag > 0; ueag--) {
			continue;
		}
	}
	if (34229 != 34229) {
		int unpfutd;
		for (unpfutd = 74; unpfutd > 0; unpfutd--) {
			continue;
		}
	}
	return 26894;
}

string oybrxsy::bbpcwclnfefmusmarifrygo() {
	int debuf = 2358;
	if (2358 == 2358) {
		int zdldnpr;
		for (zdldnpr = 2; zdldnpr > 0; zdldnpr--) {
			continue;
		}
	}
	if (2358 == 2358) {
		int ugchadbam;
		for (ugchadbam = 25; ugchadbam > 0; ugchadbam--) {
			continue;
		}
	}
	if (2358 == 2358) {
		int ajotwcmn;
		for (ajotwcmn = 24; ajotwcmn > 0; ajotwcmn--) {
			continue;
		}
	}
	if (2358 == 2358) {
		int tskrihblz;
		for (tskrihblz = 91; tskrihblz > 0; tskrihblz--) {
			continue;
		}
	}
	if (2358 != 2358) {
		int azhwcrga;
		for (azhwcrga = 41; azhwcrga > 0; azhwcrga--) {
			continue;
		}
	}
	return string("qwkmaywykvqnzincgk");
}

int oybrxsy::fzwpthqyufjzqxmbpwdf(bool hajkidilmsmvz, int ldlgjirutsr, int dypyfjgd, bool ytvsgdplnexz, double ashvqxumn, int tzfevcjays, int rtbuimhwu, string faookwhy) {
	int ollsdeqecyf = 5356;
	int jsxdvjofo = 1296;
	if (1296 != 1296) {
		int fgossute;
		for (fgossute = 59; fgossute > 0; fgossute--) {
			continue;
		}
	}
	if (1296 != 1296) {
		int pwkluf;
		for (pwkluf = 93; pwkluf > 0; pwkluf--) {
			continue;
		}
	}
	if (5356 != 5356) {
		int jctsjd;
		for (jctsjd = 51; jctsjd > 0; jctsjd--) {
			continue;
		}
	}
	if (1296 == 1296) {
		int nwtjr;
		for (nwtjr = 100; nwtjr > 0; nwtjr--) {
			continue;
		}
	}
	if (5356 == 5356) {
		int hwoddmjv;
		for (hwoddmjv = 46; hwoddmjv > 0; hwoddmjv--) {
			continue;
		}
	}
	return 38405;
}

void oybrxsy::msoxcawzlyoio(string ohskx, int fslmfexptd, string mjjzoibkm, bool bovfvnbzlr, bool mqvpsalpbo, double djanzkkba, int rkiob, int edmyhbfpigty) {
	double vvhmetbpgjeubpm = 33001;
	if (33001 != 33001) {
		int iyps;
		for (iyps = 29; iyps > 0; iyps--) {
			continue;
		}
	}
	if (33001 != 33001) {
		int kbupcs;
		for (kbupcs = 84; kbupcs > 0; kbupcs--) {
			continue;
		}
	}
	if (33001 == 33001) {
		int vhzyoofd;
		for (vhzyoofd = 0; vhzyoofd > 0; vhzyoofd--) {
			continue;
		}
	}
	if (33001 != 33001) {
		int ddtn;
		for (ddtn = 16; ddtn > 0; ddtn--) {
			continue;
		}
	}

}

oybrxsy::oybrxsy() {
	this->fzwpthqyufjzqxmbpwdf(true, 463, 2463, false, 33825, 2982, 8330, string("chipqnudftlrgiihxstxrnjjtddmrprmjqouvxfwczbzjoalbuimddc"));
	this->msoxcawzlyoio(string("emf"), 444, string("ahrufpvxbvargu"), true, true, 75248, 2681, 1161);
	this->qnumfdgyymexapfjyyfpnv(false, 72359);
	this->chvpcbctswscbdeaoh(4304, 19578, 5323, 71374, 38028, string("tmdybjacyquhdlcbuqstmqxqrmvgnevrjvkdhebjkjscikflvgcqcnvsrygixdaqfjqyhlxydboaiuvrwmudhr"), string("ohgjeumsbeyurmfiaeukuqiyebqmawgwnfrlvfhlitpyjvdkxypxesjarcjpytxcqialymvzrjrioyucyjxdcd"), 20992, 4587, 593);
	this->bqqhxcpsolkudiiqui(20898);
	this->gxkjdjygaozvcaulmljvo(4432, string("ahdulzckzojdqpjerjureivjcezloaxdtmgshzncitgjbhwtoncbrrdeykrlfyjshnoglgqpcrtbjvnkgnfdftkfucdggdcau"), 29694, 84490);
	this->pduyiuhhcqotii(17947, string("zasdhjdq"), 2984, 1532, 1770, false, 1196, 4935);
	this->bbpcwclnfefmusmarifrygo();
	this->xpgjtlgrkmzpwlmobovn(string("buyfaxwrirsqbaumjhuqmpldnvcukniatgxyagennstwnxufuvhtidwoycbnln"), 27525, 37080, 930, 39724, string("nhsjlcuqviswsczznjpiaepneggyogytefvlbn"), 37976, false, 2433);
	this->zfmefvbwyzn(string("tvsfaooufibdnriijqksiotlqgfnfrzqqzwgkofaifvbkslzbkifxnkhvspyrnun"), 2658, false, 18495, true, string("qiyeacerjhvmixcebxsmxvlcdmaalxqcnhhg"), false, string("wwszxebkepzrnnntcfejgxfhfhurzgswwcvpwygielxcydisvnurngwx"), string("pdywmzlnfcfhdnxkpebhuwacfwnopjdkbtrydzitmjtfwwtdkyvtucjdwyoxzfnywxnbzvgijyounpmwknv"), 25077);
}
