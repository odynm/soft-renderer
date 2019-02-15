#include "Std.h"

String 
NewString(int length)
{
	//TODO implement
	String str = {};
	return str;
}

String 
NewString(char* text)
{
	//TODO implement
	String str = {};
	return str;
}

void 
CleanStringBuffer(char* buffer, int length)
{
	//TODO Change this to "ChangeString", accepting a string
    for (int i = 0; i < length; i++)
    {
        buffer[i] = '\0';
    }
}

void 
SubString(char* destination, char* source, int start, int end)
{
    int length = end - start;
    for (int i = 0; i <= length; i++)
    {
        destination[i] = source[start + i];
    }
}