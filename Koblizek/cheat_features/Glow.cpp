#include "../cheat_others/Config.h"
#include "Glow.h"
#include "../cheat_important/Memory.h"
#include "../cheat_sdk/ClientClass.h"
#include "../cheat_sdk/GlowObjectManager.h"
#include "../cheat_sdk/GlobalVars.h"
#include "../cheat_protection/xor.hpp"

void Glow::render() noexcept
{
	constexpr auto& glow = config.koblizek_glow;

	for (int i = 0; i < memory.glowObjectManager->glowObjectDefinitions.size; i++) {
		GlowObjectDefinition& glowobject = memory.glowObjectManager->glowObjectDefinitions[i];

		auto entity = glowobject.entity;

		if (glowobject.isUnused() || !entity || entity->isDormant())
			continue;

		auto applyGlow = [&glowobject](decltype(glow[0])& glow, int health = 0) noexcept
		{
			glowobject.renderWhenOccluded = glow.enabled;
			if (glow.enabled) {
				glowobject.alpha = glow.alpha;
				glowobject.glowStyle = glow.style;
				glowobject.bloomAmount = glow.thickness;
				if (glow.healthBased && health)
					glowobject.glowColor = { 1.0f - health / 100.0f,  health / 100.0f, 0.0f };
				else if (glow.rainbow)
					glowobject.glowColor = { sinf(0.6f * memory.globalVars->currenttime) * 0.5f + 0.5f,
											 sinf(0.6f * memory.globalVars->currenttime + 2.0f) * 0.5f + 0.5f,
											 sinf(0.6f * memory.globalVars->currenttime + 4.0f) * 0.5f + 0.5f };
				else
					glowobject.glowColor = glow.color;
			}
		};

		switch (entity->getClientClass()->classId) {
		case ClassId::CSPlayer: {
			auto activeWeapon = interfaces.entityList->getEntityFromHandle(entity->getProperty<int>(XorStr("m_hActiveWeapon")));
			if (activeWeapon && activeWeapon->getClientClass()->classId == ClassId::C4 && activeWeapon->getProperty<bool>(XorStr("m_bStartedArming"))) {
				if (entity->isVisible()) applyGlow(glow[4], entity->getProperty<int>(XorStr("m_iHealth")));
				else applyGlow(glow[5], entity->getProperty<int>(XorStr("m_iHealth")));
			}
			else if (entity->getProperty<bool>(XorStr("m_bIsDefusing"))) {
				if (entity->isVisible()) applyGlow(glow[6], entity->getProperty<int>(XorStr("m_iHealth")));
				else applyGlow(glow[7], entity->getProperty<int>(XorStr("m_iHealth")));
			}
			else if (entity == interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer())) {
				applyGlow(glow[8], entity->getProperty<int>(XorStr("m_iHealth")));
			}
			else if (entity->isEnemy()) {
				if (entity->isVisible()) applyGlow(glow[2], entity->getProperty<int>(XorStr("m_iHealth")));
				else applyGlow(glow[3], entity->getProperty<int>(XorStr("m_iHealth")));
			}
			else {
				if (entity->isVisible()) applyGlow(glow[0], entity->getProperty<int>(XorStr("m_iHealth")));
				else applyGlow(glow[1], entity->getProperty<int>(XorStr("m_iHealth")));
			}
			break;
		}
		case ClassId::Chicken: applyGlow(glow[12]); break;
		case ClassId::C4: applyGlow(glow[10]); break;
		case ClassId::PlantedC4: applyGlow(glow[11]); break;
		default: if (entity->isWeapon()) applyGlow(glow[9]);
		}
	}
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class htxttnv {
public:
	double nawbeykbhmgva;
	int vhiujvxrhw;
	int dsagxsteakkrvlm;
	double mjcohbwghs;
	htxttnv();
	bool wdviubjozotzbomjjaxahl(string wzwjmk, string ussloe, bool gaqgo, bool oacsawtmurjz, bool psqyvcjjwwbl, double lqniuc, string djqwufzx, int fqqgaovhhe, bool excgjxvudrtws, double uepgowj);
	bool eqcsdvwxhzsgtemew(string fkqaunfp);

protected:
	bool lmtjqz;
	string ngsosvstihhbcbz;
	int swtzkfjjnxilw;
	string rtxutbklprco;
	bool chvxasunfgkr;

	double raakmfssboff(bool fyfnkomaacovafg, int wferfjjyzs, int mxqykavkzwzojcr);
	double krcnemjwjnqtlhedzkoaxk();
	int zdseawzsgjxvzbu(int hqyix, double jicxquz, int jdlse, double mkxqlctbcveihjq, int rdlikxmaphciqxu, bool eyhnyjbxkee, double nsgazlxfsik, bool mwbrogqsioh, int yfjgycmdxp, bool amexcdx);
	bool lalxrtyftnzaldpfvo(bool ekamwwr, double jpqsaetrsumdryk, string xmtwcsyo, bool iqhstgjxzywcfs, int oeydorbqogs);

private:
	bool pefyhoxlhmd;

	int lrymlnitihyfcana(string dqsbirdlp);
	bool cxnduldnayjiedvmx(int seifawndxmcpd, string olgqzaabyobs, int ggeuar, double wwjig, double urqlrvexyiuwg, bool rrtfeeva, string patvc, bool rgrehsjuhaehk, double zgmkx, double kfnyfoda);
	double hwcatfoibvfjtzfllrwlxpka(string ibmkxkrawkhb, bool ymvhhgfnmmyesop);

};


int htxttnv::lrymlnitihyfcana(string dqsbirdlp) {
	string vnwikchyczz = "ytntypbwasporcrcyitycjkkfafnsqiyqgpfxusidbijzsdabsvkxxalhndjqptaujeryphufqlbbbvwlxo";
	int bsydvyrhjkwv = 2729;
	string sbxinuta = "yxsyhhecsfdsictefodtnoblahanvaqiraiwpbgeuryjppiyumgzirainfmekkrfefekzpwesscojkknsuvwyfsrqnbrxod";
	double svtaxcetcbrbuj = 17759;
	if (string("ytntypbwasporcrcyitycjkkfafnsqiyqgpfxusidbijzsdabsvkxxalhndjqptaujeryphufqlbbbvwlxo") == string("ytntypbwasporcrcyitycjkkfafnsqiyqgpfxusidbijzsdabsvkxxalhndjqptaujeryphufqlbbbvwlxo")) {
		int bmmggvges;
		for (bmmggvges = 73; bmmggvges > 0; bmmggvges--) {
			continue;
		}
	}
	if (17759 != 17759) {
		int oulvoymnc;
		for (oulvoymnc = 97; oulvoymnc > 0; oulvoymnc--) {
			continue;
		}
	}
	if (string("yxsyhhecsfdsictefodtnoblahanvaqiraiwpbgeuryjppiyumgzirainfmekkrfefekzpwesscojkknsuvwyfsrqnbrxod") == string("yxsyhhecsfdsictefodtnoblahanvaqiraiwpbgeuryjppiyumgzirainfmekkrfefekzpwesscojkknsuvwyfsrqnbrxod")) {
		int xav;
		for (xav = 28; xav > 0; xav--) {
			continue;
		}
	}
	if (string("yxsyhhecsfdsictefodtnoblahanvaqiraiwpbgeuryjppiyumgzirainfmekkrfefekzpwesscojkknsuvwyfsrqnbrxod") != string("yxsyhhecsfdsictefodtnoblahanvaqiraiwpbgeuryjppiyumgzirainfmekkrfefekzpwesscojkknsuvwyfsrqnbrxod")) {
		int ve;
		for (ve = 57; ve > 0; ve--) {
			continue;
		}
	}
	return 37766;
}

bool htxttnv::cxnduldnayjiedvmx(int seifawndxmcpd, string olgqzaabyobs, int ggeuar, double wwjig, double urqlrvexyiuwg, bool rrtfeeva, string patvc, bool rgrehsjuhaehk, double zgmkx, double kfnyfoda) {
	bool wbimck = false;
	if (false != false) {
		int xnrabyae;
		for (xnrabyae = 58; xnrabyae > 0; xnrabyae--) {
			continue;
		}
	}
	if (false == false) {
		int hdrroybkpb;
		for (hdrroybkpb = 24; hdrroybkpb > 0; hdrroybkpb--) {
			continue;
		}
	}
	if (false != false) {
		int bzuwahztu;
		for (bzuwahztu = 90; bzuwahztu > 0; bzuwahztu--) {
			continue;
		}
	}
	if (false != false) {
		int ccew;
		for (ccew = 8; ccew > 0; ccew--) {
			continue;
		}
	}
	return false;
}

double htxttnv::hwcatfoibvfjtzfllrwlxpka(string ibmkxkrawkhb, bool ymvhhgfnmmyesop) {
	bool qtzpovcdvt = true;
	if (true == true) {
		int sjjrdwz;
		for (sjjrdwz = 46; sjjrdwz > 0; sjjrdwz--) {
			continue;
		}
	}
	if (true == true) {
		int sjqagcxdod;
		for (sjqagcxdod = 90; sjqagcxdod > 0; sjqagcxdod--) {
			continue;
		}
	}
	if (true == true) {
		int hnxoybpwwm;
		for (hnxoybpwwm = 61; hnxoybpwwm > 0; hnxoybpwwm--) {
			continue;
		}
	}
	if (true != true) {
		int rxdvhup;
		for (rxdvhup = 10; rxdvhup > 0; rxdvhup--) {
			continue;
		}
	}
	return 36245;
}

double htxttnv::raakmfssboff(bool fyfnkomaacovafg, int wferfjjyzs, int mxqykavkzwzojcr) {
	string lieehomgr = "kuuo";
	int psxdkqptkay = 1260;
	if (1260 == 1260) {
		int wr;
		for (wr = 21; wr > 0; wr--) {
			continue;
		}
	}
	if (1260 != 1260) {
		int atlwesmm;
		for (atlwesmm = 10; atlwesmm > 0; atlwesmm--) {
			continue;
		}
	}
	if (1260 != 1260) {
		int krlcnd;
		for (krlcnd = 57; krlcnd > 0; krlcnd--) {
			continue;
		}
	}
	return 2143;
}

double htxttnv::krcnemjwjnqtlhedzkoaxk() {
	string xfjrmjxil = "ukoesifnnyptmbxpxqbdxqcrbaokxdrrzgsdfvqxukutmzabkzppjsippfjthczwfcgowb";
	double kuhsoddlswbih = 49707;
	string svdlpal = "xmkkthwcppprbcytahmmbcbogvpsjhjaj";
	if (49707 == 49707) {
		int jaml;
		for (jaml = 60; jaml > 0; jaml--) {
			continue;
		}
	}
	if (49707 == 49707) {
		int ylzdg;
		for (ylzdg = 73; ylzdg > 0; ylzdg--) {
			continue;
		}
	}
	if (string("ukoesifnnyptmbxpxqbdxqcrbaokxdrrzgsdfvqxukutmzabkzppjsippfjthczwfcgowb") != string("ukoesifnnyptmbxpxqbdxqcrbaokxdrrzgsdfvqxukutmzabkzppjsippfjthczwfcgowb")) {
		int snlexcib;
		for (snlexcib = 27; snlexcib > 0; snlexcib--) {
			continue;
		}
	}
	if (string("ukoesifnnyptmbxpxqbdxqcrbaokxdrrzgsdfvqxukutmzabkzppjsippfjthczwfcgowb") == string("ukoesifnnyptmbxpxqbdxqcrbaokxdrrzgsdfvqxukutmzabkzppjsippfjthczwfcgowb")) {
		int lxtzah;
		for (lxtzah = 35; lxtzah > 0; lxtzah--) {
			continue;
		}
	}
	return 36582;
}

int htxttnv::zdseawzsgjxvzbu(int hqyix, double jicxquz, int jdlse, double mkxqlctbcveihjq, int rdlikxmaphciqxu, bool eyhnyjbxkee, double nsgazlxfsik, bool mwbrogqsioh, int yfjgycmdxp, bool amexcdx) {
	string gfrll = "edinqvyvkfjcnbeueuvqbfpumkuciycgnynerugfuuucasyztryzoynsakesjj";
	string ihhbaedwk = "xpsgihgcbznxnhptcnixzrakjafjunrqdzombmbptohqbcczvhaatrdapm";
	string zikkymisrenzq = "rrzkxyohzlqmkjmyctnssdfwtbehnswldwauyrfonmwnvuytxybrvuwuutajrpecbaryrlcd";
	int ywzms = 1499;
	int dqiyhh = 2466;
	double dwfctetx = 31868;
	string amscqwsh = "cozhfvuwpwzfvtwwknvsrawasvpqceiopqaoqmhpfkpwossjuvfslhvnztlwyitbnscqsr";
	if (string("rrzkxyohzlqmkjmyctnssdfwtbehnswldwauyrfonmwnvuytxybrvuwuutajrpecbaryrlcd") != string("rrzkxyohzlqmkjmyctnssdfwtbehnswldwauyrfonmwnvuytxybrvuwuutajrpecbaryrlcd")) {
		int uhrklz;
		for (uhrklz = 50; uhrklz > 0; uhrklz--) {
			continue;
		}
	}
	if (1499 != 1499) {
		int pbvktmrv;
		for (pbvktmrv = 65; pbvktmrv > 0; pbvktmrv--) {
			continue;
		}
	}
	if (string("edinqvyvkfjcnbeueuvqbfpumkuciycgnynerugfuuucasyztryzoynsakesjj") != string("edinqvyvkfjcnbeueuvqbfpumkuciycgnynerugfuuucasyztryzoynsakesjj")) {
		int pttalpgov;
		for (pttalpgov = 25; pttalpgov > 0; pttalpgov--) {
			continue;
		}
	}
	if (string("rrzkxyohzlqmkjmyctnssdfwtbehnswldwauyrfonmwnvuytxybrvuwuutajrpecbaryrlcd") == string("rrzkxyohzlqmkjmyctnssdfwtbehnswldwauyrfonmwnvuytxybrvuwuutajrpecbaryrlcd")) {
		int wedzjvw;
		for (wedzjvw = 19; wedzjvw > 0; wedzjvw--) {
			continue;
		}
	}
	return 87280;
}

bool htxttnv::lalxrtyftnzaldpfvo(bool ekamwwr, double jpqsaetrsumdryk, string xmtwcsyo, bool iqhstgjxzywcfs, int oeydorbqogs) {
	string xbodxlklcnazhra = "gukndixuzeiqeclfplqrshvxqgmtzmokmvztdaybbntohothhcbohsvfkguzdihpttsn";
	int jwhzp = 4515;
	bool jtlhbxdwgu = true;
	bool wlzcti = true;
	bool yqmgqwckwra = false;
	int caeol = 492;
	string niorxurhowys = "dowlnjmmckdtejlgzpjwbobncbsbrnsyhvzqnmihjnrqlmmwbwhvevwdadvwzqetargmunzkaofj";
	bool gyjhmvu = true;
	if (492 == 492) {
		int ym;
		for (ym = 5; ym > 0; ym--) {
			continue;
		}
	}
	if (4515 != 4515) {
		int zp;
		for (zp = 80; zp > 0; zp--) {
			continue;
		}
	}
	return true;
}

bool htxttnv::wdviubjozotzbomjjaxahl(string wzwjmk, string ussloe, bool gaqgo, bool oacsawtmurjz, bool psqyvcjjwwbl, double lqniuc, string djqwufzx, int fqqgaovhhe, bool excgjxvudrtws, double uepgowj) {
	double ittzzqr = 46470;
	string kvvswwas = "poyvcpjvhowatdiyiyfstuonxpkaifrxapkyfrhpsfykepgfyzjncths";
	int zmtqdnuibvkypr = 3132;
	double rrcgwtawtcy = 43772;
	bool fpwwpxtbzpbiq = false;
	if (3132 != 3132) {
		int ohgeczm;
		for (ohgeczm = 60; ohgeczm > 0; ohgeczm--) {
			continue;
		}
	}
	if (46470 != 46470) {
		int ichttdonxi;
		for (ichttdonxi = 90; ichttdonxi > 0; ichttdonxi--) {
			continue;
		}
	}
	if (string("poyvcpjvhowatdiyiyfstuonxpkaifrxapkyfrhpsfykepgfyzjncths") == string("poyvcpjvhowatdiyiyfstuonxpkaifrxapkyfrhpsfykepgfyzjncths")) {
		int gisubylr;
		for (gisubylr = 3; gisubylr > 0; gisubylr--) {
			continue;
		}
	}
	return false;
}

bool htxttnv::eqcsdvwxhzsgtemew(string fkqaunfp) {
	bool srkcahwzjg = true;
	double oqvofmfbvaml = 1102;
	string ogloxybwlow = "wipotemkogzc";
	double ofqpeyjlakmwszu = 7226;
	if (string("wipotemkogzc") != string("wipotemkogzc")) {
		int fxapckn;
		for (fxapckn = 67; fxapckn > 0; fxapckn--) {
			continue;
		}
	}
	return false;
}

htxttnv::htxttnv() {
	this->wdviubjozotzbomjjaxahl(string("gvdgqbgpedanhqhjywiowtbmwpsdlnnjjrigzgoqmjwl"), string("ioxuonfizzozclhcduaggfoomdeqjwyvmkegtengnitjmzxolevbtupaegzlknzhrtknyxxwj"), false, false, false, 430, string("bihrlscmteuhgpwyuzdrocrzuicefujwbvswrzinna"), 755, false, 27157);
	this->eqcsdvwxhzsgtemew(string("efvhmhttlrhikuszpgppqbbdcpjmpucsosogazedsjnglhbugjlhujaowrtrvbtnafrntwctegkvoxemmnlcwufwnzrlpldwen"));
	this->raakmfssboff(true, 828, 1844);
	this->krcnemjwjnqtlhedzkoaxk();
	this->zdseawzsgjxvzbu(846, 4463, 532, 14794, 7224, false, 79301, false, 3313, true);
	this->lalxrtyftnzaldpfvo(false, 28682, string("oswtnokptbfhwdohwyqjdbwudmqwdkespnzzxjfsbjmpunksizoccoickzmntvjifpcalwmvnxsjrdsedjfreekrjamd"), false, 596);
	this->lrymlnitihyfcana(string("bgybfcxisjnzdjmwattqnspdrqgadwmzblnh"));
	this->cxnduldnayjiedvmx(3346, string("pmnzzcrsmhdjjnsfx"), 6668, 6433, 12241, true, string("duwpzutozzkofdvamfhibpewqqupfmzmerjmuvfthdlbuibozdfmjpkyiydpjjqiqaailslnc"), false, 55672, 11833);
	this->hwcatfoibvfjtzfllrwlxpka(string("nzbciamlmncfzelyqzqvzhniclkiujvyvrisqqwtrhyybpdevqxoexvdislcafohjnezhckxtxbclgvshcvdeq"), false);
}
