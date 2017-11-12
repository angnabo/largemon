//
// Created by angelica on 07/10/17.
//

#include <iostream>
#include "../../include/largeMonHeader/LargeMon.h"

//using namespace std;

LargeMon::LargeMon(const int* hp, const int* damage, const int* size){
    this->hp = *hp;
    this->damage = *damage;
    this->size = *size;
    //this->type = type;
}

//string LargeMon::getName() const {
//    return "largemon name";
//}

void LargeMon::takeDamage(int damage) {
    hp -= damage;
    notify();
}

void LargeMon::defend(){
    hp += 20;
    notify();
}

int LargeMon::specialAttack() const {
    return 0;
}

void LargeMon::notify() {
    for (int i = 0; i < views.size(); i++){
        views[i]->update();
    }
}

void LargeMon::attach(Observer * obs) {
    views.push_back(obs);
}

//string LargeMon::getType() {
//    return "";
//}
