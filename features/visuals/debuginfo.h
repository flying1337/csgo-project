//
// Created by ruppet on 07.01.2020.
//

#ifndef uc-sflying-cm-_DEBUGINFO_H
#define uc-sflying-cm-_DEBUGINFO_H

class c_debuginfo {
public:
    void draw(ImDrawList* d);

    struct {
        bool draw_playerinfo = false;
        bool draw_other = false;
        bool no_sp = false;
    } b;

    struct {
        int target_ent = 0;
        int animlayer = 0;
        int poseparam = 0;
        int maxmisses = 1;
        int r8_shootuntil = 12;
        int r8_reset = 14;
        int r8_resetto = 5;
        int delayshot = 24;
        int statedelay = 7;
    } i;

    struct {

    } f;

    struct {

    } c;

    struct {

    } m;
};

#endif //uc-sflying-cm-_DEBUGINFO_H
