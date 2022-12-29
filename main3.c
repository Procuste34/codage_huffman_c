#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/traces.h"
#include "include/check.h"

typedef struct{
    char *parcours_prefixe;
    char *caracteres;
} T_entete;

//todo : renommer les fonctions (parcours_rec)

T_entete * parcours_rec(int *tree, int root, T_entete *entete);

int main(int argc, char *argv[]){
    printf("yo\n");

    int compressage = 0;

    if(argc==2){
        printf("décompressage\n");
        printf("fichier à décompresser : %s\n", argv[1]);
    }else{
        compressage = 1;
        printf("compressage\n");
        printf("fichier à compresser : %s\n", argv[1]);
        printf("fichier target : %s\n", argv[2]);
    }

    if(compressage){
        FILE *fp = fopen("a_compresser", "r");

        fseek(fp, 0, SEEK_END); //on se déplace à la fin du fichier pour en connaitre sa taille
        long size = ftell(fp); //on recupere la taille du fichier
        char *str = malloc(size + 1); //todo : checkif

        fseek(fp, 0, SEEK_SET); //on revient en début de fichier

        fread(str, 1, size, fp); //on lit le fichier en entier
        
        str[size] = '\0';

        fclose(fp);

        //////

        printf("%s\n", str);

        //todo : free(str);








    }

    return 0;
}

T_entete * parcours_rec(int *tree, int root, T_entete *entete){
    //traitement : si c'est une feuille on ajoute 1, et 0 sinon
    if(root >= 128){
        char c0 = '0';
        strncat(entete->parcours_prefixe, &c0, 1);
    }else{
        char c1 = '1';
        strncat(entete->parcours_prefixe, &c1, 1);

        char car[2];
        sprintf(car, "%c", root);
        strncat(entete->caracteres, car, 1);
    }

    //on cherche fg, on lance appel rec dessus
    int fils_gauche = -256;

    for(int i = 0; i<256; i++){
        if(tree[i] == -root){
            //on a trouvé le fils gauche
            fils_gauche = i;
        }
    }

    if(fils_gauche != -256){
        parcours_rec(tree, fils_gauche, entete);
    }

    //on cherche fd, on lance appel rec dessus
    int fils_droit = -256;

    for(int i = 0; i<256; i++){
        if(tree[i] == root){
            //on a trouvé le fils droit
            fils_droit = i;
        }
    }

    if(fils_droit != -256){
        parcours_rec(tree, fils_droit, entete);
    }

    return entete;
}