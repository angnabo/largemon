/**
 * Handles the battle instance of a largemon battle.
 * Accept user input from the controller to perform a chosen attack.
 * Generates a move for an enemy largemon.
 *
 * Created by angelica on 11/11/17.
 */

#include "BattleInstance.h"
#include "utility/FileWriter.h"

BattleInstance::BattleInstance() {
    LargemonGenerator generator;
    playerSpecAttkCount = 0;
    enemySpecAttkCount = 0;
    player = generator.generateLargemon();
    enemy = generator.generateLargemon();
    player->setAsPlayer();
    playerArgs.emplace_back("Player");
    enemyArgs.emplace_back("Enemy");
    playerArgs.emplace_back("emptyAction");
    enemyArgs.emplace_back("emptyAction");
    round = 0;
}

/**
 * Delay for 0.6 seconds
 */
inline void delay()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(600));
}

/**
 * Handles the action choice of player or enemy
 * @param lm
 * @param move
 * @return
 */
string BattleInstance::move(Largemon * lm, int move){
    string action;
    if(!isGameOver() && !lm->isStunned()) {
        switch (move) {
            case 0: //attack
                action = normalAttack(lm);
                setAttackArgs(lm);
                break;
            case 1: //defend
                action = defend(lm);
                break;
            case 2: //special attack
                action = specialAttack(lm);
                break;
            case 3: //special ability
                action = specialAbility(lm);
                enemyArgs[1] = "Stunned";
                break;
            default:
                action = normalAttack(lm);
                setAttackArgs(lm);
                break;
        }
    }
    return action;
}

/**
 * Deals with the player's turn
 * @param actionID
 * @return
 */
string BattleInstance::playerMove(int actionID) {
    string action = move(player, actionID);
    action += enemyMove();
    finishTurn(player);

    if(isEnemyDead()){
        enemyArgs[1] = "Fainted";
    }else if(isPlayerDead()){
        playerArgs[1] = "Fainted";
    }
    round++;
    notify(player, playerArgs);
    notify(enemy, enemyArgs);
    return (action.empty() ? getWinner() : action);
}

/**
 * Deals with the enemy turn
 * @return
 */
string BattleInstance::enemyMove() {
    delay();
    int actionID = RandomNumber::randomInRange(0, 6);
    if(enemy->getCurrentHpPercent()*100 < 35){//if under 35% bigger chance to heal
        actionID = RandomNumber::randomInRange(0, 1);
    }
    string action = move(enemy, actionID);
    finishTurn(enemy);
    return action;
}

/**
 * Finish the turn
 * @param lm
 */
void BattleInstance::finishTurn(Largemon * lm){
    lm->decrementStun();
    Largemon * en = getEnemyOf(lm);
    en->applyTickDamage(TICK_DAMAGE_PER_TURN);
    if(lm->getType() == Type::water){
        dynamic_cast<WaterLM*>(lm)->decrementShield();
    }
}

/**
 *  Do a special attack for the given largemon
 * @param lm
 * @return
 */
string BattleInstance::specialAttack(Largemon * lm) {
    string action;
    Largemon * en = getEnemyOf(lm);
    if (isSpecAttack(lm)) {
        if (determineCounter(lm, en)) {
            en->takeDamage(lm->specialAttack());
            action = setSpecAttackArgs(lm);
        } else {
            action = lm->isPlayer() ? "Your largemon doesn't counter the enemy. It did a normal attack. "
                                    : "Enemy attacked for " + to_string(enemy->attack()) + " damage. ";
            normalAttack(lm);
        }
    } else {
        action = lm->isPlayer() ? "Your largemon is exhausted. It did a normal attack. "
                                : "Enemy attacked for " + to_string(enemy->attack()) + " damage. ";
        normalAttack(lm);
    }
    incrCounter(lm, "special");
    return action;
}

/**
 * Does the special attack for a given largemon against it's enemy.
 * @param lm
 * @return
 */
string BattleInstance::specialAbility(Largemon * lm) {
    string action;
    Type largemon = lm->getType();
    Largemon * en = getEnemyOf(lm);
    switch(largemon){
        case Type::fire : {
            en->takeTickDamage(MAX_TICK_TURNS);
            action = playerOrEnemy(getEnemyOf(lm), "ignited") + "is being damaged over time. ";
            break;
        }
        case Type::water : {
            auto *wlm = dynamic_cast<WaterLM *> (lm);
            wlm->shield(MAX_SHIELD_TURNS);
            action = playerOrEnemy(lm, "shielded") + "shielded. ";
            break;
        }
        case Type::wood : {
            en->stun(MAX_STUN_TUNRS);
            action = playerOrEnemy(getEnemyOf(lm), "stunned") + "was stunned. ";
            break;
        }
    }
    return action;
}

/**
 * Set the player or enemy arguments for special attack and return a message to display.
 * @param lm
 * @return
 */
string BattleInstance::setSpecAttackArgs(Largemon * lm){
    string action;
    action = playerOrEnemy(lm, "Special Attack") +
            "used a powerful attack for " + to_string(getEnemyOf(lm)->specialAttack()) + " damage. ";
    return action;
}

string BattleInstance::playerOrEnemy(Largemon * lm, string args){
    string largemon;
    if(lm->isPlayer()){
        largemon = "Your largemon ";
        playerArgs[1] = args;
    } else {
        largemon = "Enemy largemon ";
        enemyArgs[1] = args;
    }
    return largemon;
}

void BattleInstance::incrCounter(Largemon * lm, string count){
    if(lm->isPlayer()){
        if(count == "special"){
            playerSpecAttkCount++;
        }
        if(count == "stun"){

        }
    } else {
        if(count == "special") {
            enemySpecAttkCount++;
        }
    }

}

/**
 * Sets the player or enemy arguments and return a message to display.
 * @param lm
 * @return
 */
string BattleInstance::setAttackArgs(Largemon * lm){
    string action;
    if(lm->isPlayer()){
        action = "You attacked for " + to_string(player->attack()) + " damage. ";
        playerArgs[1] = "Attack";
    } else {
        action = "Enemy attacked for " + to_string(enemy->attack()) + " damage. ";
        enemyArgs[1] = "Attack";
    }
    return action;
}

/**
 * Determine if the largemon can perform a special attack.
 * @param lm
 * @return
 */
bool BattleInstance::isSpecAttack(Largemon * lm){
    bool isAllowed;
    if(lm->isPlayer()){
        isAllowed = playerSpecAttkCount == 0;
    } else {
        isAllowed = enemySpecAttkCount == 0;
    }
    return isAllowed;
}

/**
 * Damage the given largemon's enemy
 * @param lm
 * @return
 */
string BattleInstance::normalAttack(Largemon *lm){
    string shieldAction;
    string action;
    Largemon * en = getEnemyOf(lm);
    if(en->getType() == Type::water){
        shieldAction = attackWaterLm(en, lm->attack());
    } else{
        en->takeDamage(lm->attack());

    }
    return setAttackArgs(lm) + shieldAction;
}

/**
 * Calc the shield amount.
 * @param damage
 * @return
 */
int BattleInstance::applyShield(int damage){
    return (damage/10);
}

/**
 * Attack a shielded water largemon
 * @param lm
 * @param damage
 * @return
 */
string BattleInstance::attackWaterLm(Largemon * lm, int damage){
    string damageTaken;
    auto * waterlm = dynamic_cast<WaterLM *> (lm);
    if(waterlm->isShielded()){
        int shield = applyShield(damage);
        int newDamage = damage-shield;
        waterlm->takeDamage(newDamage);
        if(lm->isPlayer()){
            damageTaken = "Player shielded " + to_string(shield) + " damage. ";
        } else {
            damageTaken = "Enemy shielded " + to_string(shield) + " damage. ";
        }
    } else{
        lm->takeDamage(damage);
    }
    return damageTaken;
}

/**
 * Defend the largemon by healing 20 health.
 * @param lm
 * @return
 */
string BattleInstance::defend(Largemon * lm) {
    lm->defend();
    string action;
    if(lm->isPlayer()){
        action = "Player healed for 20 health. ";
        playerArgs[1] = "Defend";
    } else {
        action = "Enemy healed for 20 health. ";
        enemyArgs[1] = "Defend";
    }
    return action;
}

/**
 * Determine if the largemon is a counter to the enemy.
 * @param lm
 * @param lmEnemy
 * @return
 */
bool BattleInstance::determineCounter(Largemon * lm, Largemon * lmEnemy) {
    Type largemon = lm->getType();
    Type enemy = lmEnemy->getType();
    bool counter = false;
    switch(largemon){
        case Type::water :
            if(enemy == Type::fire)
                counter = true;
            break;
        case Type::fire :
            if(enemy == Type::wood)
                counter = true;
            break;
        case Type::wood :
            if(enemy == Type::water)
                counter = true;
            break;
    }
    return counter;
}

Largemon * BattleInstance::getEnemyOf(Largemon * lm) {
    if(lm == player){
        return enemy;
    } else {
        return player;
    }
}

string BattleInstance::getEnemyLargemonName(){
    return enemy->getName();
}

string BattleInstance::getPlayerLargemonName(){
    return player->getName();
}

int BattleInstance::getPlayerCurrentHp(){
    return player->getCurrentHp();
}

int BattleInstance::getEnemyCurrentHp() {
    return enemy->getCurrentHp();
}

bool BattleInstance::isGameOver() {
    return isPlayerDead() || isEnemyDead();
}

bool BattleInstance::isPlayerDead() {
    return player->getCurrentHp() <= 0;
}

bool BattleInstance::isEnemyDead() {
    return enemy->getCurrentHp() <= 0;
}

string BattleInstance::getWinner() {
    return (isEnemyDead() ? "Player Won!" : "Enemy Won!");
}

void BattleInstance::attach(class ContrObserver * obs) {
    views.push_back(obs);
}

void BattleInstance::notify(Largemon * lm, vector<string> args) {
    for (int i = 0; i < views.size(); i++) {
        views[i]->update(lm, args);
    }
}

int BattleInstance::getRound() {
    return round;
}

Largemon * BattleInstance::getPlayerPtr() {
    return player;
}

Largemon * BattleInstance::getEnemyPtr() {
    return enemy;
}

BattleInstance::~BattleInstance() {

}

