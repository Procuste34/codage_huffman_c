#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/traces.h" 
#include "include/check.h" 

typedef struct{
    char *parcours_prefixe; //todo : arbitraire
    char *caracteres;
} T_entete;

int main(void){
    printf("yo\n");

    char parcours_prefixe[10] = "010101011";
    char caracteres[10] = "ABCDR";

    int huffmanTree[256];
    for(int i = 0; i < 256; i++){
        huffmanTree[i] = -256;
    }

    int j = 0;
    int compteur_car = 0;
    int last_noeud = -1;
    int vu_fg = 0;
    for(int i = 0; i<strlen(parcours_prefixe); i++){
        if(parcours_prefixe[i] == '0'){
            //update huffmanTree
            if(last_noeud != -1){
                if(vu_fg){ //on a vu le fg de last_noeud, on est donc sur son fils droit
                    huffmanTree[128+j] = +last_noeud;
                }else { //inutile normalement
                    printf("AH BiZARRE\n");
                    huffmanTree[128+j] = -last_noeud;
                }
            }

            //update last_noeud et vu_fg
            last_noeud = 128+j;
            vu_fg = 0;

            //update j
            j++;
        }else {
            //update huffmanTree
            int caractere_feuille = (int)caracteres[compteur_car];

            if(vu_fg){
                huffmanTree[caractere_feuille] = +last_noeud;
            }else {
                huffmanTree[caractere_feuille] = -last_noeud;
            }
            
            //update vu_fg (si false, on le passe a true)
            if(!vu_fg){
                vu_fg = 1;
            }

            compteur_car++;
        }
    }

    for(int i = 0; i < 256; i++){
        if(huffmanTree[i] != -256){
            printf("%d : %d\n", i, huffmanTree[i]);
        }
    }

    return 0.;
}