/**
 * Copyright (C) David Wolfe, 1999.  All rights reserved.
 * Ported to Qt and adapted for TDDD86, 2015.
 */

#ifndef ROBOT_H
#define ROBOT_H

#include "Unit.h"
#include <QGraphicsScene>

class Robot : public Unit {

const static int MOVE_DIST_SQUARED = 2;

public:
    Robot();
    virtual ~Robot() = default;
    virtual Robot* clone() const;
    virtual bool isJunk() const;

    /*
    * Can I catch u in one move?
    */
    virtual bool attacks(const Unit& u) const;

    void draw(QGraphicsScene* scene) const;

};

#endif // ROBOT_H
