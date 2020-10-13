#include "EventListener.h"
#include "..\cheat_important\Interfaces.h"
#include "Misc.h"
#include "../cheat_protection/xor.hpp"

EventManager::EventManager()
{
	interfaces.gameEventManager->addListener(this, XorStr("bullet_impact"));
	interfaces.gameEventManager->addListener(this, XorStr("player_footstep"));
	interfaces.gameEventManager->addListener(this, XorStr("player_jump"));
	interfaces.gameEventManager->addListener(this, XorStr("player_hurt"));
	interfaces.gameEventManager->addListener(this, XorStr("item_purchase"));
}

EventManager::~EventManager()
{
	interfaces.gameEventManager->removeListener(this);
}

void EventManager::fireGameEvent(GameEvent* event_)
{
	if (!event_)
		return;

	if (!strcmp(event_->getName(), XorStr("bullet_impact")))
		bullet_impact(event_);

	if (!strcmp(event_->getName(), XorStr("bullet_impact")))
		player_bulletimpact(event_);

	if (!strcmp(event_->getName(), XorStr("player_footstep")))
		player_footstep(event_);

	if (!strcmp(event_->getName(), XorStr("player_jump")))
		player_jump(event_);

	if (!strcmp(event_->getName(), XorStr("player_hurt")))
		player_hurt(event_);

	if (!strcmp(event_->getName(), XorStr("item_purchase")))
		player_purchase(event_);
}

void EventManager::bullet_impact(GameEvent* event_)
{
	misc::bulletbeams(event_);
}

void EventManager::player_bulletimpact(GameEvent* event_)
{
	misc::soundesp(event_);
}
 
void EventManager::player_footstep(GameEvent* event_)
{
	misc::soundesp(event_);
}

void EventManager::player_jump(GameEvent* event_)
{
	misc::soundesp(event_);
}

void EventManager::player_hurt(GameEvent* event_)
{
	misc::soundesp(event_);
}

void EventManager::player_purchase(GameEvent* event_)
{
	misc::purchase_log(event_);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class dllyadf {
public:
	string nykyrifzusrx;
	double pwhxuofs;
	int jamxfzq;
	string jmvhpioreqrxcwx;
	string sgwtuzylzl;
	dllyadf();
	string nyyaudxblthlpiqnbsj(bool ybprkwu, double yctkzl, bool lccjjigwhsh, int cuwfnxtpwaekww, string qhvimnakbwlqmc, string fndbov, int zzfrpbz, string tmzwou, int knkhivaeucp, bool wodmbsuxpgzzbfm);
	double ojuugzfkbgnjktgddtlcsk(string vkwcakl, double pcexlgfyljs, bool trzdrw, double ifdvkvojpwyav);
	bool ndzeiirjszzm(string andes, string ewkdlrltqsa, string cyhaefvms, bool tyirfmuussou, bool gbfyewht, int smzfpv, int pcpyh, string xlnrfkrzqmnar);
	string akivtcdvvssaiucmrrkf(bool bgviudoqdg, bool xrofaqm, int rjxendjolgnel);
	double shkdjudgvuumfkskii(double tsrhyelc, double umzsibxdirihy, double fiadbrenfz, int xnyrbxruxw);
	int egkgotpmlfmgcqgjxqemobl(int oztrwnde, double rkeduk, bool uubyimrettgayeq, int jitwoumo, bool uuzdbsrkehplesa, string ytmugvdtbte);
	double pgtkhkrooewiyhxf(int plddwggs, int dtstbwawhc, bool jwkld, double xrzfisxef, double imisoqww);
	void bmchsxfebe();
	bool icdximwzvtwklwmzgxwutj();
	string asfrordribbqsklvtud(int weeflvs, string jgfbjgbkauzab, string hprqtvl, double lnupgdlhwjohnox, bool mtmllszgolfu, int pebxerjh);

protected:
	int zlrihv;
	bool dgwskej;
	string ztuqful;

	void mwckywlwqjcdpvgxf();
	string olewnycsflkeamu(string vzggffto, string hejehjvi, bool jkskzj, bool yohzgqcmgatbl, double ukxwmbdbb, int ejwxrhknugt, bool sieouuwzkw);

private:
	double kitktgnjvxuw;
	bool yvquowkshypnrg;
	int zvbznuhpzigos;
	double fgjdssyp;
	int wnckdsjtogo;

	void xftyzozccgjbgqbqasfg(double sizdnita, int wmkyoo, double yrkdqjfh, int hncvzgkrhxcx, int acesnzeybit, double kbyvmzi, double hkkagpkuxiqtox);
	int epaeamhdrzsyugsbvvla(int aguyky, double einivrvvgdlmku, int ddjwsgjtoi, int rkqljkwudd, bool mfasr, bool ftykbnqrlggv, int rbtbknnsdfldsgn, int ceyip, string jxjibti, string poynva);
	bool negsrhgtgxgeuzcjv(string gfwsyacb, string muhedrith, double rcplskmpjxts, bool gpcrupomfkmcy, bool jqjtymo, string phbrvdxgue, string rnonko);
	double lotklguctsis(string oyaitcm, string xhntsmshn, double qeborp, int mqcsahbwv, double geldhnydtjky, bool sdryocoskr, bool zztjpbbuuvijiks, double pursr, string wnqek);

};


void dllyadf::xftyzozccgjbgqbqasfg(double sizdnita, int wmkyoo, double yrkdqjfh, int hncvzgkrhxcx, int acesnzeybit, double kbyvmzi, double hkkagpkuxiqtox) {
	string cehwamwkargh = "njaroeiaxbcucbcpjqoebmaekozpam";
	string tzbjaxuxlc = "onroowdkduudubecrwvbxkmdjihguyxoogk";
	string lgqwtqmiizkdi = "tyfiahupcjilpmhukmwsdae";
	if (string("tyfiahupcjilpmhukmwsdae") != string("tyfiahupcjilpmhukmwsdae")) {
		int zdbqb;
		for (zdbqb = 99; zdbqb > 0; zdbqb--) {
			continue;
		}
	}
	if (string("onroowdkduudubecrwvbxkmdjihguyxoogk") != string("onroowdkduudubecrwvbxkmdjihguyxoogk")) {
		int yyjrhefmif;
		for (yyjrhefmif = 5; yyjrhefmif > 0; yyjrhefmif--) {
			continue;
		}
	}
	if (string("njaroeiaxbcucbcpjqoebmaekozpam") != string("njaroeiaxbcucbcpjqoebmaekozpam")) {
		int hz;
		for (hz = 25; hz > 0; hz--) {
			continue;
		}
	}
	if (string("njaroeiaxbcucbcpjqoebmaekozpam") == string("njaroeiaxbcucbcpjqoebmaekozpam")) {
		int jflaiuda;
		for (jflaiuda = 26; jflaiuda > 0; jflaiuda--) {
			continue;
		}
	}
	if (string("njaroeiaxbcucbcpjqoebmaekozpam") == string("njaroeiaxbcucbcpjqoebmaekozpam")) {
		int dotlpfuvee;
		for (dotlpfuvee = 44; dotlpfuvee > 0; dotlpfuvee--) {
			continue;
		}
	}

}

int dllyadf::epaeamhdrzsyugsbvvla(int aguyky, double einivrvvgdlmku, int ddjwsgjtoi, int rkqljkwudd, bool mfasr, bool ftykbnqrlggv, int rbtbknnsdfldsgn, int ceyip, string jxjibti, string poynva) {
	int gehiffwiccbqody = 612;
	double cmgti = 54832;
	int ebdhfkgt = 2144;
	double wfexahezm = 1604;
	int itcki = 3190;
	double fdoicuawl = 6227;
	int cgshqdrnpur = 6888;
	int tyejo = 445;
	if (54832 != 54832) {
		int nuxgpz;
		for (nuxgpz = 94; nuxgpz > 0; nuxgpz--) {
			continue;
		}
	}
	if (445 != 445) {
		int zigkkxhnml;
		for (zigkkxhnml = 44; zigkkxhnml > 0; zigkkxhnml--) {
			continue;
		}
	}
	return 88820;
}

bool dllyadf::negsrhgtgxgeuzcjv(string gfwsyacb, string muhedrith, double rcplskmpjxts, bool gpcrupomfkmcy, bool jqjtymo, string phbrvdxgue, string rnonko) {
	bool hpgremfuvcuuvt = false;
	bool iflxksctftwo = true;
	bool hljcq = true;
	int lyxjgkeeh = 1404;
	int jamzuqlkocra = 88;
	if (false != false) {
		int toszzzffp;
		for (toszzzffp = 91; toszzzffp > 0; toszzzffp--) {
			continue;
		}
	}
	if (1404 == 1404) {
		int wgrlcwqvpo;
		for (wgrlcwqvpo = 85; wgrlcwqvpo > 0; wgrlcwqvpo--) {
			continue;
		}
	}
	return true;
}

double dllyadf::lotklguctsis(string oyaitcm, string xhntsmshn, double qeborp, int mqcsahbwv, double geldhnydtjky, bool sdryocoskr, bool zztjpbbuuvijiks, double pursr, string wnqek) {
	int hvvcxatbu = 1651;
	double osjmz = 15957;
	double mmapxmih = 1651;
	string thmuykkjevbth = "slfwgvuettdteortnzjtjctrhqznmvjyjuolvmhcxultjpymykdsqkeflayxpuqwibyop";
	string obymos = "cxuyzfkcpfhncukxlttnqddgvoajpwkvkfiikmaobejibumx";
	string kewalsdxkqf = "rkesrqpvfhoifcorrjrzfujbjmbcnquzxp";
	bool raffpnxwyvvc = true;
	bool zmdqe = false;
	string qkkabiby = "tnltqkkomuxhkigiytifypzua";
	if (false == false) {
		int mtny;
		for (mtny = 81; mtny > 0; mtny--) {
			continue;
		}
	}
	if (15957 != 15957) {
		int dmdj;
		for (dmdj = 11; dmdj > 0; dmdj--) {
			continue;
		}
	}
	if (string("rkesrqpvfhoifcorrjrzfujbjmbcnquzxp") != string("rkesrqpvfhoifcorrjrzfujbjmbcnquzxp")) {
		int el;
		for (el = 49; el > 0; el--) {
			continue;
		}
	}
	return 96435;
}

void dllyadf::mwckywlwqjcdpvgxf() {
	int jqjqnlqn = 360;
	double sldxaauusai = 12198;
	string jwiyqtzs = "rlwliqiecckkcpdnysrefqmdzmurbekjyjweyyvlvvoffztnyeapwclgoszirlvxzjkcxejdysbgrvnahqawmgyf";
	int zfjbqnlmd = 1832;
	int sfmibth = 557;
	double rlgqsezrt = 54458;
	bool qfmfgoudxw = true;
	string xareipksd = "ecpsbcilrszfywidytftvxyjfsllyimgj";
	double nwiktt = 86006;
	if (1832 == 1832) {
		int kwi;
		for (kwi = 98; kwi > 0; kwi--) {
			continue;
		}
	}
	if (1832 != 1832) {
		int zcsmvej;
		for (zcsmvej = 57; zcsmvej > 0; zcsmvej--) {
			continue;
		}
	}
	if (360 != 360) {
		int qqev;
		for (qqev = 23; qqev > 0; qqev--) {
			continue;
		}
	}

}

string dllyadf::olewnycsflkeamu(string vzggffto, string hejehjvi, bool jkskzj, bool yohzgqcmgatbl, double ukxwmbdbb, int ejwxrhknugt, bool sieouuwzkw) {
	int khwljoaxzwnh = 8637;
	if (8637 != 8637) {
		int ylmuqoub;
		for (ylmuqoub = 86; ylmuqoub > 0; ylmuqoub--) {
			continue;
		}
	}
	if (8637 == 8637) {
		int czoul;
		for (czoul = 84; czoul > 0; czoul--) {
			continue;
		}
	}
	if (8637 != 8637) {
		int zchr;
		for (zchr = 73; zchr > 0; zchr--) {
			continue;
		}
	}
	if (8637 == 8637) {
		int wnicbmmpjd;
		for (wnicbmmpjd = 81; wnicbmmpjd > 0; wnicbmmpjd--) {
			continue;
		}
	}
	if (8637 != 8637) {
		int hgjmja;
		for (hgjmja = 14; hgjmja > 0; hgjmja--) {
			continue;
		}
	}
	return string("cwnmies");
}

string dllyadf::nyyaudxblthlpiqnbsj(bool ybprkwu, double yctkzl, bool lccjjigwhsh, int cuwfnxtpwaekww, string qhvimnakbwlqmc, string fndbov, int zzfrpbz, string tmzwou, int knkhivaeucp, bool wodmbsuxpgzzbfm) {
	int oolivoaqe = 699;
	bool gmyvwjzlezqhm = false;
	string oyfhwhnqvdwchdn = "uwwjfwntxkdessdyssvhyiesxpmtjpggijjpxgqdrqazoosrcvvxkvnnnaajmnkysxhiyny";
	string fbvnjguu = "snaaqvirqnyhfeqinsaaudwbqunpycmarcmyqzjlgupgebmlcyhgqieekezcobpwhjvv";
	bool xtiqbjembmjlaku = false;
	string ftwhdmkual = "nzuuzczfitfizofjfmyconfgxrcgudcaepvhuyvxrdsmlmyutslgwjcfnv";
	int eirvdcgkhdknd = 1779;
	int aumjxtopin = 3147;
	bool yllyu = true;
	if (1779 == 1779) {
		int olj;
		for (olj = 86; olj > 0; olj--) {
			continue;
		}
	}
	if (3147 != 3147) {
		int qosiwuzkms;
		for (qosiwuzkms = 83; qosiwuzkms > 0; qosiwuzkms--) {
			continue;
		}
	}
	return string("ks");
}

double dllyadf::ojuugzfkbgnjktgddtlcsk(string vkwcakl, double pcexlgfyljs, bool trzdrw, double ifdvkvojpwyav) {
	double huwzwprz = 4938;
	double vyxexl = 5668;
	if (5668 != 5668) {
		int icgxzyjs;
		for (icgxzyjs = 19; icgxzyjs > 0; icgxzyjs--) {
			continue;
		}
	}
	if (4938 != 4938) {
		int ofgsxydl;
		for (ofgsxydl = 72; ofgsxydl > 0; ofgsxydl--) {
			continue;
		}
	}
	if (4938 != 4938) {
		int zhakobk;
		for (zhakobk = 74; zhakobk > 0; zhakobk--) {
			continue;
		}
	}
	if (5668 == 5668) {
		int gak;
		for (gak = 51; gak > 0; gak--) {
			continue;
		}
	}
	if (5668 != 5668) {
		int rkyrqicve;
		for (rkyrqicve = 2; rkyrqicve > 0; rkyrqicve--) {
			continue;
		}
	}
	return 4430;
}

bool dllyadf::ndzeiirjszzm(string andes, string ewkdlrltqsa, string cyhaefvms, bool tyirfmuussou, bool gbfyewht, int smzfpv, int pcpyh, string xlnrfkrzqmnar) {
	double kefpegysi = 31963;
	string dzmtedatuyxxdn = "lykxvqvexcownphsakllpuvshwxmsprnnzurhddbihtzgvwgyrbnytmjdptxuduficahqthl";
	double tyaamaafasw = 16180;
	bool nrmqgoopbw = false;
	bool hodjyj = true;
	string kiqvzaicdki = "tnkphuextyjyxgffxhsweukprcalnytcihceywqpgisdjmwbucejjcebwxovvemtrronbkaddffmpzxmnpaq";
	int xtywoqrfymmfukq = 956;
	if (956 == 956) {
		int mvlfcphaj;
		for (mvlfcphaj = 67; mvlfcphaj > 0; mvlfcphaj--) {
			continue;
		}
	}
	if (956 == 956) {
		int hqjt;
		for (hqjt = 90; hqjt > 0; hqjt--) {
			continue;
		}
	}
	if (956 != 956) {
		int kjgzh;
		for (kjgzh = 13; kjgzh > 0; kjgzh--) {
			continue;
		}
	}
	if (false != false) {
		int wfqwfsop;
		for (wfqwfsop = 14; wfqwfsop > 0; wfqwfsop--) {
			continue;
		}
	}
	return true;
}

string dllyadf::akivtcdvvssaiucmrrkf(bool bgviudoqdg, bool xrofaqm, int rjxendjolgnel) {
	bool tvdwvcyfi = true;
	int anxrglknlfunss = 1901;
	int uoamgq = 2159;
	bool uemycwggiugovjy = true;
	if (1901 != 1901) {
		int yog;
		for (yog = 31; yog > 0; yog--) {
			continue;
		}
	}
	if (true == true) {
		int yesdhhemly;
		for (yesdhhemly = 42; yesdhhemly > 0; yesdhhemly--) {
			continue;
		}
	}
	if (true == true) {
		int kswyvj;
		for (kswyvj = 6; kswyvj > 0; kswyvj--) {
			continue;
		}
	}
	if (2159 == 2159) {
		int hzvu;
		for (hzvu = 50; hzvu > 0; hzvu--) {
			continue;
		}
	}
	return string("mcnfumayzkkswva");
}

double dllyadf::shkdjudgvuumfkskii(double tsrhyelc, double umzsibxdirihy, double fiadbrenfz, int xnyrbxruxw) {
	double gxgqz = 53309;
	bool uigemiayazzh = false;
	bool nptfezly = false;
	int fvnmnesswpqy = 2277;
	string zpeflyvfdsashhv = "binvkectpivjtdfjibhvaaxyvowydevafmiegntxovgbyjibltdblafglmvnpdnrxfpbxljwvcaptufumplxvjxydrvajqcm";
	string uzfak = "mjsnufwswejyalebgkiartyijrraykfbsfzhjqcnwbwdroypeueznprjuwnrczihtxlxdmwxyupebpn";
	bool jzjhnb = true;
	bool aduuvqppq = false;
	int yzirdcs = 3442;
	if (3442 == 3442) {
		int idyrkvk;
		for (idyrkvk = 87; idyrkvk > 0; idyrkvk--) {
			continue;
		}
	}
	return 24709;
}

int dllyadf::egkgotpmlfmgcqgjxqemobl(int oztrwnde, double rkeduk, bool uubyimrettgayeq, int jitwoumo, bool uuzdbsrkehplesa, string ytmugvdtbte) {
	int mjgrkungui = 3700;
	int heqbkavlukbgq = 9037;
	double pwwxphmsnqmoi = 4428;
	string iqagesqxt = "jskfzjwpjrlmwsrdsftwbxqaumfbuacadwjsyqxxqroddrdoypjeisni";
	int ggksjzo = 907;
	int myvccjvifuwipv = 7148;
	if (4428 != 4428) {
		int rik;
		for (rik = 58; rik > 0; rik--) {
			continue;
		}
	}
	if (string("jskfzjwpjrlmwsrdsftwbxqaumfbuacadwjsyqxxqroddrdoypjeisni") == string("jskfzjwpjrlmwsrdsftwbxqaumfbuacadwjsyqxxqroddrdoypjeisni")) {
		int nazagcw;
		for (nazagcw = 43; nazagcw > 0; nazagcw--) {
			continue;
		}
	}
	return 761;
}

double dllyadf::pgtkhkrooewiyhxf(int plddwggs, int dtstbwawhc, bool jwkld, double xrzfisxef, double imisoqww) {
	return 8148;
}

void dllyadf::bmchsxfebe() {
	int pyekexiwpddoe = 1219;
	bool zihanuvwytmjju = false;
	int rriciygthjkasb = 4598;

}

bool dllyadf::icdximwzvtwklwmzgxwutj() {
	double lggbiklbg = 49721;
	double lbuswnroo = 17498;
	double bpqzj = 13089;
	int vivskh = 8999;
	string ylnfzsa = "ttquisrqyadlgwhftfiork";
	double irptxbjqjgxagwx = 30482;
	bool mdflvbzxoyjxvvx = true;
	double quduicsasewnzvj = 9212;
	if (string("ttquisrqyadlgwhftfiork") == string("ttquisrqyadlgwhftfiork")) {
		int cmed;
		for (cmed = 51; cmed > 0; cmed--) {
			continue;
		}
	}
	if (17498 == 17498) {
		int jeux;
		for (jeux = 65; jeux > 0; jeux--) {
			continue;
		}
	}
	if (49721 == 49721) {
		int wlfm;
		for (wlfm = 31; wlfm > 0; wlfm--) {
			continue;
		}
	}
	if (string("ttquisrqyadlgwhftfiork") == string("ttquisrqyadlgwhftfiork")) {
		int xjioraeai;
		for (xjioraeai = 79; xjioraeai > 0; xjioraeai--) {
			continue;
		}
	}
	return false;
}

string dllyadf::asfrordribbqsklvtud(int weeflvs, string jgfbjgbkauzab, string hprqtvl, double lnupgdlhwjohnox, bool mtmllszgolfu, int pebxerjh) {
	string tocis = "phbbqvnpjurzitzhfflayvrnysguqliwlmmqdzhgudmunmzvcejwsluopuibcqacrwobmxni";
	if (string("phbbqvnpjurzitzhfflayvrnysguqliwlmmqdzhgudmunmzvcejwsluopuibcqacrwobmxni") != string("phbbqvnpjurzitzhfflayvrnysguqliwlmmqdzhgudmunmzvcejwsluopuibcqacrwobmxni")) {
		int tifwwh;
		for (tifwwh = 55; tifwwh > 0; tifwwh--) {
			continue;
		}
	}
	if (string("phbbqvnpjurzitzhfflayvrnysguqliwlmmqdzhgudmunmzvcejwsluopuibcqacrwobmxni") != string("phbbqvnpjurzitzhfflayvrnysguqliwlmmqdzhgudmunmzvcejwsluopuibcqacrwobmxni")) {
		int kjyryqro;
		for (kjyryqro = 98; kjyryqro > 0; kjyryqro--) {
			continue;
		}
	}
	if (string("phbbqvnpjurzitzhfflayvrnysguqliwlmmqdzhgudmunmzvcejwsluopuibcqacrwobmxni") == string("phbbqvnpjurzitzhfflayvrnysguqliwlmmqdzhgudmunmzvcejwsluopuibcqacrwobmxni")) {
		int jqpnkdji;
		for (jqpnkdji = 75; jqpnkdji > 0; jqpnkdji--) {
			continue;
		}
	}
	return string("yuzsyhkkbotdqe");
}

dllyadf::dllyadf() {
	this->nyyaudxblthlpiqnbsj(false, 27237, false, 2923, string("kwkxxqaxmalrsmurcdlknbhqlalovapdjgfiyxmxadidrkrymurgkhnp"), string("ibf"), 1656, string("ipxpolzjfvqpvrixlaqlwkugjgidakw"), 2023, true);
	this->ojuugzfkbgnjktgddtlcsk(string("evspuzjjvghybovqrjhhgolbsndlmjioxstsxqmqrxmuzbcuhavpjgkd"), 18304, false, 61854);
	this->ndzeiirjszzm(string("rdupbgdpyxeczsoutgohif"), string("kqru"), string("rczlogqkeouwaatswocgdonerqwwfudziwpkxaqllamdtkeagpxvqvnoopshjusmk"), false, false, 2519, 5720, string("soozoesftrvrz"));
	this->akivtcdvvssaiucmrrkf(false, true, 2329);
	this->shkdjudgvuumfkskii(9086, 16625, 40009, 3228);
	this->egkgotpmlfmgcqgjxqemobl(1669, 20282, true, 4217, false, string("sdhfztggoraakntxwzpceetskegjiydsrvmrbeavrlcxcfz"));
	this->pgtkhkrooewiyhxf(5664, 1163, false, 26291, 27949);
	this->bmchsxfebe();
	this->icdximwzvtwklwmzgxwutj();
	this->asfrordribbqsklvtud(956, string("daemclihfwrzfqfcnswzktzqvwqlzmdxjs"), string("jblyxgoeiprubrgeabnywmtycmtbqvjzmortbpojinru"), 58247, true, 399);
	this->mwckywlwqjcdpvgxf();
	this->olewnycsflkeamu(string("guqurankvzjahjqvwckbcaj"), string("ipmcvjxmdhpdzyixmamoezuxgxwwqbupdqiulhloduesjfiuqhduq"), true, false, 8846, 3047, true);
	this->xftyzozccgjbgqbqasfg(11032, 4018, 20523, 5030, 4013, 92358, 67704);
	this->epaeamhdrzsyugsbvvla(1304, 28024, 4515, 4204, false, false, 1785, 4558, string("tjn"), string("nynjwszwuttsbowfzgnkzbjlirzzufvemqyhrvqpwcfmdgzpqb"));
	this->negsrhgtgxgeuzcjv(string("stfpcpuyfhwsemsemuuswocccjlhanpqirfraelhbvfxvrwuoew"), string("dcictyclrsznehfqjjynzprffgnjvlwggd"), 46603, false, false, string("hmlxzamfqyvnjwyanedwbqmkcbgoggigalhwpgrfkguyqltdcteafislvbdxijmcdwfhszzfrlutkravbr"), string("ainijgxyvdndkdmzkgnlmusjnbahflivjqkbelbxmwmiuwwvyiokjbjb"));
	this->lotklguctsis(string("pxosbxgwloaeriiyfqccdoblubipcoygefwunydrvwsvisaaauxquwqxiuoerrmpvbmnxiskbbarttczshgvvicznywx"), string("nvyyvvi"), 40122, 4932, 314, true, true, 25981, string("fjspafdodriyvkaqmtcessw"));
}
