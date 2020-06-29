#pragma once

struct s_hitlogentry {
	std::string who, to;
	int dmg, left;
	bool hurt, yourself;

	int flash, alpha;
	float creation;

	bool operator==(const s_hitlogentry& o) {
		return 
			o.who == who && o.to == to && o.dmg == dmg && o.left == left && o.hurt == hurt &&
			o.yourself == yourself && o.flash == flash && o.alpha == alpha && o.creation == creation;
	}
};

class c_hitlog {
public:
	void add(const char* who, const char* to, int damage, int left, bool hurt, bool youself);
	void draw(ImDrawList* d);

private:
	std::vector<s_hitlogentry> entries;
};