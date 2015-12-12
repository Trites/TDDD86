/**
 * Copyright (C) David Wolfe, 1999.  All rights reserved.
 * Ported to Qt and adapted for TDDD86, 2015.
 */

#ifndef UNIT_H
#define UNIT_H

#include "utilities.h"
#include <QGraphicsScene>

/* Root class for all pieces on the board.
 * Subclasses are Robot, Hero and Junk.
 */
class Unit {
public:
    Unit();
    Unit(const Unit& u);
    Unit(const Point& p);
    virtual Unit* clone() const = 0;

    /*
    * Return Point representation of Unit
    */
    Point asPoint() const;

    /*
    * Am I in the same square as u?
    */
    bool at(const Unit& u) const;

    /*
    * Take one step closer to u
    */
    virtual void moveTowards(const Unit& u);

    /*
    * Take one step closer to p
    */
    virtual void moveTowards(const Point& p);

    /*
    * Teleport. Does not check for collision
    */
    void teleport();

    /*
    * Euclidean distance to u
    */
    double distanceToSquared(const Unit& u) const;

    /*
     * Draws Unit
     */
    virtual void draw(QGraphicsScene* scene) const = 0;
private:
    int x;
    int y;

    // private helpers
    void checkBounds();
};

#endif // UNIT_H
