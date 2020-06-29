//
// Created by ruppet on 06.01.2020.
//

#ifndef uc-sflying-cm-entitylistener-h
#define uc-sflying-cm-entitylistener-h

class c_entitylistener : public c_entitylist_listener {
public:
    void on_created(c_entity* pEntity) override;
    void on_deleted(c_entity* pEntity) override;

    void initialize();
    void unhook();

private:
    c_hookable* cs_player = 0;
    c_hookable* networkable = 0;
    c_hookable* netchannel = 0;
};

#endif //uc-sflying-cm-entitylistener-h
