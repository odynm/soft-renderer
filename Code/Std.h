#ifndef STD_H
#define STD_H

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