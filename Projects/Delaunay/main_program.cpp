#include "empty_class.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <sstream>
// Classe Point 
class Point {
public:
    // definisco gli attributi in public (possono essere ereditati)
    double x; // attributi
    double y;
    // double == 
    Point(double _x, double _y) : x(_x), y(_y) {}
    // x(_x), y(_y) è la lista di inizializzazione. È una sintassi speciale che viene utilizzata per inizializzare i membri dati della classe con i valori dei parametri passati al costruttore. In questo caso, x(_x) inizializza il membro dati x con il valore di _x, e y(_y) inizializza il membro dati y con il valore di _y.
    
};

class Triangle {
public:
    Point p1;
    Point p2;
    Point p3;

    Triangle(const Point& _p1, const Point& _p2, const Point& _p3) : p1(_p1), p2(_p2), p3(_p3) {}

    double calculateArea() const {
        return std::abs((p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y)) / 2);
    }

    void orderPointsCounterclockwise() {
        std::vector<Point> points = {p1, p2, p3};
        std::sort(points.begin(), points.end(), [](const Point& p1, const Point& p2) {
            return (std::atan2(p1.y, p1.x) < std::atan2(p2.y, p2.x));
        });

        p1 = points[0];
        p2 = points[1];
        p3 = points[2];
    }

    bool isPointInside(const Point& Q) const {
        double totalArea = calculateArea();
        double area1 = calculateArea(Q, p2, p3);
        double area2 = calculateArea(p1, Q, p3);
        double area3 = calculateArea(p1, p2, Q);

        return std::abs(totalArea - (area1 + area2 + area3)) < 1e-6;
    }

private:
    double calculateArea(const Point& p1, const Point& p2, const Point& p3) const {
        return std::abs((p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y)) / 2);
    }
};

std::vector<Point> importPointsFromCSV(const std::string& filename) {
    std::vector<Point> points;

    std::ifstream file(filename);
    if (!file) {
        std::cout << "Error opening file: " << filename << std::endl;
        return points;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string xStr, yStr;
        if (std::getline(ss, xStr, ',') && std::getline(ss, yStr, ',')) {
            double x = std::stod(xStr);
            double y = std::stod(yStr);
            points.emplace_back(x, y);
        }
    }

    file.close();
    return points;
}

bool findTriangleContainingPoint(const Point& Q, const std::vector<Triangle>& triangles, Triangle& result) {
    for (const auto& triangle : triangles) {
        if (triangle.isPointInside(Q)) {
            result = triangle;
            return true;
        }
    }
    return false;
}

void connectPointWithTriangle(const Point& Q, const Triangle& T) {
    // Implementa la logica per unire il punto Q con i vertici del triangolo T
    // Ad esempio, puoi stampare i segmenti che collegano Q
    std::vector<Point> points = importPointsFromCSV("points.csv");
