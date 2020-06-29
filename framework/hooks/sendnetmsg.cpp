//
// Created by ruppet on 12.01.2020.
//

#include "../../cheat.h"

char buffer[4000] = {0};
bool __fastcall hkSendNetMsg(void* o, void*, c_netmessage* msg, bool& force_reliable, bool voice) {
//    if (msg->get_type() == 14 && config->b.misc_svpurebypass)
//        return false;
//
//    if (strstr(msg->get_name(), str("CCLCMsg_Move"))) {
//        if (msg->get_group() == 11 && msg->get_type() != 5) {
//        }
//
//        return oSendNetMsg(o, msg, force_reliable, voice);
//    }
//
//    if (msg->get_group() == 9)
//        voice = true;

    return oSendNetMsg(o, msg, force_reliable, voice);
}