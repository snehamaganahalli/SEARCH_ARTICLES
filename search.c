#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<ctype.h>

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
"w", "was","what", "when", "why", "were", "will", "we", "would", "with", "who","whose",
"x",
"yet","you", "your","yours","yourself","yourselves",
"z",
};

/* Global variable ro store all the articles. */
article* article_obj = NULL;

/* Global variable to store the no of articles. */
unsigned int no_of_articles = 0;

/* Head node of the hash table */
key_alias_word_info* key_head = NULL;

/* Dynamically read number of articles present in ./articles dir */
int get_no_of_articles()
{
  unsigned int no_of_articles = 0;
  FILE *fp = NULL;

  system("ls articles | wc -l > /tmp/no_of_articles.txt" );
  fp = fopen("/tmp/no_of_articles.txt", "r");
  fscanf(fp, "%u", &no_of_articles );

  printf("\nNo of Articles present are: %u\n", no_of_articles);

  fclose(fp);

  return no_of_articles;
}

void allocate_articles()
{
  
  article_obj = (article *) calloc(sizeof(article),  no_of_articles );
}

void set_article_obj()
{
  FILE *fp = NULL;
  int i;

  system("ls articles > /tmp/name_of_articles.txt");
  fp = fopen("/tmp/name_of_articles.txt", "r");

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

key_alias_word_info* allocate_key()
{
  key_alias_word_info* key = (key_alias_word_info*) calloc (sizeof(key_alias_word_info), 1);
  if(NULL == key)
  {
    perror("could not allocate memory");
    exit(0);
  }
  key->value_info = NULL;
  key->next = NULL;
  return key;
}

value_alias_article_info* allocate_value()
{
  value_alias_article_info* value = (value_alias_article_info *) calloc (sizeof(value_alias_article_info), 1);
  if(NULL == value)
  {
    perror("could not allocate memory");
    exit(0);
  }
  value->next = NULL;
  return value;
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
  key_alias_word_info* key = NULL;

  if(NULL == key_head)
  {
    key = allocate_key();
    key->key_info = key_info;
    key->next = NULL;
    key->value_info = NULL;
    key_head = key;
    printf("\n head key inserted successfully \n");
    return;
  }
  /* insertion at beginning */
  if(key_head->key_info > key_info )
  {
    key = allocate_key();
    key->key_info = key_info;
    key->next = key_head;
    key_head = key;
    printf("\n head key updated successfully \n");
    return;
  }
  key_alias_word_info* prev = key_head;
  key_alias_word_info* ptr = key_head;

  while(ptr->key_info < key_info && ptr->next != NULL)
  {
    prev = ptr;
    ptr = ptr->next;
  } 
  
  if(ptr->key_info == key_info)
  {
    printf("key is already present");
    return;
  }
  else if(ptr->key_info > key_info)
  {
    key = allocate_key();
    prev->next = key;
    key->next = ptr;
    key->key_info = key_info;
  }
  else if(ptr->key_info < key_info) /* Insert in the last of the list */
  {
    key = allocate_key();
    key->next = NULL;
    ptr->next = key;
    key->key_info = key_info;
  }
  printf("\n key inserted successfully \n");
}

key_alias_word_info* find_key(int key_info)
{
  if(key_head ==  NULL)
  {
    printf("\n key_head NULL \n");
    return NULL;
  }
  key_alias_word_info* ptr = key_head;
  
  while(ptr != NULL)
  {
    if(ptr->key_info == key_info)
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
  
  if(found_key == NULL)
  {
    key_info = calculate_key(value->pattern);
    insert_key(key_info);
    found_key = find_key(key_info);
  }

  if(found_key == NULL)
  {
    perror(" there is some logical/lib error ");
    exit(0);
  }
  value_alias_article_info* ptr = found_key->value_info;

  if(ptr == NULL)
  {
    found_key->value_info = value;
    return;
  }
  while(ptr->next != NULL)
  {
    ptr = ptr->next;
  }
  ptr->next = value;
  value->next =  NULL;
}

/* Return true(1) if the key is found else false (0) */
unsigned int find_value(char* pattern)
{
  unsigned int key_info = calculate_key(pattern);
  key_alias_word_info* found_key =  find_key(key_info);
  unsigned short int flag = 0;

  if(found_key == NULL)
  {
    printf("\nKey not found \n");
  }
  else
  {
    value_alias_article_info* ptr = found_key->value_info;
    while(ptr->next != NULL)
    {
      if((strcasecmp (ptr->pattern, pattern)) == 0)
      {
        flag = 1;
        printf("\nThe word found \n");
        printf("\nArticle Name: %s \n", ptr->article_name);
        printf("\nComplete Line: %s \n\n", ptr->complete_line);
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

void set_value(value_alias_article_info* value, char* word, char* article_name, char* complete_line)
{
  strncpy(value->pattern, word, STRLEN);
  strncpy(value->article_name, article_name, STRLEN);
  strncpy(value->complete_line, complete_line, SEARCH_RESULT_STR_LEN);
  printf("\n complete_line is %s\n", complete_line);
  printf("\n value->complete_line is %s\n", value->complete_line);
}

void create_key_value_pair(char* word, char* article_name, char* complete_line)
{

  unsigned int key_info = calculate_key(word);

  insert_key(key_info);

  value_alias_article_info* value = allocate_value(); 

  set_value(value, word, article_name, complete_line);
  
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
  for(i=0; i<no_of_articles; i++)
  {
    fp = open_file(article_obj[i].name, "r" );
    while(NULL != (fgets(line, sizeof(line), fp) ) )
    {
      if(*line == '\n')
        continue;
      strncpy(save_line, line, MAX_STR_LEN);
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
  key_alias_word_info* ptr = key_head;

  while( ptr != NULL)
  {
    printf("\n\nKey: %d", ptr->key_info);
    printf("\nValues:\n");
    value_alias_article_info* cur = ptr->value_info;
    while(cur != NULL)
    { 
        printf("%s  ",cur->pattern);
        cur = cur->next;
    }
    ptr = ptr->next;
  }
}
