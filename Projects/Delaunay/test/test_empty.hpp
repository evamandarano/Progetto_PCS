#ifndef TEST_TRIANGULATION_HPP
#define TEST_TRIANGULATION_HPP

#include "triangulation.hpp"
#include <gtest/gtest.h>

class TriangulationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Inizializza i dati per i test
    }

    void TearDown() override {
        // Pulisci i dati dopo i test
    }

    // Aggiungi i metodi di test per i vari casi

    std::vector<Point> points;
};

TEST_F(TriangulationTest, TestPointInsideTriangle) {
    // Testa se un punto è all'interno di un triangolo
    // Utilizza points per inizializzare i punti del triangolo
    Point p1(0, 0);
    Point p2(1, 0);
    Point p3(0, 1);
    Triangle triangle(p1, p2, p3);

    Point insidePoint(0.5, 0.5);
    ASSERT_TRUE(triangle.isPointInside(insidePoint));

    Point outsidePoint(2, 2);
    ASSERT_FALSE(triangle.isPointInside(outsidePoint));
}

TEST_F(TriangulationTest, TestOrderPointsCounterclockwise) {
    // Testa l'ordinamento dei punti in senso antiorario
    // Utilizza points per inizializzare i punti del triangolo
    Point p1(1, 0);
    Point p2(0, 1);
    Point p3(0, 0);
    Triangle triangle(p1, p2, p3);

    triangle.orderPointsCounterclockwise();

    ASSERT_DOUBLE_EQ(triangle.p1.x, 0);
    ASSERT_DOUBLE_EQ(triangle.p1.y, 0);
    ASSERT_DOUBLE_EQ(triangle.p2.x, 1);
    ASSERT_DOUBLE_EQ(triangle.p2.y, 0);
    ASSERT_DOUBLE_EQ(triangle.p3.x, 0);
    ASSERT_DOUBLE_EQ(triangle.p3.y, 1);
}

// Aggiungi altri test per gli altri metodi e classi

#endif  // TEST_TRIANGULATION_HPP
