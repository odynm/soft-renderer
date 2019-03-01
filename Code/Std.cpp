#include "Std.h"

String 
NewString(int length)
{
	String str = {};
	str.length = length;
	CleanAlloc(sizeof(char) * length);
	
	return str;
}

String 
NewString(char* text)
{
	String str = {};
	str.text = text;
	int length = 0;
	for (; str.text[length]; length++){}
	str.length = length;
	
	return str;
}

void 
CleanStringBuffer(char* buffer, int length)
{
    for (int i = 0; i < length; i++)
    {
        buffer[i] = '\0';
    }
}

void 
CleanStringBuffer(String str)
{
    for (int i = 0; i < str.length; i++)
    {
        str.text[i] = '\0';
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