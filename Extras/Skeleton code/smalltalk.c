/* This file includes the functions need for the implementation the smalltalk function along with a basic 
Natural Language Processing (NLP). */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "chat1002.h"
#include <Windows.h>

/* Link list creation of small talk */
struct smalltalk
{
    char *entity;
    char *response;
    struct smalltalk *next;
};

typedef struct smalltalk NODE;
typedef NODE *NODE_PTR;

typedef struct smalltalk reflection_NODE;
typedef reflection_NODE *RF_NODE_PTR;

NODE_PTR head = NULL;
RF_NODE_PTR rf_head = NULL;

struct list *create_smalltalk(char *entity, char *response, int type)
{
    NODE *node = (NODE_PTR)malloc(sizeof(NODE));

    NODE_PTR current = head;
    RF_NODE_PTR rf_current = rf_head;

    switch (type)
    {
    case 0:
    /* Start of linkedlist for basic small talk */
        if (head == NULL)
        {
            current = node;
            node->entity = entity;
            node->response = response;
            node->next = NULL;
            head = current;
        }
        else
        /* Continuation of linkedlist */
        {
            while (current->next != NULL)
            {
                current = current->next;
            }
            current->next = node;
            current->next->entity = entity;
            current->next->response = response;
            current->next->next = NULL;
        }
        break;

    case 1:
     /* Start of linkedlist for NLP */
        if (rf_head == NULL)
        {
            rf_current = node;
            node->entity = entity;
            node->response = response;
            node->next = NULL;
            rf_head = rf_current;
        }
        else
        /* Continuation of linkedlists */
        {
            while (rf_current->next != NULL)
            {
                rf_current = rf_current->next;
            }
            rf_current->next = node;
            rf_current->next->entity = entity;
            rf_current->next->response = response;
            rf_current->next->next = NULL;
        }
        break;

    default:
        break;
    }
}