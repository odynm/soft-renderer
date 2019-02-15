#ifndef MATH_LIB_H
#define MATH_LIB_H

#define SwapInt(x,y) do { int temp = x; x = y; y = temp; } while(0)

struct Vector3
{
    float x,y,z;
};

struct Vector2
{
    float x, y;
};

Vector3 CreateVector3(float x, float y, float z);

#endif