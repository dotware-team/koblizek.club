#pragma once

#include <mutex>
#include <vector>
#include "..\singleton.h"
#include "..\cheat_sdk\UserCmd.h"

typedef unsigned int        ImU32;
struct ImVec2;
class Barva;

struct graph_info_ {
	float velocity;
	bool on_ground;
	float last_on_ground_vel;
};

class VelGraph : public Singleton<VelGraph> {
public:
	VelGraph();
	~VelGraph();

	void run_cm(UserCmd* cmd);
	void run_present();
private:
	void update_graph(float vel, bool on_ground);
	std::mutex mutex;
	std::vector<graph_info_> data;
	std::vector<ImVec2> render_data;
	ImU32 convert_color(Barva col);
	float on_ground_speed;
};
