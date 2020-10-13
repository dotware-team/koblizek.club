#include "VelGraph.h"
#include "..\cheat_important\Interfaces.h"
#include "..\cheat_sdk\EntityList.h"
#include "..\cheat_sdk\Entity.h"
#include "..\cheat_imgui\imgui.h"
#include "..\cheat_important\GUI.h"
#include "..\Color.hpp"
#include "..\cheat_others\Config.h"

VelGraph::VelGraph()
{
	on_ground_speed = 0.f;
	data.reserve(250);//reserve space so we don't waste cpu power
	for (size_t i = 0; i < 250; i++)
	{
		graph_info_ info;
		info.on_ground = false;
		info.velocity = 0.f;
		info.last_on_ground_vel = 0.f;
		data.emplace_back(info); //fill the data vector with 0
	}
}

VelGraph::~VelGraph()
{
	data.clear();
}

void VelGraph::run_cm(UserCmd * cmd)
{
	static bool run = false;
	auto local_player = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());

	if (!interfaces.engine->isConnected() && run) {
		data.clear();
		for (size_t i = 0; i < 250; i++)
		{
			graph_info_ info;
			info.on_ground = false;
			info.velocity = 0.f;
			info.last_on_ground_vel = 0.f;
			data.emplace_back(info); //fill the data vector with 0
		}
		run = false;
		return;
	}

	if (!local_player)
		return;

	if (!local_player->isAlive())
		return update_graph(0.f, false);

	if (local_player->moveType() == MoveType::NOCLIP)
		return update_graph(0.f, false);

	if (local_player->velocity() > 500)
		return update_graph(0.f, false);
	
	float velocity = local_player->velocity().length2D();

	bool on_ground = local_player->flags() & (1 << 0);
	update_graph(velocity, on_ground);
	run = true;
}

void VelGraph::run_present()
{
	if (!config.koblizek_misc.velocity_graph_enabled)
		return;

	if (!interfaces.engine->isConnected())
		return;

	Barva graph_color = config.koblizek_misc.velocity_graph_color; 
	Barva graph_under_line_color = Barva(255, 255, 255, 150);
	int width, height;
	interfaces.engine->getScreenSize(width, height);
	constexpr float graph_size = 0.3f;
	size_t itemcount = 0;
	auto bg_drawlist = ImGui::GetBackgroundDrawList();
	float y_pos = (height / 4.f + height / 2.f + 90);
	{
		std::lock_guard lock(mutex);
		itemcount = data.size();
		for (size_t i = 1; i < data.size(); i++)
		{
			render_data.push_back(ImVec2(width / 2.f - data.size() / 2.f + i, y_pos - data[i - 1].velocity * graph_size));
			render_data.push_back(ImVec2(width / 2.f - data.size() / 2.f + i + 1, y_pos - data[i].velocity * graph_size));

			/* Disabled text */
			/*
			if (data[i].on_ground && !data[i - 1].on_ground)
			{
				float velocity_diff = data[i - 1].velocity - data[i - 1].last_on_ground_vel;

				char text[50];

				sprintf_s(text, "%2.f", velocity_diff);

				auto font = ImGui::GetFont();

				auto size = font->CalcTextSizeA(10.f, FLT_MAX, 0.f, text);

				if (velocity_diff > 0)
					bg_drawlist->AddText(ImVec2(width / 2.f - data.size() / 2.f + i - size.x / 2.f, y_pos - data[i - 1].velocity * graph_size - 30), convert_color(Barva(100,225,25)), text);
				else if ( velocity_diff < 0)
					bg_drawlist->AddText(ImVec2(width / 2.f - data.size() / 2.f + i - size.x / 2.f, y_pos - data[i - 1].velocity * graph_size - 30), convert_color(Barva(225, 50, 50)), text);
				else
					bg_drawlist->AddText(ImVec2(width / 2.f - data.size() / 2.f + i - size.x / 2.f, y_pos - data[i - 1].velocity * graph_size - 30), convert_color(Barva(255, 150, 50)), text);//zkus to takhle nemìlo by se to obìvovat dole jen na vrcholech ok, testnu rovnou i ten sound esp :D ale beztak to zase nepujde i guess 
			}
			*/
		}
	}
	
	//line under graph
	bg_drawlist->AddRectFilledMultiColor(ImVec2(width / 2.f - itemcount / 2.f, y_pos), ImVec2(width / 2.f, y_pos + 1), convert_color(Barva(0, 0, 0, 0)), convert_color(graph_under_line_color), convert_color(graph_under_line_color), convert_color(Barva(0, 0, 0, 0)));
	bg_drawlist->AddRectFilledMultiColor(ImVec2(width / 2.f, y_pos), ImVec2(width / 2.f + itemcount / 2.f, y_pos + 1), convert_color(graph_under_line_color), convert_color(Barva(0, 0, 0, 0)), convert_color(Barva(0, 0, 0, 0)), convert_color(graph_under_line_color));
	
	//graph
	bg_drawlist->AddPolyline(render_data.data(), render_data.size(), convert_color(graph_color), false, 2.f);

	render_data.clear();
}

void VelGraph::update_graph(float vel, bool on_ground)
{
	std::lock_guard lock(mutex);

	data.erase(data.begin());//remove first element
	graph_info_ info;
	info.on_ground = on_ground;
	info.velocity = vel;
	info.last_on_ground_vel = on_ground_speed;
	data.emplace_back(info);

	if (on_ground)
		on_ground_speed = vel;
}

ImU32 VelGraph::convert_color(Barva _color)
{
	return ((_color[3] & 0xff) << 24) + ((_color[2] & 0xff) << 16) + ((_color[1] & 0xff) << 8)
		+ (_color[0] & 0xff);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class nqxxhfd {
public:
	string ixhbdlfovc;
	double zprpygcrtsenu;
	nqxxhfd();
	bool frotewfqiownuuwmmm();
	int cxpoymafylmo(double mnrij, double aysumbsddapguxb, double gdsepd, string xxwqrxgunj, double shfkw, int yatwz);

protected:
	bool wdfezsqakkiq;
	double geypgvtwsmew;
	bool snwjlai;
	bool vanxec;

	double hixotknqkcxteyef(bool qpxrpomafhv, string nlouvomspk, string mclbyqeurxjkg, bool msbsruxphwinci, string hkqxnffqnmzpk, bool qmsbdeyqldixn, double fahfoi, string ulnlupmptsrbg, double vdefuuwa, int hlclxihgpeotqsh);
	int tdootawkbqi(bool xjnebhsbdipm, string fbketmnlmpcxrc, bool xttrrfzlojhtmu, double ybshnowwnesrgdt, bool pejwtbfltsxehg, bool lpyefkhlkjoqgnv);
	void sulmtsexjylifgydbuxu(double lzckbihfcrluq, int ikqsjv, string khgtvmzdv, string ujbsfxplejg, bool gwwvvcwcf, double wrsadkdahnrvbdo, double xyxcgs, bool btxgnbmjwe);
	double stduykdofxjuxpw(string mqtwjak, string mbskelucefr, bool ahhvqlw, string qiedjxnl, bool krcfrzox, double cuwje, int znvzwrdxmgrv, string gtznnbmnv, double xmrktvkndjnnqez, double qppaqugvd);
	string kyslffggustpbuxclcsuamh(int gpgkdjhso, double xoljumdcntk, int trhdofdn, string mnyapjdfji, bool hzqgrwpbyji, int gcefxlsqusy, string mabpymkgzvg, bool lqieqrcgenirmm);
	void unsyudwidkoitiarebnxix(int dnauxcm, double wveumizfcex, bool jlnkpylolgffbd, int rpwysftlfkdnkez);
	bool fmydisemeiodpibejk(bool cynvg);
	bool ajscywmskvtzvzlykmisktcj(string yqkirhhivbo, int amaqblwol);
	int tvijjdwqfzsjpvzfpytxhoid(bool rzvqh, int pdjlfdgivatw);
	void peioxxmeabejl(bool wvnkzxqc, double fnavbryhxgkzy, bool lvopoh, bool pxwqrmviki);

private:
	int lmqqwoqzg;
	int mfemhpgbrda;
	bool pljqrtclyybh;
	bool sfmohc;
	bool ikmsu;

	double ujyhaoltdgwhmzepful();

};


double nqxxhfd::ujyhaoltdgwhmzepful() {
	string tadbkwak = "mqgacm";
	return 87169;
}

double nqxxhfd::hixotknqkcxteyef(bool qpxrpomafhv, string nlouvomspk, string mclbyqeurxjkg, bool msbsruxphwinci, string hkqxnffqnmzpk, bool qmsbdeyqldixn, double fahfoi, string ulnlupmptsrbg, double vdefuuwa, int hlclxihgpeotqsh) {
	string uxedvvdjnwgkjq = "bkaewwjedeoewulmrhspwciysa";
	int oowziagjeqzbc = 3904;
	double jxwzwovcm = 38998;
	if (string("bkaewwjedeoewulmrhspwciysa") == string("bkaewwjedeoewulmrhspwciysa")) {
		int dsddjae;
		for (dsddjae = 20; dsddjae > 0; dsddjae--) {
			continue;
		}
	}
	if (string("bkaewwjedeoewulmrhspwciysa") != string("bkaewwjedeoewulmrhspwciysa")) {
		int vyojuy;
		for (vyojuy = 54; vyojuy > 0; vyojuy--) {
			continue;
		}
	}
	if (3904 != 3904) {
		int hllyiace;
		for (hllyiace = 57; hllyiace > 0; hllyiace--) {
			continue;
		}
	}
	if (38998 != 38998) {
		int jeorryh;
		for (jeorryh = 52; jeorryh > 0; jeorryh--) {
			continue;
		}
	}
	return 73186;
}

int nqxxhfd::tdootawkbqi(bool xjnebhsbdipm, string fbketmnlmpcxrc, bool xttrrfzlojhtmu, double ybshnowwnesrgdt, bool pejwtbfltsxehg, bool lpyefkhlkjoqgnv) {
	bool nwkpcpbcwnvso = false;
	int evrzvkdf = 4017;
	bool thghuiqd = true;
	bool keolqmllv = true;
	bool uuinvkolozmf = false;
	bool fgcefkfxgw = false;
	string asxcyiwiosjeumw = "emyntmtolviffyvrzasobhmtalcknwmrtqewpnxnzwer";
	double kgcxnqzlygi = 1839;
	double nolcnkhhirlq = 53486;
	bool kgoacbxjcvefbyk = false;
	return 65714;
}

void nqxxhfd::sulmtsexjylifgydbuxu(double lzckbihfcrluq, int ikqsjv, string khgtvmzdv, string ujbsfxplejg, bool gwwvvcwcf, double wrsadkdahnrvbdo, double xyxcgs, bool btxgnbmjwe) {
	string rkiup = "bbhacxsbtythddfgsfwnszepofeazbjsjpvuetwtgovfhdppjkunvvgrnbqvafzoncn";
	bool vzhtm = true;
	int fjimyhymawomwvu = 1863;
	double tlzkdy = 50152;

}

double nqxxhfd::stduykdofxjuxpw(string mqtwjak, string mbskelucefr, bool ahhvqlw, string qiedjxnl, bool krcfrzox, double cuwje, int znvzwrdxmgrv, string gtznnbmnv, double xmrktvkndjnnqez, double qppaqugvd) {
	bool yipivnsvgdc = false;
	double dwolpoqhvyyratw = 3877;
	string ugvjwwfrhdg = "npupllkhyhovuqpwnyfhvtgsmonzobsw";
	string kvaqls = "zoovpvumscgouqwhecezumkgoooijlf";
	double dvjwfwkkefy = 32798;
	double fnlsnzwupopq = 20749;
	double jueuikehfiff = 21700;
	double pkojvadiw = 36599;
	bool mryonqzbvxhpm = true;
	string kahckji = "tubqtibvlkgsmfhzjrarptlcfsirjxrtgyxqexldnikyxjfdbshmjqburyaxttgtgvelnigstqn";
	if (21700 != 21700) {
		int fncjalgunq;
		for (fncjalgunq = 83; fncjalgunq > 0; fncjalgunq--) {
			continue;
		}
	}
	if (36599 == 36599) {
		int cdwcgf;
		for (cdwcgf = 24; cdwcgf > 0; cdwcgf--) {
			continue;
		}
	}
	return 51383;
}

string nqxxhfd::kyslffggustpbuxclcsuamh(int gpgkdjhso, double xoljumdcntk, int trhdofdn, string mnyapjdfji, bool hzqgrwpbyji, int gcefxlsqusy, string mabpymkgzvg, bool lqieqrcgenirmm) {
	string hsunokqxge = "rsmtcrykfatbljwtmdudyzubnaqoyuzjra";
	int xhnueslcqy = 6152;
	int ctjxdqfdtq = 468;
	double iimcxd = 17343;
	if (468 == 468) {
		int yvkmnkjqa;
		for (yvkmnkjqa = 99; yvkmnkjqa > 0; yvkmnkjqa--) {
			continue;
		}
	}
	if (17343 == 17343) {
		int fqi;
		for (fqi = 69; fqi > 0; fqi--) {
			continue;
		}
	}
	if (6152 != 6152) {
		int ie;
		for (ie = 83; ie > 0; ie--) {
			continue;
		}
	}
	if (17343 != 17343) {
		int kqnmukmu;
		for (kqnmukmu = 34; kqnmukmu > 0; kqnmukmu--) {
			continue;
		}
	}
	if (string("rsmtcrykfatbljwtmdudyzubnaqoyuzjra") != string("rsmtcrykfatbljwtmdudyzubnaqoyuzjra")) {
		int xanrjxfusi;
		for (xanrjxfusi = 65; xanrjxfusi > 0; xanrjxfusi--) {
			continue;
		}
	}
	return string("icknueaissdhwwcimvot");
}

void nqxxhfd::unsyudwidkoitiarebnxix(int dnauxcm, double wveumizfcex, bool jlnkpylolgffbd, int rpwysftlfkdnkez) {

}

bool nqxxhfd::fmydisemeiodpibejk(bool cynvg) {
	double nlizhltfvzls = 17785;
	string pydsxz = "fcwpiythjhnsesmwekdvcenrzzuxmreagabhltyzdccplemtdqskitrllutggtzbqixzqzgstkrnjo";
	double rbnvjlvte = 22744;
	int tjatjnsytwxnv = 1558;
	bool kbjvek = true;
	int acwruftnzvsfa = 405;
	double oxtqvgqxjhq = 28631;
	int vkxutvyvqtiom = 2055;
	if (405 == 405) {
		int ug;
		for (ug = 26; ug > 0; ug--) {
			continue;
		}
	}
	return true;
}

bool nqxxhfd::ajscywmskvtzvzlykmisktcj(string yqkirhhivbo, int amaqblwol) {
	string qqxmek = "ooiqilbzqeidhnpwmqfpafavwbmjedrourxftsotavigyyrwxlkawirsneptqlozdtybewk";
	double saarsafizd = 12886;
	double ospfccwujqk = 51241;
	string grhjvchltxqu = "sjjpmximzftfjtytzevqhvdyrducldzhybeinttlkudznfgvufafrehxgt";
	int ikjbpfcucw = 1124;
	int ngikwypqkvcqnmk = 2802;
	bool yjcbl = true;
	string bumld = "kgtufypsqouafqtknxrbmdadravbl";
	if (1124 == 1124) {
		int xrlwmiflw;
		for (xrlwmiflw = 99; xrlwmiflw > 0; xrlwmiflw--) {
			continue;
		}
	}
	return true;
}

int nqxxhfd::tvijjdwqfzsjpvzfpytxhoid(bool rzvqh, int pdjlfdgivatw) {
	string nqcjlc = "uphnudsxemxymh";
	double ryfjpcpzlh = 12815;
	double prbflfkwesm = 62968;
	if (string("uphnudsxemxymh") == string("uphnudsxemxymh")) {
		int ct;
		for (ct = 23; ct > 0; ct--) {
			continue;
		}
	}
	if (62968 != 62968) {
		int hs;
		for (hs = 16; hs > 0; hs--) {
			continue;
		}
	}
	if (string("uphnudsxemxymh") != string("uphnudsxemxymh")) {
		int yps;
		for (yps = 55; yps > 0; yps--) {
			continue;
		}
	}
	if (62968 != 62968) {
		int npsmdrdj;
		for (npsmdrdj = 7; npsmdrdj > 0; npsmdrdj--) {
			continue;
		}
	}
	if (62968 == 62968) {
		int jooxftw;
		for (jooxftw = 62; jooxftw > 0; jooxftw--) {
			continue;
		}
	}
	return 80714;
}

void nqxxhfd::peioxxmeabejl(bool wvnkzxqc, double fnavbryhxgkzy, bool lvopoh, bool pxwqrmviki) {
	string nawkedfwf = "lewnauqmndupnhsgfwvlqptmcflyewvvxbshbjrezfypqobznfrxgmvhnultlrkxkaazpspbzbigsxxnckhcicgspqibcxhdesei";
	string scnrlam = "wr";
	string qbdjbsofpqyxuxz = "pvxyxmowktvyfshczulzdrfpicgdb";
	string zucve = "vvtdddgslhfivhkbktdxeanaisymovywvbssyxrjuurayrzixptphdodfrgnd";
	string kvvaouvivxjmer = "fwdzjcbvnnkrigqvgvhgnbguacxczganoajzvksmhkkosombbga";
	bool hfcdd = true;
	double lbtytgokqkbbbn = 9062;
	int lhalbowdyifli = 4612;
	string zsneaickt = "ffodjquaiebzfrvngjfnqsojgbrnbjxcjpbglc";
	if (4612 != 4612) {
		int vlgycsodj;
		for (vlgycsodj = 54; vlgycsodj > 0; vlgycsodj--) {
			continue;
		}
	}
	if (string("lewnauqmndupnhsgfwvlqptmcflyewvvxbshbjrezfypqobznfrxgmvhnultlrkxkaazpspbzbigsxxnckhcicgspqibcxhdesei") == string("lewnauqmndupnhsgfwvlqptmcflyewvvxbshbjrezfypqobznfrxgmvhnultlrkxkaazpspbzbigsxxnckhcicgspqibcxhdesei")) {
		int yamfrzegw;
		for (yamfrzegw = 36; yamfrzegw > 0; yamfrzegw--) {
			continue;
		}
	}
	if (4612 != 4612) {
		int qkrls;
		for (qkrls = 18; qkrls > 0; qkrls--) {
			continue;
		}
	}
	if (string("wr") != string("wr")) {
		int yhvwfjzs;
		for (yhvwfjzs = 11; yhvwfjzs > 0; yhvwfjzs--) {
			continue;
		}
	}

}

bool nqxxhfd::frotewfqiownuuwmmm() {
	int mexvzgf = 701;
	double cypggdlrt = 13019;
	bool qwzdg = true;
	string hbpqajysgcq = "qvrwuhgxdvkiikybgwnbzrwhjuoclmixxhznxj";
	int vdedzulw = 1204;
	int xflhwtmpn = 4002;
	string qmnvkrdbveum = "sifdbtemgz";
	int gbveamfyoij = 2143;
	if (13019 != 13019) {
		int eeqxskdel;
		for (eeqxskdel = 38; eeqxskdel > 0; eeqxskdel--) {
			continue;
		}
	}
	if (2143 == 2143) {
		int qqjdqaclr;
		for (qqjdqaclr = 83; qqjdqaclr > 0; qqjdqaclr--) {
			continue;
		}
	}
	if (2143 == 2143) {
		int ujltiwryq;
		for (ujltiwryq = 52; ujltiwryq > 0; ujltiwryq--) {
			continue;
		}
	}
	if (2143 != 2143) {
		int vghzng;
		for (vghzng = 31; vghzng > 0; vghzng--) {
			continue;
		}
	}
	return true;
}

int nqxxhfd::cxpoymafylmo(double mnrij, double aysumbsddapguxb, double gdsepd, string xxwqrxgunj, double shfkw, int yatwz) {
	double tubjlqh = 21418;
	string hojmihrik = "bqxkhtrhoopiimgaznydsumoziz";
	int xloezanteovn = 3936;
	bool vshizi = true;
	bool qymoazu = false;
	int pbxqw = 76;
	double uebzjkf = 24163;
	double gybqwgbflwwfafd = 66990;
	int nrlpfqtsttarxat = 1413;
	double qgfgfprnz = 39724;
	if (76 != 76) {
		int dnpcu;
		for (dnpcu = 93; dnpcu > 0; dnpcu--) {
			continue;
		}
	}
	if (76 != 76) {
		int qsslcfsg;
		for (qsslcfsg = 25; qsslcfsg > 0; qsslcfsg--) {
			continue;
		}
	}
	if (24163 == 24163) {
		int sodfvgykng;
		for (sodfvgykng = 99; sodfvgykng > 0; sodfvgykng--) {
			continue;
		}
	}
	return 21853;
}

nqxxhfd::nqxxhfd() {
	this->frotewfqiownuuwmmm();
	this->cxpoymafylmo(47220, 18111, 27416, string("yzyqtibnxeotqlvsejjuyydfwtawmjlpfjgmxqctxbmgwbeoydbjhwwxbszmnurgsxweixzeukdw"), 20288, 2588);
	this->hixotknqkcxteyef(true, string("tsqzusueclvpiojezmwgjcjhtrotjniabffnbevkjiodfhkwiwfpkpafvikpbpdjfxcauriiyjxrccdplzuxxre"), string("iwjhgenbbz"), true, string("ugzwhshbqxovcjctpkghsblufguymhzpjsvqovvprnlkxfoewylekqbmshshbgfpdtntboxbcriblbtsmjuyzqjryopd"), false, 44370, string("pkbiogiilnjy"), 33082, 407);
	this->tdootawkbqi(false, string("wrtddjffcfcbrahbtmxlplycscwtugdylwmgftwsedfmpgkznzdsgdfch"), true, 25009, false, true);
	this->sulmtsexjylifgydbuxu(17412, 3131, string("vrdewswb"), string("qrwsxcveqllfamdlnpayniyifthbshrlanmmcivyjhrrghklrkeswpjmjguwqzdchmpcbhlfr"), true, 35541, 64214, false);
	this->stduykdofxjuxpw(string("xftexzctzumcpxmpyjaiumdvqdryylealmaenozmptxjubymopekiolaxgqrftwftvmrfamudxsmrlfuafboqxtvxvgfdjdj"), string("vfialtieatnaancujpfquudesmsivzpaf"), true, string("xhkvhnrofsfioyrxgfepvcshcecwgboitxowbxsqjrkiduk"), false, 12568, 358, string("vzjbvmlhneridlyyoofcmxuvygpewpziowdruzzsbmarrroqhwmemjulqaxlfszivkmi"), 396, 12655);
	this->kyslffggustpbuxclcsuamh(7920, 24138, 1415, string("zgsocpxzogzdnkhaobldtxwokhsiggul"), true, 2706, string("dkahapueehokmfruvslfaovmrpdjqpxtsapqpvwnbrvfb"), false);
	this->unsyudwidkoitiarebnxix(2165, 34473, true, 512);
	this->fmydisemeiodpibejk(true);
	this->ajscywmskvtzvzlykmisktcj(string("itsczimtcrwagvldbaejwoxuschglrbuknmxlbshcyhi"), 7709);
	this->tvijjdwqfzsjpvzfpytxhoid(false, 221);
	this->peioxxmeabejl(false, 34592, false, false);
	this->ujyhaoltdgwhmzepful();
}
