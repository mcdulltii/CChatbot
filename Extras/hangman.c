
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 32
#define MAX_GUESS_CHAR 2





void guessword() {
    // List of random words to play with the user. Randomised index using srand and rand
    char *randomWords[5] = {"this","cockatoo", "helicopter", "norwegian", "sirloin"};
    srand(time(0));
    int randint = rand() %5;

    // Initialise the list of the winning and losing messages. Randomised index generator at the end
    char *endgameMessage[5] = {"You were close.. i guess", "Better luck next time!", "Try again lol","You can do it the next time", "Dont'dispair"};
    char *winMessage[5] = {"Yay you did it!", "Great Job!", "Try another round!", "Whew nice one", "Woohoo you got the answer!"};
     
    
    char p1_input[MAX_INPUT];
    strcpy(p1_input,randomWords[randint]); // Copy the chosen element into p1_input

    
    
    // Initialse local variables for this game
    int guessCounter = 10; // Number of max tries
    char guessChar[MAX_GUESS_CHAR];
    int p1_length = strlen(p1_input);
    int chk = 0; // Check whether the user has won. 0: Nope; 1: User won
    

    
    char underscore[2*MAX_INPUT]; // allow _ _ _ instead of ___
    // To set corresponding underscores for the string for the player to guess
    int temp1 = 0;
    for (int i=0; i<p1_length;i++){
        underscore[temp1] = '_';
        temp1++;
        underscore[temp1] = ' ';
        temp1++;
    } underscore[temp1] = '\0'; // make it a null terminated "String"

    
    while(guessCounter > 0 && chk ==0){ // While there are still guesses AND user hasnt won
        if (guessChar[0] != '\n'){ // Just to fix a quirk with the fgets function
            printf("You has so far guessed: %s\n",underscore);
            printf("You have %d guesses remaining. Enter your next guess :) :\n",guessCounter);
        }
        
        fgets(guessChar,MAX_GUESS_CHAR,stdin);
        guessChar[0] = tolower(guessChar[0]);
        int temp2 = 0; // Set to 1 if there is matching letter

        if (guessChar[0] != '\n'){ // Just to fix a quirk with the fgets function
            for (int i=0; i<strlen(p1_input);i++){
                if (guessChar[0] == p1_input[i]){
                underscore[2*i] = guessChar[0];
                p1_length--;
                p1_input[i] = toupper(p1_input[i]); // to uppercase to disable repeated use of the same letter
                temp2 = 1; 
                }
            }
            if (temp2 != 1){ // Reduce the guessCounter if no matching letter
                guessCounter --; 
            }

            if (p1_length == 0) {
                // Randomise the winning message
                srand(time(0));
                int randint2 = rand() %5;
                printf("You has guessed: %s\n",underscore);
                printf("%s", winMessage[randint2]);
                chk =1; // To indicate the user won
            }
        }
    }
      
      
    if (chk !=1){ // If the user did not win
    // Randomise the game over message
        srand(time(0));
        int randint2 = rand() %5;
        printf("%s",endgameMessage[randint2]); 
    }
}






int main(){
    char input [256];
    printf("User: ");
    fgets(input, 256, stdin);
    char *token = strtok(input, "\n");
   

    if (strcmp("hangman",token) == 0){
        guessword(); // Runs if "hangman". Can just run if the conditions are met, no need to parse in parameters.
    } else {printf("wrong");}
    
    return 0;
}