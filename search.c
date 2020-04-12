#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<ctype.h>
#include<limits.h>
#include<errno.h>

#include "search.h"
#include "lib.h"

/* These words are not processed and eneted into the hash table */
char ignore_words[NO_OF_IGNORE_WORDS][ARTICLE_NAME_SIZE] = {
"a", "am", "as", "at", "and","are","anyone","anything",
"be", "but", "by", 
"can", "could",
"d", "dont",
"each", "everybody", "everyone","everything",
"f", "for","from","few",
"go",
"have", "how", "he", "his", "her","has","hers","herself","himself",
"i", "is", "in", "into", "it", "its", "itself", "if",
"j",
"k",
"l",
"my", "mine","many","myself","more","most","me",
"n", "none","nor",
"of", "on", "our","ours","ourselves",
"p",
"q",
"r",
"so", "still", "she","someone",
"the", "then", "to","that", "they", "them", "there","their", "this", "these","theirs","those","themselves",
"u",
"v",
"w", "was","what", "when", "why", "were", "will", "we", "would", "with", "who","whose", "which",
"x",
"yet","you", "your","yours","yourself","yourselves",
"z",
};

/* Global variable ro store all the articles. */
article* article_obj = NULL;

/* Global variable to store the no of articles. */
unsigned int no_of_articles = 0;

/* Head node of the hash table */
key_alias_word_info  key_head[HASH_SIZE] = {0};

/* Initializing the structure array to the default values */
void initialize_key_head()
{
  for(int i=0; i<HASH_SIZE ; i++)
  {
    key_head[i].key_info = INVALID_KEY_INFO;
    key_head[i].value_info = NULL;
    key_head[i].next = NULL;
  }

}

/* Set the key info */
void set_key_head(unsigned int key)
{
  key_head[key].key_info = key;
}

unsigned int get_key_head(unsigned int key)
{
  return key_head[key].key_info; 
}

/* Dynamically read number of articles present in ./articles dir */
int get_no_of_articles()
{
  unsigned int no_of_articles = 0;
  FILE *fp = NULL;

  system("ls articles | wc -l > /tmp/no_of_articles.txt" );
  fp = fopen("/tmp/no_of_articles.txt", "r");
  if(NULL == fp)
  {
    printf("\n file pointer is null %d", errno);
    return 0;
  }
  fscanf(fp, "%u", &no_of_articles );

  printf("\nNo of Articles present are: %u\n", no_of_articles);

  fclose(fp);

  return no_of_articles;
}

void allocate_articles()
{
  
  article_obj = (article *) calloc(sizeof(article),  no_of_articles );
  if(NULL == article_obj)
  {
    printf("Could not allocate the articles object \n");
    exit(0);
  }
}

void set_article_obj()
{
  FILE *fp = NULL;
  int i;

  system("ls articles > /tmp/name_of_articles.txt");
  fp = fopen("/tmp/name_of_articles.txt", "r");

  if(NULL == fp)
  {
    printf("\n file pointer is null %d", errno);
    return;
  }

  printf("\nThe articles present are:\n");
  for(i=0; i<no_of_articles; i++)
  {
    fscanf(fp, "%s", article_obj[i].name);
    printf("%s\n", article_obj[i].name);
  }
}

article* get_article_obj()
{
  return article_obj;
}

value_alias_article_info* allocate_value()
{
  value_alias_article_info* value = (value_alias_article_info *) calloc (sizeof(value_alias_article_info), 1);
  if(NULL == value)
  {
    perror("could not allocate memory");
    return NULL;
  }
  value->no_pattern_found_in_articles = 0;
  value->next = NULL;
  return value;
}

void set_pattern(value_alias_article_info* value, char* pattern)
{

  _strncpy(value->pattern, pattern, STRLEN);

}

void set_article_name(value_alias_article_info* value, char* name)
{
  _strncpy(value->article_name[value->no_pattern_found_in_articles], name, STRLEN);
}

void allocate_complete_line(value_alias_article_info* value)
{
  if(value->no_pattern_found_in_articles < MAX_NO_PATTERN_IN_ARTICLES)
  {
    value->complete_line[value->no_pattern_found_in_articles] = (char *) malloc(SEARCH_RESULT_STR_LEN);
  }
  else
  {
    printf(" Error: Too many patterns found. Overflow !!!!!!!");
    return;
  }
}

void set_complete_line(value_alias_article_info* value, char* line)
{
  static unsigned int offset = 0;
  /* Memory Allocation*/
  if(NULL == value->complete_line[value->no_pattern_found_in_articles])
  {
    allocate_complete_line(value);
  }
  
  /*Set values*/
  _strncpy(value->complete_line[value->no_pattern_found_in_articles], line, SEARCH_RESULT_STR_LEN);

  printf("\n complete_line is %s\n", value->complete_line[value->no_pattern_found_in_articles]);
}


/* The words which are not entered into the hash table are ignore words.
   Tells whether the particular word should be ignored or not

   Return: 1: Ignore the word
           0: Consider the word for processing and to enter into the hash table
*/
unsigned short int is_ignore_word(char* word)
{
  int i;

  for(i=0; i<NO_OF_IGNORE_WORDS; i++)
  {
    if(ignore_words!= '\0')
    {
      if(0 == (strcasecmp(ignore_words[i], word)))
        return 1;
    }

  }
  return 0;
}

void insert_key(unsigned int key_info)
{
  set_key_head(key_info);
}

key_alias_word_info* find_key(int key_info)
{
  if(INVALID_KEY_INFO != get_key_head(key_info))
  {
    return &key_head[key_info]; 
  }
  else
  {
    printf("\n key is not present  \n");
    return NULL;
  }
}

value_alias_article_info* find_value(char* pattern, value_alias_article_info* value_head)
{

  value_alias_article_info* ptr = value_head;

  while(ptr != NULL)
  {
    if(0 == strncmp(ptr->pattern, pattern, STRLEN))
    {
      return ptr;
    }
    ptr = ptr->next;
  }
  return NULL;
} 

void insert_value(value_alias_article_info* value)
{

  unsigned int key_info = calculate_key(value->pattern);
  
  key_alias_word_info* found_key = find_key(key_info);

  /* insert  Key since key not found */
  if(NULL == found_key)
  {
    insert_key(key_info);
    found_key = find_key(key_info);
  }

  value_alias_article_info* ptr = found_key->value_info;
  
  /* Insert At beginning */
  if(NULL == ptr)
  {
    found_key->value_info = value;
    return;
  }

  value_alias_article_info* found_value = find_value(value->pattern, found_key->value_info);

  /*If the pattern is not found, then travel till end of the list and insert it over there. */
  if(NULL == found_value)
  {
    while(ptr->next != NULL)
    {
      ptr = ptr->next;
    }
    value->next =  NULL;
    ptr->next = value;
  }
  else
  {
    /* The pattern exists already. Adding in the same list */
    found_value->no_pattern_found_in_articles++;
    set_article_name(found_value, value->article_name[0]);
    set_complete_line(found_value, value->complete_line[0]);
    /* Since value is found already. Copy the required fields and delete the value. */
    free(value); 
  }
}

/* Return true(1) if the key is found else false (0) */
unsigned int find_pattern(char* pattern)
{
  unsigned int key_info = calculate_key(pattern);
  key_alias_word_info* found_key =  find_key(key_info);
  unsigned short int flag = 0;
  int i;

  if(found_key == NULL)
  {
    printf("\nKey not found \n");
  }
  else
  {
    value_alias_article_info* ptr = found_key->value_info;
    while(ptr)
    {
      if((strcasecmp (ptr->pattern, pattern)) == 0)
      {
        flag = 1;
        printf("\nThe pattern found !!!!!!!!!!!! \n");
        for(i=0; i<= ptr->no_pattern_found_in_articles; i++)
        {
          printf("pattern: %s\n", ptr->pattern);
          printf("Article Name: %s \n", ptr->article_name[i]);
          printf("Complete Line: %s \n\n", ptr->complete_line[i]);
        }
      }
      ptr = ptr->next;
    }
  }
  if(flag == 0)
  {
    printf("\nThe pattern \"%s\" was not found in any of the articles \n", pattern);
    return 0;
  }
  return 1;
}

void create_key_value_pair(char* word, char* article_name, char* complete_line)
{

  unsigned int key_info = calculate_key(word);

  insert_key(key_info);

  value_alias_article_info* value = allocate_value();

  if(NULL == value)
  {
    printf("\n memory Allocation failed \n");
    return;
  }
  /*Set values*/
  set_pattern(value, word);
  set_article_name(value, article_name);
  set_complete_line(value, complete_line);

  insert_value(value);

}

void create_database()
{
  int i;
  FILE* fp =  NULL;
  char line[MAX_STR_LEN] = {0};
  char save_line[MAX_STR_LEN] = {0};
  char* word = NULL;
  char* str = NULL;

  article* article_obj = get_article_obj();
  if(NULL == article_obj)
  {
    printf("\n Null ptr received in article_obj");
    return;
  }
  for(i=0; i<no_of_articles; i++)
  {
    fp = open_file(article_obj[i].name, "r" );
    while(NULL != (fgets(line, sizeof(line), fp) ) )
    {
      if(*line == '\n')
        continue;
      _strncpy(save_line, line, MAX_STR_LEN);
      str = stringRemoveNonAlphaNum(line);
      str = strlwr(str);
      word = strtok(str, " ");
      while(NULL != word )
      {
        if(!is_ignore_word(word))
        {
          create_key_value_pair(word, article_obj[i].name, save_line);
          printf("word is %s\n", word);
          printf("\n line is %s\n", save_line);
        }
        word = strtok(NULL, " ");
      }
    }

  } 

}

void display_hash()
{
  unsigned int count = 0;
  
  while( count < HASH_SIZE )
  {
  
    if(INVALID_KEY_INFO != key_head[count].key_info)
    {
      printf("\n\nKey: %d", key_head[count].key_info);
      printf("\nValues:\n");
        value_alias_article_info* cur = key_head[count].value_info;
        while(cur != NULL)
        { 
          printf("%s  ",cur->pattern);
          cur = cur->next;
        }
    }
    count++;
  }
}
