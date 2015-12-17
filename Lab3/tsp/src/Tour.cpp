#include <iostream>
#include "Tour.h"
#include "Node.h"
#include "Point.h"

Tour::Tour()
{
    first = nullptr;
}

Tour::~Tour()
{

    Node* current = first->next;
    first->next = nullptr;

    while(current != nullptr){

        first = first->next;
        delete current;
        current = first;
    }

    first = nullptr;
}

/*
 * Prints the tour as text in the console.
 */
void Tour::show()
{
    if(first == nullptr)
        return;

    Node* current = first;

    do{

        cout << current->point.toString() << endl;
        current = current->next;
    }while(current != first);
}

/*
 * Draws the tour to the scene.
 */
void Tour::draw(QGraphicsScene *scene)
{
    if(first == nullptr)
        return;

    Node* current = first;

    do{

        current->point.draw(scene);
        current->point.drawTo(current->next->point, scene);
        current = current->next;
    }while(current != first);
}


/*
 * Returns the number of points in the tour.
 */
int Tour::size()
{
    if(first == nullptr)
        return 0;

    int size = 0;
    Node* current = first;
    do{

        size++;
        current = current->next;
    }while(current != first);

    return size;
}


/*
 * Returns the total distance of the tour.
 */
double Tour::distance()
{
    if(first == nullptr)
        return 0;

    double distance = 0;
    Node* current = first;
    do{

        distance += current->point.distanceTo(current->next->point);
        current = current->next;
    }while(current != first);

    return distance;
}


/*
 * Inserts the point behind the point closest to it.
 */
void Tour::insertNearest(Point p)
{
    if(first == nullptr){

        first = new Node(p);
        first->next = first;
    }else{

        Node* current = first;
        Node* closestNode = first;
        double closestDist = first->point.distanceTo(p);

        double currentDist;
        while(current->next != first){

            current = current->next;
            currentDist = current->point.distanceTo(p);
            if(currentDist < closestDist){

                closestNode = current;
                closestDist = currentDist;
            }
        }
        insertAfter(p, closestNode);
    }
}


/*
 * Inserts the point where it results in the smallest growth to the tour.
 */
void Tour::insertSmallest(Point p)
{
    if(first == nullptr){

        first = new Node(p);
        first->next = first;
    }else{

        Node* current = first;
        Node* smallestDeltaNode = first;
        double smallestDelta = current->point.distanceTo(p) + current->next->point.distanceTo(p) - current->point.distanceTo(current->next->point);

        double currentDelta;
        while(current->next != first){

             current = current->next;
             currentDelta = current->point.distanceTo(p) + current->next->point.distanceTo(p) - current->point.distanceTo(current->next->point);

             if(currentDelta < smallestDelta){

                 smallestDeltaNode = current;
                 smallestDelta = currentDelta;
             }
        }
        insertAfter(p, smallestDeltaNode);
    }
}

void Tour::twoOpt()
{

    Node* currentOuter = first;
    Node* currentInner = nullptr;

    while(currentOuter->next != first){
         currentOuter = currentOuter->next;
         currentInner = currentOuter;

         cout << currentOuter->toString() << endl;
         while(currentOuter->next != first){
             currentInner = currentInner->next;

             //if(linesIntersect(currentInner->point, currentInner->next->point, currentOuter->point, currentOuter->next->point)){

             cout << currentOuter << ":" << currentInner << endl;
                 swapNext(currentInner, currentOuter);
             //}
         }
    }
}

/*
 * Swaps next node values for two nodes
 */
void Tour::swapNext(Node* a, Node* b)
{
    Node* aNext = a->next;
    a->next = b->next;
    b->next = aNext;
}

/*
 * Creates a new node containing the given point and inserts it after the node.
 */
void Tour::insertAfter(const Point& point, Node* node){

    Node* temp = node->next;
    node->next = new Node(point);
    node->next->next = temp;
}

inline double Dot(const Point& a,const Point& b)                        { return (a.x*b.x) + (a.y*b.y); }
inline double PerpDot(const Point& a,const Point& b)                    { return (a.y*b.x) - (a.x*b.y); }

bool Tour::linesIntersect(const Point& beginA, const Point& endA, const Point& beginB, const Point& endB) const
{
    Point a{endA.x - beginA.x, endA.y - beginA.y};
    Point b{endB.x - beginB.x, endB.y - beginB.y};

    double f = PerpDot(a, b);
    if(f == 0)
        return false;

    Point c(endB.x-endA.x, endB.y-endA.y);
    double aa = PerpDot(a,c);
    double bb = PerpDot(b,c);

    if(f < 0)
    {
        if(aa > 0)     return false;
        if(bb > 0)     return false;
        if(aa < f)     return false;
        if(bb < f)     return false;
    }
    else
    {
        if(aa < 0)     return false;
        if(bb < 0)     return false;
        if(aa > f)     return false;
        if(bb > f)     return false;
    }

    return true;
}


