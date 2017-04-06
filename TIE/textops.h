#pragma once
#include <cstdlib>

// Appends single char at the end of str 
char *AppendCharPtr(const char *str, const char c);
// Returns file extension (without dot). If no extension is found empty char* is returned
char *GetFilenameExt(char *filename);
// Checks if two char pointer arrays are equal
bool MyStrCmp(char *str1, char *str2);
// Counts chars in char pointer array
int MyStrLen(const char *str);