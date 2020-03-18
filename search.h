#ifndef SEARCH_H
#define SEARCH_H

#define ARTICLE_NAME_SIZE 100
#define STRLEN 100
#define HASH_SIZE 100
#define MAX_STR_LEN 2048
#define SEARCH_RESULT_STR_LEN 1024

#define ARTICLES_DIR "./articles/"
#define NO_OF_IGNORE_WORDS 200

/* Article information */
typedef struct
{
  char name[ARTICLE_NAME_SIZE];

}article;

/* Information of the value of key-value pair*/
typedef struct value_alias_article_info
{
  char pattern[STRLEN];

  char article_name[STRLEN];
  unsigned int line_no;
  char complete_line[1024];

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
unsigned int find_value(char* pattern);
int calculate_key(char* str);

#endif
