#ifndef LIB_H
#define LIB_H

char* stringRemoveNonAlphaNum(char *str);
char* strlwr(char* s);
FILE* open_file(char* file_name, char* mode);
void close_file(FILE* fp);
void _strncpy(char* dst, char*src, size_t size);
#endif
