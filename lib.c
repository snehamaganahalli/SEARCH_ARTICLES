#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<ctype.h>

#include "lib.h"
#include "search.h"

char* stringRemoveNonAlphaNum(char *str)
{
    unsigned long i = 0;
    unsigned long j = 0;
    char c;

    while ((c = str[i++]) != '\0')
    {
        if (isalnum(c) || c == ' ' )
        {
            str[j++] = c;
        }
    }
    str[j] = '\0';
    return str;
}
char* strlwr(char* s)
{
    char* tmp = s;

    for (;*tmp;++tmp) {
        *tmp = tolower((unsigned char) *tmp);
    }

    return s;
}

void close_file(FILE* fp)
{
  if(NULL != fp)
  {
    fclose(fp);
  }
}

FILE* open_file(char* file_name, char* mode)
{
  FILE* fp = NULL;
  char file_path[STRLEN] = {0};

  printf("file is %s\n", file_name );

  snprintf(file_path, STRLEN, "%s%s", ARTICLES_DIR, file_name);
  fp = fopen(file_path, mode);
  if(fp == NULL)
  {
    printf("Error in opening file");
    perror("open:");
    exit(1);
  }
  return fp;
}

