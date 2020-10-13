#include "trail.h"
#include "..\cheat_sdk\Entity.h"
#include "..\cheat_sdk\EntityList.h"
#include "..\IViewRenderBeams.h"
#include "..\Color.hpp"
#include "..\cheat_others\Config.h"
#include "..\cheat_sdk\Vector.h"
#include "../cheat_protection/xor.hpp"

void Trail::run_cm()
{
	if (!interfaces.engine->isInGame())
		return;

	auto local_player = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());

	if (!local_player || !config.koblizek_misc.trail_enabled)
		return;

	if (!local_player->isAlive() || local_player->isDormant())
		return;	

	static Vector old_pos = local_player->origin();//máš dobré oèi

	render_bean(old_pos, local_player->origin(), config.koblizek_misc.trail_color);

	old_pos = local_player->origin();
}

void Trail::render_bean(Vector start, Vector end, Barva color)
{
	BeamInfo_t trail;
	trail.m_nType = 0;
	trail.m_pszModelName = XorStr("sprites/purplelaser1.vmt");
	trail.m_nModelIndex = -1;
	trail.m_flHaloScale = 0.0;
	trail.m_flLife = config.koblizek_misc.trail_duration;
	trail.m_flWidth = 5;
	trail.m_flEndWidth = 5;
	trail.m_flFadeLength = 0.0;
	trail.m_flAmplitude = 2.0;
	trail.m_flBrightness = color.a();
	trail.m_flSpeed = 0.5;
	trail.m_nStartFrame = 0.;
	trail.m_flFrameRate = 0.;
	trail.m_flRed = color.r();
	trail.m_flGreen = color.g();
	trail.m_flBlue = color.b();
	trail.m_nSegments = 2;
	trail.m_bRenderable = true;
	trail.m_nFlags = 0;
	trail.m_vecStart = start;
	trail.m_vecEnd = end;

	memory.renderBeams->CreateBeamPoints(trail);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class gcdnwcl {
public:
	int enaosoh;
	bool nujsc;
	double bthqpahjmrt;
	bool cnstjufbeqmwc;
	bool rxduxjleqzjga;
	gcdnwcl();
	bool ynudhqrvpfrmolgtwqsrl(int gppfgjjzg, bool razkwkcbsugufy, double mgfsahsqzv, int upcmmawjqhwhis, bool mydvkzqd, bool nbsblwmapkljhc, string gjhjzfnerpequc, double ezkko);

protected:
	bool wilrkzgzgwe;
	bool zhcnvgnujo;
	int qukexnxd;
	double vjmvd;
	double vgewqwzypj;

	void kugecoxugrmcujedmuyqgjvmk(int tkiqu, string hencgxtidg, double rshetnnr, double shakqdtsiyhcecx, string wmwipss);
	void wmgakocnutzkv(string pwboexpunoqmiuv, double cyfydclkovhbjc, int zeatp, bool ivlfbpdqi, double tgrwayzj, int osvxzjqogtv);
	string gpkrpksnwrsbsgbppkmhrn(int lqzjlxemriznxz, double djpkhdcrhz, double wkmxjnddnmket, double tckwijlawaja, bool fchgpyyqgsfjfm, bool bposowh, string kmmjmews, bool zcgwvggoqpzd, string ypkfywvxbvfuc);

private:
	string oxces;
	int usqjemtn;
	int lxbxcawxuefj;
	double eqxyzkrzii;

	int rnmgvomrbh();
	string rjotssepzcwnspxtk(int bbmfslhqrg);

};


int gcdnwcl::rnmgvomrbh() {
	string knmygdu = "xzoqbsggzxwcnpfulkizwlehaorxahwxyvlvugmoiqlmoyytfdtdxbfbmqmndmvtrrjhvkpggfcxgbejs";
	string ijjsfi = "pbdwcsspubxrwmbxdbgjzzabwdiazdbxdpniffhcjlhplovmkqgxzwrtskinltvxcaxkbrblkbwsxtvakkukjicbzzguphi";
	double tnovocyhzivb = 23270;
	int iukgjmelntsk = 4815;
	double lvrtnero = 11744;
	int gcrhdtnqhmcu = 1810;
	double ltnbb = 1344;
	int twvqyplitfpiwh = 1364;
	int zlrnceqpriyka = 20;
	int ofnvko = 6761;
	if (string("xzoqbsggzxwcnpfulkizwlehaorxahwxyvlvugmoiqlmoyytfdtdxbfbmqmndmvtrrjhvkpggfcxgbejs") == string("xzoqbsggzxwcnpfulkizwlehaorxahwxyvlvugmoiqlmoyytfdtdxbfbmqmndmvtrrjhvkpggfcxgbejs")) {
		int yuwbqc;
		for (yuwbqc = 26; yuwbqc > 0; yuwbqc--) {
			continue;
		}
	}
	if (23270 == 23270) {
		int qeiwimhmq;
		for (qeiwimhmq = 47; qeiwimhmq > 0; qeiwimhmq--) {
			continue;
		}
	}
	return 99215;
}

string gcdnwcl::rjotssepzcwnspxtk(int bbmfslhqrg) {
	double chwbgzjm = 2118;
	double mfruhwzzskxezs = 78301;
	if (78301 == 78301) {
		int kjfw;
		for (kjfw = 30; kjfw > 0; kjfw--) {
			continue;
		}
	}
	if (78301 == 78301) {
		int vb;
		for (vb = 27; vb > 0; vb--) {
			continue;
		}
	}
	if (2118 != 2118) {
		int lbolg;
		for (lbolg = 98; lbolg > 0; lbolg--) {
			continue;
		}
	}
	if (2118 == 2118) {
		int mqerhjeoa;
		for (mqerhjeoa = 42; mqerhjeoa > 0; mqerhjeoa--) {
			continue;
		}
	}
	return string("bcrqgswoynisb");
}

void gcdnwcl::kugecoxugrmcujedmuyqgjvmk(int tkiqu, string hencgxtidg, double rshetnnr, double shakqdtsiyhcecx, string wmwipss) {
	double iahhyg = 69011;
	if (69011 != 69011) {
		int ujersxqt;
		for (ujersxqt = 27; ujersxqt > 0; ujersxqt--) {
			continue;
		}
	}

}

void gcdnwcl::wmgakocnutzkv(string pwboexpunoqmiuv, double cyfydclkovhbjc, int zeatp, bool ivlfbpdqi, double tgrwayzj, int osvxzjqogtv) {
	double tnhjnofjxnxd = 50926;
	string vgibeikevsr = "gmnuanhpsvvovrjhxgjmdmcepidvlorwkogpuekjtdykoogibkadywmfwyoeszbskyjcwqttdonodlowlbjszucwxka";
	string najbh = "zqtgsgfjavwmjoirkwppatlgphjjktkvoqnkghvkueln";
	bool cvjxv = true;
	if (string("zqtgsgfjavwmjoirkwppatlgphjjktkvoqnkghvkueln") == string("zqtgsgfjavwmjoirkwppatlgphjjktkvoqnkghvkueln")) {
		int xkl;
		for (xkl = 5; xkl > 0; xkl--) {
			continue;
		}
	}
	if (string("gmnuanhpsvvovrjhxgjmdmcepidvlorwkogpuekjtdykoogibkadywmfwyoeszbskyjcwqttdonodlowlbjszucwxka") == string("gmnuanhpsvvovrjhxgjmdmcepidvlorwkogpuekjtdykoogibkadywmfwyoeszbskyjcwqttdonodlowlbjszucwxka")) {
		int wgdu;
		for (wgdu = 56; wgdu > 0; wgdu--) {
			continue;
		}
	}
	if (string("gmnuanhpsvvovrjhxgjmdmcepidvlorwkogpuekjtdykoogibkadywmfwyoeszbskyjcwqttdonodlowlbjszucwxka") != string("gmnuanhpsvvovrjhxgjmdmcepidvlorwkogpuekjtdykoogibkadywmfwyoeszbskyjcwqttdonodlowlbjszucwxka")) {
		int trpspeilrs;
		for (trpspeilrs = 90; trpspeilrs > 0; trpspeilrs--) {
			continue;
		}
	}

}

string gcdnwcl::gpkrpksnwrsbsgbppkmhrn(int lqzjlxemriznxz, double djpkhdcrhz, double wkmxjnddnmket, double tckwijlawaja, bool fchgpyyqgsfjfm, bool bposowh, string kmmjmews, bool zcgwvggoqpzd, string ypkfywvxbvfuc) {
	double nqohtoyg = 5130;
	string qwrmwoty = "vcutuyaiunkgkqdvtyzkkxjmeodkaekpsghckmwipicphiacxpsnstfs";
	string azekxt = "xqzelsnhiifjkvooqxkuzsk";
	string lsnbqwv = "ctdipxczmmxyxddqocpieyxageaauqixpcbqsqhfgsujxjxqenbdplgm";
	if (string("vcutuyaiunkgkqdvtyzkkxjmeodkaekpsghckmwipicphiacxpsnstfs") != string("vcutuyaiunkgkqdvtyzkkxjmeodkaekpsghckmwipicphiacxpsnstfs")) {
		int bucjws;
		for (bucjws = 1; bucjws > 0; bucjws--) {
			continue;
		}
	}
	return string("qzvqzsmkv");
}

bool gcdnwcl::ynudhqrvpfrmolgtwqsrl(int gppfgjjzg, bool razkwkcbsugufy, double mgfsahsqzv, int upcmmawjqhwhis, bool mydvkzqd, bool nbsblwmapkljhc, string gjhjzfnerpequc, double ezkko) {
	int sadhwfbkyvvlo = 370;
	bool xmikk = false;
	bool rqyrk = true;
	double ihdabbuwxashuck = 4102;
	int cpmltdikadenjc = 4566;
	double lmqcqhxmuxljz = 24859;
	bool tmxuw = true;
	string ywkxacbu = "bwesfhqwimbhsutzyepvzujawtvthaqxypcxtxoazldrotoqvmzubuxpkzyvtdqaw";
	string jgpdi = "nidsvrqhyfosyuyujyjqidnldrjrbttryzchdg";
	if (string("bwesfhqwimbhsutzyepvzujawtvthaqxypcxtxoazldrotoqvmzubuxpkzyvtdqaw") != string("bwesfhqwimbhsutzyepvzujawtvthaqxypcxtxoazldrotoqvmzubuxpkzyvtdqaw")) {
		int vu;
		for (vu = 83; vu > 0; vu--) {
			continue;
		}
	}
	return false;
}

gcdnwcl::gcdnwcl() {
	this->ynudhqrvpfrmolgtwqsrl(1320, true, 16237, 7139, true, true, string("smbvyteuyuepuezhysgaszhurabyzrfhuusfjnjcrgvkytweckwihogenggzjinxyifamohpnnofuvoawhrgsolzdjik"), 23987);
	this->kugecoxugrmcujedmuyqgjvmk(1575, string("qratzgxgdlkvqsxcewoshmjdohcubltdiiuhoajujyljdbjymqcctcwkjeycekbtevheaqzsxx"), 62162, 9555, string("qbzzmuazxicvoakcrcninkdwbfjtsizafjhphi"));
	this->wmgakocnutzkv(string("evkouxeofnrpbxeygfxktwbcqttxwfycohubvhnq"), 1003, 1306, true, 15428, 3524);
	this->gpkrpksnwrsbsgbppkmhrn(2124, 9270, 34521, 40911, false, true, string("lyturldapbojbmtdxzspbzeykkvufqzneoiymfwa"), false, string(""));
	this->rnmgvomrbh();
	this->rjotssepzcwnspxtk(1355);
}
