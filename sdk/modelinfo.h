#pragma once

class c_modelinfo
{
public:
	c_studiohdr* get_studio_model(const c_model* model);
	int get_model_index(const char* name);
	char* get_model_name(const c_model* model);
	void get_model_materials(const c_model* model, int count, c_material** mats);
};