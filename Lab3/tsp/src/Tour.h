// This is the .h file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own, as well as on the members.
// TODO: remove this comment header

#ifndef TOUR_H
#define TOUR_H

#include "Node.h"
#include "Point.h"

class Tour {
public:

    Tour();
    ~Tour();
    void show();
    void draw(QGraphicsScene* scene);
    int size();
    double distance();
    void insertNearest(Point p);
    void insertSmallest(Point p);
    void twoOpt();

private:

    Node* first;

    void swapNext(Node* a, Node* b);
    void insertAfter(const Point& point, Node* node);
    bool linesIntersect(const Point& beginA, const Point& endA, const Point& beginB, const Point& endB) const;
};

#endif // TOUR_H
