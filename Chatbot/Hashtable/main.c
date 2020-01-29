/*
 * ICT1002 (C Language) Group Project, AY19 Trimester 1.
 *
 * This file implements the main loop, including dividing input into words.
 *
 * You should not need to modify this file. You may invoke its functions if you like, however.
 */

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "chat1002.h"
#include "knowledge.c"
#include "chatbot.c"

/* word delimiters */
const char *delimiters = " ?\t\n";

/*
 * Main loop.
 */
int main(int argc, char *argv[]) {

	char input[MAX_INPUT];      /* buffer for holding the user input */
	int inc;                /* the number of words in the user input */
	char *inv[MAX_INPUT];       /* pointers to the beginning of each word of input */
	char output[MAX_RESPONSE];  /* the chatbot's output */
	int len;                    /* length of a word */
	int done = 0;               /* set to 1 to end the main loop */
	char *word;
	
	hashtable_t *hashtable = ht_create(65536);
	hashtable_t *weights = ht_create(65536);
	hashtable_t *counts = ht_create(65536);
	hashtable_t *intents = ht_create(65536);
	
	/* initialise the chatbot */
	inv[0] = "reset";
	inv[1] = NULL;
	
	/* print a welcome message */
	printf("%s: Hello, I'm %s.\n", chatbot_botname(), chatbot_botname());
	
	/* main command loop */
	do {
		while (!done) {
			inc = 0;
			printf("%s: ", chatbot_username());
			fgets(input, MAX_INPUT, stdin);
			if (strlen(input) >1) {
				inv[inc] = ztrtok(input, " ");
				while (word = ztrtok(NULL, " ")) {
					inv[++inc] = word;
				}
				inv[inc] = strtok(inv[inc], delimiters);
				inc++;
				/* invoke the chatbot */
				done = chatbot_main(counts, weights, hashtable, intents, inc, inv, output, MAX_RESPONSE);
				printf("%s: %s\n", chatbot_botname(), output);
			}
		}
	} while (!done);
	
	return 0;
}

char *ztrtok(char *str, const char *delim) {
    static char *static_str=0;      /* var to store last address */
    int index=0, strlength=0;       /* integers for indexes */
    int found = 0;                  /* check if delim is found */

    /* delimiter cannot be NULL
    * if no more char left, return NULL as well
    */
    if (delim==0 || (str == 0 && static_str == 0))
        return 0;

    if (str == 0)
        str = static_str;

    /* get length of string */
    while(str[strlength])
        strlength++;

    /* find the first occurance of delim */
    for (index=0;index<strlength;index++)
        if (str[index]==delim[0]) {
            found=1;
            break;
        }

    /* if delim is not contained in str, return str */
    if (!found) {
        static_str = 0;
        return str;
    }

    /* check for consecutive delimiters
    *if first char is delim, return delim
    */
    if (str[0]==delim[0]) {
        static_str = (str + 1);
        return (char *)delim;
    }

    /* terminate the string
    * this assignmetn requires char[], so str has to
    * be char[] rather than *char
    */
    str[index] = '\0';

    /* save the rest of the string */
    if ((str + index + 1)!=0)
        static_str = (str + index + 1);
    else
        static_str = 0;

        return str;
}

/*
 * Utility function for comparing string case-insensitively.
 *
 * Input:
 *   token1 - the first token
 *   token2 - the second token
 *
 * Returns:
 *   as strcmp()
 */
int compare_token(const char *token1, const char *token2) {
	
	int i = 0;
	while (token1[i] != '\0' && token2[i] != '\0') {
		if (toupper(token1[i]) < toupper(token2[i]))
			return -1;
		else if (toupper(token1[i]) > toupper(token2[i]))
			return 1;
		i++;
	}
	
	if (token1[i] == '\0' && token2[i] == '\0')
		return 0;
	else if (token1[i] == '\0')
		return -1;
	else
		return 1;
	
}


/*
 * Prompt the user.
 *
 * Input:
 *   buf    - a buffer into which to store the answer
 *   n      - the maximum number of characters to write to the buffer
 *   format - format string, as printf
 *   ...    - as printf
 */
void prompt_user(char *buf, int n, const char *format, ...) {
	
	/* print the prompt */
	va_list args;
	va_start(args, format);
	printf("%s: ", chatbot_botname());
	vprintf(format, args);
	printf(" ");
	va_end(args);
	printf("\n%s: ", chatbot_username());
	
	/* get the response from the user */
	fgets(buf, n, stdin);
	char *nl = strchr(buf, '\n');
	if (nl != NULL)
		*nl = '\0';
}