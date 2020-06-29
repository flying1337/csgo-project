#pragma once

class c_dynamicbb : public c_entity {
public:
	bool bounding_box(ImVec2* mins, ImVec2* maxs);
};