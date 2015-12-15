// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: write comment header for this file; remove this comment

#include "costs.h"
#include "trailblazer.h"
#include "pqueue.h"
#include <queue>
#include <stack>
#include <set>
#include <unordered_set>

using namespace std;

bool depthFirstExplore(BasicGraph& graph, Node* current, Node* end, vector<Node*>& pathTrace){

    pathTrace.push_back(current);

    current->setColor(GREEN);
    current->visited = true;

    if(current == end)
        return true;

    for(Vertex* neighbor : graph.getNeighbors(current)){

        if(!neighbor->visited){

            neighbor->setColor(YELLOW);
            if(depthFirstExplore(graph, neighbor, end, pathTrace))
                return true;
        }
    }

    pathTrace.pop_back();
    current->setColor(GRAY);

    return false;
}

vector<Node*> depthFirstSearch(BasicGraph &graph, Node *start, Node *end){
    graph.resetData();
    vector<Node*> path;

    depthFirstExplore(graph, start, end, path);

    return path;
}

vector<Node *> depthFirstSearchAlternative(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();

    vector<Vertex*> path;
    stack<Vertex*> openStack;
    openStack.push(end);

    Vertex* current = nullptr;
    while(!openStack.empty()){

        current = openStack.top();
        openStack.pop();
        current->setColor(GREEN);

        if(current == start){

            Vertex* rewindTemp = current;

            cout << "Rewind..." << endl;
            while(rewindTemp != end){

                path.push_back(rewindTemp);
                rewindTemp = rewindTemp->previous;
            }

            path.push_back(end);
            return path;
        }

        for(Vertex* neighbor : graph.getNeighbors(current)){

            if(!neighbor->visited){

                neighbor->previous = current;
                neighbor->visited = true;
                openStack.push(neighbor);
                neighbor->setColor(YELLOW);
            }
        }
    }

    //No path could be found
    return path;
}

vector<Node *> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    vector<Vertex*> path;
    queue<Vertex*> openQueue;

    end->visited = true;
    openQueue.push(end);

    Vertex* current = nullptr;
    while(!openQueue.empty()){

        current = openQueue.front();
        openQueue.pop();
        current->setColor(GREEN);

        if(current == start){

            rewind(end, start, path);
            return path;
        }

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


vector<Node *> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    vector<Vertex*> path;
    PriorityQueue<Vertex*> openQueue;

    end->cost = 0;
    end->visited = true;
    openQueue.enqueue(end, end->cost);

    Vertex* current = nullptr;
    while(!openQueue.isEmpty()){

        current = openQueue.dequeue();
        current->setColor(GREEN);

        if(current == start){

            rewind(end, start, path);
            return path;
        }

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

vector<Node *> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    graph.resetData();
    vector<Vertex*> path;
    PriorityQueue<Vertex*> openQueue;

    end->cost = 0;
    end->visited = true;
    openQueue.enqueue(end, end->cost);

    Vertex* current = nullptr;
    while(!openQueue.isEmpty()){

        current = openQueue.dequeue();
        current->setColor(GREEN);

        if(current == start){

            rewind(end, start, path);
            return path;
        }

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

void rewind(Node *from, Node *to, vector<Node*>& output)
{
    Vertex* rewindTemp = to;

    while(rewindTemp != from){

        output.push_back(rewindTemp);
        rewindTemp = rewindTemp->previous;
    }

    output.push_back(from);
}
