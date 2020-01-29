/*
 * ICT1002 (C Language) Group Project, AY19 Trimester 1.
 *
 * This file implements the behaviour of the chatbot. The main entry point to
 * this module is the chatbot_main() function, which identifies the intent
 * using the chatbot_is_*() functions then invokes the matching chatbot_do_*()
 * function to carry out the intent.
 *
 * chatbot_main() and chatbot_do_*() have the same method signature, which
 * works as described here.
 *
 * Input parameters:
 *   inc      - the number of words in the question
 *   inv      - an array of pointers to each word in the question
 *   response - a buffer to receive the response
 *   n        - the size of the response buffer
 *
 * The first word indicates the intent. If the intent is not recognised, the
 * chatbot should respond with "I do not understand [intent]." or similar, and
 * ignore the rest of the input.
 *
 * If the second word may be a part of speech that makes sense for the intent.
 *    - for WHAT, WHERE and WHO, it may be "is" or "are".
 *    - for SAVE, it may be "as" or "to".
 *    - for LOAD, it may be "from".
 * The word is otherwise ignored and may be omitted.
 *
 * The remainder of the input (including the second word, if it is not one of the
 * above) is the entity.
 *
 * The chatbot's answer should be stored in the output buffer, and be no longer
 * than n characters long (you can use snprintf() to do this). The contents of
 * this buffer will be printed by the main loop.
 *
 * The behaviour of the other functions is described individually in a comment
 * immediately before the function declaration.
 *
 * You can rename the chatbot and the user by changing chatbot_botname() and
 * chatbot_username(), respectively. The main loop will print the strings
 * returned by these functions at the start of each line.
 */

#include <stdio.h>
#include <string.h>
#include "smalltalk.c"
#include "chat1002.h"
#include "help.c"
int position = 0;

/*
 * Get the name of the chatbot.
 *
 * Returns: the name of the chatbot as a null-terminated string
 */
const char *chatbot_botname()
{
	return "bobo";
}

/*
 * Get the name of the user.
 *
 * Returns: the name of the user as a null-terminated string
 */
const char *chatbot_username()
{
	return "User";
}

/*
 * Get a response to user input.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop (i.e. it detected the EXIT intent)
 */
int chatbot_main(int inc, char *inv[], char *response, int n)
{

	/* check for empty input */
	if (inc < 1)
	{
		snprintf(response, n, "");
		return 0;
	}

	/*Find any intent within the user input*/
	for (int i = 0; i < inc; i++)
	{
		if (compare_token(inv[i], "what") == 0 || compare_token(inv[i], "where") == 0 || compare_token(inv[i], "who") == 0 || compare_token(inv[i], "when") == 0 || compare_token(inv[i], "why") == 0 || compare_token(inv[i], "how") == 0)
		{
			position = i;
			break;
		}
	}

	/* look for an intent and invoke the corresponding do_* function */
	if (chatbot_is_exit(inv[0]))
		return chatbot_do_exit(inc, inv, response, n);
	else if (chatbot_is_load(inv[0]))
		return chatbot_do_load(inc, inv, response, n);
	else if (chatbot_is_reset(inv[0]))
		return chatbot_do_reset(inc, inv, response, n);
	else if (chatbot_is_save(inv[0]))
		return chatbot_do_save(inc, inv, response, n);
	else if (chatbot_is_help(inv[0]))
		return chatbot_do_help(inc, inv, response, n);
	else if (chatbot_is_smalltalk(inc, inv))
		return chatbot_do_smalltalk(inc, inv, response, n);
	else if (chatbot_is_question(inv[position]))
	{
		return chatbot_do_question(inc, inv, response, n);
	}

	else
	{
		snprintf(response, n, "I don't understand \"%s\".", inv[0]);
		return 0;
	}
}

/*
 * Determine whether an intent is EXIT.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "exit" or "quit"
 *  0, otherwise
 */
int chatbot_is_exit(const char *intent)
{

	return compare_token(intent, "exit") == 0 || compare_token(intent, "quit") == 0 || compare_token(intent, "bye") == 0 || compare_token(intent, "goodbye") == 0;
}

/*
 * Perform the EXIT intent.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_exit(int inc, char *inv[], char *response, int n)
{

	snprintf(response, n, "Goodbye!");

	return 1;
}

/*
 * Determine whether an intent is LOAD.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "load"
 *  0, otherwise
 */
int chatbot_is_load(const char *intent)
{
	return compare_token(intent, "load") == 0;
}

/*
 * Load a chatbot's knowledge base from a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after loading knowledge)
 */
int chatbot_do_load(int inc, char *inv[], char *response, int n)
{
	FILE *f;
	/* This checks for the amount of word. If inc is more than 3, it goes into the loop.*/
	if (inc >= 3)
	{
		f = fopen(inv[2], "r");
		int i = knowledge_read(f);
		if (i == 0)
		{
			snprintf(response, n, "%s has been loaded.", inv[2]);
		}
		else
		{
			snprintf(response, n, "Unable to load %s, please sure that your file name is correct and in the right path.", inv[2]);
		}
	}
	else
	{
		f = fopen(inv[1], "r");
		int i = knowledge_read(f);
		if (i == 0)
		{
			snprintf(response, n, "%s has been loaded.", inv[1]);
		}
		else
		{
			snprintf(response, n, "Unable to load %s, please sure that your file name is correct and in the right path.", inv[1]);
		}
	}

	return 0;
}

/*  Determine whether an intent is help.
* 	Input : intent -
* 	the intent
* 	Returns : 1,
* 	if the intent is "help" * 0, otherwise 
*/
int chatbot_is_help(const char *intent)
{
	return compare_token(intent, "help") == 0;
}

/*
 * Print the help commands when the intent 'help'.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after loading knowledge)
 */
int chatbot_do_help(int inc, char *inv[], char *response, int n)
{
	snprintf(response, n, "All help commands has been loaded");
	helpMessage();
	return 0;
}

/*
 * Determine whether an intent is a question.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_question(const char *intent)
{

	/* to be implemented */
	return compare_token(intent, "who") == 0 || compare_token(intent, "what") == 0 || compare_token(intent, "where") == 0 || compare_token(intent, "when") == 0 || compare_token(intent, "why") == 0 || compare_token(intent, "how") == 0;
}

/*
 * Answer a question.
 *
 * inv[0] contains the the question word.
 * inv[1] may contain "is" or "are"; if so, it is skipped.
 * The remainder of the words form the entity.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_question(int inc, char *inv[], char *response, int n)
{
	/*Initial question*/
	char entity[MAX_RESPONSE] = "\0";
	char *joiner = " ";
	char question_verb[MAX_RESPONSE] = "\0";
	int valid;

	/*New question and answer*/
	char new_answer[MAX_RESPONSE] = "\0";

	/*Check for entity and proper sentence structure*/
	if (inc <= 1)
	{
		snprintf(response, n, "I don't understand your question");
		return 0;
	}
	else if (inv[position + 1] == NULL)
	{
		snprintf(response, n, "Your question do not contain an entity.");
		return 0;
	}

	/*checks for is or are, and anything after that, the word will become entity*/
	if ((compare_token(inv[position + 1], "is") == 0) || compare_token(inv[position + 1], "are") == 0)
	{
		/*loops through the response and concat everything into 1 string*/
		for (int i = position + 2; i < inc; i++)
		{
			strncat(entity, inv[i], MAX_RESPONSE);
			i != inc - 1 ? strncat(entity, joiner, MAX_RESPONSE) : "";
			strcpy(question_verb, inv[position + 1]);
		}
		if (inv[position + 2] == NULL)
		{
			snprintf(response, n, "Your question do not contain an entity.");
			return 0;
		}
		valid = knowledge_get(inv[position], entity, response, n);
	}
	/*concat everything into 1 string*/
	else
	{
		for (int i = position + 1; i < inc; i++)
		{
			strncat(entity, inv[i], MAX_RESPONSE);
			i != inc - 1 ? strncat(entity, joiner, MAX_RESPONSE) : "";
		}
		valid = knowledge_get(inv[position], entity, response, n);
	}
	/*if the code is not valid, it will print I am not sure*/
	if (valid == -1)
	{
		prompt_user(response, n, "I'm not sure! %s %s %s?", inv[position], question_verb, entity);
		valid = knowledge_put(inv[position], entity, response);
		position = 0;
		/*Prompts response has been saved*/
		if (valid == -4)
		{
			snprintf(response, n, "Response has been saved. Thank you.");
			return 0;
		}
	}
	else
	{
		position = 0;
		return 0;
	}
}

/*
 * Determine whether an intent is RESET.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "reset"
 *  0, otherwise
 */
int chatbot_is_reset(const char *intent)
{
	return compare_token(intent, "reset") == 0;
}

/*
 * Reset the chatbot.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after begin reset)
 */
int chatbot_do_reset(int inc, char *inv[], char *response, int n)
{
	/* to be implemented */
	knowledge_reset();
	snprintf(response, n, "All data has been reset.");
	return 0;
}

/*
 * Determine whether an intent is SAVE.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_save(const char *intent)
{
	return compare_token(intent, "save") == 0;
}

/*
 * Save the chatbot's knowledge to a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after saving knowledge)
 */
int chatbot_do_save(int inc, char *inv[], char *response, int n)
{
	char *joiner = ".\\";
	int i;
	FILE *f;

	if (inv[1] != NULL)
	{
		char fp[MAX_RESPONSE] = "\0";
		strncat(fp, joiner, MAX_RESPONSE);
		strncat(fp, inv[inc - 1], MAX_RESPONSE);

		f = fopen(fp, "w+");

		knowledge_write(f);
		fclose(f);
		snprintf(response, n, "File has been saved");
	}
	else
	{
		snprintf(response, n, "No file name is specified");
	}
	return 0;
}

/*
 * Determine which an intent is smalltalk.
 *
 *
 * Input:
 *  inc - maximum 
 *	inv - user input
 * Returns:
 *  1, if the intent is the first word of one of the smalltalk phrases
 *  0, otherwise
 */
int chatbot_is_smalltalk(int inc, char *inv[])
{
	int check_rf = 0;

	/*A matching will be done with the linked list, if a 2nd person pronoun is found, it will trigger an increment of check_rf*/
	struct smalltalk *tmp = head;
	while (tmp != NULL)
	{
		for (int i = 0; i < inc; i++)
		{
			if ((compare_token(inv[i], "you") == 0) || (compare_token(inv[i], "your") == 0))
			{
				check_rf = 1;
				break;
			}
		}
		if (check_rf == 1)
		{
			break;
		}
		else
		{
			tmp = tmp->next;
		}
	}
	/*If the user input have any one if the intent (5W1H) and with a pronoun, it will enter the small talk section rather than a IS-DO question*/
	if (((compare_token(inv[position], "why") == 0) && check_rf == 1) || ((compare_token(inv[position], "who") == 0) && check_rf == 1) || ((compare_token(inv[position], "where") == 0) && check_rf == 1) || ((compare_token(inv[position], "when") == 0) && check_rf == 1) || ((compare_token(inv[position], "what") == 0) && check_rf == 1) || ((compare_token(inv[position], "how") == 0) && check_rf == 1))
		return 1;
	else if (compare_token(inv[position], "why") != 0 && compare_token(inv[position], "when") != 0 && compare_token(inv[position], "where") != 0 && compare_token(inv[position], "who") != 0 && compare_token(inv[position], "what") != 0 && compare_token(inv[position], "how") != 0)
		return 1;
	else
	{
		return 0;
	}
}

/*
 * Respond to smalltalk.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop chatting (e.g. the smalltalk was "goodbye" etc.)
 */

/*Once the program start, the chatbot will load the smalltalk script and placed them into a linked list*/
void load_smalltalk()
{
	char meleon[MAX_RESPONSE + 1 + MAX_INPUT + 1];
	char lizard[MAX_RESPONSE + 1 + MAX_INPUT + 1];

	/*delimiter for strtok*/
	char delim3[] = "=";

	FILE *s;
	FILE *r;
	s = fopen("smalltalk.ini", "r");
	r = fopen("reflect.ini", "r");
	while (fgets(meleon, MAX_ENTITY + 1 + MAX_RESPONSE + 1, s))
	{
		meleon[strcspn(meleon, "\n")] = 0;
		char *st_entity = strtok(meleon, delim3);
		char *st_response = strtok(NULL, delim3);
		create_smalltalk(strdup(st_entity), strdup(st_response), 0);
	}
	while (fgets(lizard, MAX_ENTITY + 1 + MAX_RESPONSE + 1, r))
	{
		lizard[strcspn(lizard, "\n")] = 0;
		char *st_entity = strtok(lizard, delim3);
		char *st_response = strtok(NULL, delim3);
		create_smalltalk(strdup(st_entity), strdup(st_response), 1);
	}
	fclose(s);
	fclose(r);
}

/*Converts a string to lowercase*/
const int y = 32;
char *LowerCase(char *a)
{
	for (int i = 0; a[i] != '\0'; i++)
		a[i] = a[i] | y;

	return a;
}

/*A function to remove all spaces*/
char *deletespace(char *string)
{
	int len, j;
	len = strlen(string);
	for (int i = 0; i < len; i++)
	{
		if (string[i] == ' ')
		{
			for (j = i; j < len; j++)
			{
				string[j] = string[j + 1];
			}
			len--;
		}
	}
	return string;
}
/*	A basic NLP is implemented in this stage with basic reflective algorithm. 
 *	When a user input reached this stage, the function will proceed to split the sentence into different section based on subject verb agreement.
 * 	Firstly, the user input will be matched with out linked list via linear probing. If found, chatbot will grab the response. Else a standard response will be give.
 * 	Once the response is retrieved, the chatbot will then check for '{0}'. 
 *  If '{0}' is found, the function will be replaced that with a verb that is given by the user input
 * 	Else, the function will just reply with the grabbed response 
 */
int chatbot_do_smalltalk(int inc, char *inv[], char *response, int n)
{
	/*start the pointers*/
	struct smalltalk *tmp = head;
	struct smalltalk *rf_tmp = rf_head;

	/*variables for reflection*/
	char verb[128];
	char question_connector[128];
	char *ret = NULL;
	char catch[MAX_RESPONSE + MAX_INPUT] = "\0";
	char reply[MAX_RESPONSE] = "\0";
	char *con = NULL;
	char *rf = NULL;

	/*storing user input*/
	char input[MAX_INPUT + MAX_RESPONSE] = "\0";
	char haystack[MAX_RESPONSE] = "\0";
	char needle[MAX_RESPONSE] = "\0";
	char split_input[MAX_RESPONSE] = "\0";
	char split[MAX_INPUT][MAX_RESPONSE];
	char needle_split[MAX_INPUT][MAX_RESPONSE];

	/*array to store random response*/
	char ran_array[5][MAX_INPUT];
	char rf_array[MAX_INPUT][MAX_INPUT];

	/*while-loop counter*/
	int counter = 0;
	int rng = 0;
	int sizeofarray = 0;
	int checkclear = 0;

	/*transform the user input into one string and stored in - input*/
	for (int i = 0; i < inc; i++)
	{
		strncat(input, inv[i], MAX_RESPONSE);
		if (i != inc - 1)
			strncat(input, " ", MAX_RESPONSE);
	}

	/*while pointer is not null, the while-loop will do a string match for every line in smalltalk to locate the right response*/
	while (tmp != NULL)
	{
		int checkclear = 0;
		strcpy(haystack, input);
		strcpy(needle, tmp->entity);
		LowerCase(haystack);
		LowerCase(needle);
		ret = strstr(haystack, needle);

		/*Once a match is found, it will immediately store the value into 'catch'*/
		if (ret != NULL)
		{
			strcpy(catch, tmp->response);
			break;
		}
		tmp = tmp->next;
	}
	
	/*convert string into an array of items*/
	char *find_verb = strtok(input, " ");
	int num_split = 0;
	while (find_verb != NULL)
	{
		strcpy(split[num_split], find_verb);
		find_verb = strtok(NULL, " ");
		num_split++;
	}
	
	/*convert string into an array of items*/
	char *compare = strtok(needle, " ");
	int num_compare = 0;
	while (compare != NULL)
	{
		strcpy(needle_split[num_compare], compare);
		compare = strtok(NULL, " ");
		num_compare++;
	}

	int check = 0;
	char *space = " ";
	for (int i = 0; i < num_split; i++)
	{
		if (check == num_compare)
		{
			strncat(split_input, split[i], MAX_RESPONSE);
			if (i != num_split - 1)
			{
				strncat(split_input, space, MAX_RESPONSE);
			}
		}
		else if (compare_token(split[i], needle_split[check]) == 0)
		{
			check++;
		}
		else if (compare_token(split[i], needle_split[check]) != 0)
		{
			check = 0;
		}
	}

	/*If no entity is matched with user input, chatbot will return the following*/
	if (compare_token(catch, "") == 0)
	{
		snprintf(response, n, "Very interesting");
		return 0;
	}

	/*Every entity in the smalltalk.ini have multiple response. The while loop will split the response and store into array*/
	char *split_catch = strtok(catch, ":");
	while (split_catch != NULL)
	{
		strcpy(ran_array[counter], split_catch);
		split_catch = strtok(NULL, ":");
		counter++;
	}

	/*A random number will decide which response the chatbot should run*/
	rng = (rand() % (counter));
	strcpy(reply, ran_array[rng]);

	/*Reflection script from reflect.ini. Transform all singular 2nd person pronoun to 1st person pronoun
	The while loop will check for all entities that exist in the reflect.ini*/
	while (rf_tmp != NULL)
	{
		checkclear = 0;
		strcpy(haystack, split_input);
		strcpy(needle, rf_tmp->entity);
		LowerCase(haystack);
		LowerCase(needle);
		rf = strstr(haystack, needle);

		/*if a reflection entity is found (e.g 'you' is found in the user input), the bot will proceed to make a switch from 'you' to 'me'*/
		if (rf != NULL)
		{
			/*split input into arrays*/
			char *rf_split = strtok(haystack, " ");
			while (rf_split != NULL)
			{
				strcpy(rf_array[checkclear], rf_split);
				rf_split = strtok(NULL, " ");
				checkclear++;
			}

			/*locate the subject word and proceed to make the change (e.g change 'you' to 'me')*/
			for (int i = 0; i < checkclear; i++)
			{
				if (compare_token(rf_array[i], needle) == 0)
				{
					strcpy(rf_array[i], rf_tmp->response);
				}
			}

			/*clean out split_input first*/
			strcpy(split_input, "");

			/*after replacing the pronoun, the following loop will transform the array back into a string*/
			for (int i = 0; i < checkclear; i++)
			{
				strncat(split_input, rf_array[i], MAX_RESPONSE);
				if (i != checkclear - 1)
				{
					strcat(split_input, " ");
				}
			}

			break;
		}
		rf_tmp = rf_tmp->next;
	}

	/*If a '{0}' is found in the response, the chatbot will fill it up with the user adjective/verb. Else, it will take the response with '{0}' and reply accordingly
	Eg.(input: i need to eat. 'need to' will replace '{0}'. Thus the forecasted complete sentence is "why do you need to eat?")*/
	con = strstr(reply, "{0}");
	if (con != NULL)
	{
		char *split_reply = strtok(reply, "{0}");
		char *last_reply = strtok(NULL, "{0}");
		strncat(split_input, last_reply, MAX_RESPONSE);
		strncat(split_reply, split_input, MAX_RESPONSE);
		snprintf(response, n, "%s", split_reply);
		return 0;
	}
	else
	{
		snprintf(response, n, "%s", reply);
		return 0;
	}
	return 0;
}
