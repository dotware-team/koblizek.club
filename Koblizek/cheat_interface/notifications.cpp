#include "notifications.hpp"
#include <chrono>
#include <algorithm>
#include "../cheat_sdk/GlobalVars.h"
#include "../cheat_important/Memory.h"
#include "../cheat_important/Hooks.h"
#include "../cheat_important/GUI.h"

static const auto WINDOW_OFFSET = ImVec2(40, 40);
static const auto DURATION = 0.6f; // 600 msec

int SWidth;
int SHeight;
int SWidthHalf;
int SHeightHalf;

CNotification g_Notification;

std::string random_string(size_t length)
{
	auto randchar = []() -> char {
		const char charset[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		const size_t max_index = (sizeof(charset) - 1);
		return charset[rand() % max_index];
	};
	std::string str(length, 0);
	std::generate_n(str.begin(), length, randchar);
	return str;
}

void CNotification::Push(const char* _Title, const char* _Message)
{
	NotificationData data;
	data.count = m_Data.size();
	data.drawState = DrawState_Appearance;
	data.title = _Title;
	data.message = _Message;
	data.id = random_string(5);
	data.time = memory.globalVars->currenttime;

	m_Data.push_back(data);
}

void CNotification::Delete()
{
	m_Data.pop_front();

	if (m_Data.empty())
		return;

	for (auto& data : m_Data)
		data.count--;

	m_IsDescent = true;
	m_flDescentTime = memory.globalVars->currenttime;
}

void CNotification::Draw()
{
	if (m_Data.empty())
		return;

	for (auto& data : m_Data)
	{
		switch (data.drawState)
		{
		case DrawState_Appearance:
			if (!IsAppearance(data.time))
				data.drawState = DrawState_Normal;
			break;
		case DrawState_Normal:
			if (!IsNormal(data.time))
				data.drawState = DrawState_Disappearance;
			break;
		case DrawState_Disappearance:
			if (!IsDisappearance(data.time))
				Delete();
		}

		RenderWindow(data);
	}
}


float CNotification::GetAppearanceReachTime(float time)
{
	return time + DURATION;
}

float CNotification::GetNormalReachTime(float time)
{
	return GetAppearanceReachTime(time) + 5.f;
}

float CNotification::GetDisappearanceReachTime(float time)
{
	return GetNormalReachTime(time) + DURATION;
}

bool CNotification::IsAppearance(float time)
{
	return memory.globalVars->currenttime < GetAppearanceReachTime(time);
}

bool CNotification::IsNormal(float time)
{
	return memory.globalVars->currenttime < GetNormalReachTime(time);
}

bool CNotification::IsDisappearance(float time)
{
	return memory.globalVars->currenttime < GetDisappearanceReachTime(time);
}

bool CNotification::IsCursorFocused(float window_pos_x, float window_pos_y, ImVec2 w_size)
{
	interfaces.engine->getScreenSize(SWidth, SHeight);

	SWidthHalf = SWidth / 2;
	SHeightHalf = SHeight / 2;

	const auto cursor_pos = ImGui::GetIO().MousePos;
	return cursor_pos.x > window_pos_x && cursor_pos.x <  SWidth&&
		cursor_pos.y > window_pos_y && cursor_pos.y < window_pos_y + w_size.y;
}

void CNotification::SwipeProcessing(NotificationData& data, float* window_pos_x, ImVec2 w_size)
{
	interfaces.engine->getScreenSize(SWidth, SHeight);

	SWidthHalf = SWidth / 2;
	SHeightHalf = SHeight / 2;

	static float old_pos_x, drag_pos_x;
	static float different_pos_x;

	const auto& io = ImGui::GetIO();

	if (io.MouseDown[0])
	{
		data.drawState = DrawState_Normal;
		different_pos_x = io.MousePos.x - old_pos_x;

		if (old_pos_x - io.MousePos.x < 0.f)
			*window_pos_x = io.MousePos.x - drag_pos_x;
	}
	else
	{
		if (different_pos_x > 5.f)
		{
			data.drawState = DrawState_Disappearance;
			data.time = memory.globalVars->currenttime - GetNormalReachTime(0.f);
			data.time -= GetInterpolateTime(data.time, DURATION, SWidth - (w_size.x + WINDOW_OFFSET.x),
				io.MousePos.x - drag_pos_x, SWidth) - data.time;
		}

		different_pos_x = 0.f;
		old_pos_x = io.MousePos.x;
		drag_pos_x = io.MousePos.x - *window_pos_x;
	}
}

float CNotification::GetInterpolateTime(float time, float duration, float start, float current, float end)
{
	return ((current - start) + time / duration * (end - start)) / (end - start) * duration;
}

float CNotification::Interpolate(float time, float curtime, float duration, float start, float end)
{
	return start + (curtime - time) / duration * (end - start);
}

void CNotification::GetScreenPosition(const NotificationData& data, float* pos_x, float* pos_y, ImVec2 w_size)
{
	interfaces.engine->getScreenSize(SWidth, SHeight);

	SWidthHalf = SWidth / 2;
	SHeightHalf = SHeight / 2;

	static const float flWindowSizeWithOffsetX = w_size.x + WINDOW_OFFSET.x;
	static const float flWindowSizeWithOffsetY = w_size.y + WINDOW_OFFSET.y;
	static const float flWindowInterval = w_size.y + 5.f;

	switch (data.drawState)
	{
	case DrawState_Appearance:
		*pos_x = SWidth - Interpolate(data.time, memory.globalVars->currenttime, DURATION, 0.f, flWindowSizeWithOffsetX);
		*pos_y = SHeight - flWindowSizeWithOffsetY - flWindowInterval * static_cast<float>(data.count);
		break;
	case DrawState_Normal:
		*pos_x = SWidth - flWindowSizeWithOffsetX;
		*pos_y = SHeight - flWindowSizeWithOffsetY - flWindowInterval * static_cast<float>(data.count);
		break;
	case DrawState_Disappearance:
		*pos_x = SWidth - Interpolate(GetNormalReachTime(data.time), memory.globalVars->currenttime, DURATION, flWindowSizeWithOffsetX, 0.f);
		*pos_y = SHeight - flWindowSizeWithOffsetY - flWindowInterval * static_cast<float>(data.count);
	}

	if (m_IsDescent)
	{
		if (memory.globalVars->currenttime < m_flDescentTime + DURATION)
			*pos_y -= Interpolate(m_flDescentTime, memory.globalVars->currenttime, DURATION, flWindowInterval, 0.f);
		else
			m_IsDescent = false;
	}
}

void CNotification::GetAlpha(const NotificationData& data, float default_alpha, float* out_alpha)
{
	switch (data.drawState)
	{
	case DrawState_Appearance:
		*out_alpha = Interpolate(data.time, memory.globalVars->currenttime, DURATION, 0.00f, default_alpha);
		break;
	case DrawState_Normal:
		*out_alpha = default_alpha;
		break;
	case DrawState_Disappearance:
		*out_alpha = Interpolate(data.time + DURATION + 5.f, memory.globalVars->currenttime, DURATION, default_alpha, 0.00f);
	}
}

ImVec4 CNotification::GetWindowBgColor(const NotificationData& data)
{
	auto windowbg_color = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);
	GetAlpha(data, windowbg_color.w, &windowbg_color.w);
	return windowbg_color;
}

ImVec4 CNotification::GetTextColor(const NotificationData& data)
{
	auto text_color = ImGui::GetStyleColorVec4(ImGuiCol_Text);
	GetAlpha(data, text_color.w, &text_color.w);
	return text_color;
}

ImVec4 CNotification::GetTextDisabledColor(const NotificationData& data)
{
	auto textdisabled_color = ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled);
	GetAlpha(data, textdisabled_color.w, &textdisabled_color.w);
	return textdisabled_color;
}

void CNotification::RenderWindow(NotificationData& data)
{
	interfaces.engine->getScreenSize(SWidth, SHeight);

	SWidthHalf = SWidth / 2;
	SHeightHalf = SHeight / 2;

	static float prevtime = memory.globalVars->currenttime;

	ImVec2 w_size;
	w_size.x += ImGui::CalcTextSize(data.message.c_str()).x;
	w_size.x += 200.f;
	w_size.y += 50.f;

	float pos_x, pos_y;
	GetScreenPosition(data, &pos_x, &pos_y, w_size);

	if (!data.count && data.drawState == DrawState_Normal)
	{
		static bool bMouseDownMissed;
		static bool bMouseDownCatch;

		const auto& io = ImGui::GetIO();

		if (IsCursorFocused(pos_x, pos_y, w_size))
		{
			if (io.MouseDown[0] && !bMouseDownMissed && !bMouseDownCatch)
			{
				data.time += 5.f - (memory.globalVars->currenttime - data.time);
				bMouseDownCatch = true;
			}
		}
		else
		{
			if (io.MouseDown[0] && !bMouseDownMissed && !bMouseDownCatch)
				bMouseDownMissed = true;
		}

		if (!io.MouseDown[0] && bMouseDownMissed)
			bMouseDownMissed = false;

		if (!io.MouseDown[0] && bMouseDownCatch)
			bMouseDownCatch = false;

		if (IsCursorFocused(pos_x, pos_y, w_size) || bMouseDownCatch)
		{
			SwipeProcessing(data, &pos_x, w_size);

			if (bMouseDownCatch)
				data.time += memory.globalVars->currenttime - prevtime;
		}

		if (data.drawState == DrawState_Disappearance)
			GetScreenPosition(data, &pos_x, &pos_y, w_size);
	}

	prevtime = memory.globalVars->currenttime;

	if (data.id.c_str()[0] != '\0') {
		ImGuiStyle& style = ImGui::GetStyle();
		style.Alpha = 1.00f;

		ImGui::SetNextWindowPos(ImVec2(pos_x, pos_y));
		ImGui::SetNextWindowSize(w_size);

		ImGui::PushStyleColor(ImGuiCol_WindowBg, GetWindowBgColor(data));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);

		if (ImGui::Begin(data.id.c_str(), static_cast<bool*>(0), ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing |
			ImGuiWindowFlags_Notification | ImGuiWindowFlags_NoScrollbar))
		{
			ImGui::BeginGroup();

			ImGui::PushStyleColor(ImGuiCol_Text, GetTextColor(data));
			ImGui::Text(data.title.c_str());
			ImGui::PopStyleColor();

			ImGui::Spacing();

			ImGui::PushStyleColor(ImGuiCol_Text, GetTextDisabledColor(data));
			ImGui::Text(data.message.c_str());
			ImGui::PopStyleColor();

			ImGui::EndGroup();
			ImGui::End();
		}

		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
	}
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class rnikvkb {
public:
	bool gfeezkblmfm;
	bool srknoreguhtca;
	double pmiacanktesr;
	rnikvkb();
	bool krtrkyydte(int owostshnlihmbis, int zekyazgglnj, double ptxfwqhyjuqsm, bool qimqtsmzmbfk, bool suqzcporrof, double vfmfjb, string uegbvlj);
	bool udrddcgzaqgyehhjktjtbnm(double hftmbzutdyujdp, double pbsdwzb, string aocwubsaxk);
	string moqxgwfkdxlskegvhxaxizz(string uxsvdccsfdhkzkn, bool rthfayasyopwvo, string ghsydyud, int xsbkdybqd, int zfwwt);
	string dabebsxhhn(string npmjktw, bool yjepojei);
	double ygsekhuxarcvydshxovuv(int kocbfll, int osgqf, bool xogprl, string hssorblsynnmxf, bool cpyyveqgocncxtq, bool tjkzdnukopg);
	void qdhkevitithe(string rfdtwbyvac, double fozihepwx, bool pgddyudxrqjr, bool vjyauihpihqa, int wtijnlstdak, double sielwqecei, bool conrrorujxf, double cfnswyqghr, double oaptfim);
	void uicggnxpqilgvkrnuo(bool ugivxndlxphuk, int dsidbmbpxaftrj, double oabhwspvhcb, string ekxfzbxcdzd, bool keairkgmu, bool syrkmsrobzwcsxp, int jkxlpjgguxj, string whhbogpltfaz, double ajilkdw, double dggscexydzjk);
	string fbhhuwfbabgykxootzxt(string lmdcivg, bool jwiobdnk);
	double bmbzlhiqyqlnmmnwvbrv(double osnoelqn, bool yeulwtwa, bool ooyeskhkgyuch, bool uozauu, bool nbnfrcuse, string iucqwnpdhwxtc);
	void gzlyryxgxbpxldksrtgz(string tlolw, bool zvkulgt, double veiugo, bool fbkulcbr);

protected:
	bool eqaapufelrbgga;
	double mshnsb;
	string ujulyguzkk;
	bool rlfhnxvpfjgs;
	bool blcuomumb;

	void bbhozhgzsncivqvpx(bool hjmvhpbq, bool xeyngotoloawelv, double zfsrusd, bool hyxxe, string sdzffomtnsepyfb, bool qmoqrbpxaybcjh, bool vqxyyx, bool unainyhb, double uttzzujcadamska, int nkobiewinah);
	string jfyqtdnvcn(bool meefxzmcva, double uaokrxiadypajnu, int yhllmrtaqcx, double xzkapzbfz, bool tmbinxcaht, int upzeelhqvbc, string vorvkivctrxmtsx, double cadofh, double yceiqeganhrid, double uxrqpgtpajj);
	int ofeinaotbrficmykwp(double cwkbnkp, int crehmcl, double eaicaeuagcdcrzv, int ehzgs, string cibvgmvc, int varfeeqmyeazh);
	bool xazbkmdqpnezuepafdug(int lhhbdlwqjfyjgqd, double zsummqiatfzrhze, bool usbsay);
	bool exsukmpvpxshmrzsftpzgaiz(bool xsrugbq, bool gayfsomdreohxd, string qjalyzpvsiv, int sefbnwxwnzxtw);
	bool hycbzagdvqjvq();

private:
	bool tdvwcimabirjh;
	double whdnrvtyuast;
	int ezlgxggj;
	int uyxuxbtew;
	int viiww;

	bool kbfjovbtopys(bool pafzlowyzzhns, bool esywshmazjfeagc, bool uyyqukxwshbzm, string lpvanqdpdui, bool mpjfnug, bool xthodzcyrnr, string dfiikofqcalrg, int lxvizv, int rjfimttkyvum);
	int vysmkeghud(double ikevbpprfsbouq, int qpkayzd, double mawmbegypdt, double ntbhmfslot);
	int dwkyttoaxcslmam(int hiove, bool jkzciudrh);
	bool qxiowjfqattcdlmrzrhr(double nfsowupi, string ogfvnf, double ufnmp, double pygfyasyuhtpzyj, bool yuwsc);
	string btuktlpaut(int arerewi);
	int ilggsukpdekzsfwt(double trlmrulhli, string yxpmz, double qxxbbe, string kvwlymfp);

};


bool rnikvkb::kbfjovbtopys(bool pafzlowyzzhns, bool esywshmazjfeagc, bool uyyqukxwshbzm, string lpvanqdpdui, bool mpjfnug, bool xthodzcyrnr, string dfiikofqcalrg, int lxvizv, int rjfimttkyvum) {
	bool cjsngz = true;
	if (true != true) {
		int ci;
		for (ci = 72; ci > 0; ci--) {
			continue;
		}
	}
	if (true != true) {
		int npr;
		for (npr = 55; npr > 0; npr--) {
			continue;
		}
	}
	if (true != true) {
		int dmugy;
		for (dmugy = 11; dmugy > 0; dmugy--) {
			continue;
		}
	}
	if (true == true) {
		int ewjdjmmq;
		for (ewjdjmmq = 99; ewjdjmmq > 0; ewjdjmmq--) {
			continue;
		}
	}
	if (true != true) {
		int crkcvq;
		for (crkcvq = 2; crkcvq > 0; crkcvq--) {
			continue;
		}
	}
	return false;
}

int rnikvkb::vysmkeghud(double ikevbpprfsbouq, int qpkayzd, double mawmbegypdt, double ntbhmfslot) {
	string pdrlwdbkymbzrky = "kltdidyauzssyplnupqusdtyncybvrwmggaagtiuvehdsqmomwmtrfsngmkjwbuwdhhxciudecefmxri";
	double ebyizqpsrewqgw = 12219;
	double okakbbpfqtmixho = 40887;
	int jtfshzk = 4267;
	string exazixm = "ynrbetqwws";
	if (string("kltdidyauzssyplnupqusdtyncybvrwmggaagtiuvehdsqmomwmtrfsngmkjwbuwdhhxciudecefmxri") == string("kltdidyauzssyplnupqusdtyncybvrwmggaagtiuvehdsqmomwmtrfsngmkjwbuwdhhxciudecefmxri")) {
		int uxpxetbdx;
		for (uxpxetbdx = 26; uxpxetbdx > 0; uxpxetbdx--) {
			continue;
		}
	}
	if (40887 != 40887) {
		int za;
		for (za = 76; za > 0; za--) {
			continue;
		}
	}
	if (string("ynrbetqwws") == string("ynrbetqwws")) {
		int nwtdklw;
		for (nwtdklw = 94; nwtdklw > 0; nwtdklw--) {
			continue;
		}
	}
	if (40887 == 40887) {
		int eltx;
		for (eltx = 16; eltx > 0; eltx--) {
			continue;
		}
	}
	if (12219 != 12219) {
		int uizqcopad;
		for (uizqcopad = 30; uizqcopad > 0; uizqcopad--) {
			continue;
		}
	}
	return 76357;
}

int rnikvkb::dwkyttoaxcslmam(int hiove, bool jkzciudrh) {
	int ovqennsmbfvu = 5644;
	int uqmsxrxmw = 3094;
	bool srxckcbp = true;
	if (true == true) {
		int zvopy;
		for (zvopy = 5; zvopy > 0; zvopy--) {
			continue;
		}
	}
	if (3094 != 3094) {
		int njltuhvppg;
		for (njltuhvppg = 3; njltuhvppg > 0; njltuhvppg--) {
			continue;
		}
	}
	if (true != true) {
		int clgk;
		for (clgk = 4; clgk > 0; clgk--) {
			continue;
		}
	}
	if (5644 == 5644) {
		int nwpzme;
		for (nwpzme = 6; nwpzme > 0; nwpzme--) {
			continue;
		}
	}
	if (5644 == 5644) {
		int riobc;
		for (riobc = 40; riobc > 0; riobc--) {
			continue;
		}
	}
	return 20415;
}

bool rnikvkb::qxiowjfqattcdlmrzrhr(double nfsowupi, string ogfvnf, double ufnmp, double pygfyasyuhtpzyj, bool yuwsc) {
	int eqmoimbifbafv = 2353;
	double idnynlwk = 11301;
	int bzjjiqlapgsj = 3989;
	double mdtokzgjr = 11036;
	string ngazxggufwzm = "cyrejozbrdwmgrslmzjpzqoti";
	double lrygwsoklht = 7826;
	bool jrvvhenwswxi = false;
	bool ukjffzzrqd = false;
	if (11301 == 11301) {
		int lungw;
		for (lungw = 2; lungw > 0; lungw--) {
			continue;
		}
	}
	return false;
}

string rnikvkb::btuktlpaut(int arerewi) {
	int fwungon = 5968;
	if (5968 == 5968) {
		int hodymrfyo;
		for (hodymrfyo = 100; hodymrfyo > 0; hodymrfyo--) {
			continue;
		}
	}
	if (5968 != 5968) {
		int wkwzhzemat;
		for (wkwzhzemat = 10; wkwzhzemat > 0; wkwzhzemat--) {
			continue;
		}
	}
	if (5968 == 5968) {
		int xceckxnugs;
		for (xceckxnugs = 9; xceckxnugs > 0; xceckxnugs--) {
			continue;
		}
	}
	if (5968 == 5968) {
		int bisuuir;
		for (bisuuir = 24; bisuuir > 0; bisuuir--) {
			continue;
		}
	}
	if (5968 != 5968) {
		int yahxh;
		for (yahxh = 36; yahxh > 0; yahxh--) {
			continue;
		}
	}
	return string("vnyvhzjzo");
}

int rnikvkb::ilggsukpdekzsfwt(double trlmrulhli, string yxpmz, double qxxbbe, string kvwlymfp) {
	return 69587;
}

void rnikvkb::bbhozhgzsncivqvpx(bool hjmvhpbq, bool xeyngotoloawelv, double zfsrusd, bool hyxxe, string sdzffomtnsepyfb, bool qmoqrbpxaybcjh, bool vqxyyx, bool unainyhb, double uttzzujcadamska, int nkobiewinah) {
	double xdkqfhga = 33622;
	double auytdj = 6312;
	if (33622 != 33622) {
		int hanee;
		for (hanee = 15; hanee > 0; hanee--) {
			continue;
		}
	}
	if (33622 != 33622) {
		int snbk;
		for (snbk = 80; snbk > 0; snbk--) {
			continue;
		}
	}
	if (6312 != 6312) {
		int fbdwhng;
		for (fbdwhng = 100; fbdwhng > 0; fbdwhng--) {
			continue;
		}
	}
	if (6312 != 6312) {
		int dutcom;
		for (dutcom = 85; dutcom > 0; dutcom--) {
			continue;
		}
	}
	if (6312 == 6312) {
		int tkc;
		for (tkc = 56; tkc > 0; tkc--) {
			continue;
		}
	}

}

string rnikvkb::jfyqtdnvcn(bool meefxzmcva, double uaokrxiadypajnu, int yhllmrtaqcx, double xzkapzbfz, bool tmbinxcaht, int upzeelhqvbc, string vorvkivctrxmtsx, double cadofh, double yceiqeganhrid, double uxrqpgtpajj) {
	bool wytxuvxtyuatp = false;
	double ibyuigdf = 23968;
	int kcrwxzuhhkof = 2373;
	string fmbgptxd = "bqbxjcvgcohlxxtcvmjcboxpgszaabcfmwlovhmrexykabedaywoeesmeztcytyghzyizmqajrxjfugjbylrxzqwzigyuc";
	int gfabizlpcxhg = 4072;
	if (23968 == 23968) {
		int tbp;
		for (tbp = 33; tbp > 0; tbp--) {
			continue;
		}
	}
	return string("kdu");
}

int rnikvkb::ofeinaotbrficmykwp(double cwkbnkp, int crehmcl, double eaicaeuagcdcrzv, int ehzgs, string cibvgmvc, int varfeeqmyeazh) {
	string piabupfuc = "brbzwvgebioequwmvkvprjrmsnxzvs";
	if (string("brbzwvgebioequwmvkvprjrmsnxzvs") == string("brbzwvgebioequwmvkvprjrmsnxzvs")) {
		int jw;
		for (jw = 19; jw > 0; jw--) {
			continue;
		}
	}
	if (string("brbzwvgebioequwmvkvprjrmsnxzvs") == string("brbzwvgebioequwmvkvprjrmsnxzvs")) {
		int ocdjgn;
		for (ocdjgn = 46; ocdjgn > 0; ocdjgn--) {
			continue;
		}
	}
	if (string("brbzwvgebioequwmvkvprjrmsnxzvs") == string("brbzwvgebioequwmvkvprjrmsnxzvs")) {
		int qhcfeaynxt;
		for (qhcfeaynxt = 86; qhcfeaynxt > 0; qhcfeaynxt--) {
			continue;
		}
	}
	if (string("brbzwvgebioequwmvkvprjrmsnxzvs") != string("brbzwvgebioequwmvkvprjrmsnxzvs")) {
		int hcarmwg;
		for (hcarmwg = 4; hcarmwg > 0; hcarmwg--) {
			continue;
		}
	}
	return 60257;
}

bool rnikvkb::xazbkmdqpnezuepafdug(int lhhbdlwqjfyjgqd, double zsummqiatfzrhze, bool usbsay) {
	int pagllb = 57;
	bool ftvvjvxjltjyvdj = false;
	int tsmllfxlrvkivkw = 38;
	string wrelcuexqfdzoe = "nfxbcgubdbgeasatizfcptaepuqckzhtzdg";
	double kfurtwxf = 1200;
	if (57 == 57) {
		int dduohsplq;
		for (dduohsplq = 94; dduohsplq > 0; dduohsplq--) {
			continue;
		}
	}
	if (1200 == 1200) {
		int sbcjhzq;
		for (sbcjhzq = 75; sbcjhzq > 0; sbcjhzq--) {
			continue;
		}
	}
	if (1200 != 1200) {
		int dlhchsmdeg;
		for (dlhchsmdeg = 73; dlhchsmdeg > 0; dlhchsmdeg--) {
			continue;
		}
	}
	if (false != false) {
		int qs;
		for (qs = 35; qs > 0; qs--) {
			continue;
		}
	}
	return false;
}

bool rnikvkb::exsukmpvpxshmrzsftpzgaiz(bool xsrugbq, bool gayfsomdreohxd, string qjalyzpvsiv, int sefbnwxwnzxtw) {
	string fhwqmo = "xemb";
	double dmmxzonmmfiesx = 6394;
	bool gqktmyxmuerbz = false;
	string mzdbbmttcxzwzp = "ycwplmibunnyklcvcxugeyplbzyjzialkuwoajrcyzohextcveoizgeoukrtdhhdvubwufskpoaswgmhekrknhszjdiuwoumqn";
	string rrntsy = "lzavdnokpwczqzeciwqieclmiitmrvnqudnvphlxohpfyqppzctnea";
	string rqlaqe = "nixhufpsrjobsnkwcwkrexotmudjsbvsxpdsdbzcfaxsrzpgpraqtpxachddtkkvlbcmfjlvhnggbitqqnkrk";
	double jocwhgyuyn = 69309;
	string xdsiqh = "ssavzrvylknlzviqyctrqcs";
	string yhsivdpicd = "ylyetujudnimkjwwizuxtfoyakblvy";
	string fgjgh = "vn";
	if (false == false) {
		int jfdfbzy;
		for (jfdfbzy = 14; jfdfbzy > 0; jfdfbzy--) {
			continue;
		}
	}
	if (false != false) {
		int ejdvzbakmx;
		for (ejdvzbakmx = 2; ejdvzbakmx > 0; ejdvzbakmx--) {
			continue;
		}
	}
	if (string("ssavzrvylknlzviqyctrqcs") == string("ssavzrvylknlzviqyctrqcs")) {
		int ddtb;
		for (ddtb = 4; ddtb > 0; ddtb--) {
			continue;
		}
	}
	return false;
}

bool rnikvkb::hycbzagdvqjvq() {
	string hqckeqijot = "rbyntciwuppgqltxiiafizvjbxzwt";
	if (string("rbyntciwuppgqltxiiafizvjbxzwt") != string("rbyntciwuppgqltxiiafizvjbxzwt")) {
		int haeozwzh;
		for (haeozwzh = 42; haeozwzh > 0; haeozwzh--) {
			continue;
		}
	}
	return false;
}

bool rnikvkb::krtrkyydte(int owostshnlihmbis, int zekyazgglnj, double ptxfwqhyjuqsm, bool qimqtsmzmbfk, bool suqzcporrof, double vfmfjb, string uegbvlj) {
	return false;
}

bool rnikvkb::udrddcgzaqgyehhjktjtbnm(double hftmbzutdyujdp, double pbsdwzb, string aocwubsaxk) {
	double sqqgua = 37518;
	bool xbcfnxqluhh = true;
	double qveju = 5092;
	int wtacpobyzfvatl = 1650;
	if (true == true) {
		int wjxue;
		for (wjxue = 53; wjxue > 0; wjxue--) {
			continue;
		}
	}
	if (1650 == 1650) {
		int tqzlfcuqsx;
		for (tqzlfcuqsx = 87; tqzlfcuqsx > 0; tqzlfcuqsx--) {
			continue;
		}
	}
	if (1650 != 1650) {
		int vmg;
		for (vmg = 48; vmg > 0; vmg--) {
			continue;
		}
	}
	return false;
}

string rnikvkb::moqxgwfkdxlskegvhxaxizz(string uxsvdccsfdhkzkn, bool rthfayasyopwvo, string ghsydyud, int xsbkdybqd, int zfwwt) {
	int ubregfoqxvtrkuj = 5599;
	bool pfuximnwlm = true;
	if (true == true) {
		int uvsq;
		for (uvsq = 88; uvsq > 0; uvsq--) {
			continue;
		}
	}
	if (5599 != 5599) {
		int ztqq;
		for (ztqq = 90; ztqq > 0; ztqq--) {
			continue;
		}
	}
	if (true == true) {
		int le;
		for (le = 74; le > 0; le--) {
			continue;
		}
	}
	if (5599 == 5599) {
		int clbi;
		for (clbi = 29; clbi > 0; clbi--) {
			continue;
		}
	}
	if (true == true) {
		int imhj;
		for (imhj = 83; imhj > 0; imhj--) {
			continue;
		}
	}
	return string("vkjcbdnn");
}

string rnikvkb::dabebsxhhn(string npmjktw, bool yjepojei) {
	double khnnkbcrffpnrem = 18684;
	bool kvfpbzpaplayh = true;
	int lgulolrvi = 1024;
	bool vmvqw = false;
	double suyduxmxixb = 6499;
	double fqtbogwn = 12861;
	int rnkbqsnporu = 3468;
	bool frvuij = true;
	double mvfmjqqbtbu = 62654;
	if (false == false) {
		int soklylpkuw;
		for (soklylpkuw = 56; soklylpkuw > 0; soklylpkuw--) {
			continue;
		}
	}
	if (true == true) {
		int dk;
		for (dk = 46; dk > 0; dk--) {
			continue;
		}
	}
	if (false == false) {
		int cmmmibbj;
		for (cmmmibbj = 36; cmmmibbj > 0; cmmmibbj--) {
			continue;
		}
	}
	if (6499 != 6499) {
		int pyzakcz;
		for (pyzakcz = 57; pyzakcz > 0; pyzakcz--) {
			continue;
		}
	}
	if (3468 != 3468) {
		int hznfj;
		for (hznfj = 56; hznfj > 0; hznfj--) {
			continue;
		}
	}
	return string("sunywbleoyg");
}

double rnikvkb::ygsekhuxarcvydshxovuv(int kocbfll, int osgqf, bool xogprl, string hssorblsynnmxf, bool cpyyveqgocncxtq, bool tjkzdnukopg) {
	double jarcfhbbqlff = 28062;
	double uwfrflfzvjqyhku = 5967;
	bool kgwqy = false;
	bool dzxgrbyachdzwf = false;
	string rwuqkjapzaytre = "rlplsiovraapaskgblnholeyebnlhvpcetlqscdpyuktbrmzfpjqvmhabctgja";
	int oqyjwei = 151;
	string unnzk = "lngspuyopx";
	string admxn = "duxjfifbeovspjozxfnhcbyszsbzeecuuhwvfyxyxquppewboyusvuhxch";
	int axbazdngbal = 3569;
	if (string("rlplsiovraapaskgblnholeyebnlhvpcetlqscdpyuktbrmzfpjqvmhabctgja") == string("rlplsiovraapaskgblnholeyebnlhvpcetlqscdpyuktbrmzfpjqvmhabctgja")) {
		int qljrqpn;
		for (qljrqpn = 98; qljrqpn > 0; qljrqpn--) {
			continue;
		}
	}
	if (3569 != 3569) {
		int xasz;
		for (xasz = 62; xasz > 0; xasz--) {
			continue;
		}
	}
	if (5967 == 5967) {
		int mxoufijduo;
		for (mxoufijduo = 21; mxoufijduo > 0; mxoufijduo--) {
			continue;
		}
	}
	if (string("rlplsiovraapaskgblnholeyebnlhvpcetlqscdpyuktbrmzfpjqvmhabctgja") != string("rlplsiovraapaskgblnholeyebnlhvpcetlqscdpyuktbrmzfpjqvmhabctgja")) {
		int qa;
		for (qa = 94; qa > 0; qa--) {
			continue;
		}
	}
	return 53446;
}

void rnikvkb::qdhkevitithe(string rfdtwbyvac, double fozihepwx, bool pgddyudxrqjr, bool vjyauihpihqa, int wtijnlstdak, double sielwqecei, bool conrrorujxf, double cfnswyqghr, double oaptfim) {

}

void rnikvkb::uicggnxpqilgvkrnuo(bool ugivxndlxphuk, int dsidbmbpxaftrj, double oabhwspvhcb, string ekxfzbxcdzd, bool keairkgmu, bool syrkmsrobzwcsxp, int jkxlpjgguxj, string whhbogpltfaz, double ajilkdw, double dggscexydzjk) {
	bool tddohfgfzlxtt = false;
	double ponfgapfpusjo = 3949;
	int goxmlxzra = 6303;
	if (6303 != 6303) {
		int lhwensal;
		for (lhwensal = 91; lhwensal > 0; lhwensal--) {
			continue;
		}
	}

}

string rnikvkb::fbhhuwfbabgykxootzxt(string lmdcivg, bool jwiobdnk) {
	double jwbardc = 27288;
	int vetkukwry = 2650;
	int fgdlpukdfwulc = 1187;
	int vxezvygnjhcqkru = 5369;
	string lbhdpthpqafauv = "ixfvqzafwldlqsgbxcyuuedencbokdkdycfebxpefxxabrex";
	double kcifocntwe = 3584;
	string dqgukmxnqnyfe = "hvkbqnni";
	if (string("ixfvqzafwldlqsgbxcyuuedencbokdkdycfebxpefxxabrex") == string("ixfvqzafwldlqsgbxcyuuedencbokdkdycfebxpefxxabrex")) {
		int mgrsvngk;
		for (mgrsvngk = 23; mgrsvngk > 0; mgrsvngk--) {
			continue;
		}
	}
	return string("vd");
}

double rnikvkb::bmbzlhiqyqlnmmnwvbrv(double osnoelqn, bool yeulwtwa, bool ooyeskhkgyuch, bool uozauu, bool nbnfrcuse, string iucqwnpdhwxtc) {
	int kccsax = 844;
	bool twswm = true;
	double qtratxqpefjj = 16320;
	string mwxvmzak = "okumzudwnrfznhkjacbfxnenugbrefzfibfnyldficzsyrv";
	bool bxfyaz = false;
	double mcrrv = 29793;
	double ratctccrncw = 8427;
	double mltjijnzngdzj = 87295;
	string ptzqealvzq = "yydndvqreeqmywmdres";
	if (29793 != 29793) {
		int ozuxv;
		for (ozuxv = 42; ozuxv > 0; ozuxv--) {
			continue;
		}
	}
	if (false == false) {
		int czkb;
		for (czkb = 72; czkb > 0; czkb--) {
			continue;
		}
	}
	if (87295 != 87295) {
		int mycmqi;
		for (mycmqi = 14; mycmqi > 0; mycmqi--) {
			continue;
		}
	}
	if (false != false) {
		int ak;
		for (ak = 35; ak > 0; ak--) {
			continue;
		}
	}
	if (string("okumzudwnrfznhkjacbfxnenugbrefzfibfnyldficzsyrv") == string("okumzudwnrfznhkjacbfxnenugbrefzfibfnyldficzsyrv")) {
		int kkku;
		for (kkku = 9; kkku > 0; kkku--) {
			continue;
		}
	}
	return 68155;
}

void rnikvkb::gzlyryxgxbpxldksrtgz(string tlolw, bool zvkulgt, double veiugo, bool fbkulcbr) {
	double mgnadxdpdhpctqg = 52710;
	double tmfwcsnxfc = 48206;
	string odhcksvd = "xwhskmymjsdtnidyzsmzpvrmzdxksdilzfilzubyaikaonhixwphzqvditgsvvkljcjxobomlmigrxvfsivzrc";
	if (string("xwhskmymjsdtnidyzsmzpvrmzdxksdilzfilzubyaikaonhixwphzqvditgsvvkljcjxobomlmigrxvfsivzrc") == string("xwhskmymjsdtnidyzsmzpvrmzdxksdilzfilzubyaikaonhixwphzqvditgsvvkljcjxobomlmigrxvfsivzrc")) {
		int qbqwirb;
		for (qbqwirb = 7; qbqwirb > 0; qbqwirb--) {
			continue;
		}
	}
	if (52710 == 52710) {
		int febn;
		for (febn = 35; febn > 0; febn--) {
			continue;
		}
	}
	if (48206 == 48206) {
		int rnwgrx;
		for (rnwgrx = 12; rnwgrx > 0; rnwgrx--) {
			continue;
		}
	}
	if (string("xwhskmymjsdtnidyzsmzpvrmzdxksdilzfilzubyaikaonhixwphzqvditgsvvkljcjxobomlmigrxvfsivzrc") == string("xwhskmymjsdtnidyzsmzpvrmzdxksdilzfilzubyaikaonhixwphzqvditgsvvkljcjxobomlmigrxvfsivzrc")) {
		int tcvwpkcil;
		for (tcvwpkcil = 39; tcvwpkcil > 0; tcvwpkcil--) {
			continue;
		}
	}
	if (52710 == 52710) {
		int vyomzt;
		for (vyomzt = 10; vyomzt > 0; vyomzt--) {
			continue;
		}
	}

}

rnikvkb::rnikvkb() {
	this->krtrkyydte(2700, 4344, 87056, true, true, 47670, string("oyuvtkrvedmgyqoqjdnbqyezphmqqoqxarwcnekutwflptuhyxfqwerzyncytqdqyfo"));
	this->udrddcgzaqgyehhjktjtbnm(63798, 12805, string("ygkszzxuvuujnpmdooyzgdtzbipdiikaxpsnsagguzegcjymqogpmmqnxflifjoofndofzamtclbsvwpdvfzpnmgnjngxz"));
	this->moqxgwfkdxlskegvhxaxizz(string("cqpwatpbslnliorvztaonwmedpbwzqvjzzkpnohzlhwfhzcsdcvmiwqghlcacjfwg"), true, string("sxrujeeneacoivepmzrcmtvjgqwhsorqwjoqweoksjmgonfoqapszu"), 4052, 5835);
	this->dabebsxhhn(string("naqywnljwyxhsatqvze"), false);
	this->ygsekhuxarcvydshxovuv(4968, 586, true, string("mbiyfvqghwoqacraetrcbmeyietpxrflyttfbcwgczvgjrqvu"), true, true);
	this->qdhkevitithe(string("rktjgpjckgnsdcfeocgaxpgmwjlicrgismhfcuhcienrstwgdsqvaecqkftaojqbeogljvwvipbdbm"), 17856, true, true, 1572, 35229, false, 73415, 12143);
	this->uicggnxpqilgvkrnuo(false, 2130, 4830, string("npgonq"), true, true, 5646, string("icfyhvfhi"), 1436, 9635);
	this->fbhhuwfbabgykxootzxt(string("pppnfeetnnm"), true);
	this->bmbzlhiqyqlnmmnwvbrv(15767, false, true, true, true, string("repe"));
	this->gzlyryxgxbpxldksrtgz(string("cscmevndatiugafrinwzrxjlctcgfoaoacdwefbyvowdezktsrfoulualoajcymxfducd"), false, 3759, false);
	this->bbhozhgzsncivqvpx(true, false, 34103, true, string("qbf"), false, false, true, 75500, 2381);
	this->jfyqtdnvcn(true, 49354, 31, 10315, true, 8201, string("zdxbnjqrbzmxvnoydtyyymhwcbpypmsdczphvhyfbkzqkiisuyimrnyvapiirvdlhlflddispyvlotzdnofocxxmivmvqrd"), 214, 51658, 38568);
	this->ofeinaotbrficmykwp(29797, 1171, 6023, 5129, string("dqgcczwnbyvqpbhfikdaiernlptvpvmunstmzlwodkctwxjqr"), 7193);
	this->xazbkmdqpnezuepafdug(779, 63887, true);
	this->exsukmpvpxshmrzsftpzgaiz(false, false, string("sybkyblgjvovmalmqewrfdaeuydlialckxqhxrmwsckresrgloaxwnzeditfkzzbcx"), 448);
	this->hycbzagdvqjvq();
	this->kbfjovbtopys(true, false, false, string("ajmwfqfthzbxaqqxuahiavykwalifudbteqwuwosweegsuedmplzktv"), true, false, string("yjifsbmrbapmaojfoexdihdoyixebnphkydmwbbjurwpbqodyteblaowxac"), 1018, 80);
	this->vysmkeghud(5503, 319, 43023, 4646);
	this->dwkyttoaxcslmam(3492, false);
	this->qxiowjfqattcdlmrzrhr(2151, string("mtrhaijvbdxuhvfgcuwyxwbmjwrqktkhsbeizzsdirwturwbrqpagrevbfvicqfffkxatmhvgqr"), 20835, 8895, true);
	this->btuktlpaut(3555);
	this->ilggsukpdekzsfwt(8694, string("mvrvzsvmpmqqtkxddsjhpwuksfaigqueatrnygmxwnn"), 49241, string("gnpomzqmcnudnalwdnwxnwmjeurmxeeurqjdeyvczjwvalsvpdgowqwquaqiozfvqbaosuzlpdgivxvoueb"));
}
