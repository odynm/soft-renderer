#ifndef RENDERER_H
#define RENDERER_H

#include "Model.h"

struct OffscreenBuffer
{
	void* memory;
	int width;
	int height;
	int pitch;
};

void* Alloc(int size);
void Release(void* address);
void Print(char* buffer);
void UpdateAndRender(OffscreenBuffer* buffer);
void ProfileStart();
void ProfileEnd();
char* OpenFile(char* path);

#endif

