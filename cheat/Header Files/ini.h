/*ini.h*/
#ifndef INI_H
#define INI_H

#include <stdio.h>  
#include <string.h>
#include <string>

char* bool2c(bool a);

bool c2bool(char* a);

int GetIniKeyString(const char* title, const char* key, const char* filename, char* buf);


int PutIniKeyString(const char* title, const char* key, char* val, const char* filename);


#endif /*INI_H*/