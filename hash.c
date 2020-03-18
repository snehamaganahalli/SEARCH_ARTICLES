#include<stdio.h>

#include "search.h"

int calculate_key(char* str)
{
  unsigned int hash = 5381;
  int c;

   while (c = *str++)
     hash = ((hash << 5) + hash) + c;

   return (hash % HASH_SIZE );
}


