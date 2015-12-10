/**
 * Copyright (C) David Wolfe, 1999.  All rights reserved.
 * Ported to Qt and adapted for TDDD86, 2015.
 */

#ifndef ROBOT_H
#define ROBOT_H

#include "Unit.h"
#include <QGraphicsScene>

class Robot : public Unit {
public:
    Robot();
    virtual ~Robot() = default;
    virtual Robot* clone() const;
    virtual bool isJunk() const;
    void draw(QGraphicsScene* scene) const;
};

#endif // ROBOT_H
