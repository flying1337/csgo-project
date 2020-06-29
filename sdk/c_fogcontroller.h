//
// Created by ruppet on 2/12/2020.
//

#ifndef uc-sflying-cm-_C_FOGCONTROLLER_H
#define uc-sflying-cm-_C_FOGCONTROLLER_H


class c_fogcontroller {
public:
    int& color_secondary();
    int& color_primary();
    bool& enable();
    int& blend();
    float& start();
    float& end();
    float& maxdensity();
};


#endif //uc-sflying-cm-_C_FOGCONTROLLER_H
