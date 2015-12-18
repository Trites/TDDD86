/*
 * TDDD86 Pattern Recognition
 * This program computes and plots all line segments involving 4 points
 * in a file using Qt.
 */

#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <map>
#include <set>
#include <queue>
#include <chrono>
#include "Point.h"

// constants
static const int SCENE_WIDTH = 512;
static const int SCENE_HEIGHT = 512;

void render_points(QGraphicsScene* scene, const vector<Point>& points) {
    for(const auto& point : points) {
        point.draw(scene);
    }
}

void render_line(QGraphicsScene* scene, const Point& p1, const Point& p2) {
    p1.lineTo(scene, p2);
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // open file
    string filename = "input100.txt";
    ifstream input;
    input.open(filename);

    // read points from file
    int N;
    int x;
    int y;

    input >> N;

    //Put points in set for sorting
    set<Point> orderedPoints;
    for (int i = 0; i < N; ++i) {
        input >> x >> y;
        orderedPoints.insert(Point(x, y));
    }
    input.close();

    //Transfer sorted points to vector
    vector<Point> points {std::begin(orderedPoints), std::end(orderedPoints)};


    // setup graphical window
    QGraphicsView *view = new QGraphicsView();
    QGraphicsScene *scene = new QGraphicsScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    view->setScene(scene);
    // draw points to screen all at once
    render_points(scene, points);
    view->scale(1, -1); //screen y-axis is inverted
    view->resize(view->sizeHint());
    view->setWindowTitle("Brute Force Pattern Recognition");
    view->show();


    auto begin = chrono::high_resolution_clock::now();


    map<double, set<Point>> drawnSlopes; //Allows us to avoid drawing sub-lines, reduces draw calls from 369 (103 lines) to 277 (62 lines)


    int head = 1; //Used to limit search space

    //Let all points be origo
    //NOTE: points vector is sorted in this implementation.
    for(const Point& origo : points){ //n

        //Keep a map where points will be inserted by their slope to current origin
        //They will also be sorted based on Point::operator<
        map<double, set<Point>> lineMap;

        //We can start the search at head, since head-1 has already acted as origin an will not be a part of any new lines
        for(unsigned int i = head; i < points.size(); ++i){ // n (Dependent on 'head': n, n-1, n-2, ... , n-n)

            lineMap[origo.slopeTo(points[i])].insert(points[i]); // Log(n)
        }

        //For every entry in the lineMap
        for (const std::pair<double, set<Point>> &entry: lineMap){ //n (If no points share slope to origo)

            //If the size of the line is larger than 2 excluding the origin, it is a valid slope.
            if(entry.second.size() > 2){

                //Add origin to line
                set<Point> line = entry.second;
                line.insert(origo);
                Point lineEnd = *line.rbegin();

                //If the point has already been drawn with the same slope value we can discard the line as a duplicate
                if(drawnSlopes[entry.first].count(lineEnd) == 0){

                    //Record that each point has been a part of this slope
                    drawnSlopes[entry.first].insert(lineEnd);


                    Point lineStart = *line.begin();
                    lineStart.lineTo(scene, lineEnd);
                }
            }
        }

        ++head;
    }

    auto end = chrono::high_resolution_clock::now();
    cout << "Computing line segments took "
         << std::chrono::duration_cast<chrono::milliseconds>(end - begin).count()
         << " milliseconds." << endl;
    return a.exec(); // start Qt event loop
}

