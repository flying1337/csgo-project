#pragma once

class c_prediction {
public:
	void setup_move(c_entity* player, c_usercmd* ucmd, c_move* moveHelper, void* pMoveData) {
		typedef void(__thiscall* fn)(void*, c_entity*, c_usercmd*, c_move*, void*);
		return g_vfunc(fn, this, 20)(this, player, ucmd, moveHelper, pMoveData);
	}

	void finish_move(c_entity* player, c_usercmd* ucmd, void* pMoveData) {
		typedef void(__thiscall* fn)(void*, c_entity*, c_usercmd*, void*);
		return g_vfunc(fn, this, 21)(this, player, ucmd, pMoveData);
	}
};