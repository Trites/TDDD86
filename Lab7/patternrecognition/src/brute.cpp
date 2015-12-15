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

template<int origoX, int origoY>
class DistanceCompare
{
public:
    bool operator() (Point a, Point b)
    {


        float distA = a.x - origoX + a.y - origoY;
        float distB = b.x - origoX + b.y - origoY;

        return distA*distA < distB*distB;
    }
};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // open file
    string filename = "input100.txt";
    ifstream input;
    input.open(filename);

    // the vector of points
    vector<Point> points;

    // read points from file
    int N;
    int x;
    int y;

    input >> N;

    for (int i = 0; i < N; ++i) {
        input >> x >> y;
        points.push_back(Point(x, y));
    }
    input.close();

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

    int head = 1;
    for(const Point& origo : points){

        map<double, priority_queue<Point>> lineMap;
        for(unsigned int i = head; i < points.size(); ++i){


            lineMap[origo.slopeTo(points[i])].push(points[i]);
        }

        for (const std::pair<double, priority_queue<Point>> &entry: lineMap){

            if(entry.second.size() > 2){


                priority_queue<Point> line = entry.second;
                line.push(origo);


                while(line.size() > 1){

                    Point p = line.top();
                    line.pop();
                    p.lineTo(scene, line.top());
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

