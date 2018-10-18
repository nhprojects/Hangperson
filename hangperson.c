#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>


static const int MAX_MISSES = 7;

/*
 * Be happy you didn't have to write this function.
 * Prints a low-tech ASCII gallows.  Max number of misses
 * is MAX_MISSES (i.e., if there are MAX_MISSES misses, the
 * player loses and the poor sap gets hung).
 */
void print_gallows(int num_missed) {
    // make sure that we got called in a reasonable way
    assert(num_missed >= 0 && num_missed <= MAX_MISSES);

    printf("\n\n       |||========|||\n");
    if (num_missed > 0) {
        printf("       |||         |\n");
    } else {
        printf("       |||          \n");
    }

    if (num_missed > 1) {
        printf("       |||         O\n");
    } else {
        printf("       |||          \n");
    }

    if (num_missed > 2) {
        if (num_missed > 4) {
            printf("       |||        /|\\\n");
        } else if (num_missed > 3) {
            printf("       |||        /| \n");
        } else {
            printf("       |||        /  \n");
        }
    } else {
        printf("       |||           \n");
    }

    if (num_missed > 5) {
        if (num_missed > 6) {
            printf("       |||        / \\\n");
        } else {
            printf("       |||        /  \n");
        }
    } else {
        printf("       |||           \n");
    }

    printf("       |||\n");
    printf("       |||\n");
    printf("     =================\n\n");
}

/*
 * The main function is where everything starts.  Choose a random
 * word and call one_game.
 */

char get_guess() {
    char user_input[64];
    char guess;

    do {
        printf(">>> What is your guess? ");
        if (fgets(user_input, 64, stdin)==NULL){
            exit(1);
        }
        guess = user_input[0];
        if (isalpha(guess)) {
            break;
        }
        printf("Guess must be an alphabetical character.\n");
    } while (true);

    return toupper(guess);
}

int is_repeat(char guess, char guesses_made[]){
    for(int i = 0; i < strlen(guesses_made); i++){
        if(guesses_made[i] ==guess){
            printf("Already guessed: %c\n", guess);
            return 1;
        }
    }
    return 0;
}

int valid_guess(char guess, char secret_word[], char blank_word[]){
    int flag = 0;
    for (int i = 0; i < strlen(secret_word); i++) {
        if (guess == secret_word[i]) {
            blank_word[i] = guess;
            flag = 1;
        }
    }
    if (flag) {
        printf("Good guess.\n");
    }
    return flag;
}

 int check_guess(char guesses_made[], char secret_word[], char blank_word[]){
     char guess = get_guess();
     while (is_repeat(guess, guesses_made)) {
         guess = get_guess();
     }
     guesses_made[strlen(guesses_made)] = guess;
     return valid_guess(guess, secret_word, blank_word);
 }

int game_over(int misses, char blank_word[]){
    if(misses >= MAX_MISSES){
        printf("You lost and made stick-person sad...\n");
        return 1;
    }
    for (int i = 0; i < strlen(blank_word); i++) {
        if (blank_word[i]=='_') {
            return 0;
        }
    }
    printf("Congratulations! You won and saved stick-person!\n");
    return 1;
}

void print_word(char word[], char delim){
    printf("%c",word[0]);
    for (int i = 1; i < strlen(word) ;i++) {
        printf("%c",delim);
        printf("%c",word[i]);
    }
    printf("\n");
}

int main(int argc, char **argv) {
    /* You can modify the words used in the game if you want;
     * just be sure to also update the num_words constant */
     //
     // start your code here.  don't forget to structure it
     // in SOFA functions (functions should be Short, do One thing,
     // take Few arguments, and adhere to a single level of Abstraction)
     //


     // Example "win" message (the word "won" needs to be in it for the tester)
     // printf("Congratulations!  You won and saved stick-person!\n");

     // Example "loss" message (the word "lost" needs to be in it for the tester)
     // printf("You lost and made stick-person sad...\n");
     //
    char secret_word[64];
    const char *words[] = {"WINTER", "SNOW", "ICE", "FROZEN"};

    if (argc == 1) {
        srand(time(NULL));
        int num_words = sizeof(words) / sizeof(char *);
        strcpy(secret_word, words[rand() % num_words]);
    } else {
        strcpy(secret_word, argv[1]);
        for (int i = 0; i < strlen(secret_word); i++) {
            secret_word[i] = toupper(secret_word[i]);
        }
    }

    int misses=0;
    // initialize blank word and guesses made
    char blank_word[strlen(secret_word)+1];  //word with underscores
    blank_word[strlen(secret_word)]='\0';
    for (int i=0;i<strlen(secret_word);i=i+1){
        blank_word[i]='_';
    }

    char guesses_made[64];
    for (int i=0;i<sizeof(guesses_made);i++){
        guesses_made[i]='\0';
    }

    while(!game_over(misses, blank_word)){
        print_gallows(misses);
        print_word(blank_word, ' ');
        misses += !check_guess(guesses_made, secret_word, blank_word);
        printf("Missed: %d\n", misses);
    }
    
    return EXIT_SUCCESS;
}
