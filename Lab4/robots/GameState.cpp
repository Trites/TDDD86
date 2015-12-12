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

    hero = Hero(other.hero);
    robots.resize(other.robots.size());

    //Use clone pattern to copy junk and robots
    for(size_t i = 0; i < other.robots.size(); i++)
        robots[i] = other.robots[i]->clone();

    robotsAlive = other.robotsAlive;
}

GameState::~GameState(){

    for (Robot* robot : robots)
        delete robot;

    robots.clear();
}

GameState::GameState(int numberOfRobots) {

    for (int i = 0; i < numberOfRobots; i++) {

        Robot* robot = new Robot();

        //Randomly teleport the robot until it finds an empty location
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

    for(size_t i = 0; i < robots.size(); i++){

        //Junk does not need to check collision
        if(!robots[i]->isJunk()){

            //A collision happens if there are more than one unit at the position of the current robot
            bool collision = countRobotsAt(*robots[i]) > 1;

            if(collision){

                //Replace robot with junk
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

    for (size_t i = 0; i < robots.size(); i++){

        if (robots[i]->attacks(unit))
            return false;
    }

    if (!isEmpty(unit))
        return false;

    return true;
}

void GameState::moveHeroTowards(const Point& dir) {
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

    hero = rhs.hero;

    //Delete any units occupying the list
    for(size_t i = 0; i < robots.size(); i++)
        delete robots[i];

    //Resize list to accommodate new units
    robots.resize(rhs.robots.size());

    //Fill list with clones of units from rhs
    for(size_t i = 0; i < robots.size(); i++){

        robots[i] = rhs.robots[i]->clone();
    }

    robotsAlive = rhs.robotsAlive;
    return *this;
}
