#ifndef SEARCH_H
#define SEARCH_H

#define ARTICLE_NAME_SIZE 100
#define STRLEN 100
#define HASH_SIZE 100
#define MAX_STR_LEN 2048
#define SEARCH_RESULT_STR_LEN 1024
/* Defining the MAX VALUE of unsigned int as the invalid key NO */
#define INVALID_KEY_INFO UINT_MAX

#define ARTICLES_DIR "./articles/"
#define NO_OF_IGNORE_WORDS 200
#define MAX_NO_OF_ARTICLES 100

#define MAX_NO_PATTERN_IN_ARTICLES 1024

/* Article information */
typedef struct
{
  char name[ARTICLE_NAME_SIZE];

}article;

/* Information of the value of key-value pair*/
typedef struct value_alias_article_info
{
  char pattern[STRLEN];

  /* If a Pattern is found in the article, its name is stored in this variable.
     Similary it holds all the names of the articles in which the pattern is found.*/
  char article_name[MAX_NO_PATTERN_IN_ARTICLES][STRLEN];

  /*It holds the no of times the pattern was found in all the articles */
  unsigned int no_pattern_found_in_articles;

  /*It holds all the lines in which the pattern is found. It holds the lines of all the artices.*/
  char* complete_line[MAX_NO_PATTERN_IN_ARTICLES];

  struct value_alias_article_info* next;

}value_alias_article_info;

/*key value pair of the hash*/
typedef struct key_alias_word_info {

  unsigned int key_info;
  value_alias_article_info* value_info;

  struct key_alias_word_info* next;
}key_alias_word_info;

/*Function prototypes*/
void input_pattern(char* pattern);
int get_no_of_articles();
void allocate_articles();
void set_article_obj();
void create_database();
void display_hash();
void input_pattern(char* pattern);
unsigned int find_pattern(char* pattern);
int calculate_key(char* str);
void initialize_key_head();

#endif
