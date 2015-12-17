// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: write comment header for this file; remove this comment

#include "costs.h"
#include "trailblazer.h"
#include "pqueue.h"
#include <queue>

using namespace std;

/*
 * Recursive depth-first exploration algorithm, called by depthFirstSearch.
 */
bool depthFirstExplore(BasicGraph& graph, Node* current, Node* end, vector<Node*>& pathTrace){

    //Set current node data
    pathTrace.push_back(current);
    current->setColor(GREEN);
    current->visited = true;

    //Check if current node is target
    if(current == end)
        return true;

    //For every unvisited neighbor, make recursive call to depthFirstExplore
    for(Vertex* neighbor : graph.getNeighbors(current)){

        if(!neighbor->visited){

            neighbor->setColor(YELLOW);
            if(depthFirstExplore(graph, neighbor, end, pathTrace))
                return true;
        }
    }

    //Backtrack
    pathTrace.pop_back();
    current->setColor(GRAY);

    return false;
}

/*
 * Depth-first search. Will explore from 'start' until it finds 'end' or can determine that no valid path exists.
 */
vector<Node*> depthFirstSearch(BasicGraph &graph, Node *start, Node *end){
    graph.resetData();

    vector<Node*> path;
    depthFirstExplore(graph, start, end, path);

    return path;
}

/*
 * Breadth-first search. Will explore from 'end' until it finds 'start' or can determine that no valid path exists.
 * It explores in reverse to make it easier to build path array.
 */
vector<Node *> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    vector<Vertex*> path;
    queue<Vertex*> openQueue;

    //Setup starting state
    end->visited = true;
    openQueue.push(end);
    Vertex* current = nullptr;

    //While there are still reachable nodes to explore
    while(!openQueue.empty()){

        //Set current node to the next node in the queue
        current = openQueue.front();
        openQueue.pop();
        current->setColor(GREEN);

        //If current node is target, build path and return
        if(current == start){

            rewind(end, start, path);
            return path;
        }

        //Add any unvisited neighbor to queue
        for(Vertex* neighbor : graph.getNeighbors(current)){
            if(!neighbor->visited){

                neighbor->previous = current;
                neighbor->visited = true;
                openQueue.push(neighbor);
                neighbor->setColor(YELLOW);
            }
        }
    }

    //No path could be found
    return path;
}

/*
 * Dijkstra search. Will explore from 'end' until it finds 'start' or can determine that no valid path exists.
 * It explores in reverse to make it easier to build path array.
 */
vector<Node *> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    vector<Vertex*> path;
    PriorityQueue<Vertex*> openQueue;

    //Setup starting state
    end->cost = 0;
    end->visited = true;
    openQueue.enqueue(end, end->cost);
    Vertex* current = nullptr;

    //While there are still reachable nodes to explore
    while(!openQueue.isEmpty()){

        //Set current node to the next node in the queue
        current = openQueue.dequeue();
        current->setColor(GREEN);

        //If current node is target, build path and return
        if(current == start){

            rewind(end, start, path);
            return path;
        }

        //Add any unvisited neighbor to queue, adjust cost for already visited neighbor nodes
        for(Vertex* neighbor : graph.getNeighbors(current)){
            if(!neighbor->visited){

                neighbor->previous = current;
                neighbor->visited = true;
                neighbor->cost = current->cost + graph.getArc(current, neighbor)->cost;
                openQueue.enqueue(neighbor, neighbor->cost);
                neighbor->setColor(YELLOW);
            }else{

                double newCost = current->cost + graph.getArc(current, neighbor)->cost;
                if(neighbor->cost > newCost){

                    neighbor->previous = current;
                    neighbor->cost = newCost;
                    openQueue.changePriority(neighbor, neighbor->cost);
                    neighbor->setColor(YELLOW);
                }
            }
        }
    }
    return path;
}

/*
 * A* search. Will explore from 'end' until it finds 'start' or can determine that no valid path exists.
 * It explores in reverse to make it easier to build path array.
 */
vector<Node *> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    vector<Vertex*> path;
    PriorityQueue<Vertex*> openQueue;

    //Setup starting state
    end->cost = 0;
    end->visited = true;
    openQueue.enqueue(end, end->cost);
    Vertex* current = nullptr;

    //While there are still reachable nodes to explore
    while(!openQueue.isEmpty()){

        //Set current node to the next node in the queue
        current = openQueue.dequeue();
        current->setColor(GREEN);

        //If current node is target, build path and return
        if(current == start){

            rewind(end, start, path);
            return path;
        }

        //Add any unvisited neighbor to queue, adjust cost for already visited neighbor nodes
        for(Vertex* neighbor : graph.getNeighbors(current)){

            if(!neighbor->visited){

                neighbor->previous = current;
                neighbor->visited = true;
                neighbor->cost = current->cost + graph.getArc(current, neighbor)->cost;
                openQueue.enqueue(neighbor, neighbor->cost + neighbor->heuristic(start));
                neighbor->setColor(YELLOW);
            }else{

                double newCost = current->cost + graph.getArc(current, neighbor)->cost;
                if(neighbor->cost > newCost){

                    neighbor->previous = current;
                    neighbor->cost = newCost;
                    openQueue.changePriority(neighbor, neighbor->cost + neighbor->heuristic(start));
                    neighbor->setColor(YELLOW);
                }
            }
        }
    }
    return path;
}

/*
 * Where 'from' is the head of a linked list and 'to' is the tail,
 * builds an array containing the linked list in reverse order.
 */
void rewind(Node *from, Node *to, vector<Node*>& output)
{
    Vertex* rewindTemp = to;

    while(rewindTemp != from){

        output.push_back(rewindTemp);
        rewindTemp = rewindTemp->previous;
    }

    output.push_back(from);
}
