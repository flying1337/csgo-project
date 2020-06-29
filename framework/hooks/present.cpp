//
// Created by ruppet on 12.01.2020.
//

#include "../../cheat.h"
#include "../config.h"

BYTE img_pc[/*85*/] =
        {
                0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00,
                0x00, 0x0D, 0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x03,
                0x00, 0x00, 0x00, 0x03, 0x08, 0x06, 0x00, 0x00, 0x00, 0x56,
                0x28, 0xB5, 0xBF, 0x00, 0x00, 0x00, 0x1C, 0x49, 0x44, 0x41,
                0x54, 0x08, 0x99, 0x63, 0x60, 0x60, 0x60, 0x68, 0xF8, 0xFF,
                0xFF, 0xFF, 0x7F, 0x10, 0xCD, 0x08, 0x65, 0xC0, 0x01, 0x44,
                0x86, 0x81, 0xA1, 0x01, 0x00, 0x1D, 0x56, 0x0D, 0xF6, 0x78,
                0x72, 0x65, 0x77, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E,
                0x44, 0xAE, 0x42, 0x60, 0x82,
        };

IDirect3DTexture9* penecross = nullptr;

long __stdcall hkPresent(IDirect3DDevice9* device, RECT* a, RECT* b, HWND c, RGNDATA* d) {
    if (!core->is_dx_loaded) return oPresent(device, a, b, c, d);

    IDirect3DStateBlock9* pixel_state = NULL; IDirect3DVertexDeclaration9* vertDec; IDirect3DVertexShader9* vertShader;
    device->CreateStateBlock(D3DSBT_PIXELSTATE, &pixel_state);
    device->GetVertexDeclaration(&vertDec);
    device->GetVertexShader(&vertShader);

    DWORD colorwrite, srgbwrite;
    device->GetRenderState(D3DRS_COLORWRITEENABLE, &colorwrite);
    device->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbwrite);
    device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
    device->SetRenderState(D3DRS_SRGBWRITEENABLE, false);

    static auto once = false;
    if (!once)
    {
        D3DXCreateTextureFromFileInMemoryEx(device, img_pc, 85, 3, 3, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN,
                                            D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, 0, 0, &penecross);
        once = true;
    }

    menu->initialize(device);
    menu->draw_begin();

    auto w = 0, h = 0;
    csgo->engine->screen_size(w, h);

    auto dd = &ImGui::GetCurrentContext()->BackgroundDrawList;

    if (core->is_fully_loaded) {
        if (csgo->local_player && csgo->engine->is_in_game() && csgo->local_player->health() > 0) {
            if (config->b.rage_aimpoints) {
                for (auto &ap : csgo->aimpoints) {
                    c_vector sp;
                    if (utils->world_to_screen(ap.point, &sp))
                        dd->AddCircleFilled(ImVec2(sp.x, sp.y), 2.f,
                                ImColor(config->c.aimpoints[0], config->c.aimpoints[1], config->c.aimpoints[2], config->c.aimpoints[3]));
                }
            }

            auto fps = 1.f / csgo->globals->absframetime;
            if (fps <= 96.f)
                csgo->mitigate_fps = true;
            if (fps >= 164.f)
                csgo->mitigate_fps = false;
        }

        features->visuals->draw(dd);
        features->hitlog->draw(dd);
        features->scope->draw(dd);
        features->bombtimer->draw(dd);
        features->indicators->draw(dd);

        if (csgo->local_player && csgo->engine->is_in_game() && csgo->local_player->health() > 0) {
            if (config->b.vis_penecross) {
                auto col = features->visuals->percent_to_color(csgo->aw_damage > 0 ? 100 : 0);


                dd->AddImage(penecross, ImVec2(w / 2 - 1, h / 2 - 1),
                             ImVec2(w / 2 + 2, h / 2 + 2), ImVec2(0, 0), ImVec2(1, 1), col);
            }
        }
    }

    auto can_draw_loader = false;
    if (core->is_loadanim_shown) {
        if (core->loadanim_alpha < 1.f)
            core->loadanim_alpha += 1.f / 0.25f * csgo->globals->frametime;

        can_draw_loader = true;
    }
    else {
        if (core->loadanim_alpha > 0.f) {
            core->loadanim_alpha -= 1.f / 0.25f * csgo->globals->frametime;
            can_draw_loader = true;
        }
    }

    if (can_draw_loader) {
        auto x = w - 200, y = h - 30;
        dd->AddRectFilled(ImVec2(x, y), ImVec2(w - 5, h - 5), ImColor(0.f, 0.f, 0.f, core->loadanim_alpha), 2.f);
        dd->AddImage(menu->spinners[core->loadanim_spinner], ImVec2(x + 6, y + 6), ImVec2(x + 19, y + 19));
        dd->AddText(ImVec2(x + 27.5f, y + 6.5f), ImColor(1.f, 1.f, 1.f, core->loadanim_alpha), core->loadanim_text.c_str());

        if (csgo->globals->realtime - core->loadanim_lasttime > 0.1f) {
            core->loadanim_lasttime = csgo->globals->realtime;
            core->loadanim_spinner++;

            if (core->loadanim_spinner > 7)
                core->loadanim_spinner = 0;
        }
    }

    if (core->is_fully_loaded) {
        features->speclist->draw();
        features->events->draw(dd);

        scripting->run(dd);
        menu->draw();
    }

    features->debuginfo->draw(dd);

    menu->draw_end();

    device->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
    device->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);

    pixel_state->Apply();
    pixel_state->Release();
    device->SetVertexDeclaration(vertDec);
    device->SetVertexShader(vertShader);

    return oPresent(device, a, b, c, d);
}