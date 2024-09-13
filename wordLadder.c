/*-------------------------------------------
Program 3: Word Ladder
Course: CS 211, Spring 2024, UIC
System: Advanced zyLab
Author: Dale Buencillo
------------------------------------------- */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



int strNumMods(char* str1, char* str2) {
    int numMods = 0;
    // Loop until either strings are not '\0' and compare character by character
    while (*str1 != '\0' && *str2 != '\0') {
            // If characters are different, count it as a difference
            if (*str1 != *str2) {
                numMods++;
            }
            // Move to the next character in both strings
            str1++;
            str2++;
        }
    //if str1 is longer than str2
    while (*str1 != '\0') {
        numMods++;
        str1++;
    }
    //if str2 is longer than str1
    while (*str2 != '\0') {
        numMods++;
        str2++;
    }
    return numMods;
}

int strDiffInd(char* str1, char* str2) {
    int index = 0;

    // Loop until either string reaches its end or characters are different
    while (str1[index] != '\0' && str2[index] != '\0' && str1[index] == str2[index]) {
        index++;
    }

    // Return the index where the first difference occurs
    return index;
}

void addWord(char*** words, int* numWords, int* maxWords, char* newWord) {
    // Check if the current number of words exceeds the maximum capacity
    if (*numWords >= *maxWords) {
        // Double the capacity of the words array
        *maxWords *= 2;
        // Reallocate memory for the words array
        *words = (char**)realloc(*words, (*maxWords) * sizeof(char*));
        if (*words == NULL) {
            fprintf(stderr, "Memory reallocation failed\n");
            exit(EXIT_FAILURE);
        }
    }

    // Allocate memory for the new word and copy it
    (*words)[*numWords] = (char*)malloc((strlen(newWord) + 1) * sizeof(char));
    if ((*words)[*numWords] == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    strcpy((*words)[*numWords], newWord);

    // Increment the number of words
    (*numWords)++;
}

int findWord(char** words, int numWords, char* aWord) { 
   // Loop through the words array to find a match for aWord
    for (int i = 0; i < numWords; i++) {
        // Compare the current word with aWord
        if (strcmp(words[i], aWord) == 0) {
            // Return the index if a match is found
            return i;
        }
    }
    // Return -1 if no match is found
    return -1;
}

bool validWordCheck(char** words, int numWords, char** ladder, int ladderHeight, char* aWord) {
    // Check if the user wants to stop
    if (strcmp(aWord, "DONE") == 0) {
        printf("Stopping with an incomplete word ladder...\n");
        return true;
    }

    // Check if the word is in the dictionary and has the correct length
    int wordIndex = findWord(words, numWords, aWord);
    if (wordIndex == -1) {
        printf("Entered word NOT in dictionary. Try again...\n");
        return false;
    }

    // Check if the word is a single character difference from the previous word
    if (ladderHeight > 0) {
        char* prevWord = ladder[ladderHeight - 1];
        if (strNumMods(prevWord, aWord) != 1) {
            printf("Entered word is NOT a one-character change from the previous word. Try again...\n");
            return false;
        }
    }

    // If the word passes all checks, print a message and return true
    printf("Entered word is valid and will be added to the word ladder.\n");
    return true;
}

bool ladderComplete(char** ladderWords, int n, char* finalWord) {
    // Check if the final word in the ladder matches the target final word
    if (strcmp(ladderWords[n - 1], finalWord) == 0) {
        return true; // Ladder is complete
    } else {
        return false; // Ladder is incomplete
    }
}

void displayIncompleteLadder(char** ladderWords, int n) {
    // Display three lines of "..." at the top of the ladder
    for (int i = 0; i < 3; i++) {
        printf("  ...\n");
    }

    // Display ladder words in reverse order (bottom to top)
    for (int i = n - 1; i >= 0; i--) {
        printf("  %s\n", ladderWords[i]);
    }
}


//-------------------------------------------------------------------
// TODO - Task X: write the displayCompleteLadder() function, 
//      which should display the C-strings in the [ladderWords] array 
//      with the first word at the bottom of the ladder, and each 
//      successive C-string one rung higher on the ladder. In between
//      each ladder rung, display the symbol '^' to signify the 
//      character that changes between the two rungs of the ladder. 
//      The ladder should be displayed with an indentation of two 
//      whitespaces to the left of every word;
//      Ex: if the start word is "data" and final word is "code" 
//          then the output display for a complete ladder should be  
//          as follows (where the quotation marks are NOT a part of  
//          the actual display):
//              "  code"
//              "    ^ "
//              "  cove"
//              "   ^  "
//              "  cave"
//              "  ^   "
//              "  gave"
//              "    ^ "
//              "  gate"
//              "  ^   "
//              "  date"
//              "     ^"
//              "  data" 
//-------------------------------------------------------------------
void displayCompleteLadder(char** ladderWords, int n) {
    // Display the ladder in reverse order
    for (int i = n - 1; i >= 0; i--) {
        printf("  %s\n", ladderWords[i]); // Indentation of two whitespaces
        if (i > 0) {
            int j;
            // Calculate the length of the shorter word between the current and the previous word
            int minLength = (strlen(ladderWords[i]) < strlen(ladderWords[i - 1])) ? strlen(ladderWords[i]) : strlen(ladderWords[i - 1]);
            // Find the index where the words differ
            for (j = 0; j < minLength; j++) {
                if (ladderWords[i][j] != ladderWords[i - 1][j]) {
                    break;
                }
            }
            // Display the symbol '^' at the index where the words differ
            for (int k = 0; k < 2 + j; k++) {
                printf(" ");
            }
            printf("^");
            // Prints remaining whitespace after ^
            for (int m = 2+j; m <= minLength; m++){
                printf(" ");
            }
            //Prints new line
            printf("\n");
        }
    }
}


int main(int argc, char* argv[]) {

    printf("\nProcessing command-line arguments...\n");

    int wordLen = 0;
    int maxLadder = 1;
    char dict[100] = "notAfile";     
    char startWord[30] = "notAword";
    char finalWord[30] = "notValid"; 

    //-------------------------------------------------------------------
    // TODO - Task XII: handle command-line arguments
    // command-line arguments:
    //  [-n wordLen] = sets word length for word ladder;
    //                 if [wordLen] is not a valid input 
    //                 (cannot be less than 2 or greater than 20),
    //                 or missing from command-line arguments, 
    //                 then let user set it using interactive user input
    // [-m maxLadder] = sets maximum word ladder height;
    //                  [maxLadder] must be at least 2;
    //                  if [maxLadder] is invalid or missing from 
    //                  command-line arguments, then let user set it 
    //                  using interactive user input
    // [-d dictFile] = sets dictionary file;
    //                 if [dictFile] is invalid (file not found) or 
    //                 missing from command-line arguments, then let
    //                 user set it using interactive user input
    // [-s startWord] = sets the starting word;
    //                  if [startWord] is invalid 
    //                  (not in dictionary or incorrect length) or 
    //                  missing from command-line arguments, then let
    //                  user set it using interactive user input
    // [-f finalWord] = sets the final word;
    //                  if [finalWord] is invalid  
    //                  (not in dictionary or incorrect length) or 
    //                  missing from command-line arguments, then let
    //                  user set it using interactive user input
    //-------------------------------------------------------------------

     for (int i = 1; i < argc; i += 2) {
        if (strcmp(argv[i], "-n") == 0) {
            // Set word length
            wordLen = atoi(argv[i + 1]);
            if (wordLen < 2 || wordLen > 20) {
                printf("Invalid word length. Please enter a valid word length (2-20): ");
                scanf("%d", &wordLen);
            }
        } else if (strcmp(argv[i], "-m") == 0) {
            // Set maximum word ladder height
            maxLadder = atoi(argv[i + 1]);
            if (maxLadder < 2) {
                printf("Invalid max ladder height. Please enter a height greater than 1: ");
                scanf("%d", &maxLadder);
            }
        } else if (strcmp(argv[i], "-d") == 0) {
            // Set dictionary file
            strcpy(dict, argv[i + 1]);
            FILE* fileTry = fopen(dict, "r");
            if (fileTry == NULL) {
                printf("Dictionary file %s not found...\n", dict);
                printf("Enter filename for dictionary: ");
                scanf("%s", dict);
                printf("\n");
            }
            fclose(fileTry);
        } else if (strcmp(argv[i], "-s") == 0) {
            // Set starting word
            strcpy(startWord, argv[i + 1]);
            // Add validation if necessary
        } else if (strcmp(argv[i], "-f") == 0) {
            // Set final word
            strcpy(finalWord, argv[i + 1]);
            // Add validation if necessary
        }
    }

    printf("Interactively (re)setting invalid parameters:\n");

    // set word length using interactive user-input if necessary
    while (wordLen < 2 || wordLen > 20) {
        printf("Invalid word length = %d\n", wordLen);
        printf("Enter a valid word length (must be 2-20): ");
        scanf("%d",&wordLen);
        printf("\n");
    }

    // set max ladder height using interactive user-input if necessary
    if (maxLadder < 2) {
        printf("Invalid max ladder height = %d\n", maxLadder);
        printf("Enter a valid max ladder height (must be >1): ");
        scanf("%d",&maxLadder);
        printf("\n");
    }

    // interactive user-input sets the dictionary file;
    //  check that file exists; if not, user enters another file
    FILE* fileTry = fopen(dict,"r");
    while (fileTry == NULL) {
        printf("Dictionary file %s not found...\n", dict);
        printf("Enter filename for dictionary: ");
        scanf("%s", dict);
        printf("\n");
        fileTry = fopen(dict,"r");
    }
    fclose(fileTry);
    
    // build the [words] array, a heap-allocated array of C-strings
    // where the words are a read-in from the dictionary file
    // and only those words of the desired length [wordLen] 

    int numWords = 0; // initially, no words in the array
    int maxWords = 4; // initially, capacity = 4

    char** words = (char**)malloc(maxWords*sizeof(char*));
    //Open file for reading
    FILE* file = fopen(dict, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", dict);
        exit(EXIT_FAILURE);
    }

    // Buffer to read words from the file
    char buffer[100]; // Assuming max word length is 100 characters

    // Read words from the file
    while (fscanf(file, "%s", buffer) != EOF) {
        // Check if the word length matches the required length
        if (strlen(buffer) == wordLen) {
            // Add the word to the dynamic array
            addWord(&words, &numWords, &maxWords, buffer);
        }
    }

    // Close the file
    fclose(file);

    printf("The dictionary contains %d words of length %d.\n",numWords,wordLen);
    printf("Max size of the dynamic words array is %d.\n",maxWords);
    printf("\n");

    // end program if file does not have at least two words of desired length
    if (numWords < 2) {
        printf("Dictionary %s contains insufficient %d-letter words...\n",dict,wordLen);
        printf("Terminating program...\n");
        return -1;
    }

    // resetting start word using interactive user-input if necessary
    while (findWord(words,numWords,startWord) == -1) {
        printf("Start word %s is not valid...\n", startWord);
        printf("Enter a valid start word: ");
        scanf("%s", startWord);
        printf("\n");
    }

    // resetting final word using interactive user-input if necessary
    while (findWord(words,numWords,finalWord) == -1) {
        printf("Final word %s is not valid...\n", finalWord);
        printf("Enter a valid final word: ");
        scanf("%s", finalWord);
        printf("\n");
    }


    printf("\nWelcome to the CS 211 Word Ladder Builder!\n");
    printf("\n"); 

    printf("Your goal is to make a word ladder between two ");
    printf("%d-letter words: \n  %s -> %s\n\n",wordLen, startWord,finalWord);  
    
    // Allocating the word ladder for the maximum allowed height
    char** ladder = (char**)malloc(maxLadder*sizeof(char*));

    int ladderHeight = 0; // initially, the ladder is empty
    
    // add the start word to the ladder, i.e. the "bottom rung"
    addWord(&ladder,&ladderHeight,&maxLadder,startWord);
    
    char aWord[30] = "XYZ";
    printf("\n");

 
    while (ladderHeight < maxLadder && strcmp(aWord, "DONE") != 0 && !ladderComplete(ladder, ladderHeight, finalWord)) {
        printf("The goal is to reach the final word: %s\n",finalWord);
        printf("The ladder is currently: \n");
        displayIncompleteLadder(ladder,ladderHeight);
        printf("Current ladder height: %d\n",ladderHeight);
        printf("Maximum ladder height: %d\n",maxLadder);
        printf("Enter the next word (or DONE to stop): ");
        scanf("%s",aWord);
        printf("\n");

        // Make sure the entered word is valid for the next ladder rung;
        // if not, repeatedly allow user to enter another word until one is valid
        while (!validWordCheck(words, numWords, ladder, ladderHeight, aWord)) {
            printf("Enter another word (or DONE to stop): ");
            scanf("%s",aWord);
            printf("\n");
        }

        // add the entered word to the ladder (unless it is "DONE")
        if (strcmp(aWord,"DONE") != 0) {
            addWord(&ladder,&ladderHeight,&maxLadder,aWord);
        }
        printf("\n");

    }
    

    // Check if the built word ladder is complete and 
    // display the word ladder appropriately. 
    if (ladderComplete(ladder, ladderHeight, finalWord)) {
        printf("Word Ladder complete!\n");
        displayCompleteLadder(ladder,ladderHeight);
        printf("Word Ladder height = %d. \n", ladderHeight);
        printf("Can you find a shorter Word Ladder next time??? \n");
    } else {
        if (ladderHeight == maxLadder) printf("The Word Ladder reached the maximum height.\n");
        printf("The final Word Ladder is incomplete:\n");
        displayIncompleteLadder(ladder,ladderHeight);
        printf("Word Ladder height = %d. \n", ladderHeight);
        printf("Can you complete the Word Ladder next time??? \n");
    }
    
    
    //-------------------------------------------------------------------
    // TODO - Task XI: free all heap-allocated memory to avoid potential 
    //                  memory leaks. Since the word length for the word 
    //                  ladder is variable (i.e. set by a command-line 
    //                  argument or interactive user-input) any array
    //                  whose size depends on the word length should be
    //                  dynamically heap-allocated, and thus, must be
    //                  tracked and freed before program termination
    //-------------------------------------------------------------------

 // Free dynamically allocated memory for ladder words
for (int i = 0; i < ladderHeight; i++) {
    free(ladder[i]);
}
free(ladder);

// Free dynamically allocated memory for dictionary words
for (int i = 0; i < numWords; i++) {
    free(words[i]);
}
free(words);

}