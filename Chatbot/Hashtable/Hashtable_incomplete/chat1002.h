/*
 * ICT1002 (C Language) Group Project, AY19 Trimester 1.
 *
 * This file contains the definitions and function prototypes for all of
 * features of the ICT1002 chatbot.
 */
 
#ifndef _CHAT1002_H
#define _CHAT1002_H

#include <stdio.h>

/* the maximum number of characters we expect in a line of input (including the terminating null)  */
#define MAX_INPUT    255

/* the maximum number of characters allowed in the name of an intent (including the terminating null)  */
#define MAX_INTENT   32

/* the maximum number of characters allowed in the name of an entity (including the terminating null)  */
#define MAX_ENTITY   64

/* the maximum number of characters allowed in a response (including the terminating null) */
#define MAX_RESPONSE 256

/* return codes for knowledge_get() and knowledge_put() */
#define KB_OK        0
#define KB_NOTFOUND -1
#define KB_INVALID  -2
#define KB_NOMEM    -3
#define KB_FOUND    -4

#define SEPCHARS "* .,\"\n"

/* Structs for PowerSet */
typedef struct {
  int* items;
  size_t size;
} Set;

typedef struct {
  Set* subsets;
  size_t size;
} Powerset;

/* Structs for hashtables */
struct entry_s {
  char *key;
  char *value;
  struct entry_s *next;
};

typedef struct entry_s entry_t;

struct hashtable_s {
  int size;
  struct entry_s **table;
};

typedef struct hashtable_s hashtable_t;

hashtable_t *ht_create( int size );

/* functions defined in main.c */
int compare_token(const char *token1, const char *token2);
void prompt_user(char *buf, int n, const char *format, ...);

/* functions defined in chatbot.c */
const char *chatbot_botname();
const char *chatbot_username();
int chatbot_main(hashtable_t *weights, hashtable_t *hashtable, hashtable_t *intents, int inc, char *inv[], char *response, int n);
int chatbot_is_exit(const char *intent);
int chatbot_do_exit(int inc, char *inv[], char *response, int n);
int chatbot_is_load(const char *intent);
int chatbot_do_load(hashtable_t *weights, hashtable_t *hashtable, hashtable_t *intents, int inc, char *inv[], char *response, int n);
int chatbot_is_question(char *intent[], int inc);
int chatbot_do_question(hashtable_t *weights, hashtable_t *hashtable, int inc, char *inv[], char *response, int n);
int chatbot_is_reset(const char *intent);
int chatbot_do_reset(int inc, char *inv[], char *response, int n);
int chatbot_is_save(const char *intent);
int chatbot_do_save(int inc, char *inv[], char *response, int n);
int chatbot_is_smalltalk(const char *intent);
int chatbot_do_smalltalk(int inc, char *inv[], char *resonse, int n);

/* functions defined in knowledge.c */
int knowledge_get(hashtable_t *weights, hashtable_t *hashtable, char **string, char *response, int strlength, int n);
int knowledge_put(hashtable_t *weights, hashtable_t *hashtable, hashtable_t *intents, const char *intent, const char *entity, const char *response);
void knowledge_reset();
int knowledge_read(hashtable_t *weights, hashtable_t *hashtable, hashtable_t *intents, FILE *f);
void knowledge_write(FILE *f);
char *ztrtok(char *str, const char *delim);

#endif