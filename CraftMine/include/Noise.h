#pragma once

#include <math.h>

#include <vector>
#include <iostream>

#include "Parameters.h"


struct MyVec2
{
    double x, y;
};


class Noise final {
private:
    static Noise* instance;

    int p[512];
    static const int permutation[256];

    Noise(double s) : seed{ s } { init(); };

    void init();
    inline double dot(MyVec2 a, MyVec2 b) { return a.x * b.x + a.y * b.y; }
    double grad(int hash, double x, double y, double z);
    MyVec2 GetConstantVector(int v);

    inline double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
    inline double lerp(double t, double a, double b) { return a + t * (b - a); }

public:
    double seed;

    static Noise* getInstance();
    
    double classicNoise(double x, double y);
    double* detailed2DNoise(double x, double y, int resolution);
    double* detailed3DNoise(double x, double y, double z, int resolution, int height);
    double smoothNoise(double x, double y, double z);

    double noise1D(double x);
    double noise2D(double x, double y, int numOfOctave);
    double noise3D(double x, double y, double z);
};
