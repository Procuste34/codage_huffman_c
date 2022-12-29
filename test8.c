#include <stdio.h>

int main(int argc, char *argv[]){
    printf("yo\n");

    if(argc==2){
        printf("décompressage\n");
        printf("fichier à décompresser : %s\n", argv[1]);
    }else{
        printf("compressage\n");
        printf("fichier à compresser : %s\n", argv[1]);
        printf("fichier target : %s\n", argv[2]);
    }

    return 0;
}