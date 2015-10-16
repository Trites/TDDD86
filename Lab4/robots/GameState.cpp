/**
 * Copyright (C) David Wolfe, 1999.  All rights reserved.
 * Ported to Qt and adapted for TDDD86, 2015.
 */

#include "GameState.h"
#include "utilities.h"
#include "constants.h"
#include <iostream>

using namespace std;

GameState::GameState(){}

GameState::GameState(int numberOfRobots) {

    for (int i = 0; i < numberOfRobots; i++) {

        Robot* robot = new Robot();

        while (!isEmpty(*robot))
            robot->teleport();

        robots.push_back(robot);
    }

    robotsAlive = numberOfRobots;
    teleportHero();
}

void GameState::draw(QGraphicsScene *scene) const {
    scene->clear();


    hero.draw(scene);

    for (size_t i = 0; i < robots.size(); ++i)
        robots[i]->draw(scene);
}

void GameState::teleportHero() {
    do hero.teleport();
    while (!isEmpty(hero));
}

void GameState::moveRobots() {
    for (unsigned int i = 0; i < robots.size(); i++)
        robots[i]->moveTowards (hero);
}

int GameState::countCollisions() {
    int numberDestroyed = 0;
    unsigned int i = 0;

    while (i < robots.size()) {
        if(!robots[i]->isJunk()){
            bool collision = (countRobotsAt(*robots[i]) > 1);

            if (collision) {
                Robot* robot = robots[i];
                robots[i] = new Junk(*robot);
                delete robot;

                numberDestroyed++;
            }
        }
        i++;
    }
    robotsAlive -= numberDestroyed;
    return numberDestroyed;
}

bool GameState::anyRobotsLeft() const {
    return (robotsAlive > 0);
}

bool GameState::heroDead() const {
    return !isEmpty(hero);
}

bool GameState::isSafe(const Unit& unit) const {
    for (unsigned int i = 0; i < robots.size(); i++){

        if (robots[i]->attacks(unit))
            return false;
    }

    if (!isEmpty(unit))
        return false;

    return true;
}

void GameState::moveHeroTowards(const Unit& dir) {
    hero.moveTowards(dir);
}

Hero GameState::getHero() const {return hero;}

/*
 * Free of robots and junk only
 */
bool GameState::isEmpty(const Unit& unit) const {

    for(size_t i = 0; i < robots.size(); ++i){
        if(robots[i]->at(unit))
            return false;
    }

    return true;
}

/*
 * How many robots are there at unit?
 */
int GameState::countRobotsAt(const Unit& unit) const {
    int count = 0;
    for (size_t i = 0; i < robots.size(); ++i) {
        if (robots[i]->at(unit))
            count++;
    }
    return count;
}
