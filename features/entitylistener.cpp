//
// Created by ruppet on 06.01.2020.
//

#include "../cheat.h"

void c_entitylistener::on_created(c_entity* pEntity) {
    auto e = (c_player*)pEntity;
    if (e->is_player() && !networkable && !netchannel) {
        //cs_player = new c_hookable(e);
        networkable = new c_hookable(e->get_networkable());
        netchannel = new c_hookable(csgo->clientstate->netchannel);

//        oDoExtraBonesProcessing = (fnDoExtraBonesProcessing)cs_player->orig(197);
//        oEyeAngles = (fnEyeAngles)cs_player->orig(169);
//        oUpdateClientsideAnimation = (fnUpdateClientsideAnimation)cs_player->orig(223);
//        oStandartBlendingRules = (fnStandartBlendingRules)cs_player->orig(205);
//
//        cs_player->hook(197, hkDoExtraBonesProcessing);
//        cs_player->hook(169, hkEyeAngles);
//        cs_player->hook(223, hkUpdateClientsideAnimation);
//        cs_player->hook(205, hkStandartBlendingRules);

        oPostDataUpdate = (fnPostDataUpdate)networkable->orig(7);
        networkable->hook(7, hkPostDataUpdate);

        oSendNetMsg = (fnSendNetMsg)netchannel->orig(40);
        oSetChoked = (fnSetChoked)netchannel->orig(45);

        netchannel->hook(40, hkSendNetMsg);
        netchannel->hook(45, hkSetChoked);
    }
}

void c_entitylistener::on_deleted(c_entity* pEntity) {}

void c_entitylistener::unhook() {
//    if (cs_player) {
//        cs_player->unhook(197);
//        cs_player->unhook(169);
//        cs_player->unhook(223);
//        cs_player->unhook(205);
//
//        cs_player = 0;
//    }

    if (networkable) {
        networkable->unhook(7);
        networkable = 0;
    }

    if (netchannel) {
        netchannel->unhook(40);
        netchannel->unhook(45);
        netchannel = 0;
    }
}

void c_entitylistener::initialize() {
    ((c_cliententitylist*)csgo->entities)->listeners.AddToTail(this);
}