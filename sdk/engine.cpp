//
// Created by ruppet on 2/27/2020.
//

#include "../cheat.h"

float c_engine::get_ping() {
    return ((c_netchannel*)get_net_channel())->get_avg_latency(0);
}