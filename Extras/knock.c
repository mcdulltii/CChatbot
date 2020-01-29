#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>



void knockKnock (){
    // These are the list of responses, all randomised by the random function.
    char *randomResponse [5] = {"HAHA funny", "Nice one", "Lame","... ok","erm... i don't get it"};
    srand(time(0));
    int randint = rand() %5; // Generate the random index

    char knock1[256]; // First user input for the function
    char knockwhatever[256]; // Second user input for the function


    printf("Chatbot: Who's there?\nUser: ");
    fgets(knock1, 256, stdin);
    char *tok = strtok(knock1,"\n");
    printf("Chatbot: %s who?\nUser: ", tok);
    fgets(knockwhatever, 256, stdin);
    printf("Chatbot: %s", randomResponse[randint]);
}


int main(){
    
    char input [256];
    printf("User: ");
    fgets(input, 256, stdin);

    /*To re-implement within the main body so that it works*/
    char *token = strtok(input, "\n"); // Get the user input until before '\n'
   

    if (strcmp("knock knock",token) == 0){
        knockKnock(); // Runs if "knock knock". Runs with no arguments inside so you can just call the function if conditions are met without having to pass in the parameters
    } else {printf("wrong");}
}