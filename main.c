#include <stdio.h>
#include <string.h>
#include "lib.h"
#include "search.h"

extern int no_of_articles;

void input_pattern(char* pattern)
{

  printf("\nEnter the pattern \n");
  fgets(pattern, STRLEN, stdin);

  if ((strlen(pattern) > 0) && (pattern[strlen (pattern) - 1] == '\n'))
    pattern[strlen (pattern) - 1] = '\0';

}

int main()
{

  int i = 0;
  char pattern[STRLEN] = {0};
  char src[STRLEN] = {0};
  char display_buf[STRLEN] = {0};
  char* str = NULL;

  no_of_articles =  get_no_of_articles();

  allocate_articles(no_of_articles);

  set_article_obj();

  create_database();

  display_hash();

  while(1)
  {
    input_pattern(pattern);

    str = strlwr(pattern);
    find_value(str);
  }

  return 0;
}

