#pragma once

struct Vector2
{
    Vector2();
    Vector2(float x, float y) : x(x), y(y) {}
    
    float x = 0;
    float y = 0;
};

struct Vector3
{
    Vector3() {}
    Vector3(float x, float y) : x(x), y(y) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    
    float x = 0;
    float y = 0;
    float z = 0;
};

struct Vector2d
{
    Vector2d();
    Vector2d(double x, double y) : x(x), y(y) {}
    
    double x = 0;
    double y = 0;
};

struct Vector3d
{
    Vector3d() {}
    Vector3d(double x, double y) : x(x), y(y) {}
    Vector3d(double x, double y, double z) : x(x), y(y), z(z) {}
    
    double x = 0;
    double y = 0;
    double z = 0;
};