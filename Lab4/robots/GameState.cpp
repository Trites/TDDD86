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

GameState::GameState(const GameState &other)
{

    cout << "Copy begin..." << endl;

    hero = Hero(other.hero);
    robots.resize(other.robots.size());

    for(size_t i = 0; i < other.robots.size(); i++)
        robots[i] = other.robots[i]->clone(); //clone

    robotsAlive = other.robotsAlive;

    cout << "Copy end" << endl;
}

GameState::~GameState(){

    cout << "Destroy begin..." << endl;
    for (Robot* robot : robots){

        delete robot;
    }
    robots.clear();
    cout << "Destroy end." << endl;
}

GameState::GameState(int numberOfRobots) {

    cout << "R-Order: " << numberOfRobots << endl; 
    cout << "R-Acc-Pre: " << robots.size() << endl;

    for (int i = 0; i < numberOfRobots; i++) {

        Robot* robot = new Robot();

        while (!isEmpty(*robot))
            robot->teleport();

        robots.push_back(robot);
    }


    cout << "R-Acc: " << robots.size() << endl;
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

    for(size_t i = 0; i < robots.size(); i++){

        if(!robots[i]->isJunk()){
            bool collision = countRobotsAt(*robots[i]) > 1;

            if(collision){

                Junk* junk = new Junk(*robots[i]);
                delete robots[i];
                robots[i] = junk;

                numberDestroyed++;
            }
        }
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

GameState& GameState::operator =(const GameState& rhs){


    cout << "Assign begin..." << endl;
    hero = rhs.hero;

    cout << "Size of copy: " << rhs.robots.size() << endl;
    cout << "My size pre: " << rhs.robots.size() << endl;
    robots.resize(rhs.robots.size());

    for(size_t i = 0; i < rhs.robots.size(); i++){

        delete robots[i];
        robots[i] = rhs.robots[i]->clone();
    }

    robotsAlive = rhs.robotsAlive;


    cout << "My size: " << robots.size() << endl;
    cout << "Alive: " << robotsAlive << endl;

    cout << "Assign end..." << endl;
    return *this;
}
