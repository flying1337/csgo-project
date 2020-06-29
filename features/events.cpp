#include "../cheat.h"

void c_events::add(const char* text, ...) {
	auto buffer = (char*)malloc(1024);

	va_list va;
	va_start(va, text);
	vsprintf(buffer, text, va);
	va_end(va);

	if (entries.size() > 5) entries.erase(entries.begin());
	entries.push_back({ buffer, csgo->globals->realtime });
}

void c_events::draw(ImDrawList* d) {
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);

	std::vector<c_evententry> rm;

	auto i = 0;
	for (auto &e : entries) {
	    if (csgo->globals->realtime - e.spawntime > 5.f)
	        e.alpha -= 255.f / 0.5f * csgo->globals->frametime;
		if (csgo->globals->realtime - e.spawntime > 5.5f)
			rm.push_back(e);

		d->AddTextSoftShadow(ImVec2(5, 5 + i), ImColor(255, 255, 255, e.alpha), e.text);
		i += 12;
	}
	ImGui::PopFont();

	for (auto r : rm)
		entries.erase(std::find(entries.begin(), entries.end(), r));
}