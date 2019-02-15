#ifndef MODEL_H
#define MODEL_H

#include "Renderer.h"
#include "MathLib.h"
#include "Std.h"

typedef struct Face
{
    int data[3];
} Face;

typedef struct Model
{
    int facesCount;
    int vertexCount;
    Face* faces;
    Vector3* vertices;
} Model;

Face CreateFace(int a, int b, int c);
Model* LoadModel(char* file);

#endif