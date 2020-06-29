//
// Created by ruppet on 27.12.2019.
//

#ifndef uc-sflying-cm-_INDICATORS_H
#define uc-sflying-cm-_INDICATORS_H

enum e_indicator_addition {
    ADDITION_BAR,
    ADDITION_SIDE,
    ADDITION_DESC,
    ADDITION_NONE
};

class c_indicator_bar {
public:
    bool state = false;
    ImU32 color;
    float max;
    float min;
    float value;
    bool large = false;
};

class c_indicator_side {
public:
    bool state = false;
    ImU32 color;
    std::string text;
};

class c_indicator_desc {
public:
    bool state = false;
    ImU32 color;
    std::string text;
};

struct s_indicator {
    std::string text;
    ImU32 color;
    c_indicator_bar bar;
    c_indicator_side side;
    c_indicator_desc desc;
};

class c_indicators {
public:
    void draw(ImDrawList* d);

    void add(s_indicator);
    void add_with_bar(std::string text, ImU32 color, ImU32 barcolor, float max, float value);

private:
    std::vector<s_indicator> ext_indicators;
};

#endif //uc-sflying-cm-_INDICATORS_H
