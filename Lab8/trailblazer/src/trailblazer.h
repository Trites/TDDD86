/*
 * TDDD86 Trailblazer
 * This file declares the functions you will write in this assignment.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * Author: Marty Stepp
 * Slight modifications by Tommy Farnqvist
 */

#ifndef _trailblazer_h
#define _trailblazer_h

#include <vector>
#include "BasicGraph.h"

vector<Node*> depthFirstSearch(BasicGraph& graph, Node* start, Node* end);
bool depthFirstExplore(BasicGraph& graph, Node* current, Node* end, vector<Node*>& pathTrace);
vector<Node*> depthFirstSearchAlternative(BasicGraph& graph, Node* start, Node* end);
vector<Node*> breadthFirstSearch(BasicGraph& graph, Node* start, Node* end);
vector<Node*> dijkstrasAlgorithm(BasicGraph& graph, Node* start, Node* end);
vector<Node*> aStar(BasicGraph& graph, Node* start, Node* end);
void rewind(Node* from, Node* to, vector<Node*>& output);

#endif
