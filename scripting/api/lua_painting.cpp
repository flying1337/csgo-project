//
// Created by ruppet on 3/14/2020.
//

#include "../../cheat.h"

#define _s base_state

void c_scripting::u_painting() {
    auto t = _s.new_usertype<ImDrawList>(str("render"), sol::no_constructor);

    t.set_function(str("line"),
    [](ImDrawList* self, float fx, float fy, float tx, float ty, int r, int g, int b, int a, std::optional<float> thickness) {
        self->AddLine(ImVec2(fx, fy), ImVec2(tx, ty), ImColor(r,g,b,a), thickness.value_or(1.f));
    });

    t.set_function(str("rect"),
    [](ImDrawList* self, float fx, float fy, float tx, float ty, int r, int g, int b, int a, std::optional<float> rounding, std::optional<float> thickness) {
        self->AddRect(ImVec2(fx, fy), ImVec2(tx, ty), ImColor(r,g,b,a), rounding.value_or(0.f), 15, thickness.value_or(1.f));
    });

    t.set_function(str("rect_filled"),
    [](ImDrawList* self, float fx, float fy, float tx, float ty, int r, int g,int b, int a, std::optional<float> rounding) {
        self->AddRectFilled(ImVec2(fx, fy), ImVec2(tx, ty), ImColor(r,g,b,a), rounding.value_or(0.f));
    });

    t.set_function(str("text"),
    [](ImDrawList* self, float x, float y, int r, int g, int b, int a, const std::string& text, std::optional<ImFont*> font) {
        if (font)
            ImGui::PushFont(font.value());
        self->AddText(ImVec2(x, y), ImColor(r,g,b,a), text.c_str());
        if (font)
            ImGui::PopFont();
    });

    t.set_function(str("text_outline"),
    [](ImDrawList* self, float x, float y, int r, int g, int b, int a, const std::string& text, std::optional<ImFont*> font) {
        if (font)
            ImGui::PushFont(font.value());
        self->AddTextShadow(ImVec2(x, y), ImColor(r,g,b,a), text.c_str());
        if (font)
            ImGui::PopFont();
    });
    t.set_function(str("text_shadow"),
    [](ImDrawList* self, float x, float y, int r, int g, int b, int a, const std::string& text, std::optional<ImFont*> font) {
        if (font)
            ImGui::PushFont(font.value());
        self->AddTextSoftShadow(ImVec2(x, y), ImColor(r,g,b,a), text.c_str());
        if (font)
            ImGui::PopFont();
    });

    _s.new_usertype<ImFont>(str("font"), sol::no_constructor);

    auto fonts = _s.create_table();
    fonts[str("default")] = []() {
        auto io = &ImGui::GetIO();
        return io->Fonts->Fonts[0];
    };
    fonts[str("bold")] = []() {
        auto io = &ImGui::GetIO();
        return io->Fonts->Fonts[1];
    };
    fonts[str("small")] = []() {
        auto io = &ImGui::GetIO();
        return io->Fonts->Fonts[2];
    };
    fonts[str("big")] = []() {
        auto io = &ImGui::GetIO();
        return io->Fonts->Fonts[6];
    };

    auto drawing = _s.create_table();
    drawing[str("create_font")] = [](g_str path, float size) {
        auto io = &ImGui::GetIO();

        auto fnt = io->Fonts->AddFontFromFileTTF(path.c_str(), size, 0, io->Fonts->GetGlyphRangesCyrillic());
        ImGui_ImplDX9_CreateDeviceObjects();

        return fnt;
    };

    _s[str("fonts")] = protect(fonts, allow_none);
    _s[str("drawing")] = protect(drawing, allow_none);
}

#undef _s