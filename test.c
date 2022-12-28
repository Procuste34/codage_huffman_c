#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    printf("Program name: %s\n", argv[0]);
    printf("First argument: %s\n", argv[1]);

    printf("affichage : \n");

    /* you must add one to cover the byte needed for the terminating null character */
    char *stringB = (char *) malloc(strlen(argv[1]) + 1); 
    strcpy(stringB, argv[1]);

    for(int i = 0; i < strlen(stringB); i++){
        printf("%c\n", stringB[i]);
    }



    return 0;
}