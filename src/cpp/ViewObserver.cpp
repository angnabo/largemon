//
// Created by angelica on 11/11/17.
//

#include <iostream>
#include "../include/ViewObserver.h"

ViewObserver::ViewObserver(LargeMon *lm) : Observer(lm) {
    //largeMon->attach(this);
}

ViewObserver::~ViewObserver() {

}

void ViewObserver::update() const {
    cout << largeMon->getType() << "LargeMon health: " << largeMon->getHp() << endl;
}

int ViewObserver::getCurrentHp() const{
    return largeMon->getHp();
}

string ViewObserver::getName() const{
    return "A wild " + largeMon->getName() + " appears!";
}

//ViewObserver::ViewObserver() = default;