#pragma once

class c_evententry {
public:
	const char* text;
	float spawntime;
	int alpha = 255;

	bool operator==(const c_evententry& o) { return !strcmp(text, o.text) && spawntime == o.spawntime; }
};

class c_events {
public:
	void add(const char* text, ...);
	void draw(ImDrawList* d);

private:
	std::vector<c_evententry> entries;
};