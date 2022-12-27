#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    printf("Program name: %s\n", argv[0]);
    printf("First argument: %s\n", argv[1]);

    printf("affichage : \n");

    for(int i = 0; i < strlen(argv[1]); i++){
        printf("%c\n", argv[1][i]);
    }


    return 0;
}
