#include "empty_class.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <sstream>

using namespace std;
// Classe Point
class Point {
public:
    double x; // attributi
    double y;
    // Costruttore della classe point, prende in input due valori double
    Point(double _x, double _y) : x(_x), y(_y) {}

};
// Definisco il concetto di distanza euclidea fuori della classe Triangle così che posso usarla all'interno della classe sempre senza problemi 
double distance(const Point& p1, const Point& p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return sqrt(dx * dx + dy * dy); // distanza euclidea sqrt(x^2 + y^2)
}
// Definisco la classe Triangle
class Triangle
{
public:
    // Attributi
    Point p1;
    Point p2;
    Point p3;
    // Costruttore
    Triangle(const Point& _p1, const Point& _p2, const Point& _p3) : p1(_p1), p2(_p2), p3(_p3) {}
public:
    // primo metodo della classe, restituisce un doubke
    // const alla fine della dichiarazione indica che la funzione è un membro const e non modificherà lo stato degli attributi dell'oggetto
    double calculateArea() const
    {
        // Calcolo l'area del triangolo
        return abs((p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y)) / 2);
    }
    // Ordina i punti in senso antiorario rispetto al centroide del triangolo
    void orderPointsCounterclockwise() {
        // creo un oggetto di tipo point, chiamato centroide, che rappresenta il cnetro geometrico del triangolo
        // il centroide viene calcolato facendo la media della coordinate 
        Point centroid((p1.x + p2.x + p3.x) / 3.0, (p1.y + p2.y + p3.y) / 3.0);
        // Creo un vettore di punti vector<point>  chiamato points che contiene i punti p1,p2 e p3 
        vector<Point> points = { p1, p2, p3 };

        // Ordina i punti in base all'angolo rispetto al centroide
        // Utilizza la funzione sort della libreria standard per ordinare i punti nel vettore points
        // La funzione sort prende come parametri il punto di inizio (points.begin()) e il punto di fine (points.end()) del range da ordinare.
        // Ordina i punti utilizzando il prodotto scalare per determinare l'ordine
        // Il terzo elemento della funzione sort indica il criterio di confronto per ordinare gli elementi nel range specificato
        // è definito come una lambda function che prende due punti p1 e p2 come input. 
        // La lambda function calcola i vettori v1 e v2 come differenze tra i punti e il baricentro (centroid)
        sort(points.begin(), points.end(), [&centroid](const Point& p1, const Point& p2)
             // Il terzo argomento della funzione sort è una lamda function, vedi referenza 
        {
            const Point v1(p1.x - centroid.x, p1.y - centroid.y);
            const Point v2(p2.x - centroid.x, p2.y - centroid.y);
            double dot = v1.x * v2.x + v1.y * v2.y;
            return dot > 0;
            // v1 rappresenta il vettore che va dal baricentro al punto p1 
            // v2 rappresenta il vettore che va dal baricentro al punto p2 
            // La funzione dot calcola il prodotto scalare 
            // per verificare se il vettore v1 è "più a sinistra" del vettore v2, rispetto al baricentro.
            // Verifica che il prodotto scalare sia positivo
            // Se il prodotto scalare è positivo, significa che il vettore v1 è più a sinistra di v2 rispetto al baricentro, quindi p1 verrà posizionato prima di p2 nell'ordinamento. Se il prodotto scalare è negativo, l'ordine sarà inverso.
        });
        p1 = points[0];
        p2 = points[1];
        p3 = points[2];
    }
    // Verifica se un punto è interno al triangolo
    bool isPointInside(const Point& Q) const {
    double Ax = p1.x;
    double Ay = p1.y;
    double Bx = p2.x;
    double By = p2.y;
    double Cx = p3.x;
    double Cy = p3.y;
    double Qx = Q.x;
    double Qy = Q.y;
    // metodo del determinante per verificare che il punto sia interno alla circonferenza circoscritta al triangolo 
    double determinant = Ax * (By - Cy) - Ay * (Bx - Cx) + (Bx * Cy - By * Cx) - Qx * (Ay - Cy) + Qy * (Ax - Cx);

    return determinant > 0;
    }
};
// Cerca il triangolo di area massima, a partire dal vettore di punti points; è quello da cui partire 
    Triangle findTriangleWithMaxArea(const vector<Point>& points) {
        double maxArea = 0.0;
        Triangle maxAreaTriangle(points[0], points[1], points[2]);

        // Itera su tutte le combinazioni possibili di tre punti
        for (size_t i = 0; i < points.size() - 2; i++) {
            for (size_t j = i + 1; j < points.size() - 1; j++) {
                for (size_t k = j + 1; k < points.size(); k++) {
                    Triangle triangle(points[i], points[j], points[k]);
                    double area = triangle.calculateArea();

                    if (area > maxArea) {
                        maxArea = area;
                        maxAreaTriangle = triangle;
                    }
                }
            }
        }

        return maxAreaTriangle;
}
    vector<Triangle> DelaunayTriangulation(const vector<Point>& points) 
    {
        // vettore vuoto di triangoli
        vector<Triangle> triangles;

        // Trova il primo triangolo con area massima
        Triangle maxAreaTriangle = findTriangleWithMaxArea(points);
        // aggiungo il triangolo di area massima al vettore di triangoli
        triangles.push_back(maxAreaTriangle);

        // Ordina i punti in senso antiorario rispetto al triangolo massimo
        maxAreaTriangle.orderPointsCounterclockwise();

        // Itera sui punti rimanenti
        // PERCHè SIZE_T
        // size_t è un tipo di dato intero non negativo utilizzato per rappresentare la dimensione o l'indice di un oggetto o di un contenitore. 
        // size_t è un tipo intero senza segno che viene utilizzato principalmente per indicare dimensioni di array, indici di ciclo e valori di ritorno per funzioni che rappresentano la dimensione o l'indice di un oggetto
        for (size_t i = 3; i < points.size(); i++) {
            const Point& currentPoint = points[i];

            // Verifica se il punto è interno a uno dei triangoli esistenti (iterando su tutti i punti del vettore points) 
            bool isInside = false;
            // Itero su tutti i triangoli presenti nel vettore triangles 
            for (const auto& triangle : triangles) {
                if (triangle.isPointInside(currentPoint)) {
                    isInside = true;
                    break;
                }
            }
            if (isInside) {
                // Se il punto è interno, localizza il triangolo che lo contiene
                // crea un nuovo oggetto di tipo Triangle chiamato containingTriangle utilizzando i primi tre punti dell'array points come argomenti per il costruttore di Triangle
                Triangle containingTriangle(points[0], points[1], points[2]);
                // Quindi, containingTriangle rappresenta un triangolo iniziale in cui si suppone che il punto corrente sia contenuto
                for (const auto& triangle : triangles) {
                    if (triangle.isPointInside(currentPoint)) { //  per controllare se il punto è interno al triangolo corrente
                        containingTriangle = triangle; // Se il punto è interno a uno dei triangoli, viene impostato il flag isInside a true
                        break;
                        // l'istruzione break viene utilizzata per interrompere il ciclo nel momento in cui troviamo il triangolo "utile"
                    }
                    // Il ciclo for (const auto& triangle : triangles) itera attraverso tutti gli elementi dell'array triangles, assegnando ogni elemento alla variabile triangle nel corpo del ciclo. 
                    // Quindi, per ogni triangolo nell'array triangles, viene eseguito il blocco di istruzioni all'interno del ciclo. 
                    // In questo caso, il ciclo viene utilizzato per cercare un triangolo che contenga il punto corrente utilizzando il metodo isPointInside(currentPoint) dell'oggetto triangle. 
                    // Se viene trovato un triangolo che contiene il punto, viene assegnato a containingTriangle il valore di triangle e il ciclo viene interrotto utilizzando l'istruzione break.
                }
                // se il punto currentPoint è interno a uno dei triangoli esisteti allor aprocedo con l'esecuzione di questo blocco 
                // Crea una sottotriangolazione collegando il punto con i vertici del triangolo
                // viene trovato il triangolo containingTriangle che contiene il punto, rimuovendolo dall'array triangles. 
                // Successivamente, vengono creati tre nuovi triangoli collegando il punto currentPoint ai vertici di containingTriangle, e questi vengono aggiunti all'array triangles.
                triangles.erase(remove(triangles.begin(), triangles.end(), containingTriangle), triangles.end());
                triangles.push_back(Triangle(containingTriangle.p1, containingTriangle.p2, currentPoint));
                triangles.push_back(Triangle(containingTriangle.p2, containingTriangle.p3, currentPoint));
                triangles.push_back(Triangle(containingTriangle.p3, containingTriangle.p1, currentPoint));
            } else {
                // Se il punto è esterno, collega il punto con i vertici di tutti i triangoli esistenti
                for (const auto& triangle : triangles) {
                    triangles.push_back(Triangle(triangle.p1, triangle.p2, currentPoint));
                    triangles.push_back(Triangle(triangle.p2, triangle.p3, currentPoint));
                    triangles.push_back(Triangle(triangle.p3, triangle.p1, currentPoint));
                }
            }
        }

        return triangles;
    }

