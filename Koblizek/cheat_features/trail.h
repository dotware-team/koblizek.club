#pragma once

#include "..\singleton.h"

struct Barva;
class Vector;

class Trail : public Singleton<Trail> {
public:
	void run_cm();
private:
	void render_bean(Vector start, Vector end, Barva color);
};