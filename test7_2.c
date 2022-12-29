#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/traces.h" 
#include "include/check.h" 

//generer huffmanTree à partir d'une entete

typedef struct{
    char *parcours_prefixe; //todo : arbitraire
    char *caracteres;
} T_entete;

void parcours(int *tree, char *parcours_prefixe, char *caracteres);
void parcours_rec(int *tree, char *parcours_prefixe, char *caracteres, int appel_fg, int *j, int *i, int *compteur_car, int pere);

int main(void){
    printf("yo\n");

    char parcours_prefixe[10] = "001010111";
    char caracteres[10] = "BCDRA";

    int huffmanTree[256];
    for(int i = 0; i < 256; i++){
        huffmanTree[i] = -256;
    }

    parcours(huffmanTree, parcours_prefixe, caracteres);

    for(int i = 0; i < 256; i++){
        if(huffmanTree[i] != -256){
            printf("%d : %d\n", i, huffmanTree[i]);
        }
    }

    return 0.;
}

void parcours(int *tree, char *parcours_prefixe, char *caracteres){
    int j_base = 128;
    int i_base = 0;
    int compteur_car_base = 0;

    int *j = &j_base;
    int *i = &i_base;
    int *compteur_car = &compteur_car_base;
    parcours_rec(tree, parcours_prefixe, caracteres, 0, j, i, compteur_car, -1);
}


void parcours_rec(int *tree, char *parcours_prefixe, char *caracteres, int appel_fg, int *j, int *i, int *compteur_car, int pere){
    if(parcours_prefixe[*i] == '0'){ //noeud interne
        if(appel_fg){
            tree[*j] = -pere;
        }else{
            tree[*j] = +pere;
        }

        (*i)++;
        //appel rec. a gauche
        int pere = *j;
        (*j)++;

        //tree[*j] = -pere;
        parcours_rec(tree, parcours_prefixe, caracteres, 1, j, i, compteur_car, pere);

        //appel rec. à droite
        //tree[*j] = +pere;
        parcours_rec(tree, parcours_prefixe, caracteres, 0, j, i, compteur_car, pere);

    }else{ //feuille
        (*i)++;
        
        if(appel_fg){
            tree[(int)caracteres[*compteur_car]] = -pere;
        }else{
            tree[(int)caracteres[*compteur_car]] = +pere;
        }

        (*compteur_car)++;
    }
}