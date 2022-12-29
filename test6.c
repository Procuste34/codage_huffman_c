#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/traces.h" 
#include "include/check.h" 

typedef struct{
    char *parcours_prefixe; //todo : arbitraire
    char *caracteres;
} T_entete;

T_entete * parcours_rec(int *tree, int root, T_entete *entete);

int main(void){

    printf("yo\n");

    int huffmanTree[256];
    for(int i = 0; i < 256; i++){
        huffmanTree[i] = -256;
    }

    huffmanTree[65] = -131;
    huffmanTree[66] = -130;
    huffmanTree[67] = -129;
    huffmanTree[68] = -128;
    huffmanTree[82] = 128;
    huffmanTree[128] = 129;
    huffmanTree[129] = 130;
    huffmanTree[130] = 131;

    //char entete[100] = ""; // todo : arbitraire

    T_entete *entete;

    CHECK_IF(entete = malloc(sizeof(T_entete)), NULL, "erreur malloc");
	CHECK_IF(entete->parcours_prefixe = malloc(100 * sizeof(char)), NULL, "erreur malloc");
	CHECK_IF(entete->caracteres = malloc(100 * sizeof(char)), NULL, "erreur malloc");

    entete = parcours_rec(huffmanTree, 131, entete);

    printf("%s\n", entete->parcours_prefixe);
    printf("%s\n", entete->caracteres);

    return 0.;
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