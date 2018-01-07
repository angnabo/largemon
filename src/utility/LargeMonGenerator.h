//
// Created by angelica on 07/10/17.
//


#include "../model/largemon/LargeMon.h"
#include "../model/largemon/WaterLM.h"
#include "../model/largemon/FireLM.h"
#include "../model/largemon/WoodLM.h"
#include <string>
#include <vector>

using namespace std;

#ifndef LARGEMON_LARGEMONGENERATOR_H
#define LARGEMON_LARGEMONGENERATOR_H


class LargeMonGenerator {
public:
    LargeMonGenerator();
    LargeMon * generateLargeMon();
    int randomInRange(int, int);
    string generateName(int);
private:
    const int MIN_HEALTH = 200;
    const int MAX_HEALTH = 400;
    vector<string> fireNames = {"Fire Troll"};
    vector<string> waterNames = {"Water Troll"};
    vector<string> woodNames = {"Wood Troll"};
};


#endif //LARGEMON_LARGEMONGENERATOR_H
