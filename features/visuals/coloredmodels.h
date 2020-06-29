#pragma once

class c_coloredmodels {
public:
	bool work(c_entity* ent, void* a, void* b, c_drawmodelstate c, c_modelrenderinfo d, c_matrix3x4* e);

private:
	bool process_player(c_player* p);

	void* obj = 0;
	void* arg_2 = 0;
	c_drawmodelstate state;
	c_modelrenderinfo render_info;
	c_matrix3x4* bones;

	int type = -1;
	c_material* open = 0;
	c_material* cover = 0;

    int backtrack_type = -1;
    c_material* backtrack_cover = 0;

	int team_type = -1;
	c_material* team_open = 0;
	c_material* team_cover = 0;

    int self_type = -1;
    c_material* self_open = 0;

    int fake_type = -1;
    c_material* fake_cover = 0;

    c_material* glow_enemy = 0;
    c_material* glow_team = 0;
    c_material* glow_self = 0;
};