#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

/* Handle for window API */
HANDLE hConsole;

// Will need to implement the modified verrsion of the criteria needed to trigger the help msg (i.e to indentify the word "help")
void helpMessage()
{
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    /* Set colour to dark yellow */
    SetConsoleTextAttribute(hConsole, 6);

    printf("\n======================================\n");
    printf("CHATBOT HELP\n");
    printf("======================================\n\n");
    printf("Start typing to interact with the chatbot.\nIn the event that the chatbot is unable to answer your question, you will be given the option to provide the answers\n");
    printf("Smalltalk functionalities are in beta, only a small pool of responses are available.\n\n");
    printf("INSTRUCTIONS/COMMANDS\n");
    printf("HELP\t\t\t\tShow the help menu.\n");
    printf("LOAD [from] 'filename'\t\tLoad from a certain config file within the current working directory.\n");
    printf("SAVE [to] [as] 'filename'\tSave the current configuration into the current working directory.\n");
    printf("RESET\t\t\t\tReset the chatbot to its original base state\n");
    printf("EXIT\t\t\t\tTerminate the chatbot program.\n");
    printf("QUIT\t\t\t\tSame as 'EXIT'\n\n");
}
