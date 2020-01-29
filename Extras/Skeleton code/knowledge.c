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
#include <string.h>
#include "chat1002.h"
#include <Windows.h>

const int x = 32;

// Converts a string to lowercase
char *toLowerCase(char *a)
{
	for (int i = 0; a[i] != '\0'; i++)
		a[i] = a[i] | x;

	return a;
}

struct LIST
{
	char *entity;
	char *response;
	struct LIST *next;
};
typedef struct LIST LIST_NODE;
typedef LIST_NODE *LIST_NODE_PTR;

typedef struct LIST WHO_NODE;
typedef WHO_NODE *WHO_NODE_PTR;

typedef struct LIST WHAT_NODE;
typedef WHAT_NODE *WHAT_NODE_PTR;

typedef struct LIST WHERE_NODE;
typedef WHERE_NODE *WHERE_NODE_PTR;

typedef struct LIST WHEN_NODE;
typedef WHEN_NODE *WHEN_NODE_PTR;

typedef struct LIST WHY_NODE;
typedef WHY_NODE *WHY_NODE_PTR;

typedef struct LIST HOW_NODE;
typedef HOW_NODE *HOW_NODE_PTR;

WHO_NODE_PTR who_head = NULL;
WHAT_NODE_PTR what_head = NULL;
WHERE_NODE_PTR where_head = NULL;
WHEN_NODE_PTR when_head = NULL;
WHY_NODE_PTR why_head = NULL;
HOW_NODE_PTR how_head = NULL;

struct list *createLL(char *intent, char *entity, char *response)
{
	LIST_NODE *node = (LIST_NODE_PTR)malloc(sizeof(LIST_NODE));

	WHAT_NODE_PTR what_node = what_head;
	WHERE_NODE_PTR where_node = where_head;
	WHO_NODE_PTR who_node = who_head;
	WHEN_NODE_PTR when_node = when_head;
	WHY_NODE_PTR why_node = why_head;
	HOW_NODE_PTR how_node = how_head;

	if (compare_token(intent, "what") == 0)
	{
		if (what_head == NULL)
		{
			what_node = node;
			node->entity = entity;
			node->response = response;
			node->next = NULL;
			what_head = what_node;
		}
		else
		{
			while (what_node->next != NULL)
			{
				what_node = what_node->next;
			}
			what_node->next = node;
			what_node->next->entity = entity;
			what_node->next->response = response;
			what_node->next->next = NULL;
		}
	}
	else if (compare_token(intent, "where") == 0)
	{
		if (where_head == NULL)
		{
			where_node = node;
			node->entity = entity;
			node->response = response;
			node->next = NULL;
			where_head = where_node;
		}
		else
		{
			while (where_node->next != NULL)
			{
				where_node = where_node->next;
			}
			where_node->next = node;
			where_node->next->entity = entity;
			where_node->next->response = response;
			where_node->next->next = NULL;
		}
	}
	else if (compare_token(intent, "who") == 0)
	{
		if (who_head == NULL)
		{
			who_node = node;
			node->entity = entity;
			node->response = response;
			node->next = NULL;
			who_head = who_node;
		}
		else
		{
			while (who_node->next != NULL)
			{
				who_node = who_node->next;
			}
			who_node->next = node;
			who_node->next->entity = entity;
			who_node->next->response = response;
			who_node->next->next = NULL;
		}
	}
	else if (compare_token(intent, "when") == 0)
	{
		if (when_head == NULL)
		{
			when_node = node;
			node->entity = entity;
			node->response = response;
			node->next = NULL;
			when_head = when_node;
		}
		else
		{
			while (when_node->next != NULL)
			{
				when_node = when_node->next;
			}
			when_node->next = node;
			when_node->next->entity = entity;
			when_node->next->response = response;
			when_node->next->next = NULL;
		}
	}
	else if (compare_token(intent, "why") == 0)
	{
		if (why_head == NULL)
		{
			why_head = node;
			node->entity = entity;
			node->response = response;
			node->next = NULL;
			why_head = why_head;
		}
		else
		{
			while (why_head->next != NULL)
			{
				why_head = why_head->next;
			}
			why_head->next = node;
			why_head->next->entity = entity;
			why_head->next->response = response;
			why_head->next->next = NULL;
		}
	}
	else if (compare_token(intent, "how") == 0)
	{
		if (how_head == NULL)
		{
			how_head = node;
			node->entity = entity;
			node->response = response;
			node->next = NULL;
			how_head = how_head;
		}
		else
		{
			while (how_head->next != NULL)
			{
				how_head = how_head->next;
			}
			how_head->next = node;
			how_head->next->entity = entity;
			how_head->next->response = response;
			how_head->next->next = NULL;
		}
	}
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
int knowledge_get(const char *intent, const char *entity, char *response, int n)
{
	char delim[] = " ";
	char *ret = NULL;
	char haystack[MAX_RESPONSE], needle[MAX_RESPONSE];

	if (compare_token(intent, "who") == 0)
	{
		struct LIST *ptr = who_head;
		while (ptr != NULL)
		{
			if (compare_token(ptr->entity, entity) == 0)
			{
				snprintf(response, n, "%s", ptr->response);
				return KB_FOUND;
			}
			else
			{
				strcpy(needle, entity);
				strcpy(haystack, ptr->entity);
				toLowerCase(needle);
				toLowerCase(haystack);
				ret = strstr(haystack, needle);

				if (ret != NULL)
				{
					snprintf(response, n, "%s", ptr->response);
					return KB_FOUND;
				}
			}
			ptr = ptr->next;
		}
		return KB_NOTFOUND;
	}

	else if (compare_token(intent, "what") == 0)
	{
		struct LIST *ptr = what_head;
		while (ptr != NULL)
		{
			if (compare_token(ptr->entity, entity) == 0)
			{
				snprintf(response, n, "%s", ptr->response);
				return KB_FOUND;
			}
			else
			{
				strcpy(needle, entity);
				strcpy(haystack, ptr->entity);
				toLowerCase(needle);
				toLowerCase(haystack);
				ret = strstr(haystack, needle);

				if (ret != NULL)
				{
					snprintf(response, n, "%s", ptr->response);
					return KB_FOUND;
				}
			}
			ptr = ptr->next;
		}
	}
	else if (compare_token(intent, "where") == 0)
	{
		struct LIST *ptr = where_head;
		while (ptr != NULL)
		{
			if (compare_token(ptr->entity, entity) == 0)
			{
				snprintf(response, n, "%s", ptr->response);
				return KB_FOUND;
			}
			else
			{
				strcpy(needle, entity);
				strcpy(haystack, ptr->entity);
				toLowerCase(needle);
				toLowerCase(haystack);
				ret = strstr(haystack, needle);

				if (ret != NULL)
				{
					snprintf(response, n, "%s", ptr->response);
					return KB_FOUND;
				}
			}
			ptr = ptr->next;
		}
	}
	else if (compare_token(intent, "when") == 0)
	{
		struct LIST *ptr = when_head;
		while (ptr != NULL)
		{
			if (compare_token(ptr->entity, entity) == 0)
			{
				snprintf(response, n, "%s", ptr->response);
				return KB_FOUND;
			}
			else
			{
				strcpy(needle, entity);
				strcpy(haystack, ptr->entity);
				toLowerCase(needle);
				toLowerCase(haystack);
				ret = strstr(haystack, needle);

				if (ret != NULL)
				{
					snprintf(response, n, "%s", ptr->response);
					return KB_FOUND;
				}
			}
			ptr = ptr->next;
		}
	}
	else if (compare_token(intent, "why") == 0)
	{
		struct LIST *ptr = why_head;
		while (ptr != NULL)
		{
			if (compare_token(ptr->entity, entity) == 0)
			{
				snprintf(response, n, "%s", ptr->response);
				return KB_FOUND;
			}
			else
			{
				strcpy(needle, entity);
				strcpy(haystack, ptr->entity);
				toLowerCase(needle);
				toLowerCase(haystack);
				ret = strstr(haystack, needle);

				if (ret != NULL)
				{
					snprintf(response, n, "%s", ptr->response);
					return KB_FOUND;
				}
			}
			ptr = ptr->next;
		}
	}
	else if (compare_token(intent, "how") == 0)
	{
		struct LIST *ptr = how_head;
		while (ptr != NULL)
		{
			if (compare_token(ptr->entity, entity) == 0)
			{
				snprintf(response, n, "%s", ptr->response);
				return KB_FOUND;
			}
			else
			{
				strcpy(needle, entity);
				strcpy(haystack, ptr->entity);
				toLowerCase(needle);
				toLowerCase(haystack);
				ret = strstr(haystack, needle);

				if (ret != NULL)
				{
					snprintf(response, n, "%s", ptr->response);
					return KB_FOUND;
				}
			}
			ptr = ptr->next;
		}
	}
	return KB_NOTFOUND;
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
int knowledge_put(const char *intent, const char *entity, const char *response)
{
	createLL(strdup(intent), strdup(entity), strdup(response));
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
int knowledge_read(FILE *f)
{
	char mander[128];
	int num = 10;

	/*delimiter for strtok*/
	char delim1[] = "[";
	char delim2[] = "]";
	char delim3[] = "=";

	/*entity and response*/
	char *entity;
	char *response;
	
	while (fgets(mander, MAX_ENTITY + 1 + MAX_RESPONSE + 1, f))
	{
		mander[strcspn(mander, "\n")] = 0;
		for (int i = 0; i < strlen(mander); i++)
		{
			if (isalpha(mander[i]) != 0 && !strstr(mander, "="))
			{
				char *charptr = strtok(mander, delim1);
				char *charmander = strtok(charptr, delim2);

				if (compare_token(charmander, "who") == 0)
				{
					num = 0;
				}
				else if (compare_token(charmander, "what") == 0)
				{
					num = 1;
				}
				else if (compare_token(charmander, "where") == 0)
				{
					num = 2;
				}
				else if (compare_token(charmander, "when") == 0)
				{
					num = 3;
				}
				else if (compare_token(charmander, "why") == 0)
				{
					num = 4;
				}
				else if (compare_token(charmander, "how") == 0)
				{
					num = 5;
				}
			}
			else if (isalpha(mander[i]) == 0 && strstr(mander, "="))
			{
				char *entity = strtok(mander, delim3);
				char *response = strtok(NULL, delim3);
				switch (num)
				{
				case 0:
					knowledge_put("who", entity, response);
					break;

				case 1:
					knowledge_put("what", entity, response);
					break;

				case 2:
					knowledge_put("where", entity, response);
					break;

				case 3:
					knowledge_put("when", entity, response);
					break;

				case 4:
					knowledge_put("why", entity, response);
					break;

				case 5:
					knowledge_put("how", entity, response);
					break;

				default:
					break;
				}
			}
		}
	}
	if (num == 10)
	{
		return KB_NOTFOUND;
	}
	else
	{
		return 0;
	}
}

/*
 * Reset the knowledge base, removing all know entitities from all intents.
 */
void knowledge_reset()
{

	/* to be implemented */

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
	}

	while (when_head != NULL)
	{
		tmp = when_head;
		when_head = when_head->next;
		free(tmp);
	}

	while (why_head != NULL)
	{
		tmp = why_head;
		why_head = why_head->next;
		free(tmp);
	}

	while (how_head != NULL)
	{
		tmp = how_head;
		how_head = how_head->next;
		free(tmp);
	}

}

/* Write the knowledge base to a file.
 *
 * Input:
 *   f - the file
 */
void knowledge_write(FILE *f)
{
	/*What intent*/
	struct LIST *what_ptr = what_head;
	fputs("[what]\n", f);
	while (what_ptr != NULL)
	{
		fprintf(f, "%s=%s\n", what_ptr->entity, what_ptr->response);
		what_ptr = what_ptr->next;
	}

	/*Where intent*/
	struct LIST *where_ptr = where_head;
	fputs("\n[where]\n", f);
	while (where_ptr != NULL)
	{
		fprintf(f, "%s=%s\n", where_ptr->entity, where_ptr->response);
		where_ptr = where_ptr->next;
	}

	/*Who intent*/
	struct LIST *who_ptr = who_head;
	fputs("\n[who]\n", f);
	while (who_ptr != NULL)
	{
		fprintf(f, "%s=%s\n", who_ptr->entity, who_ptr->response);
		who_ptr = who_ptr->next;
	}

	/*When intent*/
	struct LIST *when_ptr = when_head;
	fputs("\n[when]\n", f);
	while (when_ptr != NULL)
	{
		fprintf(f, "%s=%s\n", when_ptr->entity, when_ptr->response);
		when_ptr = when_ptr->next;
	}

	/*Why intent*/
	struct LIST *why_ptr = why_head;
	fputs("\n[why]\n", f);
	while (why_ptr != NULL)
	{
		fprintf(f, "%s=%s\n", why_ptr->entity, why_ptr->response);
		why_ptr = why_ptr->next;
	}

	/*How intent*/
	struct LIST *how_ptr = how_head;
	fputs("\n[how]\n", f);
	while (how_ptr != NULL)
	{
		fprintf(f, "%s=%s\n", how_ptr->entity, how_ptr->response);
		how_ptr = how_ptr->next;
	}
}