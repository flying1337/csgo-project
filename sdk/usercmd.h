#pragma once
#include "checksum_md5.h"
class c_usercmd {
public:
	c_usercmd() {}

	char pad_0x0000[0x4];
	int		commandnumber;
	int		tickcount;
	c_angle	viewangles;
	c_angle	aimangles;
    float	forwardmove;
    float	sidemove;
    float	upmove;
	int		buttons;
	char    impulse;
	int		weaponselect;
	int		weaponsubtype;
	int		randomseed;
	short	mousedx;
	short	mousedy;
	bool	hasbeenpredicted;
	char	pad_0x4C[0x18];
};