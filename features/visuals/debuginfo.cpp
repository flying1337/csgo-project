//
// Created by ruppet on 07.01.2020.
//

#include "../../cheat.h"
#include "debuginfo.h"

void c_debuginfo::draw(ImDrawList* d) {

    return;
//#ifndef _DEBUG
//    return;
//#endif

//    if (!b.draw_other) return;

    std::vector<std::string> data_col1;
    std::vector<std::string> data_col2;

    data_col1.push_back(str("uc-sflying // DEBUG MODE"));
//    data_col1.push_back(utils->format(str("csgo ->")));
//    data_col1.push_back(utils->format(str("    local_player: 0x%X"), (ptr)csgo->local_player));
//    data_col1.push_back(utils->format(str("    is_in_tp: %i"), csgo->is_in_tp));
//    data_col1.push_back(utils->format(str("    allow_sp: %i"), csgo->allow_sp));
//    data_col1.push_back(utils->format(str("    want_sp: %i"), csgo->want_sp));
//    data_col1.push_back(utils->format(str("    aa_worked: %i"), csgo->aa_worked));
//    data_col1.push_back(utils->format(str("    aa_desync: %i"), csgo->aa_desync));
//    data_col1.push_back(utils->format(str("    allow_anims: %i"), csgo->allow_anims));
//    data_col1.push_back(utils->format(str("    switch_sidemove: %i"), csgo->switch_sidemove));
//    data_col1.push_back(utils->format(str("    last_allow_sp: %i"), csgo->last_allow_sp));
//    data_col1.push_back(utils->format(str("    choke: %i"), csgo->choke));
//    data_col1.push_back(utils->format(str("    aw_damage: %i"), csgo->aw_damage));
//    data_col1.push_back(utils->format(str("    shift: %i"), csgo->shift));
//    data_col1.push_back(utils->format(str("    is_in_sendmove: %i"), csgo->is_in_sendmove));
//    data_col1.push_back(utils->format(str("    is_first_sendmove: %i"), csgo->is_first_sendmove));
//    data_col1.push_back(utils->format(str("    next_sidemove: %i"), csgo->next_sidemove));
//    data_col1.push_back(utils->format(str("    gfy_delta: %.2f"), csgo->gfy_delta));
//    data_col1.push_back(utils->format(str("    sent: %.2f %.2f"), csgo->sent.p, csgo->sent.y));
//    data_col1.push_back(utils->format(str("    unsent: %.2f %.2f"), csgo->unsent.p, csgo->unsent.y));
//    data_col1.push_back(utils->format(str("    real: %.2f %.2f"), csgo->real.p, csgo->real.y));
//    data_col1.push_back(utils->format(str("    shot: tick - %i hit - %i hurt - %i"), csgo->shot.tick, csgo->shot.hit, csgo->shot.hurt));

    auto target = csgo->entities->get_entity(i.delayshot);
    if (target && target->is_player()) {
        auto p = (c_player*)target;

        data_col2.push_back(utils->format(str("player 0x%X [%s] ->"), (ptr)p, p->get_player_info().name));
        data_col2.push_back(utils->format(str("    is_bot: %i"), p->get_player_info().is_bot));
        data_col2.push_back(utils->format(str("    steam_id: %s"), p->get_player_info().steam_id));
        data_col2.push_back(utils->format(str("    health: %i"), p->health()));
        data_col2.push_back(utils->format(str("    is_dormant: %i"), p->is_dormant()));
        data_col2.push_back(utils->format(str("    lby: %.2f"), p->lby()));
        data_col2.push_back(utils->format(str("    simtime: %.2f"), p->simtime()));
        data_col2.push_back(utils->format(str("    animlayer[%i] ->"), i.statedelay));
        data_col2.push_back(utils->format(str("        playback_rate: %f"), p->animlayers()[i.statedelay].playback_rate));
        data_col2.push_back(utils->format(str("        order: %i"), p->animlayers()[i.statedelay].order));
        data_col2.push_back(utils->format(str("        sequence: %i"), p->animlayers()[i.statedelay].sequence));
        data_col2.push_back(utils->format(str("        prev_cycle: %f"), p->animlayers()[i.statedelay].prev_cycle));
        data_col2.push_back(utils->format(str("        weight: %f"), p->animlayers()[i.statedelay].weight));
        data_col2.push_back(utils->format(str("        weight_delta: %f"), p->animlayers()[i.statedelay].weight_delta));
        data_col2.push_back(utils->format(str("        cycle: %f"), p->animlayers()[i.statedelay].cycle));
        data_col2.push_back(utils->format(str("    poseparam[%i]: %f"), i.poseparam, p->poseparams()[i.poseparam]));
    }

    auto y = 0;
    for (auto dv : data_col1) {
        d->AddTextShadow(ImVec2(100, 100 + y), ImColor(255, 255, 255), dv.c_str());
        y += ImGui::CalcTextSize(dv.c_str()).y + 2.f;
    }

    y = 0;
    for (auto dv : data_col2) {
        d->AddTextShadow(ImVec2(500, 100 + y), ImColor(255, 255, 255), dv.c_str());
        y += ImGui::CalcTextSize(dv.c_str()).y + 2.f;
    }
}