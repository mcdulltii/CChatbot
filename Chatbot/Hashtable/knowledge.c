/*
 * ICT1002 (C Language) Group Project, AY19 Trimester 1.
 *
 * This file implements the chatbot's knowledge base.
 *
 * knowledge_get() retrieves the response to a question.
 * knowledge_put() inserts a new response to a question.
 * knowledge_read() reads the knowledge base from a file.
 * knowledge_reset() erases all of the knowledge.
 * kowledge_write() saves the knowledge base in a file.
 *
 * You may add helper functions as necessary.
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>
#include <math.h>
#include "chat1002.h"

int totalWeights;
int intentCount = 0;

char *toLowerStr(char *string);

/* Create a new hashtable. */
hashtable_t *ht_create( int size ) {

  hashtable_t *hashtable = NULL;
  int i;

  if( size < 1 ) return NULL;

  /* Allocate the table itself. */
  if( ( hashtable = malloc( sizeof( hashtable_t ) ) ) == NULL ) {
    return NULL;
  }

  /* Allocate pointers to the head nodes. */
  if( ( hashtable->table = malloc( sizeof( entry_t * ) * size ) ) == NULL ) {
    return NULL;
  }
  for( i = 0; i < size; i++ ) {
    hashtable->table[i] = NULL;
  }

  hashtable->size = size;

  return hashtable;
}

/* Hash a string for a particular hash table. */
int ht_hash( hashtable_t *hashtable, char *key ) {

  unsigned long int hashval = 0;
  unsigned long i = 0;

  /* Convert our string to an integer */
  while( hashval < ULONG_MAX && i < strlen( key ) ) {
    hashval = hashval << 8;
    hashval += key[ i ];
    i++;
  }

  return hashval % hashtable->size;
}

/* Create a key-value pair. */
entry_t *ht_newpair( char *key, char *value ) {
  entry_t *newpair;

  if( ( newpair = malloc( sizeof( entry_t ) ) ) == NULL ) {
    return NULL;
  }

  if( ( newpair->key = strdup( key ) ) == NULL ) {
    return NULL;
  }

  if( ( newpair->value = strdup( value ) ) == NULL ) {
    return NULL;
  }

  newpair->next = NULL;

  return newpair;
}

/* Insert a key-value pair into a hash table. */
void ht_set( hashtable_t *hashtable, char *key, char *value ) {
  int bin = 0;
  entry_t *newpair = NULL;
  entry_t *next = NULL;
  entry_t *last = NULL;

  bin = ht_hash( hashtable, key );

  next = hashtable->table[ bin ];

  while( next != NULL && next->key != NULL && strcmp( key, next->key ) > 0 ) {
    last = next;
    next = next->next;
  }

  /* There's already a pair.  Let's replace that string. */
  if( next != NULL && next->key != NULL && strcmp( key, next->key ) == 0 ) {

    free( next->value );
    next->value = strdup( value );

  /* Nope, could't find it.  Time to grow a pair. */
  } else {
    newpair = ht_newpair( key, value );

    /* We're at the start of the linked list in this bin. */
    if( next == hashtable->table[ bin ] ) {
      newpair->next = next;
      hashtable->table[ bin ] = newpair;

    /* We're at the end of the linked list in this bin. */
    } else if ( next == NULL ) {
      last->next = newpair;

    /* We're in the middle of the list. */
    } else  {
      newpair->next = next;
      last->next = newpair;
    }
  }
}

/* Retrieve a key-value pair from a hash table. */
entry_t *ht_get( hashtable_t *hashtable, char *key ) {
	int bin = 0;
	entry_t *pair;

	bin = ht_hash( hashtable, key );

	/* Step through the bin, looking for our value. */
	pair = hashtable->table[ bin ];
	while( pair != NULL && pair->key != NULL && strcmp( key, pair->key ) > 0 ) {
		pair = pair->next;
	}

	/* Did we actually find anything? */
	if( pair == NULL || pair->key == NULL || strcmp( key, pair->key ) != 0 ) {
		return NULL;

	} else {
		return pair;
	}

}

int addWeight( hashtable_t *hashtable, char *word ) {
	// Returns integer weight value from hashtable
	char *lowerWord = toLowerStr(word);
	entry_t *found = NULL;
	if (lowerWord != NULL) {
		found = ht_get(hashtable, lowerWord);
	}
	free(lowerWord);
	if (found) {
		return atoi(found->value);
	} else {
		return 0;
	}
}

char *findValue( hashtable_t *hashtable, int weight ) {
	// Finds the corresponding string from the highest weight
	char key[5];
	sprintf(key, "%d", weight);
	entry_t *found = ht_get(hashtable, key);
	return (found == NULL) ? NULL : found->value;
}

int is_bit_on(int n, int pos) {
  return (n >> pos) & 1;
}

int set_bits(int n) {
  // Set bits counting function
  return __builtin_popcount(n);
}

Set subset(Set set, int step) {
  // Returns each step of permutation
  int* subset = malloc(sizeof(int) * set_bits(step));
  if (subset == NULL) {
    Set failure = { .items = NULL, .size = 0 };
    return failure;
  }
  int elem_n = 0;
  for (; set.size > 0; --set.size) {
    if (is_bit_on(step, set.size - 1))
      subset[elem_n++] = set.items[set.size-1];
  }
  Set ret = { .items = subset, .size = elem_n };
  return ret;
}

Powerset powerset(Set set) {
  // Struct to contain all permutations
  size_t powerset_size = pow(2, set.size);
  Powerset powerset = {
    .subsets = malloc(sizeof(Set) * powerset_size),
    .size  = powerset_size
  };
  Powerset failure = { .subsets = NULL, .size = 0 };
  if (powerset.subsets == NULL) return failure;

  for (size_t i = 0; i < powerset_size; ++i) {
    powerset.subsets[i] = subset(set, i);
    if (powerset.subsets[i].items == NULL) {
      for (size_t j = 0; j < i; ++j) {
        free(powerset.subsets[j].items);
      }
      return failure;
    }
  }
  return powerset;
}

void free_powerset(Powerset powerset) {
  // Release memory from structs
  for (size_t i = 0; i < powerset.size; ++i) {
    free(powerset.subsets[i].items);
  }
  free(powerset.subsets);
}

void swap(int *x1,int *x2)
{
	// Swap elements
	int x=*x1;
	*x1=*x2;
	*x2=x;
}

void permutate(int *arr,int st,int ls, char **string, hashtable_t *weights)
{
	// Permutates array
	// arr = {1, 2, 3, ...}
	// st : <start>
	// ls : <end>
	// string : string array of input
	// weights : Hashtable of weightage for intents and entities
	int i=0;
	char str[MAX_INPUT];
	strcpy(str, "");
	char *joiner = " ";
	if(st==ls)
	{
		int k;
		for(k=0;k<ls;k++)
		{
			if (string[arr[k]] != NULL) {
				strncat(str, string[arr[k]], strlen(string[arr[k]]));
				strncat(str, joiner, strlen(joiner));
			}
		}
		str[strlen(str)-strlen(joiner)] = 0;
		totalWeights += addWeight(weights, str);
		strcpy(str, "");
	}
		else
	{
		for(i=st;i<ls;i++)
		{
			swap(arr+st,arr+i);
			permutate(arr,st+1,ls, string, weights);
			swap(arr+st,arr+i);
		}
	}
}

void print_array(Set array, char **string, hashtable_t *weights) {
	// Find from each permutation of an array within a power set
	int arr[array.size];
	for (size_t i = 0; i < array.size; ++i)
		arr[i] = array.items[i];
	permutate(arr, 0, array.size, string, weights);
}

void append(hashtable_t *counts, hashtable_t *weights, hashtable_t *hashtable, char *intent, char *entity, char *response) {
	// Appends onto each hashtable depending on the intent
	// Weights for each intent are separated by intervals of 1000, starting from 1000
	// Counts contain the number of entities stored within each intent: (key)intent,(value)count
	// Hashtable contains each struct pair of: (key)entity,(value)response
	int num = 1;
	char stringNum[10];
	char stringNum2[10];
	char *lowerWord = toLowerStr(intent);
	entry_t *count = NULL;

	if (lowerWord != NULL) {
		count = ht_get(counts, lowerWord);
	}
	free(lowerWord);
	if (count == NULL) {
		sprintf(stringNum, "%d", num);
		lowerWord = toLowerStr(intent);
		if (lowerWord != NULL) {
			ht_set(counts, lowerWord, stringNum);
		}
		free(lowerWord);
		intentCount++;
	} else {
		num = atoi(count->value);
		num++;
		sprintf(stringNum, "%d", num);
		strcpy(count->value, stringNum);
	}
	sprintf(stringNum2, "%d", intentCount*1000);
	entry_t *findweight = ht_get(weights, stringNum2);
	if (findweight == NULL) {
		lowerWord = toLowerStr(intent);
		if (lowerWord != NULL) {
			ht_set(weights, lowerWord, stringNum2);
		}
		free(lowerWord);
	}
	lowerWord = toLowerStr(entity);
	if (lowerWord != NULL) {
		ht_set(weights, lowerWord, stringNum);
	}
	free(lowerWord);
	sprintf(stringNum2, "%d", intentCount*1000+num);
	ht_set(hashtable, stringNum2, response);
}

char *toLowerStr(char *string) {
	char *new = malloc(MAX_INPUT);
	strcpy(new, "");
	char each;
	for (int i=0; i<strlen(string); i++) {
		if (isalpha(string[i])) {
			each = tolower(string[i]);
		} else {
			each = string[i];
		}
		strncat(new, &each, 1);
	}
	return new;
}

/*
 * Get the response to a question.
 *
 * Input:
 *   intent   - the question word
 *   entity   - the entity
 *   response - a buffer to receive the response
 *   n        - the maximum number of characters to write to the response buffer
 *
 * Returns:
 *   KB_OK, if a response was found for the intent and entity (the response is copied to the response buffer)
 *   KB_NOTFOUND, if no response could be found
 *   KB_INVALID, if 'intent' is not a recognised question word
 */
int knowledge_get(hashtable_t *weights, hashtable_t *hashtable, char *string[], char *response, int strlength, int n)
{
	// Calculates the power set, where within each set will be permutated
	// and checked within the hash table for their respective weights
	// Then are finally checked using the total weight for the respective response
	totalWeights = 0;
	int items[MAX_INPUT];
	for (int i=0; i<strlength; i++) {
		items[i] = i;
	}
	Set set = { .items = items, .size = strlength};
	Powerset test = powerset(set);
	
	if (test.subsets == NULL) {
		printf("Bad allocation");
		return KB_NOTFOUND;
	}
	
	for (size_t i = 0; i < test.size; ++i) print_array(test.subsets[i], string, weights);
	free_powerset(test);
	
	char *value = findValue(hashtable, totalWeights);
	if (value != NULL) {
		snprintf(response, n, "%s", value);
		return KB_FOUND;
	} else {
		return KB_NOTFOUND;
	}
}

/* Insert a new response to a question. If a response already exists for the
 * given intent and entity, it will be overwritten. Otherwise, it will be added
 * to the knowledge base.
 *
 * Input:
 *   intent    - the question word
 *   entity    - the entity
 *   response  - the response for this question and entity
 *
 * Returns:
 *   KB_FOUND, if successful
 *   KB_NOMEM, if there was a memory allocation failure
 *   KB_INVALID, if the intent is not a valid question word
 */
int knowledge_put(hashtable_t *counts, hashtable_t *weights, hashtable_t *hashtable, const char *intent, const char *entity, const char *response)
{
	append(counts, weights, hashtable, strdup(intent), strdup(entity), strdup(response));
	return KB_FOUND;
}

/*
 * Read a knowledge base from a file.
 *
 * Input:
 *   f - the file
 *
 * Returns: the number of entity/response pairs successful read from the file
 */
int knowledge_read(hashtable_t *counts, hashtable_t *weights, hashtable_t *hashtable, hashtable_t *intents, FILE *f)
{
	char string[128];
	char intend[10];
	char stringNum[10];
	int isAlpha = 1;
	int cnt = 0;

	//delimiter for strtok
	char delim1[] = "[";
	char delim2[] = "]";
	char delim3[] = "=";

	//entity and response
	char *entity;
	char *response;
	char *lowerWord;
	
	while (!feof(f)) {
		isAlpha = 1;
		fgets(string, sizeof(string), f);
		for (int i=0; i<strlen(string); i++) {
			if (!isalpha(string[i])) {
				isAlpha = 0;
			}
		}
		// Reads file for intent
		if (!strstr(string, "=") && !isAlpha) {
			char *charstring = strtok(strtok(string, delim2), delim1);
			entry_t *intent;
			for (int i=0; i<cnt+1; i++) {
				sprintf(stringNum, "%d", cnt);
				intent = ht_get(intents, stringNum);
				if (intent == NULL && i==cnt) {
					lowerWord = toLowerStr(charstring);
					if (lowerWord != NULL) {
						ht_set(intents, stringNum, lowerWord);
					}
					free(lowerWord);
					strcpy(intend, charstring);
					cnt++;
					break;
				} else if (intent != NULL && intent->value == charstring) {
					strcpy(intend, intent->value);
					break;
				}
			}
		// Reads file for Entity = Response
		} else {
			char *entity = strtok(string, delim3);
			char *response = strtok(NULL, delim3);
			knowledge_put(counts, weights, hashtable, intend, entity, response);
		}
	}
	return 0;
}

/*
 * Reset the knowledge base, removing all know entitities from all intents.
 */
void knowledge_reset()
{
	/*
	struct LIST *tmp;

	while (what_head != NULL)
	{
		tmp = what_head;
		what_head = what_head->next;
		free(tmp);
	}

	while (where_head != NULL)
	{
		tmp = where_head;
		where_head = where_head->next;
		free(tmp);
	}

	while (who_head != NULL)
	{
		tmp = who_head;
		who_head = who_head->next;
		free(tmp);
	}*/
}

/* Write the knowledge base to a file.
 *
 * Input:
 *   f - the file
 */
void knowledge_write(FILE *f)
{
	/*
	entry_t *head = ht_get(hashtable, 0);
	while ( head != NULL && head->key != NULL ) {
		if (
		fprintf(f, "%s = %s\n", head->key, head->value);
		head = head->next;
	} 
	
	// What intent
	fputs("[what]\n", f);
	while (what_head != NULL)
	{
		fprintf(f, "%s = %s\n", what_head->entity, what_head->response);
		what_head = what_head->next;
	}

	// Where intent
	fputs("\n[where]\n", f);
	while (where_head != NULL)
	{
		fprintf(f, "%s = %s\n", where_head->entity, where_head->response);
		where_head = where_head->next;
	}

	// Who intent
	fputs("\n[who]\n", f);
	while (who_head != NULL)
	{
		fprintf(f, "%s = %s\n", who_head->entity, who_head->response);
		who_head = who_head->next;
	} */
}