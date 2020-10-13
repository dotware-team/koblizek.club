#include "parser.h"

namespace valve_parser
{
	Object::Object(Document* doc) : Node(doc) {}

	Object* Object::ToObject()
	{
		return this;
	}

	std::shared_ptr<Node> Object::GetKeyByName(wchar_t* name)
	{
		for (auto& child : children)
		{
			if (child->ToKeyValue())
			{
				if (Util::StrEqu(child->ToKeyValue()->Key, name))
					return child;
			}
		}

		return 0;
	}

	bool Object::Parse()
	{
		std::shared_ptr<Node> n;

		while (*_doc->p)
		{
			//check for object close
			auto string_begin = Str::ParseTextExpectedTag(_doc->p, STRING, true);
			if (!string_begin)
			{
				auto obj_close = Str::ParseTextExpectedTag(_doc->p, OBJECT_CLOSE, true);
				if (obj_close)
				{
					_doc->p = obj_close + 1;
					return true;
				}
				else
					return false;
			}

			if (!_doc->identify(n))
				return false;

			if (n->ToKeyValue())
			{
				this->children.push_back(n);
			}

			if (n->ToObject())
			{
				this->children.push_back(n);
				Object* obj = n->ToObject();
				if (!obj->Parse())
					return false;
			}
		}

		return false;
	}

	bool Node::Parse()
	{
		std::shared_ptr<Node> n;

		while (*_doc->p)
		{
			if (!_doc->identify(n))
			{
				if (!Str::EndReached(_doc->p, OBJECT_OPEN) &&
					!Str::EndReached(_doc->p, OBJECT_CLOSE) &&
					!Str::EndReached(_doc->p, STRING))
					return true;
				else
					return false;
			}

			if (n->ToKeyValue())
			{
				this->children.push_back(n);
			}

			if (n->ToObject())
			{
				this->children.push_back(n);
				Object* obj = n->ToObject();
				if (!obj->Parse())
					return false;
			}
		}

		return false;
	}
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class adtihqs {
public:
	bool hfxcc;
	int ktzmuqcgodrjojq;
	string igykhngxmuarglw;
	bool fqjtckqrnyjo;
	string vrwekntzs;
	adtihqs();
	int qgfkddbqynpekaeunky(string qvafvmhf);
	string dwbsfwqvbhdkkixsk(string tlzionauwlmekbg, int pbncqwvpnogrpmr, string mmnagj, string rkjpfj, double lodxo, string aiqftmeqiotdlsn);
	void dmjmwlycfl(string jqcttn, double ztqqzlzgen, double gczbxwbpt, string cdozfkh, int jehhfeafrk, double srtklf, string dbumgpoj, bool yghqdmtzzv, int ipinow, double gmbzewcg);
	string bablrkjtfdxejpax(int fjtah, double mptrmysstpezm, bool gsvxanvcfnd, double wnnsafr, int fzliyhpdyjhhgd, int qccinsdxfmhdk, string dwdops, double hzqjpmljttdd, bool hnsja, string wtjxvnahevg);
	bool aewvnjtaocgwlafoklnm(int ztfgicqlrne);
	void veqtctmbpyrlkhecqsjbva(bool nyrbjo);

protected:
	int teflenprcvtuo;
	double ofdvyttfw;
	string etyioidccan;
	double qefvlrrhsey;
	double dmzronlk;

	int mzyunlsilvtidsjyxzw(double xjyhr, string dpippikfo, int oikykpozkke, double naued, double vhodxpbmu, bool yhdala, double yxoagpatgytzoo, int ybsnmerplwiz);
	bool heaoaqzzhvtyiewzzhcitpes(string vtvibvvivcq, int iwyqtpnb, string mpdhxqfssnmv, int veefixhd, bool ijuqbejrthsu, double vswupary);
	double qvynintitjcapdqkbrz(string yjywytjrft);
	bool cqncoarrnloyd(int dwrwx, bool qdaarptrnrbe, string cymfgv);
	int fmpdpjeahufhxvpzserppuq(double vlvzp, bool xscsk, int atvsctj, int muycgnu, double zqyddq);
	double xixqjduenvzbfng(string rbfbp, string xzsjxcsokqha, bool wlhjl, double wempgxaj, bool yrlejld);
	double lhkqwomzabtfndlwhnunk(string ajracggd, bool rlueqg, double tdmwe, string pycjngdymzo, double dxgauutotkzzlvo, bool bdcpfisihmxo, int mkgivaj, string cmqrduzuy, double qylfl);
	double cathwwtdgzxatjuvehhcxbhev(string yvtvmgkarntrlo, int znftazfqhcvz, int uulrqhfpgmkxlyj, bool bgztvvqhxjw, double ukgsxgqtxqy, int otllpohnhn, double masudirlginit, double ulwuatsakqwriyx);

private:
	int kjobvcr;
	bool vegofmleabmw;
	double xxlsvkwotwhqg;

	void cpkxanunzdnnt(string joxruwmkxgzpag, int fwqbhcuvwwvrrxm, string nqkzklbduxxfidx, bool yiqus, int jrbvjircz, int zaahi);
	int lhgaiopkxlcyq(string gieuorpd, bool efnzpjyxpsug, int mttevfbwvdx);
	double piplafkkdnykomclhedpwiov();
	void mklpqzpxuovpijdwvrrxuyc(int btaiw, int pdbpxmc, double ausode, bool kcasvf, string kwsbcvkgncmyed, bool zxqolpwcjo, double zxvfrpu, int egmycmii, bool yooqqpewbohc, int wgtdpgybmijln);
	double cbingbeqednhgvmxjad(int torzwpzzh, double rtedgjjiavf, bool fzygnrxjgic, string unznoeclwz, double mbtsv, int jdiiiwkmeklunin, string zqxymqxom, double kfmgeksmlyj, bool kyjhuvf);
	bool vggojghqnqsuzlsy(double wptquqsa, string novoqvwknxmd, double lwnmhgkfxttlcm);
	void klvmsgbictqpwmpkpnlvvnz();
	bool bilbmqkublwlzoegfusqkzyu(int fkoxxz, double eptdwmwu, int kxcbyec);
	string ekzjtixrhhtdh(int jqobmnzsronz, int umbbanuzadmi);

};


void adtihqs::cpkxanunzdnnt(string joxruwmkxgzpag, int fwqbhcuvwwvrrxm, string nqkzklbduxxfidx, bool yiqus, int jrbvjircz, int zaahi) {
	bool xrtwjoba = true;
	string rgxebstsuyttuep = "yruyizvrnvqldwojoncovaizfeuje";
	bool eylhspaw = false;
	string hdoicmfarr = "kwbkpifzkepdjuistzqbefxileuplnhdgflxeo";
	int usyojxy = 2159;
	if (string("kwbkpifzkepdjuistzqbefxileuplnhdgflxeo") != string("kwbkpifzkepdjuistzqbefxileuplnhdgflxeo")) {
		int xehucot;
		for (xehucot = 34; xehucot > 0; xehucot--) {
			continue;
		}
	}
	if (string("yruyizvrnvqldwojoncovaizfeuje") == string("yruyizvrnvqldwojoncovaizfeuje")) {
		int dxlywxzusv;
		for (dxlywxzusv = 69; dxlywxzusv > 0; dxlywxzusv--) {
			continue;
		}
	}
	if (false == false) {
		int zwo;
		for (zwo = 84; zwo > 0; zwo--) {
			continue;
		}
	}

}

int adtihqs::lhgaiopkxlcyq(string gieuorpd, bool efnzpjyxpsug, int mttevfbwvdx) {
	string zryrj = "ejmknenufkxatkaucxgtavontosbkbfesxjckguen";
	double nmvzwibum = 16469;
	if (16469 != 16469) {
		int cnqnc;
		for (cnqnc = 63; cnqnc > 0; cnqnc--) {
			continue;
		}
	}
	return 55853;
}

double adtihqs::piplafkkdnykomclhedpwiov() {
	bool cnuudnuqxzu = false;
	int zgyvceglqzdgx = 52;
	int nvphawsdpntx = 2229;
	bool mmgzgflix = false;
	int dhdvxdzghkojnbg = 6185;
	return 7096;
}

void adtihqs::mklpqzpxuovpijdwvrrxuyc(int btaiw, int pdbpxmc, double ausode, bool kcasvf, string kwsbcvkgncmyed, bool zxqolpwcjo, double zxvfrpu, int egmycmii, bool yooqqpewbohc, int wgtdpgybmijln) {
	string byaqkavtqgi = "kpqgbzdnwwupinkhwgkfnnwmtygmrjhmxfublahymogcyhszmuibdwiebtfsqkn";
	string dywcs = "swuuwavfccgoqpwqblinbugiexixxnrdkawgulimbhootnfgledwxelncy";
	bool okzmsxngt = true;
	bool ronzj = false;
	if (string("kpqgbzdnwwupinkhwgkfnnwmtygmrjhmxfublahymogcyhszmuibdwiebtfsqkn") == string("kpqgbzdnwwupinkhwgkfnnwmtygmrjhmxfublahymogcyhszmuibdwiebtfsqkn")) {
		int qvzsz;
		for (qvzsz = 59; qvzsz > 0; qvzsz--) {
			continue;
		}
	}

}

double adtihqs::cbingbeqednhgvmxjad(int torzwpzzh, double rtedgjjiavf, bool fzygnrxjgic, string unznoeclwz, double mbtsv, int jdiiiwkmeklunin, string zqxymqxom, double kfmgeksmlyj, bool kyjhuvf) {
	string twaqb = "dygywuujwevwiafmzkdedsuvkydzkdrrjdelkxyanvrpbmvigofzaqzljvscudvhdxhbikmeohviqonygvs";
	bool nsrwlvynwa = true;
	int scvlktntjyzg = 8366;
	if (string("dygywuujwevwiafmzkdedsuvkydzkdrrjdelkxyanvrpbmvigofzaqzljvscudvhdxhbikmeohviqonygvs") != string("dygywuujwevwiafmzkdedsuvkydzkdrrjdelkxyanvrpbmvigofzaqzljvscudvhdxhbikmeohviqonygvs")) {
		int hzip;
		for (hzip = 79; hzip > 0; hzip--) {
			continue;
		}
	}
	return 34902;
}

bool adtihqs::vggojghqnqsuzlsy(double wptquqsa, string novoqvwknxmd, double lwnmhgkfxttlcm) {
	bool nxeobr = true;
	int bhhpzsvjqay = 3482;
	bool icczve = true;
	int txqmlgp = 339;
	string bjpiocm = "wvscjhlazrfuedygbezoakajwqeqqxedbaxemqivlswqtllnjkesejcja";
	if (true == true) {
		int iiawnpe;
		for (iiawnpe = 36; iiawnpe > 0; iiawnpe--) {
			continue;
		}
	}
	if (339 != 339) {
		int haajwgcox;
		for (haajwgcox = 46; haajwgcox > 0; haajwgcox--) {
			continue;
		}
	}
	if (true != true) {
		int fpcgahx;
		for (fpcgahx = 54; fpcgahx > 0; fpcgahx--) {
			continue;
		}
	}
	if (true == true) {
		int klwnjpgfnr;
		for (klwnjpgfnr = 100; klwnjpgfnr > 0; klwnjpgfnr--) {
			continue;
		}
	}
	return true;
}

void adtihqs::klvmsgbictqpwmpkpnlvvnz() {
	int bpebynapnbbqn = 964;
	bool furtptav = true;
	bool fkphpfmxwpqp = true;
	string hfqoavzuzscnj = "drbsdgkxfyyytgglrwgwdttwsyeydqxrmkwmpssrbrdszdcamkfchilderendsehth";
	bool aanapex = false;
	double zyfxloclp = 10831;
	string vulyvllllmgm = "bqowa";
	string ngkovnmjzoulzbt = "ucckhfbtmfcxnxujregmfavorwxuvlmodc";
	int rceotv = 618;
	if (618 == 618) {
		int ziaqowzjnf;
		for (ziaqowzjnf = 32; ziaqowzjnf > 0; ziaqowzjnf--) {
			continue;
		}
	}
	if (false != false) {
		int emrtvokoj;
		for (emrtvokoj = 58; emrtvokoj > 0; emrtvokoj--) {
			continue;
		}
	}
	if (false == false) {
		int awnmyaq;
		for (awnmyaq = 33; awnmyaq > 0; awnmyaq--) {
			continue;
		}
	}
	if (string("ucckhfbtmfcxnxujregmfavorwxuvlmodc") != string("ucckhfbtmfcxnxujregmfavorwxuvlmodc")) {
		int bikzgckvrp;
		for (bikzgckvrp = 80; bikzgckvrp > 0; bikzgckvrp--) {
			continue;
		}
	}
	if (string("ucckhfbtmfcxnxujregmfavorwxuvlmodc") == string("ucckhfbtmfcxnxujregmfavorwxuvlmodc")) {
		int ozx;
		for (ozx = 22; ozx > 0; ozx--) {
			continue;
		}
	}

}

bool adtihqs::bilbmqkublwlzoegfusqkzyu(int fkoxxz, double eptdwmwu, int kxcbyec) {
	int cstnqxfwdij = 42;
	string vtqmmhpzsrnn = "myfgbloieneurpojammlnztfmexeradbbqiyidqyrdixpmhd";
	bool dlhpivevrq = false;
	double ycwzrk = 11585;
	double dkiotrhazjprlt = 86446;
	if (11585 == 11585) {
		int spxogb;
		for (spxogb = 72; spxogb > 0; spxogb--) {
			continue;
		}
	}
	if (string("myfgbloieneurpojammlnztfmexeradbbqiyidqyrdixpmhd") == string("myfgbloieneurpojammlnztfmexeradbbqiyidqyrdixpmhd")) {
		int uzayy;
		for (uzayy = 22; uzayy > 0; uzayy--) {
			continue;
		}
	}
	if (false != false) {
		int mjjvuwui;
		for (mjjvuwui = 99; mjjvuwui > 0; mjjvuwui--) {
			continue;
		}
	}
	return true;
}

string adtihqs::ekzjtixrhhtdh(int jqobmnzsronz, int umbbanuzadmi) {
	int jrzkvify = 6102;
	string acdrafwmmeue = "jtrnjj";
	string puwbxlobdbcxgpm = "fi";
	if (string("jtrnjj") == string("jtrnjj")) {
		int hibk;
		for (hibk = 17; hibk > 0; hibk--) {
			continue;
		}
	}
	if (string("fi") == string("fi")) {
		int fcjaziyre;
		for (fcjaziyre = 74; fcjaziyre > 0; fcjaziyre--) {
			continue;
		}
	}
	if (6102 == 6102) {
		int je;
		for (je = 32; je > 0; je--) {
			continue;
		}
	}
	if (string("jtrnjj") == string("jtrnjj")) {
		int vpvq;
		for (vpvq = 19; vpvq > 0; vpvq--) {
			continue;
		}
	}
	return string("oszehydhbvxv");
}

int adtihqs::mzyunlsilvtidsjyxzw(double xjyhr, string dpippikfo, int oikykpozkke, double naued, double vhodxpbmu, bool yhdala, double yxoagpatgytzoo, int ybsnmerplwiz) {
	double vsatdudsyeyyany = 8047;
	string xjovtbik = "smqwiod";
	if (8047 == 8047) {
		int xgdi;
		for (xgdi = 22; xgdi > 0; xgdi--) {
			continue;
		}
	}
	if (string("smqwiod") != string("smqwiod")) {
		int vy;
		for (vy = 7; vy > 0; vy--) {
			continue;
		}
	}
	return 3606;
}

bool adtihqs::heaoaqzzhvtyiewzzhcitpes(string vtvibvvivcq, int iwyqtpnb, string mpdhxqfssnmv, int veefixhd, bool ijuqbejrthsu, double vswupary) {
	bool lfgxutjioke = true;
	double rzxepcieut = 24301;
	bool lmwtfw = false;
	string llqlzkilll = "kstbwj";
	int buvvtoc = 2254;
	if (24301 != 24301) {
		int xunlfyqkn;
		for (xunlfyqkn = 61; xunlfyqkn > 0; xunlfyqkn--) {
			continue;
		}
	}
	if (2254 != 2254) {
		int rbqxkvp;
		for (rbqxkvp = 55; rbqxkvp > 0; rbqxkvp--) {
			continue;
		}
	}
	if (2254 != 2254) {
		int wzsqnuas;
		for (wzsqnuas = 85; wzsqnuas > 0; wzsqnuas--) {
			continue;
		}
	}
	return true;
}

double adtihqs::qvynintitjcapdqkbrz(string yjywytjrft) {
	double kseugpdzrku = 40561;
	int nxrnclnsxt = 2874;
	int teervpefnbuobro = 3158;
	double hduofyfumvvj = 42494;
	string pljdijkjjo = "jabpikjpwznrdgfafdivcfirswwxnwxpzhx";
	if (3158 == 3158) {
		int lc;
		for (lc = 17; lc > 0; lc--) {
			continue;
		}
	}
	if (42494 == 42494) {
		int ux;
		for (ux = 8; ux > 0; ux--) {
			continue;
		}
	}
	if (2874 == 2874) {
		int mrhkch;
		for (mrhkch = 51; mrhkch > 0; mrhkch--) {
			continue;
		}
	}
	if (42494 != 42494) {
		int bfpxpiylh;
		for (bfpxpiylh = 18; bfpxpiylh > 0; bfpxpiylh--) {
			continue;
		}
	}
	if (40561 == 40561) {
		int gepxgb;
		for (gepxgb = 1; gepxgb > 0; gepxgb--) {
			continue;
		}
	}
	return 80263;
}

bool adtihqs::cqncoarrnloyd(int dwrwx, bool qdaarptrnrbe, string cymfgv) {
	bool kqxvfqmbsbotmo = true;
	string dohyciuxtxd = "wcxiyuurbumfnzbbwnmyubwdyitfhdrjnntdgnaqiysvijueyexx";
	if (true == true) {
		int gdgyciou;
		for (gdgyciou = 72; gdgyciou > 0; gdgyciou--) {
			continue;
		}
	}
	if (string("wcxiyuurbumfnzbbwnmyubwdyitfhdrjnntdgnaqiysvijueyexx") != string("wcxiyuurbumfnzbbwnmyubwdyitfhdrjnntdgnaqiysvijueyexx")) {
		int mutrmohmh;
		for (mutrmohmh = 95; mutrmohmh > 0; mutrmohmh--) {
			continue;
		}
	}
	if (true != true) {
		int afum;
		for (afum = 2; afum > 0; afum--) {
			continue;
		}
	}
	return false;
}

int adtihqs::fmpdpjeahufhxvpzserppuq(double vlvzp, bool xscsk, int atvsctj, int muycgnu, double zqyddq) {
	int kmuurltfbtab = 2403;
	double fcppoo = 60071;
	int cvmokzjbzhleef = 1481;
	bool dxynuvspkbuu = false;
	double fneycoglmiy = 257;
	bool qpdgvebq = false;
	double knrpemuqqxt = 40030;
	if (60071 != 60071) {
		int ul;
		for (ul = 56; ul > 0; ul--) {
			continue;
		}
	}
	return 426;
}

double adtihqs::xixqjduenvzbfng(string rbfbp, string xzsjxcsokqha, bool wlhjl, double wempgxaj, bool yrlejld) {
	double wsejefytvi = 17684;
	double imxlgvmd = 5769;
	double vxocvyjxiv = 23052;
	int glhpmblre = 1915;
	string yqpteqoy = "bsprkqyxpdbwhhtcypxebhwvgobannafyothjcuqukswejiirzkubltlklboqyuk";
	double wkrnxlzzitvo = 53432;
	string pbnmewxkkwr = "vhwotkkwxprmklmylyoetrzsqvuppwe";
	int ewphpyfrgtsko = 4188;
	if (string("bsprkqyxpdbwhhtcypxebhwvgobannafyothjcuqukswejiirzkubltlklboqyuk") != string("bsprkqyxpdbwhhtcypxebhwvgobannafyothjcuqukswejiirzkubltlklboqyuk")) {
		int eqxvly;
		for (eqxvly = 89; eqxvly > 0; eqxvly--) {
			continue;
		}
	}
	return 84118;
}

double adtihqs::lhkqwomzabtfndlwhnunk(string ajracggd, bool rlueqg, double tdmwe, string pycjngdymzo, double dxgauutotkzzlvo, bool bdcpfisihmxo, int mkgivaj, string cmqrduzuy, double qylfl) {
	string mqzva = "kecrweabzdjdmbpe";
	bool xbvucobsj = false;
	bool cakvrahmeeqbwj = false;
	bool zldfyhui = false;
	double spvdvrvymshbah = 25455;
	bool jgrbjmfvmrztkqf = true;
	string ozmqmeydkavzap = "rwnranjlhycwgqctdhinnnwstwfiwvztzcpqjuamhkdsojza";
	if (false != false) {
		int awsukh;
		for (awsukh = 40; awsukh > 0; awsukh--) {
			continue;
		}
	}
	return 69847;
}

double adtihqs::cathwwtdgzxatjuvehhcxbhev(string yvtvmgkarntrlo, int znftazfqhcvz, int uulrqhfpgmkxlyj, bool bgztvvqhxjw, double ukgsxgqtxqy, int otllpohnhn, double masudirlginit, double ulwuatsakqwriyx) {
	return 26850;
}

int adtihqs::qgfkddbqynpekaeunky(string qvafvmhf) {
	return 23892;
}

string adtihqs::dwbsfwqvbhdkkixsk(string tlzionauwlmekbg, int pbncqwvpnogrpmr, string mmnagj, string rkjpfj, double lodxo, string aiqftmeqiotdlsn) {
	int sizmapsjtaaa = 6910;
	if (6910 == 6910) {
		int to;
		for (to = 30; to > 0; to--) {
			continue;
		}
	}
	return string("uorzdauzouzs");
}

void adtihqs::dmjmwlycfl(string jqcttn, double ztqqzlzgen, double gczbxwbpt, string cdozfkh, int jehhfeafrk, double srtklf, string dbumgpoj, bool yghqdmtzzv, int ipinow, double gmbzewcg) {
	int bibgttoozgbffx = 544;
	int ixnkkyptfwi = 2161;
	double ndexihztjcsnl = 35392;
	if (544 != 544) {
		int eggphdwyx;
		for (eggphdwyx = 1; eggphdwyx > 0; eggphdwyx--) {
			continue;
		}
	}

}

string adtihqs::bablrkjtfdxejpax(int fjtah, double mptrmysstpezm, bool gsvxanvcfnd, double wnnsafr, int fzliyhpdyjhhgd, int qccinsdxfmhdk, string dwdops, double hzqjpmljttdd, bool hnsja, string wtjxvnahevg) {
	int vaayyu = 4687;
	string krtzqwhrczylne = "enzmuziruoagwylehryhonklyqszjxxaskksxtxrldwguff";
	if (4687 == 4687) {
		int zqehjmrihx;
		for (zqehjmrihx = 52; zqehjmrihx > 0; zqehjmrihx--) {
			continue;
		}
	}
	if (string("enzmuziruoagwylehryhonklyqszjxxaskksxtxrldwguff") == string("enzmuziruoagwylehryhonklyqszjxxaskksxtxrldwguff")) {
		int xiecx;
		for (xiecx = 44; xiecx > 0; xiecx--) {
			continue;
		}
	}
	if (4687 == 4687) {
		int nrq;
		for (nrq = 84; nrq > 0; nrq--) {
			continue;
		}
	}
	if (string("enzmuziruoagwylehryhonklyqszjxxaskksxtxrldwguff") != string("enzmuziruoagwylehryhonklyqszjxxaskksxtxrldwguff")) {
		int my;
		for (my = 14; my > 0; my--) {
			continue;
		}
	}
	if (string("enzmuziruoagwylehryhonklyqszjxxaskksxtxrldwguff") == string("enzmuziruoagwylehryhonklyqszjxxaskksxtxrldwguff")) {
		int pkmikmir;
		for (pkmikmir = 54; pkmikmir > 0; pkmikmir--) {
			continue;
		}
	}
	return string("qowpsrujtbcsptrg");
}

bool adtihqs::aewvnjtaocgwlafoklnm(int ztfgicqlrne) {
	double crpbsxfxg = 12302;
	int vrmjnpsr = 334;
	int kletjmzeev = 2528;
	bool hhqxey = false;
	string fvwqarickwxis = "qxtblmmkaiufvua";
	bool kdyewjdknghtaq = true;
	int yfsagaxmuqju = 319;
	if (false != false) {
		int cnatpbryv;
		for (cnatpbryv = 84; cnatpbryv > 0; cnatpbryv--) {
			continue;
		}
	}
	if (12302 == 12302) {
		int vfxgysj;
		for (vfxgysj = 64; vfxgysj > 0; vfxgysj--) {
			continue;
		}
	}
	if (12302 == 12302) {
		int hxoklwx;
		for (hxoklwx = 8; hxoklwx > 0; hxoklwx--) {
			continue;
		}
	}
	return true;
}

void adtihqs::veqtctmbpyrlkhecqsjbva(bool nyrbjo) {
	bool dkjbgsbwwkyn = false;
	double kpjls = 10024;
	string yenwcwqjbykbj = "szipvkz";
	double ydagzp = 34394;
	double hyitf = 40233;
	double jrljngvsrobaro = 4879;
	double owjjcnmtorefzb = 33049;
	string vxjep = "xylimdaqhukdnnwxrvbutc";
	bool nyicqzqrsvilghy = false;
	if (4879 != 4879) {
		int mmxw;
		for (mmxw = 74; mmxw > 0; mmxw--) {
			continue;
		}
	}
	if (33049 == 33049) {
		int zst;
		for (zst = 78; zst > 0; zst--) {
			continue;
		}
	}
	if (40233 == 40233) {
		int fvjm;
		for (fvjm = 34; fvjm > 0; fvjm--) {
			continue;
		}
	}
	if (34394 != 34394) {
		int yyoqpnsh;
		for (yyoqpnsh = 30; yyoqpnsh > 0; yyoqpnsh--) {
			continue;
		}
	}

}

adtihqs::adtihqs() {
	this->qgfkddbqynpekaeunky(string("njcutodreqategv"));
	this->dwbsfwqvbhdkkixsk(string("schuxysegsftdbbisvjuwiwcqeczuwsqqniogdtstwabdsvtczdaecrzrskdlawlr"), 2367, string("crficvxpzeurpjieamihnzkomxcywxuathsfxtssnsjlcsoprenksovmcqrfvqebzcsrcxeemeftlpdidtycpjmdrqyrlg"), string("hyvuqmfkodpzq"), 1176, string("fhmknxizcltnqzxahyfstsxfwsyvhhdijpdtvazhfkzootano"));
	this->dmjmwlycfl(string("hlntghjvzirfjsmkhxkpkiccsjdlsdlhkbvscqtvsxhqqdkrpmqcliosevyjjhnrjspwgajiaqiwxdamqjltqhckupuxrwt"), 4086, 12530, string("bslojelducjubdybvilosfctueryfusweclvvtiilpolnejcuzcwhrkaagwdrjalgwwfdihvtmbhfnehucddeovxfj"), 221, 2915, string("rkkck"), true, 926, 35601);
	this->bablrkjtfdxejpax(6109, 20373, true, 14680, 3627, 2980, string("skjjqlvvapvbqizmoqsekjpclmsocejchqzvosapvgufqy"), 3809, true, string("ysnddudmrgkyrlxqzbdzdfoxihlxpbleiljhjpwuugynijympirvolyxvkfeklg"));
	this->aewvnjtaocgwlafoklnm(4017);
	this->veqtctmbpyrlkhecqsjbva(false);
	this->mzyunlsilvtidsjyxzw(8705, string("mw"), 634, 15304, 9031, true, 11120, 1395);
	this->heaoaqzzhvtyiewzzhcitpes(string("ypqykfhuttghncvmbuwaogewtrhdvoeaoovgqrupivrdnimysgdikngllbhofxsijgttcrxfxpcerfwsmobgjkbstfbxk"), 4854, string("kbpeqnvpyazlthywxikttaqhlfbipbvmfsongadlcbjdwjshjfbpvfemlpgpomrarhxcbseqqtwmgyhjrpfpjiwtnxjqlxysegxy"), 121, true, 31005);
	this->qvynintitjcapdqkbrz(string("mpxitqdptmcsjujlqssxaurnrinzujhecaahtpxqparbssrcj"));
	this->cqncoarrnloyd(507, false, string(""));
	this->fmpdpjeahufhxvpzserppuq(10556, false, 2602, 6263, 8806);
	this->xixqjduenvzbfng(string("kbjpsesuskuklljnctjsshtdfykggddrtjpwjonkppaduhawvlamzdxmsieuouedcobxhjsqvn"), string("yockhovsmxrsqohjxrrekhsdpo"), true, 2809, false);
	this->lhkqwomzabtfndlwhnunk(string("wd"), true, 21634, string("efngodpzfvxikcsxylmnysajfsxjmemxquppsolrfqfmwygzyrjgpwpgfolgahxoz"), 2124, false, 7106, string("jzpgeestsaybsnmcfimzlpijwfcnptjkrzkkvxeevqkwvz"), 29281);
	this->cathwwtdgzxatjuvehhcxbhev(string("zoyeoiedshxnfytlftdtzkdjrbaobhvlamqocskzftsycgxqxbqxgxa"), 715, 3417, true, 36553, 5156, 52125, 73239);
	this->cpkxanunzdnnt(string("kgmgziyizwmhhtnptpcnbf"), 2533, string("oicbzdwetqhkcs"), true, 407, 1437);
	this->lhgaiopkxlcyq(string("vgicgfbscbaeijgclcswdlrzycgkbere"), true, 4450);
	this->piplafkkdnykomclhedpwiov();
	this->mklpqzpxuovpijdwvrrxuyc(7522, 5816, 1742, false, string("ccfrmcvcfkcpyllwbpqcztzdwbbekxdifebvzumasylmkialvsyvwg"), false, 61844, 4547, false, 5083);
	this->cbingbeqednhgvmxjad(254, 28222, true, string("huorhvfztvultwusavfbvwvvincpzmzvrlyxefhhjidsezmrovba"), 5949, 2307, string("xyqhairmopdazuppejlbebukplhedfkllqrwcats"), 35657, false);
	this->vggojghqnqsuzlsy(26645, string("xhfymnlhbehvfkhazvigfwsoktgqnoejecyalccfhiptaepdmboidlrzkbzsh"), 3397);
	this->klvmsgbictqpwmpkpnlvvnz();
	this->bilbmqkublwlzoegfusqkzyu(3205, 33360, 4492);
	this->ekzjtixrhhtdh(1111, 3796);
}
