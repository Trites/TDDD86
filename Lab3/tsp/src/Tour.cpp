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

/*
 * Creates a new node containing the given point and inserts it after the node.
 */
void Tour::insertAfter(const Point& point, Node* node){

    Node* temp = node->next;
    node->next = new Node(point);
    node->next->next = temp;
}
