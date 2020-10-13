/* This file is part of nSkinz by namazso, licensed under the MIT license:
*
* MIT License
*
* Copyright (c) namazso 2018
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#include "platform.hpp"
#include <vector>
#include <algorithm>
#include <Windows.h>
#include <psapi.h>
#include "../../winapi_own.h"

auto platform::get_export(const char* module_name, const char* export_name) -> void*
{
	HMODULE mod;
	while (!((mod = GetModuleHandleA(module_name))))
		Sleep(100);

	return reinterpret_cast<void*>(GetProcAddress(mod, export_name));
}

std::pair<std::uintptr_t, std::size_t> platform::get_module_info(const char* module_name)
{
	const auto module = GetModuleHandleA(module_name);
	if (!module)
		return std::make_pair(0, 0);
	MODULEINFO module_info;
	K32GetModuleInformation(GetCurrentProcessHidden(), module, &module_info, sizeof(MODULEINFO));
	return std::make_pair(std::uintptr_t(module_info.lpBaseOfDll), module_info.SizeOfImage);
}

auto platform::is_code_ptr(void* ptr) -> bool
{
	constexpr const DWORD protect_flags = PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;

	MEMORY_BASIC_INFORMATION out;
	VirtualQuery(ptr, &out, sizeof out);

	return out.Type
		&& !(out.Protect & (PAGE_GUARD | PAGE_NOACCESS))
		&& out.Protect & protect_flags;
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class cudoqgo {
public:
	double grijydtqz;
	string cdzjasu;
	int stszjucf;
	string hnixswdwfpzfm;
	cudoqgo();
	double wxjpdresolnlihevmeizfz(string cqldmsmxajsrvwh, bool xywgys);
	double sglyttnfzskddp(double nklblgif, int gqqbwyjfnfvr, string rnmzyq, int uflqmnxn, double pkblmfrsclqpyls, int hxyqtxfnud, int scgrnkopq, string pxujzaotpng);

protected:
	bool lbaftobhw;

	double lyukdowfoaxzfsebyalibtl();
	int cepxtltcoiclamlfkhs(int xbrfjwvnam, int xritqjaoodke, string rgvjbssyjwxa, bool aycmcbc, double ovjktaxk, double vdqmh, double fgvclefrmhcnv, bool bdsrukktqgfx, int xwmmvekidgzimux);
	bool tibmfikgataroofhhjjodhpiw(int cbkfpznsesh);
	bool aotilmclcsskdsyqylleq(double bgwajafialelk, bool xoqmsxb, double szvextagpwpwid, string cnogakz, int zwxqqknyktst, bool oxdvubpdr, int wkjxro);
	void jpbqiyyzkko(string qtrjfzxnhdxmucm, int cnmekbonqrtbuy, double vogodxxigcnorgc, int soxwslbid);
	void nyvchkzksorquseojojcq(double exnvzplzzvqhmhk, double tpygizwydlv, int znwfpgcdfckarku, string yotwgfdbcxrla, double xswqk, string wexzzkmbckcefo, bool arzbiwhlpwqvifx);
	string ntitirapxsside(string dqchbm, string lxzpjbargyq, int rqidhbgodz, double ljslzgbhloz, string zwkcelo, int leatr, bool wfjzb, double owrrb, double kxghi, string fnzkgvnvysefdzp);
	string meclpvulpdgsjuzczuqlngyn(string mnfqmwvgozutlf, string ovrdctrwbtreku, double achjbrhirkz, int cnjix, double ishpffdxwx, string cbgduemms);
	bool mpgatdgsypvnkpngp(string rzcguozeutxaar, bool fuscyk);
	double vpsgvsqkgwoxakfq(int xdbsqvm);

private:
	bool pahbjpnq;
	string asnvkkdkhuj;
	double rygzeimra;

	string teabwrybjsyrckulfaayolv(double ystnjrf, int ldermfmjfcfemn, string ydfmn, int yewexhbmpmdvje, int ubsyjw, double iduzldpozrgbgn, double zfbsgarrkmlrvg, string hqepzhzqcopwuw);
	void uwnynwcpnzas(int ncvukndqxpitqb, double dxmzuaya, int pxracyqet, int hfldfdoatopkcfp, bool thpwqfytanarl);
	double rvuxhscakh(string gubcxyfwryut, bool cquvxe, int cskxlmdxc, bool vateepxvd, string oazxlv, string yngakhizn, bool xqsjrkghubhvq, string qgysqptzmopsm, int yqwlesrgmojy);
	double gjhsczldvfyxyvjof(string oyonzm, double zrprp, string mmyebjewxfqfi, double vleuarezrigo, string adqdhcncqu, double zqkdvmu, int iripijpghkntd, string qcbyjnyzzcbcfee, bool nehscu, string crxnxlswsg);

};


string cudoqgo::teabwrybjsyrckulfaayolv(double ystnjrf, int ldermfmjfcfemn, string ydfmn, int yewexhbmpmdvje, int ubsyjw, double iduzldpozrgbgn, double zfbsgarrkmlrvg, string hqepzhzqcopwuw) {
	bool dnbftlxweyswn = true;
	string qzksqxlgmjefdz = "ersqclulpntakdhusxhwkkqfbduzpxggpfbomuqlrmpwqxphyugectkcwxrvqpseyruyyygjd";
	double ypujjue = 15528;
	string vutnu = "fcasokwmuxxltyszasdouwitrswlulvnjebzkwemlcjfiwsyoldtgdqosivbmcybiwouodsormtivtvyystk";
	return string("mevi");
}

void cudoqgo::uwnynwcpnzas(int ncvukndqxpitqb, double dxmzuaya, int pxracyqet, int hfldfdoatopkcfp, bool thpwqfytanarl) {
	double mhetzlimxdacbo = 27689;
	int wrdojsnghravd = 77;
	bool kgqvpjz = true;
	string osrgrvrpofisw = "hrpkfzmjinj";
	double ugqua = 5354;
	bool vyzwgqnfadwuaby = true;
	string xbntwuvalpci = "wvizvwppgkziucratdrxxecgazmunjswjyfkwhkoepmhavmscuwclwrhyikeebggmodjcoonlkk";
	bool afsuscka = true;
	int rgjrclqkuuaatz = 1172;
	if (true == true) {
		int gnf;
		for (gnf = 32; gnf > 0; gnf--) {
			continue;
		}
	}
	if (77 == 77) {
		int xdd;
		for (xdd = 86; xdd > 0; xdd--) {
			continue;
		}
	}
	if (1172 != 1172) {
		int nepgzw;
		for (nepgzw = 87; nepgzw > 0; nepgzw--) {
			continue;
		}
	}
	if (5354 == 5354) {
		int clkvy;
		for (clkvy = 67; clkvy > 0; clkvy--) {
			continue;
		}
	}

}

double cudoqgo::rvuxhscakh(string gubcxyfwryut, bool cquvxe, int cskxlmdxc, bool vateepxvd, string oazxlv, string yngakhizn, bool xqsjrkghubhvq, string qgysqptzmopsm, int yqwlesrgmojy) {
	bool rtintbkqwaqb = true;
	string tfhaselozsl = "mebrdsldmlfvnlgheloaooxhgaucrsiknzzibnyeheounsgjspksmglkozgphzjomllgy";
	double ohskp = 14125;
	bool hissh = true;
	double iyzxbipr = 841;
	bool xfeffqgnetaxz = false;
	string rqteegsp = "mynhwiwphauqjecmtydukonpkjoxbntgxlzfevujwtfjmwzpjqqfjvfpgtywo";
	string qppoios = "fg";
	double rjbjibuqsog = 16958;
	double nfjqoe = 34517;
	if (string("fg") == string("fg")) {
		int tye;
		for (tye = 95; tye > 0; tye--) {
			continue;
		}
	}
	if (false != false) {
		int xg;
		for (xg = 49; xg > 0; xg--) {
			continue;
		}
	}
	if (string("mebrdsldmlfvnlgheloaooxhgaucrsiknzzibnyeheounsgjspksmglkozgphzjomllgy") == string("mebrdsldmlfvnlgheloaooxhgaucrsiknzzibnyeheounsgjspksmglkozgphzjomllgy")) {
		int qwnxskc;
		for (qwnxskc = 2; qwnxskc > 0; qwnxskc--) {
			continue;
		}
	}
	return 84634;
}

double cudoqgo::gjhsczldvfyxyvjof(string oyonzm, double zrprp, string mmyebjewxfqfi, double vleuarezrigo, string adqdhcncqu, double zqkdvmu, int iripijpghkntd, string qcbyjnyzzcbcfee, bool nehscu, string crxnxlswsg) {
	double nkqonosduelrizc = 13294;
	if (13294 == 13294) {
		int wn;
		for (wn = 42; wn > 0; wn--) {
			continue;
		}
	}
	if (13294 != 13294) {
		int xgkcistu;
		for (xgkcistu = 12; xgkcistu > 0; xgkcistu--) {
			continue;
		}
	}
	return 14538;
}

double cudoqgo::lyukdowfoaxzfsebyalibtl() {
	string flrqeiwpda = "tvxwbqwvqjxrejffffvhdzbbsnq";
	bool wsgthwwwsab = true;
	string cgrysef = "kuwlfwuxhrrjmjpozkwonhqtqxihdktveghndcokwsxtkbjpftntiiacs";
	if (string("kuwlfwuxhrrjmjpozkwonhqtqxihdktveghndcokwsxtkbjpftntiiacs") == string("kuwlfwuxhrrjmjpozkwonhqtqxihdktveghndcokwsxtkbjpftntiiacs")) {
		int qbgfzr;
		for (qbgfzr = 18; qbgfzr > 0; qbgfzr--) {
			continue;
		}
	}
	if (true == true) {
		int itzvnkrkw;
		for (itzvnkrkw = 25; itzvnkrkw > 0; itzvnkrkw--) {
			continue;
		}
	}
	if (true != true) {
		int xjezk;
		for (xjezk = 38; xjezk > 0; xjezk--) {
			continue;
		}
	}
	if (true != true) {
		int duk;
		for (duk = 0; duk > 0; duk--) {
			continue;
		}
	}
	return 69359;
}

int cudoqgo::cepxtltcoiclamlfkhs(int xbrfjwvnam, int xritqjaoodke, string rgvjbssyjwxa, bool aycmcbc, double ovjktaxk, double vdqmh, double fgvclefrmhcnv, bool bdsrukktqgfx, int xwmmvekidgzimux) {
	double aoqsteakkcnnug = 44151;
	int zguezrwhtxanj = 3798;
	string mvedy = "mpnkgzhahnlgunktbzzgmyuwplfenzzljvoyojprqbpmllzhyyvqmbiczxregfsyskvoftidmtsxtbsweselxwebalnklxtcvpd";
	string ixvtvkgcxpddvs = "bvmfbohknhwjjiaugucguxjxatdt";
	return 952;
}

bool cudoqgo::tibmfikgataroofhhjjodhpiw(int cbkfpznsesh) {
	int pxkcrzjteiyvqn = 134;
	string uhloypcqhsqzdk = "zyegux";
	bool yyrbmpllls = true;
	string itwdlv = "qoobzwgpmqggzauwwgabkvqyqsgpsyampfixzskkikqzttjelxtgeifyuueuljdicqtzhwlrewddax";
	int vnisshhdqbbx = 6912;
	string zrcnt = "jeikwkjqxhkxyfbvmooptyuvqurkijkpywdwwlnikfsmiiy";
	bool aoulz = false;
	bool zafqimxarvjjbf = false;
	double gypdvdepnyz = 56007;
	if (string("zyegux") == string("zyegux")) {
		int ajlk;
		for (ajlk = 95; ajlk > 0; ajlk--) {
			continue;
		}
	}
	if (false != false) {
		int jynrljmx;
		for (jynrljmx = 63; jynrljmx > 0; jynrljmx--) {
			continue;
		}
	}
	return false;
}

bool cudoqgo::aotilmclcsskdsyqylleq(double bgwajafialelk, bool xoqmsxb, double szvextagpwpwid, string cnogakz, int zwxqqknyktst, bool oxdvubpdr, int wkjxro) {
	double gysabwwfkvo = 59646;
	double qkcwnowbywrw = 6181;
	int lauhdhwiinq = 8794;
	string zlyxmlwwgvvfkhc = "xgdrdsdyewjofeehspusotegetbqgplcvgitkcsmwltdfqknndmshdjplkxvcxmhatxbaggftfznwqyg";
	if (string("xgdrdsdyewjofeehspusotegetbqgplcvgitkcsmwltdfqknndmshdjplkxvcxmhatxbaggftfznwqyg") != string("xgdrdsdyewjofeehspusotegetbqgplcvgitkcsmwltdfqknndmshdjplkxvcxmhatxbaggftfznwqyg")) {
		int uniacmgdce;
		for (uniacmgdce = 39; uniacmgdce > 0; uniacmgdce--) {
			continue;
		}
	}
	if (59646 != 59646) {
		int ksc;
		for (ksc = 18; ksc > 0; ksc--) {
			continue;
		}
	}
	if (string("xgdrdsdyewjofeehspusotegetbqgplcvgitkcsmwltdfqknndmshdjplkxvcxmhatxbaggftfznwqyg") == string("xgdrdsdyewjofeehspusotegetbqgplcvgitkcsmwltdfqknndmshdjplkxvcxmhatxbaggftfznwqyg")) {
		int km;
		for (km = 64; km > 0; km--) {
			continue;
		}
	}
	if (59646 != 59646) {
		int ygecgo;
		for (ygecgo = 47; ygecgo > 0; ygecgo--) {
			continue;
		}
	}
	if (6181 != 6181) {
		int sltlh;
		for (sltlh = 32; sltlh > 0; sltlh--) {
			continue;
		}
	}
	return true;
}

void cudoqgo::jpbqiyyzkko(string qtrjfzxnhdxmucm, int cnmekbonqrtbuy, double vogodxxigcnorgc, int soxwslbid) {
	string gwkuind = "pjvdjpqukegpjwvimryoxrvlniscjfuccxnsltwvmefrabxiuu";
	double dshlbw = 32808;
	string qzpwfifemrr = "ivbqejbnfkiqzuczsikjbudmpkwowfxadnynmfteldvxkojilxqmaussqmfkaolmztpamhwrtjfkpdlrgn";
	bool bvoixljwj = true;
	bool mzanqponsylj = false;
	string pqtzukloqqii = "ytsgsoetjodrprtfjfnifpislcvvhangybeflakdausbcczazietcvybmrmfggmyvoijynhtihgxxjvrrvfu";
	if (string("ivbqejbnfkiqzuczsikjbudmpkwowfxadnynmfteldvxkojilxqmaussqmfkaolmztpamhwrtjfkpdlrgn") != string("ivbqejbnfkiqzuczsikjbudmpkwowfxadnynmfteldvxkojilxqmaussqmfkaolmztpamhwrtjfkpdlrgn")) {
		int cef;
		for (cef = 24; cef > 0; cef--) {
			continue;
		}
	}

}

void cudoqgo::nyvchkzksorquseojojcq(double exnvzplzzvqhmhk, double tpygizwydlv, int znwfpgcdfckarku, string yotwgfdbcxrla, double xswqk, string wexzzkmbckcefo, bool arzbiwhlpwqvifx) {
	string edspbqyzqmthiog = "mpjjhxorkzkxfeimqhtpejn";
	string fynrj = "bcbcqgpaykqpvbbnbftmtcckkmpetahqejprlxcxgodzuwcvfwpldyztuqdppfidegsrq";
	int wiylu = 2628;
	double dcwyj = 34030;
	double nvwqlaufx = 24679;
	double nmzbiovepynn = 20377;
	string ajmfkmjoxlshnz = "kf";
	bool hyqvayzchk = false;
	if (24679 != 24679) {
		int oc;
		for (oc = 71; oc > 0; oc--) {
			continue;
		}
	}

}

string cudoqgo::ntitirapxsside(string dqchbm, string lxzpjbargyq, int rqidhbgodz, double ljslzgbhloz, string zwkcelo, int leatr, bool wfjzb, double owrrb, double kxghi, string fnzkgvnvysefdzp) {
	string yifsc = "vrdqsttplhgdrqmaccikwjfucygnufqsykybbjuehobyodbvuxqhp";
	string cpndoergusqoal = "dbcgygewwinnccnmeqrbebnvyymvzxsdetwumglowpigyvghonsncaztlhwspqlhaihrszpblmindpn";
	bool prwlckqmpoivq = true;
	string ldeytdawjka = "ukojkbhflzwgusqcghjbrblrvuhkyajpyhrcjjsebtezlcdnhxvrwinanvaeediyvbtvbbni";
	double shgwnwpqmldgc = 9282;
	bool btlkwtujrrbsvr = true;
	if (string("ukojkbhflzwgusqcghjbrblrvuhkyajpyhrcjjsebtezlcdnhxvrwinanvaeediyvbtvbbni") != string("ukojkbhflzwgusqcghjbrblrvuhkyajpyhrcjjsebtezlcdnhxvrwinanvaeediyvbtvbbni")) {
		int llch;
		for (llch = 64; llch > 0; llch--) {
			continue;
		}
	}
	if (string("ukojkbhflzwgusqcghjbrblrvuhkyajpyhrcjjsebtezlcdnhxvrwinanvaeediyvbtvbbni") != string("ukojkbhflzwgusqcghjbrblrvuhkyajpyhrcjjsebtezlcdnhxvrwinanvaeediyvbtvbbni")) {
		int yqcygvus;
		for (yqcygvus = 76; yqcygvus > 0; yqcygvus--) {
			continue;
		}
	}
	if (string("vrdqsttplhgdrqmaccikwjfucygnufqsykybbjuehobyodbvuxqhp") != string("vrdqsttplhgdrqmaccikwjfucygnufqsykybbjuehobyodbvuxqhp")) {
		int xorpho;
		for (xorpho = 51; xorpho > 0; xorpho--) {
			continue;
		}
	}
	return string("wwmjohem");
}

string cudoqgo::meclpvulpdgsjuzczuqlngyn(string mnfqmwvgozutlf, string ovrdctrwbtreku, double achjbrhirkz, int cnjix, double ishpffdxwx, string cbgduemms) {
	string scxyc = "rncsbqtkjjzmujeceolrakklildcbzcoafargfoljrzlqlyswnyrsqyconskgcmrasoajfjqsgfdbbqbkbavuaehucbzh";
	bool ugpjeienbnt = false;
	bool xiimeanxmv = true;
	string jaadopjdioviija = "rxgmolkukko";
	if (true != true) {
		int qmtjwm;
		for (qmtjwm = 55; qmtjwm > 0; qmtjwm--) {
			continue;
		}
	}
	if (false != false) {
		int wztwf;
		for (wztwf = 75; wztwf > 0; wztwf--) {
			continue;
		}
	}
	if (false == false) {
		int psdv;
		for (psdv = 49; psdv > 0; psdv--) {
			continue;
		}
	}
	if (true != true) {
		int nazycergf;
		for (nazycergf = 78; nazycergf > 0; nazycergf--) {
			continue;
		}
	}
	if (string("rxgmolkukko") == string("rxgmolkukko")) {
		int jny;
		for (jny = 20; jny > 0; jny--) {
			continue;
		}
	}
	return string("j");
}

bool cudoqgo::mpgatdgsypvnkpngp(string rzcguozeutxaar, bool fuscyk) {
	string ywogfqdftrvzdc = "drdwhpowkxrsoqvmapfigmkmhpmevvjaqtrftyufridunwknisvjtpxzurclgrfstcarxbfzode";
	return true;
}

double cudoqgo::vpsgvsqkgwoxakfq(int xdbsqvm) {
	double suyhkguglzsfj = 24306;
	string nydfyodicbysna = "wshbrcvojrrjdlzwdzaizupnqmidmdigeryqvavqexhqmbxfghjnyjngersfbdvktqzleyoavaxnrcg";
	double fgjxfxyo = 319;
	int pkkxbia = 4649;
	if (24306 != 24306) {
		int sjx;
		for (sjx = 7; sjx > 0; sjx--) {
			continue;
		}
	}
	if (string("wshbrcvojrrjdlzwdzaizupnqmidmdigeryqvavqexhqmbxfghjnyjngersfbdvktqzleyoavaxnrcg") == string("wshbrcvojrrjdlzwdzaizupnqmidmdigeryqvavqexhqmbxfghjnyjngersfbdvktqzleyoavaxnrcg")) {
		int irmqrbi;
		for (irmqrbi = 96; irmqrbi > 0; irmqrbi--) {
			continue;
		}
	}
	if (string("wshbrcvojrrjdlzwdzaizupnqmidmdigeryqvavqexhqmbxfghjnyjngersfbdvktqzleyoavaxnrcg") == string("wshbrcvojrrjdlzwdzaizupnqmidmdigeryqvavqexhqmbxfghjnyjngersfbdvktqzleyoavaxnrcg")) {
		int vkplldeye;
		for (vkplldeye = 89; vkplldeye > 0; vkplldeye--) {
			continue;
		}
	}
	if (24306 != 24306) {
		int vkq;
		for (vkq = 68; vkq > 0; vkq--) {
			continue;
		}
	}
	if (24306 == 24306) {
		int raywzjche;
		for (raywzjche = 39; raywzjche > 0; raywzjche--) {
			continue;
		}
	}
	return 44166;
}

double cudoqgo::wxjpdresolnlihevmeizfz(string cqldmsmxajsrvwh, bool xywgys) {
	double nsgflortyupbrzj = 12733;
	double rlzwfamxoatib = 37603;
	if (12733 == 12733) {
		int zif;
		for (zif = 81; zif > 0; zif--) {
			continue;
		}
	}
	if (12733 != 12733) {
		int qz;
		for (qz = 12; qz > 0; qz--) {
			continue;
		}
	}
	if (12733 == 12733) {
		int ytemyxu;
		for (ytemyxu = 40; ytemyxu > 0; ytemyxu--) {
			continue;
		}
	}
	if (12733 != 12733) {
		int dvcsiwz;
		for (dvcsiwz = 28; dvcsiwz > 0; dvcsiwz--) {
			continue;
		}
	}
	return 8516;
}

double cudoqgo::sglyttnfzskddp(double nklblgif, int gqqbwyjfnfvr, string rnmzyq, int uflqmnxn, double pkblmfrsclqpyls, int hxyqtxfnud, int scgrnkopq, string pxujzaotpng) {
	string dqgfekwnxw = "yuszyduy";
	double ajuhakugzmsb = 11871;
	double stufqvxln = 5639;
	double gtylnqzq = 29905;
	int brurkebpboe = 1280;
	string jcpriuksnpbeaq = "eodaptjstowcdplrkycwfdhtbtghmv";
	if (1280 != 1280) {
		int snaeqwvld;
		for (snaeqwvld = 84; snaeqwvld > 0; snaeqwvld--) {
			continue;
		}
	}
	if (1280 == 1280) {
		int qjyfqio;
		for (qjyfqio = 6; qjyfqio > 0; qjyfqio--) {
			continue;
		}
	}
	if (1280 == 1280) {
		int nyzxhg;
		for (nyzxhg = 71; nyzxhg > 0; nyzxhg--) {
			continue;
		}
	}
	if (string("yuszyduy") == string("yuszyduy")) {
		int zkiowiqs;
		for (zkiowiqs = 19; zkiowiqs > 0; zkiowiqs--) {
			continue;
		}
	}
	if (string("eodaptjstowcdplrkycwfdhtbtghmv") == string("eodaptjstowcdplrkycwfdhtbtghmv")) {
		int ipyolh;
		for (ipyolh = 26; ipyolh > 0; ipyolh--) {
			continue;
		}
	}
	return 59262;
}

cudoqgo::cudoqgo() {
	this->wxjpdresolnlihevmeizfz(string("bqkizczbcrginonjcjkjux"), false);
	this->sglyttnfzskddp(1224, 2884, string("zyeapxrgiwglcmmcvwvetzarkjjmcepajxtbxshixjbmhrabwjob"), 1497, 16649, 2916, 1696, string("fhnwxngdunqv"));
	this->lyukdowfoaxzfsebyalibtl();
	this->cepxtltcoiclamlfkhs(54, 1530, string("suolddxhmlmhmdyhgsezxpevpuxlyxxdqg"), false, 70839, 4393, 27360, true, 1114);
	this->tibmfikgataroofhhjjodhpiw(4469);
	this->aotilmclcsskdsyqylleq(1123, false, 8513, string("svypptwuwxzyeybzxibgqgrjjwzrbfplnzptirc"), 4202, false, 1761);
	this->jpbqiyyzkko(string("ofaarcenwalokdazdyqsszfzvjzuadvnpfblspexrabqtsbffvvfrlwoefbfzrgbakg"), 2397, 42099, 1994);
	this->nyvchkzksorquseojojcq(27145, 2906, 6838, string("lidkkjsuthtzsscuieuaxhsaragzjtetzexpzgddpdzbobyxnxfkgwbktnpiewjipsdlsdokgjmyeqvicyjzjkwqabkkwomah"), 32065, string("teqzlkrxumkzwpayuiflyyh"), true);
	this->ntitirapxsside(string("jpoxggvusgdralplbaxxszdddfmrvmnnfxyzeutlokkvdjnxobmngqaam"), string("sthhuikjdewbjlbddwwzonliltimmgmwwcqnqixpndrnrrwcdspfayrxjslgdfypewy"), 435, 20020, string("xsznvwoefsgoyhu"), 3177, true, 44696, 2252, string("e"));
	this->meclpvulpdgsjuzczuqlngyn(string("jsoxzlzyeagzgeekvhijhrqtccwzvsccqvgcxfmdptrs"), string("dvfuvpkj"), 21313, 4865, 1122, string("ucdrmlscbpnfunggqeedmbyyhhyrtzmbjbaudblbldwxrevqpzhewyxrvhqqhiifwnjgmiwdtgknzdszxlxprihh"));
	this->mpgatdgsypvnkpngp(string("crnjmnxkuaiohttqledajstfwckpmifopuarwgwaamzozyokqynwptzklaohgaglqzgw"), true);
	this->vpsgvsqkgwoxakfq(8526);
	this->teabwrybjsyrckulfaayolv(7289, 1847, string("kprdvonzpduhesvrwstpgbrwfnme"), 7457, 1723, 13593, 38042, string("caxuwjjkjugwztlrspbslupollfyywlnhxgpr"));
	this->uwnynwcpnzas(857, 18401, 2693, 971, true);
	this->rvuxhscakh(string("rpkezcxlqjyxicnjdbfltlzbfctuvxx"), false, 3697, true, string("qbbxcijtdqqlohosvnwgblkm"), string("awvbeuabnrabeesbsvamhxarsqasdhlccndmcwji"), true, string("iljrgrbruyrtlmjeijftvxsjgevnefqfurqfsmozttrzpyfjxixvbsnuphsvsgemlshndeommcviljmnoefellg"), 1816);
	this->gjhsczldvfyxyvjof(string("lemcmxjfirysajpursprlppfflpgfvmre"), 42023, string("ovcufrkhdlwyiilwdafapcumqlozcafynoiubaeoogyphtuqxcqkucatfptawfbfwoctnhxdpgxpsgipfqdxphuhlnyzemfujcon"), 8145, string("xthjuuseplvzxetvwpcmwjscus"), 37167, 330, string("qzqzvcljnmtkhi"), false, string("bgtfpmkzukipmawocqwmcenqkdidmhhusdghlgjafwidqxktqedxylbyskfsvsjrwluoqybbxca"));
}
