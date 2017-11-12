//
// Created by angelica on 07/10/17.
//
#include "largeMonHeader/LargeMon.h"
#include "ViewObserver.h"

#ifndef LARGEMON_BATTLEINSTANCE_H
#define LARGEMON_BATTLEINSTANCE_H


class BattleInstance {
public:
    BattleInstance();
    void fight();
    void computerMove();
    int randomInRange(int, int);
    bool determineCounter(string *, string *);
    virtual ~BattleInstance();
private:
    LargeMon * player;
    LargeMon * computer;
    int playerSpecAttkCount;
    int computerSpecAttkCounter;
//    ViewObserver playerView;
//    ViewObserver enemyView;
    //LargeMon * playerPtr;
};


#endif //LARGEMON_BATTLEINSTANCE_H