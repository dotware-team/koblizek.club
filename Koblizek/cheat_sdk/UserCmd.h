#pragma once

#include "Vector.h"

struct UserCmd final {
    enum {
		IN_ATTACK = 1 << 0,
		IN_JUMP = 1 << 1,
		IN_DUCK = 1 << 2,
		IN_SPEED = 1 << 17,
		IN_FORWARD = 1 << 3,
		IN_BACK = 1 << 4,
		IN_USE = 1 << 5,
		IN_MOVELEFT = 1 << 9,
		IN_MOVERIGHT = 1 << 10,
		IN_ATTACK2 = 1 << 11,
		IN_SCORE = 1 << 16,
		IN_WALK = 1 << 18,
		IN_BULLRUSH = 1 << 22
    };
	enum entity_flags {
		fl_onground = (1 << 0),
		fl_ducking = (1 << 1),
	};
    int pad;
    int command_number;
    int tick_count;
    Vector viewangles;
    Vector aimdirection;
    float forwardmove;
    float sidemove;
    float upmove;
    int buttons;
    char impulse;
    int weaponselect;
    int weaponsubtype;
    int random_seed;
    short mousedx;
    short mousedy;
    bool hasbeenpredicted;
};
