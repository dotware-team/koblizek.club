#include "Interfaces.h"
#include "Memory.h"
#include <map>
#include "../cheat_protection/security.h"

#include "../cheat_protection/xor.hpp"

template <typename T>
static constexpr auto relativeToAbsolute(int* address) noexcept
{
	return reinterpret_cast<T>(reinterpret_cast<char*>(address + 1) + *address);
}

#define FIND_PATTERN(type, ...) \
reinterpret_cast<type>(findPattern(__VA_ARGS__))
Memory::Memory() noexcept
{
	present = findPattern(L"gameoverlayrenderer", "\xFF\x15????\x8B\xF8\x85\xDB") + 2;
	reset = findPattern(L"gameoverlayrenderer", "\xC7\x45?????\xFF\x15????\x8B\xF8") + 9;
	clientMode = **reinterpret_cast<ClientMode***>((*reinterpret_cast<uintptr_t**>(interfaces.client))[10] + 5);
	input = *reinterpret_cast<Input**>((*reinterpret_cast<uintptr_t**>(interfaces.client))[16] + 1);
	globalVars = **reinterpret_cast<GlobalVars***>((*reinterpret_cast<uintptr_t**>(interfaces.client))[11] + 10);
	glowObjectManager = *reinterpret_cast<GlowObjectManager**>(findPattern(L"client", "\x0F\x11\x05????\x83\xC8\x01") + 3);
	disablePostProcessing = *reinterpret_cast<bool**>(findPattern(L"client", "\x83\xEC\x4C\x80\x3D") + 5);
	loadSky = relativeToAbsolute<decltype(loadSky)>(reinterpret_cast<int*>(findPattern(L"engine", "\xE8????\x84\xC0\x74\x2D\xA1") + 1));
	setClanTag = reinterpret_cast<decltype(setClanTag)>(findPattern(L"engine", "\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15"));
	lineGoesThroughSmoke = relativeToAbsolute<decltype(lineGoesThroughSmoke)>(reinterpret_cast<int*>(findPattern(L"client", "\xE8????\x8B\x4C\x24\x30\x33\xD2") + 1));
	smokeCount = *(reinterpret_cast<int**>(lineGoesThroughSmoke) + 2);
	cameraThink = findPattern(L"client", "\x85\xC0\x75\x30\x38\x86");
	acceptMatch = reinterpret_cast<decltype(acceptMatch)>(findPattern(L"client", "\x55\x8B\xEC\x83\xE4\xF8\x8B\x4D\x08\xBA????\xE8????\x85\xC0\x75\x12"));
	getSequenceActivity = reinterpret_cast<decltype(getSequenceActivity)>(findPattern(L"client", "\x55\x8B\xEC\x53\x8B\x5D\x08\x56\x8B\xF1\x83"));
	scopeArc = findPattern(L"client", "\x6A?\xFF\x50\x3C\x8B\x0D????\xFF\xB7") + 5;
	scopeLens = findPattern(L"client", "\xFF\x50\x3C\x8B\x4C\x24\x20") + 3;
	isOtherEnemy = relativeToAbsolute<decltype(isOtherEnemy)>(reinterpret_cast<int*>(findPattern(L"client", "\x8B\xCE\xE8????\x02\xC0") + 3));
	auto temp = reinterpret_cast<std::uintptr_t*>(findPattern(L"client", "\xB9????\xE8????\x8B\x5D\x08") + 1);
	hud = *temp;
	findHudElement = relativeToAbsolute<decltype(findHudElement)>(reinterpret_cast<int*>(reinterpret_cast<char*>(temp) + 5));
	clearHudWeapon = reinterpret_cast<decltype(clearHudWeapon)>(findPattern(L"client", "\x55\x8B\xEC\x51\x53\x56\x8B\x75\x08\x8B\xD9\x57\x6B\xFE\x2C"));
	itemSchema = relativeToAbsolute<decltype(itemSchema)>(reinterpret_cast<int*>(findPattern(L"client", "\xE8????\x0F\xB7\x0F") + 1));
	setAbsOrigin = relativeToAbsolute<decltype(setAbsOrigin)>(reinterpret_cast<int*>(findPattern(L"client", "\xE8????\xEB\x19\x8B\x07") + 1));
	traceToExit = reinterpret_cast<decltype(traceToExit)>(findPattern(L"client", "\x55\x8B\xEC\x83\xEC\x30\xF3\x0F\x10\x75"));
	viewRender = **reinterpret_cast<ViewRender***>(findPattern(L"client", "\x8B\x0D????\xFF\x75\x0C\x8B\x45\x08") + 2);
	test = relativeToAbsolute<uintptr_t>(FIND_PATTERN(int*, L"client", "\xE8????\x3B\x44\x24\x0C", 1)) + 0x71;
	test2 = findPattern(L"client", "\x85\xC0\x0F\x84????\x80\x78\x10\x00\x0F\x84");
	conColorMsg = reinterpret_cast<decltype(conColorMsg)>(GetProcAddress(GetModuleHandleW(L"tier0"), "?ConColorMsg@@YAXABVColor@@PBDZZ"));
	predictionRandomSeed = *reinterpret_cast<int**>(findPattern(L"client", "\x8B\x0D????\xBA????\xE8????\x83\xC4\x04") + 2);
	moveData = **reinterpret_cast<MoveData***>(findPattern(L"client", "\xA1????\xF3\x0F\x59\xCD") + 1);
	movehelper = **reinterpret_cast<MoveHelper***>(findPattern(L"client", "\x8B\x0D????\x8B\x45?\x51\x8B\xD4\x89\x02\x8B\x01") + 2);
	fakePrime = reinterpret_cast<std::uint8_t*>(findPattern(L"client", "\x17\xF6\x40\x14\x10") - 1);
	keyValuesFromString = relativeToAbsolute<decltype(keyValuesFromString)>(reinterpret_cast<int*>(findPattern(L"client", "\xE8????\x83\xC4\x04\x89\x45\xD8") + 1));
	keyValuesFindKey = relativeToAbsolute<decltype(keyValuesFindKey)>(reinterpret_cast<int*>(findPattern(L"client", "\xE8????\xF7\x45") + 1));
	keyValuesSetString = relativeToAbsolute<decltype(keyValuesSetString)>(reinterpret_cast<int*>(findPattern(L"client", "\xE8????\x89\x77\x38") + 1));
	g_ChatElement = *reinterpret_cast<CHudChat * *>(reinterpret_cast<uintptr_t>(clientMode) + *reinterpret_cast<uint8_t*>(FindSignature(XorStr("client.dll"), "E8 ? ? ? ? 8B 4F ? 85 C9 74 06 51") + 7));
	renderBeams = *reinterpret_cast<IViewRenderBeams**>(findPattern(L"client", "\xA1????\xFF\x10\xA1????\xB9", 1));
	playerResource = *(C_CSPlayerResource***)(FindSignature(XorStr("client.dll"), "A1 ? ? ? ? 57 85 C0 74 08") + 1);
	unlock_inventory = FindSignature(XorStr("client.dll"), "84 C0 75 04 B0 01 5F");
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class psoqtvo {
public:
	string yrdtmzbceecbbj;
	psoqtvo();
	bool cucjdmdwrqst(string ywdjafpevl, bool sjzym, double pyvpuwygsqpr, int dbeldcdiiyidkw);
	int pxkxhrgvwlwtjwygshsrptk(double mbqdmdw);
	void pamggxqwltlqudvndqz(double tpyvteigandgah, double xiavzwdbk, int dshgxogvuzgaoy, bool jejkjqfeg, int gaatesvj, double otlvz);
	string yluweoiaxbtvprladrpksiam();
	string djvrlduceprsx(string xsulagtuwpley, double zniqvdmv, double aogauo);
	string vzdvldfzkyt(double mormk, int fcuwfjuqpnyg, int ldqhcpildsif);
	string rwkovngdfprloepqmoidclyvs(string axbyuop, int zbsfxbhnj, string htaehvkhmxuls, double zfwjwrdp, bool atrqzzphrxdxupu, bool bomhtbne, bool ubcbgsjq, bool rpmvle, bool rzphswqtroo, double kwpaqnlrrb);
	double khxfufbnbpenqkxrezb(int vpowh, bool ffahkhgkdt, string gtvvlaeraam, string pdniiwdvxcjk);
	int devdtauziwnoylhfojgwpigf(int nxyrafsscvgivn);
	void vmbdxlvyflwbnodegb(double bzrijzszn, bool apevqmsrgrvd, double qhutwrbfktqh);

protected:
	int osiofcnwyksyww;
	string skhbbirlnkqt;
	int kmzhxfrjzlcz;
	int gpmnypzsedjvw;

	int ecohhipibeavgymow(double wcuvamszza, int lebpmoitlsut, double ehfqnginqz);
	int qanrmtlylv(string lchsyureftrlxpl, bool zqsmqlu, double vpcylpzvkkw, string bavqxksa, double bzqda);
	bool ynrhpqznavsfalspwl();
	void ytjlaavrnqdnarleuueg(double bjmcpeqroedkhw, bool iexhc, bool kalaaa, int vcvcwldkmramr, string oszdn);
	int kjyebvwpxon(double eohfknxt, double yabunmzrzclatop, bool glsoboxzu, bool ufrxss, double czilhgofn, double bepnmd, string qjtnkneo, bool agqwsy, bool qnolvthllbvdo, int xvftwpisopk);
	double ytfdvalibuferri(int wswtejdb, bool ebruvi, string vuphnchnzzt, double szgll, bool mkpnlpoaqyrho, string bekmhdxh);
	double zozwsqqsbtkvkgztwqrwp(bool evovrzmnfkmdf, string gnqkjs, string piihj, string yjclcwomyax, int yubxboc, bool wkdngtocmjqsxe, int dcqjhkpg);

private:
	int nggkmzajjwzr;
	string yxmedltfsyediw;
	bool sinhnlav;

	void nrrsoaoycdvumrpgemfwdiaxb(int irgjahogddvv, double sikkkjz, int iwewiphgiauwg);
	double njnnrbpgalwsdz(bool cvmrftgndqkv, double tivdrdebycbkj, string dzgseeze, double eqsfnqrmagu);
	bool uobllxaexnstbmvv();
	void yjikokgozpauhhfrudmimapm(double hcieiislzqdpiww, bool bklitgzhzyxji, bool yikxbgt, string sjkhmqyprgphubo, double glieqdc, double dnovucypwgqp, double ebixsiu);
	bool mifsngspsqulispuj(double iymoxjgvvtpx, double ygezh, string burvqndqcw, string ystiibaujekws, double qntqqyiyynny, string cvotgindrpls, double qibemzfm);
	bool uckaikdtmqqr(double mlqjetevmwvsif, string gzqyp, bool yvddm, string wsrheatvr, bool ijtvrbin, bool lcglbexkuzxrnhr, string oerxlkvzyt, double ptamgofyvrpqz, bool iyifkvubil);
	bool iervssrqloejeharsmwzgrsnd(bool fgiufohzutpemkl, int mbzxmv, double ehdcptfi, double pseifngpyh, string tixwr, string fbtcxt, int acsnnhxqryjrih);
	void tdpfrgseflrfczulgmdc(string qsxnb, int bloznavszrovndh, string mngeyc, int qwjcorfkfzt, string virpi, int mxrfsmjdaaxxoj, string oqrzycbhl, double kwboifnjcivrxlc, string mfhmlirlyorugih, string jddkxzvka);
	void murrgxitvjzhulzxpkgufeidx(bool bnultuousswmec, bool inwuajfbqpouqa, int kpjwiypwp, int kdoftxz, double nuvzpgbdlb, bool oghjseo, int bxfooftubzxqfo, bool ktrzggnxocsgdqu);

};


void psoqtvo::nrrsoaoycdvumrpgemfwdiaxb(int irgjahogddvv, double sikkkjz, int iwewiphgiauwg) {
	double lyglbwdakr = 11344;
	bool cnudlgy = true;
	bool gadvdc = true;
	int coswm = 53;
	bool tqqnrjzoexzcr = true;
	string jniwdmrunf = "crbihdakzi";
	string dqvms = "zthlzoatlljpwyhrkletgmuxndgnexsltjczkbf";
	string vggzybxwqzcz = "yvmvlmbprloejuypywwd";
	int xabqambub = 3562;
	if (string("crbihdakzi") == string("crbihdakzi")) {
		int qfdaddef;
		for (qfdaddef = 25; qfdaddef > 0; qfdaddef--) {
			continue;
		}
	}

}

double psoqtvo::njnnrbpgalwsdz(bool cvmrftgndqkv, double tivdrdebycbkj, string dzgseeze, double eqsfnqrmagu) {
	int iegmhyurtnbf = 900;
	double ydfok = 56415;
	if (900 == 900) {
		int dbqjecdq;
		for (dbqjecdq = 60; dbqjecdq > 0; dbqjecdq--) {
			continue;
		}
	}
	if (56415 == 56415) {
		int jkei;
		for (jkei = 12; jkei > 0; jkei--) {
			continue;
		}
	}
	if (900 == 900) {
		int qdllhnyiip;
		for (qdllhnyiip = 13; qdllhnyiip > 0; qdllhnyiip--) {
			continue;
		}
	}
	if (900 == 900) {
		int eyzuggxtzy;
		for (eyzuggxtzy = 63; eyzuggxtzy > 0; eyzuggxtzy--) {
			continue;
		}
	}
	if (56415 == 56415) {
		int vdvnczsn;
		for (vdvnczsn = 43; vdvnczsn > 0; vdvnczsn--) {
			continue;
		}
	}
	return 46929;
}

bool psoqtvo::uobllxaexnstbmvv() {
	double qkyprvsouk = 19513;
	double hldbezeex = 6489;
	double icbesctuomp = 35921;
	if (19513 != 19513) {
		int bbcmk;
		for (bbcmk = 46; bbcmk > 0; bbcmk--) {
			continue;
		}
	}
	if (6489 != 6489) {
		int vqzdruc;
		for (vqzdruc = 71; vqzdruc > 0; vqzdruc--) {
			continue;
		}
	}
	if (19513 != 19513) {
		int buwotk;
		for (buwotk = 44; buwotk > 0; buwotk--) {
			continue;
		}
	}
	return true;
}

void psoqtvo::yjikokgozpauhhfrudmimapm(double hcieiislzqdpiww, bool bklitgzhzyxji, bool yikxbgt, string sjkhmqyprgphubo, double glieqdc, double dnovucypwgqp, double ebixsiu) {
	int qtotbswjbgrxpi = 3410;
	double rwixibyskg = 32260;
	double saeizmvfyu = 13755;
	string tiayucf = "msyzpheppzkvpsejkvzb";
	double bxhcgpqtgiu = 15363;
	bool tzmtlujaaqrtn = false;
	bool btoqxtz = true;
	double nbfcviuggkewwtz = 33416;
	int uvfkzmwwldh = 857;
	int iflprojyljhwgu = 5381;

}

bool psoqtvo::mifsngspsqulispuj(double iymoxjgvvtpx, double ygezh, string burvqndqcw, string ystiibaujekws, double qntqqyiyynny, string cvotgindrpls, double qibemzfm) {
	double msdblhv = 24249;
	double gcwael = 36226;
	int durbiarcv = 8775;
	string paqws = "sqwiwxjruzjhhwgyluubfujhduxewippyncmgcffdemhgmdymfdbickficlcnokyytk";
	string wnjzarvlbu = "ranjjtvzzrmdezrvmsrkotdkveqxzjjlmexbs";
	string uburxjjvicmhar = "bvizdnateithorcdjrhzyxuajehclwmchbclfjtngumztjbkhpmmynnfmyxuvesvqb";
	double ntxxu = 41482;
	return true;
}

bool psoqtvo::uckaikdtmqqr(double mlqjetevmwvsif, string gzqyp, bool yvddm, string wsrheatvr, bool ijtvrbin, bool lcglbexkuzxrnhr, string oerxlkvzyt, double ptamgofyvrpqz, bool iyifkvubil) {
	int eujcemlajeew = 2751;
	bool qcnmccx = false;
	string oajqtdoyb = "ldmdtpnhvlbjaegwvynrjbsjxetmpofsmmmkbqtufsoegygffytzrjmsxbhqcllhcnms";
	bool qiyfah = false;
	bool ldalvjkigfq = true;
	if (2751 == 2751) {
		int ftuipqih;
		for (ftuipqih = 56; ftuipqih > 0; ftuipqih--) {
			continue;
		}
	}
	if (2751 == 2751) {
		int eiycnoahf;
		for (eiycnoahf = 11; eiycnoahf > 0; eiycnoahf--) {
			continue;
		}
	}
	if (string("ldmdtpnhvlbjaegwvynrjbsjxetmpofsmmmkbqtufsoegygffytzrjmsxbhqcllhcnms") != string("ldmdtpnhvlbjaegwvynrjbsjxetmpofsmmmkbqtufsoegygffytzrjmsxbhqcllhcnms")) {
		int ni;
		for (ni = 51; ni > 0; ni--) {
			continue;
		}
	}
	return true;
}

bool psoqtvo::iervssrqloejeharsmwzgrsnd(bool fgiufohzutpemkl, int mbzxmv, double ehdcptfi, double pseifngpyh, string tixwr, string fbtcxt, int acsnnhxqryjrih) {
	bool jxjghrccoteyyko = true;
	double fnmatafgsd = 13300;
	int nqqroapsifxn = 349;
	int ncvwqdncq = 809;
	bool azhneghcupxcztd = false;
	bool rlspjziekxktqa = true;
	double fepgy = 21894;
	return false;
}

void psoqtvo::tdpfrgseflrfczulgmdc(string qsxnb, int bloznavszrovndh, string mngeyc, int qwjcorfkfzt, string virpi, int mxrfsmjdaaxxoj, string oqrzycbhl, double kwboifnjcivrxlc, string mfhmlirlyorugih, string jddkxzvka) {
	bool tjqtntrltryodic = false;
	double qavcg = 17716;
	int mtelayazyzltsag = 4197;
	int acfzb = 4374;
	if (17716 != 17716) {
		int xzpigyk;
		for (xzpigyk = 22; xzpigyk > 0; xzpigyk--) {
			continue;
		}
	}
	if (17716 == 17716) {
		int cwldnbs;
		for (cwldnbs = 10; cwldnbs > 0; cwldnbs--) {
			continue;
		}
	}
	if (4374 == 4374) {
		int fiqdb;
		for (fiqdb = 56; fiqdb > 0; fiqdb--) {
			continue;
		}
	}

}

void psoqtvo::murrgxitvjzhulzxpkgufeidx(bool bnultuousswmec, bool inwuajfbqpouqa, int kpjwiypwp, int kdoftxz, double nuvzpgbdlb, bool oghjseo, int bxfooftubzxqfo, bool ktrzggnxocsgdqu) {
	string ysoaxadbwptjjh = "yihdvctmpbh";
	int swgwy = 2006;
	bool rcukplxebpeomk = true;
	string gvemjufhojos = "napuijodhsvftwyxiwprkytlftonxvumzqxpizrbjwmutguppgouyzhvobhss";
	double ykuerrnd = 23258;
	bool nftnrfiqfrrfm = false;
	string cnakjuqavmxjmi = "imqapechxmdmoudrgdzlywcvivcknfnzjfqjhzfgmydvombszotboyimuvsvo";
	string gxfun = "tjvxqoddcachnqeugtnvsrximzsosgxwqihdnehbzx";

}

int psoqtvo::ecohhipibeavgymow(double wcuvamszza, int lebpmoitlsut, double ehfqnginqz) {
	bool jaeqmvihcpvzl = true;
	string dverbouo = "iwavbzjdgacxfixs";
	if (true != true) {
		int akdyvxab;
		for (akdyvxab = 51; akdyvxab > 0; akdyvxab--) {
			continue;
		}
	}
	if (true != true) {
		int wk;
		for (wk = 87; wk > 0; wk--) {
			continue;
		}
	}
	if (string("iwavbzjdgacxfixs") != string("iwavbzjdgacxfixs")) {
		int gsaaesuhd;
		for (gsaaesuhd = 96; gsaaesuhd > 0; gsaaesuhd--) {
			continue;
		}
	}
	if (string("iwavbzjdgacxfixs") == string("iwavbzjdgacxfixs")) {
		int qeupidhla;
		for (qeupidhla = 9; qeupidhla > 0; qeupidhla--) {
			continue;
		}
	}
	if (string("iwavbzjdgacxfixs") == string("iwavbzjdgacxfixs")) {
		int ayvllyep;
		for (ayvllyep = 20; ayvllyep > 0; ayvllyep--) {
			continue;
		}
	}
	return 76453;
}

int psoqtvo::qanrmtlylv(string lchsyureftrlxpl, bool zqsmqlu, double vpcylpzvkkw, string bavqxksa, double bzqda) {
	bool vfypbpctoitgcr = true;
	string qombojt = "qdvefhmofyevyxeujvgldbitegvizcrmynbeilvplducplncg";
	int byyhuxdn = 287;
	int sfopwk = 158;
	string fkmxux = "ghsl";
	double hjhobpdfjiqy = 9366;
	if (true == true) {
		int lmzgpyhequ;
		for (lmzgpyhequ = 9; lmzgpyhequ > 0; lmzgpyhequ--) {
			continue;
		}
	}
	return 66847;
}

bool psoqtvo::ynrhpqznavsfalspwl() {
	string irvbpeqkvjlh = "mloxiityypclopqcnicqdsuodpzgihdgijpdebkxvxtpcugdyeoigqdfqswaowgwhirhefagmgwjp";
	int exoui = 1335;
	string elchjihp = "rbiuxredzligzfpixdhoogzjympoeskfaoln";
	int jkpoo = 4125;
	double aewktnplvdmf = 12582;
	return false;
}

void psoqtvo::ytjlaavrnqdnarleuueg(double bjmcpeqroedkhw, bool iexhc, bool kalaaa, int vcvcwldkmramr, string oszdn) {
	bool hummktuirzimrrx = true;
	int jzjffotnw = 1950;
	double mylyloj = 7275;
	if (1950 == 1950) {
		int xdpwxpg;
		for (xdpwxpg = 82; xdpwxpg > 0; xdpwxpg--) {
			continue;
		}
	}
	if (7275 == 7275) {
		int tmmteronn;
		for (tmmteronn = 56; tmmteronn > 0; tmmteronn--) {
			continue;
		}
	}
	if (1950 == 1950) {
		int joy;
		for (joy = 73; joy > 0; joy--) {
			continue;
		}
	}

}

int psoqtvo::kjyebvwpxon(double eohfknxt, double yabunmzrzclatop, bool glsoboxzu, bool ufrxss, double czilhgofn, double bepnmd, string qjtnkneo, bool agqwsy, bool qnolvthllbvdo, int xvftwpisopk) {
	return 54069;
}

double psoqtvo::ytfdvalibuferri(int wswtejdb, bool ebruvi, string vuphnchnzzt, double szgll, bool mkpnlpoaqyrho, string bekmhdxh) {
	string rianyqfuax = "ybqoqxjslujovqymzlafsaydsrntatlrxuzvrlbzfpqnajiqknrbpnsnlfzmgmgzyhplutmsuqillnofcndmgjpkweywldzoclhc";
	double lsuyyffg = 39195;
	bool jgmalxvlggnm = true;
	bool mxuaplhvvtbzfxc = false;
	int aexfpfw = 5810;
	double dusojzjbmmx = 1108;
	double fhwfxtthk = 77274;
	bool ipuajtybvuagf = true;
	double xtizyghjlvei = 35004;
	int goqkcowsbcemu = 1006;
	if (1006 == 1006) {
		int ewwyxwm;
		for (ewwyxwm = 43; ewwyxwm > 0; ewwyxwm--) {
			continue;
		}
	}
	return 25952;
}

double psoqtvo::zozwsqqsbtkvkgztwqrwp(bool evovrzmnfkmdf, string gnqkjs, string piihj, string yjclcwomyax, int yubxboc, bool wkdngtocmjqsxe, int dcqjhkpg) {
	double hfsqkb = 25391;
	if (25391 != 25391) {
		int whnp;
		for (whnp = 42; whnp > 0; whnp--) {
			continue;
		}
	}
	if (25391 != 25391) {
		int uyeb;
		for (uyeb = 29; uyeb > 0; uyeb--) {
			continue;
		}
	}
	if (25391 == 25391) {
		int pmfzhfi;
		for (pmfzhfi = 23; pmfzhfi > 0; pmfzhfi--) {
			continue;
		}
	}
	if (25391 != 25391) {
		int qbysbhnvi;
		for (qbysbhnvi = 3; qbysbhnvi > 0; qbysbhnvi--) {
			continue;
		}
	}
	if (25391 == 25391) {
		int nmdj;
		for (nmdj = 80; nmdj > 0; nmdj--) {
			continue;
		}
	}
	return 7335;
}

bool psoqtvo::cucjdmdwrqst(string ywdjafpevl, bool sjzym, double pyvpuwygsqpr, int dbeldcdiiyidkw) {
	string wmvsuqmdmzjfu = "xfegxjnfjzbqybfiubqcxjbgtjvysvirbmtmwghgpyqoedxpafofbrqiiohohqqynjerdf";
	int nqotbr = 2334;
	string fumxnhluhcahsc = "rjtyvwornfqozjfyvyhwkcuunuwpbygagxhrxmsjebonxfa";
	bool seyvqm = true;
	bool gowpdvmncogwdvt = false;
	int aurnlnazpuv = 641;
	int nteibha = 387;
	bool rgoqci = true;
	if (false == false) {
		int kms;
		for (kms = 98; kms > 0; kms--) {
			continue;
		}
	}
	return true;
}

int psoqtvo::pxkxhrgvwlwtjwygshsrptk(double mbqdmdw) {
	bool eenyvpma = false;
	string umfpkncglhz = "qctjfnriprygwaiydbtoyjtriflawjyfehtds";
	int fmbnuxbxq = 1939;
	int opgrkkq = 3694;
	bool pketfqjtd = true;
	bool inycsoohus = false;
	bool psfzyzmulxus = true;
	if (true != true) {
		int jqgmyabt;
		for (jqgmyabt = 1; jqgmyabt > 0; jqgmyabt--) {
			continue;
		}
	}
	if (string("qctjfnriprygwaiydbtoyjtriflawjyfehtds") == string("qctjfnriprygwaiydbtoyjtriflawjyfehtds")) {
		int ebk;
		for (ebk = 32; ebk > 0; ebk--) {
			continue;
		}
	}
	if (1939 != 1939) {
		int rjwhbtkcp;
		for (rjwhbtkcp = 62; rjwhbtkcp > 0; rjwhbtkcp--) {
			continue;
		}
	}
	return 22660;
}

void psoqtvo::pamggxqwltlqudvndqz(double tpyvteigandgah, double xiavzwdbk, int dshgxogvuzgaoy, bool jejkjqfeg, int gaatesvj, double otlvz) {
	bool kbggg = false;
	string xluxxujxobxtf = "hznadgglmdpxuvpcvmtomkiafkcjntifgwbkqnzbranrtfgoqdendqehrmvipfvnaseahtpzkgyypufutcuyuk";
	double yqwcvjpkwcvj = 33836;
	int shcexxvqz = 7113;
	string kgrfrlwdxn = "qgvpeygkzsopovtmiovzwexmtqsmw";
	bool rvdnhonaawowo = true;
	int upoznh = 2918;
	bool dgowgkxqzb = false;
	if (7113 != 7113) {
		int amewdx;
		for (amewdx = 10; amewdx > 0; amewdx--) {
			continue;
		}
	}

}

string psoqtvo::yluweoiaxbtvprladrpksiam() {
	double wqqzlivhkgrvek = 62762;
	string fnoyogonmvrjv = "zoflhhkquapohiojmxmvgkrgmzbrqji";
	if (62762 == 62762) {
		int lhoicp;
		for (lhoicp = 38; lhoicp > 0; lhoicp--) {
			continue;
		}
	}
	return string("fnkwbwptgwn");
}

string psoqtvo::djvrlduceprsx(string xsulagtuwpley, double zniqvdmv, double aogauo) {
	double wpdcugp = 24545;
	double btndwvoruygehf = 8567;
	int fyqrhp = 2725;
	double ksltpvpzxoeoj = 11191;
	bool dybgrznltzok = true;
	double pflggnbcyhzj = 37761;
	string tfpdbeaiqcwh = "dgnacfkvgcnhgbjmkxksyotvfbaegefviugk";
	bool dwikxkefeciebza = true;
	double gzzlbsfbzr = 3095;
	if (37761 != 37761) {
		int bebyb;
		for (bebyb = 11; bebyb > 0; bebyb--) {
			continue;
		}
	}
	if (string("dgnacfkvgcnhgbjmkxksyotvfbaegefviugk") == string("dgnacfkvgcnhgbjmkxksyotvfbaegefviugk")) {
		int yzfii;
		for (yzfii = 60; yzfii > 0; yzfii--) {
			continue;
		}
	}
	if (37761 == 37761) {
		int ke;
		for (ke = 4; ke > 0; ke--) {
			continue;
		}
	}
	if (true != true) {
		int otxfx;
		for (otxfx = 23; otxfx > 0; otxfx--) {
			continue;
		}
	}
	if (string("dgnacfkvgcnhgbjmkxksyotvfbaegefviugk") != string("dgnacfkvgcnhgbjmkxksyotvfbaegefviugk")) {
		int aa;
		for (aa = 40; aa > 0; aa--) {
			continue;
		}
	}
	return string("ij");
}

string psoqtvo::vzdvldfzkyt(double mormk, int fcuwfjuqpnyg, int ldqhcpildsif) {
	bool ikrddsj = true;
	int sspuioeaodo = 1295;
	bool laspti = true;
	double hlqqjxzjce = 80265;
	int bpfyjqqbvdpms = 1080;
	string jhiqzttsct = "udfqqzklwulwqwfclkbhydyzhundwyeqlwh";
	if (1295 != 1295) {
		int fu;
		for (fu = 23; fu > 0; fu--) {
			continue;
		}
	}
	return string("nufaqdjb");
}

string psoqtvo::rwkovngdfprloepqmoidclyvs(string axbyuop, int zbsfxbhnj, string htaehvkhmxuls, double zfwjwrdp, bool atrqzzphrxdxupu, bool bomhtbne, bool ubcbgsjq, bool rpmvle, bool rzphswqtroo, double kwpaqnlrrb) {
	return string("zesfxmarx");
}

double psoqtvo::khxfufbnbpenqkxrezb(int vpowh, bool ffahkhgkdt, string gtvvlaeraam, string pdniiwdvxcjk) {
	string eqpmobldaoaa = "zzhaz";
	bool ohopo = false;
	if (false != false) {
		int exzzndzp;
		for (exzzndzp = 6; exzzndzp > 0; exzzndzp--) {
			continue;
		}
	}
	if (false == false) {
		int aakom;
		for (aakom = 1; aakom > 0; aakom--) {
			continue;
		}
	}
	if (false != false) {
		int zjhxjuit;
		for (zjhxjuit = 30; zjhxjuit > 0; zjhxjuit--) {
			continue;
		}
	}
	if (string("zzhaz") == string("zzhaz")) {
		int oxrge;
		for (oxrge = 71; oxrge > 0; oxrge--) {
			continue;
		}
	}
	return 44212;
}

int psoqtvo::devdtauziwnoylhfojgwpigf(int nxyrafsscvgivn) {
	double kjuznmjdpnp = 23801;
	if (23801 != 23801) {
		int wpoornpoi;
		for (wpoornpoi = 0; wpoornpoi > 0; wpoornpoi--) {
			continue;
		}
	}
	if (23801 != 23801) {
		int krzekhjmx;
		for (krzekhjmx = 66; krzekhjmx > 0; krzekhjmx--) {
			continue;
		}
	}
	if (23801 != 23801) {
		int mjsxdfdms;
		for (mjsxdfdms = 2; mjsxdfdms > 0; mjsxdfdms--) {
			continue;
		}
	}
	if (23801 != 23801) {
		int kxfsg;
		for (kxfsg = 29; kxfsg > 0; kxfsg--) {
			continue;
		}
	}
	if (23801 == 23801) {
		int xwnrbf;
		for (xwnrbf = 57; xwnrbf > 0; xwnrbf--) {
			continue;
		}
	}
	return 99774;
}

void psoqtvo::vmbdxlvyflwbnodegb(double bzrijzszn, bool apevqmsrgrvd, double qhutwrbfktqh) {
	int nhrzsubvnkiv = 1075;
	string mjheg = "evjzblcrrbbldh";
	if (1075 == 1075) {
		int ompktyzphj;
		for (ompktyzphj = 4; ompktyzphj > 0; ompktyzphj--) {
			continue;
		}
	}

}

psoqtvo::psoqtvo() {
	this->cucjdmdwrqst(string("xogmjtqkiwprbzztmghkyoarcdiyediynqxjqdrndtpavjqjhqvbccxbxyiehdkxzukcgo"), true, 2197, 1649);
	this->pxkxhrgvwlwtjwygshsrptk(53403);
	this->pamggxqwltlqudvndqz(26485, 35253, 322, false, 409, 54892);
	this->yluweoiaxbtvprladrpksiam();
	this->djvrlduceprsx(string("cbdkwiohtendia"), 33231, 4081);
	this->vzdvldfzkyt(54873, 921, 1341);
	this->rwkovngdfprloepqmoidclyvs(string("ewfwcmho"), 1589, string("nkjumpocpskfliyybmictrytfjprkc"), 15528, true, false, false, false, false, 172);
	this->khxfufbnbpenqkxrezb(1568, true, string("s"), string("djdjqsukfiyhtwvflcmba"));
	this->devdtauziwnoylhfojgwpigf(7115);
	this->vmbdxlvyflwbnodegb(30776, false, 32049);
	this->ecohhipibeavgymow(85159, 4017, 20277);
	this->qanrmtlylv(string("kodbemepumoorsjauqqmzppyuqbmdjtbwfaq"), false, 28616, string("mrkcpcbyvvumdjmpnczr"), 43731);
	this->ynrhpqznavsfalspwl();
	this->ytjlaavrnqdnarleuueg(37528, false, true, 2691, string("tttwaebnnndpoewicihlvzqmckbjrhruhcotnrghkzxrvgehcctmybbfvrepnpnhzbyorrdpxrwjlfdro"));
	this->kjyebvwpxon(1291, 3876, true, false, 6404, 6867, string("gaw"), false, true, 2056);
	this->ytfdvalibuferri(574, true, string("vmcsdijyxbowzfmeedmkyb"), 21616, true, string("hubmzwzxgkropgtchfhqydhcvwjoffftmsnmhuomszemurplfboxylitq"));
	this->zozwsqqsbtkvkgztwqrwp(true, string("xbudefnclienknouqjdgxtsyeextdzvpiowuolrdwosydcgmlcgtjxzepwmolkurcztllfllwpe"), string("jivvcawxrxkjdbwqukoxfqpxxhkyhnxppdqt"), string("ufoygtkdkdndqbzpramojkomlqbywlblaqfqxqndnvqdaabgtavaxmkvqxtzywgikmfiduoloxhoivxkrytbxfjcfhfd"), 9074, true, 3217);
	this->nrrsoaoycdvumrpgemfwdiaxb(5052, 61157, 817);
	this->njnnrbpgalwsdz(true, 2199, string("ucctqgamostjgiuzbvciwpxaofaiahiumgwazrvgpvkbglrxnxhjyfxheosgcgemnofumfuctdocbzltbrmreoyehcrsxzfoc"), 8777);
	this->uobllxaexnstbmvv();
	this->yjikokgozpauhhfrudmimapm(8439, true, true, string("bmjemphaynfqmnzulvxsvrspwqoskwfkiwsfpwytddbpcznsarkfbhtzmnjmjumdxnjgfxnoia"), 22354, 38253, 11369);
	this->mifsngspsqulispuj(35141, 60824, string("bbegujaaqxsuexvawdzwyuieufzubqrlolmfevneudjukgljgaztirmtawzhxgxlvoefcdvznvxciz"), string("asjhtvypsoosufnqbhpcuiutrkieotzlnizzsancxmcpujdyazjxehuqyqyclxoflwkralqxptdkqbmydwkocmnejvkaazhknsu"), 22676, string("aqjhagmlatkticwfzbfoulvstqfsczgyqrfhyfwlplkktayeqtqywluohuldajpgaobtkinptrz"), 55309);
	this->uckaikdtmqqr(36920, string("genviuvatnizhehocehfimygopmllephddjzqeluosx"), false, string("rsggxtqizihgtelmuemxeewohzcqcpcuirjq"), false, true, string("azmsvqsgwwwyarlossrqggjnuxvmphsajmdojowrtpjkznkjeoovusrujjfastryfoixoshphcqhjefsynncbbdap"), 43312, false);
	this->iervssrqloejeharsmwzgrsnd(true, 2640, 16998, 64223, string("ijfbrmqrxrxcnnnvuxbwpfhzjaxaaarikeutouelbxbpiabiarm"), string("pmwzlsxcyagwlygrr"), 3995);
	this->tdpfrgseflrfczulgmdc(string("fixepobvwzdwsnxtdwimsrqkqkuwplaqqkmawsaqyhwfzjhhnisowqshdlercbwnuhpkmarhhlyezshmlr"), 4790, string("mbcebdyxowtt"), 2676, string("qagwbdmrlrernuyvngeibtmisdjzmqyrfxek"), 3355, string("pbiqwoaesaqogoiriollblutgmuzajzjvwei"), 27586, string("aqsqqsfejdjdfjdn"), string("qfhnmkyfnkrbmzedtavjwdmhtihpxeyhtsyymvzsdbiqiwbtftafxiesighpcgkgadjjxkrqpuylfjnzxydruc"));
	this->murrgxitvjzhulzxpkgufeidx(true, true, 2332, 124, 76738, true, 4701, true);
}
