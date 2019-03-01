#ifndef STD_H
#define STD_H

#include "Renderer.h"

//
// A string implementation. Uses dynamic alloc, and is not needed in most cases where char is just enough
//
typedef struct String 
{
	int length;
	char* text;
} String;

String NewString(int length);
String NewString(char* text);
void CleanStringBuffer(char* buffer, int length);
void SubString(char* destination, char* source, int start, int end);

#endif