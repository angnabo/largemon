//
// Created by angelica on 20/10/17.
//

#include "Controller.h"

Controller::Controller() {
    battleInstance = BattleInstance();
}

/**
 * Delay for 0.6 seconds
 */
inline void delay() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

/**
 * Changes the selected button on the menu, and un-selects the no longer selected button
 * @param selected
 * @param event
 * @return
 */
int Controller::handleMenuKeyPress(MenuButtonEnum *selected, int event) {
    switch (event) {
        case SDLK_RIGHT:
            switch (*selected) {
                case left:
                    *selected = right;
                    break;
                default:
                    break;
            }
            return *selected;
        case SDLK_LEFT:
            switch (*selected) {
                case right:
                    *selected = left;
                    break;
                default:
                    break;
            }
            return *selected;
        default:
            break;
    }
}

/**
 * Changes the selected button, and un-selects the no longer selected button
 * @param selected
 * @param event
 * @return
 */
int Controller::handleKeyPress(ButtonEnum *selected, int event) {
    switch (event) {
        case SDLK_UP:
            switch (*selected) {
                case bottom_left :
                    *selected = top_left;
                    break;
                case bottom_right :
                    *selected = top_right;
                    break;
                default:
                    break;
            }
            return *selected;
        case SDLK_RIGHT:
            switch (*selected) {
                case top_left:
                    *selected = top_right;
                    break;
                case bottom_left:
                    *selected = bottom_right;
                    break;
                default:
                    break;
            }
            return *selected;
        case SDLK_DOWN:
            switch (*selected) {
                case top_left:
                    *selected = bottom_left;
                    break;
                case top_right:
                    *selected = bottom_right;
                    break;
                default:
                    break;
            }
            return *selected;
        case SDLK_LEFT:
            switch (*selected) {
                case top_right:
                    *selected = top_left;
                    break;
                case bottom_right:
                    *selected = bottom_left;
                    break;
                default:
                    break;
            }
            return *selected;
        default:
            break;
    }
    return *selected;
}

/**
 * Creates a new battle, passes arguments to the view and runs the loop to accept user input
 * @return
 */
int Controller::run() {

    int exitCode = 0;

    FileWriter writer = FileWriter(&battleInstance);

    HealthBarObserver playerhp = HealthBarObserver(battleInstance.getPlayerPtr(), &view, &battleInstance);
    HealthBarObserver enemyhp = HealthBarObserver(battleInstance.getEnemyPtr(), &view, &battleInstance);

    setViewArguments();

    view.run(arguments);

    int pressedButton = 0;

    ButtonEnum selected = ButtonEnum::top_left;

    bool running = true;

    SDL_Event e{};
    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
                // handle key presses
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    //Handle LEFT key press
                    case SDLK_UP:
                        pressedButton = handleKeyPress(&selected, SDLK_UP);
                        view.updateButtons(pressedButton);
                        break;
                        //Handle RIGHT key press
                    case SDLK_RIGHT:
                        pressedButton = handleKeyPress(&selected, SDLK_RIGHT);
                        view.updateButtons(pressedButton);
                        break;
                        //Handle DOWN key press
                    case SDLK_DOWN:
                        pressedButton = handleKeyPress(&selected, SDLK_DOWN);
                        view.updateButtons(pressedButton);
                        break;
                        //Handle UP key press
                    case SDLK_LEFT:
                        pressedButton = handleKeyPress(&selected, SDLK_LEFT);
                        view.updateButtons(pressedButton);
                        break;
                        //Handle M key press for pause music
                    case SDLK_m:
                        if (Mix_PausedMusic()) {
                            Mix_ResumeMusic();
                        } else {
                            Mix_PauseMusic();
                        }
                        break;
                    default:
                        break;
                }
                if (e.key.keysym.sym == SDLK_RETURN) {
                    if (!battleInstance.isGameOver()) {
                        string textUpdate;
                        // pass the action to the battle
                        textUpdate = battleInstance.playerMove(selected);
                        // update the view and
                        view.updateText(textUpdate);
                    }
                }
                if (battleInstance.isGameOver()) {
                    delay();
                    // open menu panel
                    exitCode = menuPanel();
                    return exitCode;
                }
            }
        }
    }
}

/**
 * Creates and opens the menu panel
 * @return
 */
int Controller::menuPanel() {

    view.menuPanel(battleInstance.getWinner());

    int pressedMenuButton;

    MenuButtonEnum selectedMenu = MenuButtonEnum::left;

    bool running = true;

    //Event handler
    SDL_Event e{};
    while (running) {
        while (SDL_WaitEvent(&e) >= 0) {
            if (e.type == SDL_QUIT) {
                running = false;
                view.close();
            }
                // handle key presses
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    //Handle RIGHT key press
                    case SDLK_RIGHT:
                        pressedMenuButton = handleMenuKeyPress(&selectedMenu, SDLK_RIGHT);
                        view.updateMenuButtons(pressedMenuButton);
                        break;
                        //Handle LEFT key press
                    case SDLK_LEFT:
                        pressedMenuButton = handleMenuKeyPress(&selectedMenu, SDLK_LEFT);
                        view.updateMenuButtons(pressedMenuButton);
                        break;
                    default:
                        break;
                }
                if (e.key.keysym.sym == SDLK_RETURN) {
                    if (selectedMenu == MenuButtonEnum::left) {
                        return 2;
                    } else {
                        view.close();
                        return 0;
                    }
                }
                if (battleInstance.isGameOver()) {
                    view.menuPanel(battleInstance.getWinner());
                }
            }
        }
    }
}

/**
 * Set the arguments to init the view with
 */
void Controller::setViewArguments() {
    DescriptGen descriptGen = DescriptGen();
    arguments.push_back(descriptGen.getDescription(battleInstance.getEnemyPtr()));
    arguments.push_back(to_string(battleInstance.getPlayerCurrentHp()));
    arguments.push_back(to_string(battleInstance.getEnemyCurrentHp()));
    arguments.push_back(getLargemonSpritePath(battleInstance.getPlayerLargemonName()));
    arguments.push_back(getLargemonSpritePath(battleInstance.getEnemyLargemonName()));
    arguments.push_back(getTypeIconPath(battleInstance.getPlayerLargemonName()));
    arguments.push_back(getTypeIconPath(battleInstance.getEnemyLargemonName()));
    arguments.push_back(descriptGen.getAttack(battleInstance.getPlayerPtr()->getType()));
    arguments.push_back(descriptGen.getAbility(battleInstance.getPlayerPtr()->getType()));
    arguments.push_back(to_string(battleInstance.getPlayerPtr()->attack()));
    arguments.push_back(to_string(battleInstance.getEnemyPtr()->attack()));
}

string Controller::getLargemonSpritePath(string type) {
    string path;
    if (type == "fire troll") {
        path = "../resources/sprites/fire_troll_sprite_sheet.png";
    } else if (type == "water troll") {
        path = "../resources/sprites/water_troll_sprite_sheet.png";
    } else {
        path = "../resources/sprites/wood_troll_sprite_sheet.png";
    }
    return path;
}

string Controller::getTypeIconPath(string type) {
    string path;
    if (type == "fire troll") {
        path = "../resources/ui/fire_type_icon.png";
    } else if (type == "water troll") {
        path = "../resources/ui/water_type_icon.png";
    } else {
        path = "../resources/ui/wood_type_icon.png";
    }
    return path;
}

void Controller::close() {
    view.close();
}

