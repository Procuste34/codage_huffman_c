#include <stdio.h>
#include <string.h>

#include "pb2.h"

//todo attention aux define il y en a peut etre trop, ou alors en renommer
//todo refaire une couche de comms

int main(void){
    char str[MAX_STRING_LENGTH];
    char *ma_string;
    
    printf("Veuillez tapper un texte :");
    fgets(str, MAX_STRING_LENGTH, stdin); //on écoute l'entrée standard
    ma_string = strtok(str, "\n");
    //si jamais le texte est tappé dans le terminal, le caractère \n est ajouté on doit donc l'enlever

    int z = 0;
    int *nb_car_uniques = &z;
    T_indirectHeap *ih = creer_tas(ma_string, nb_car_uniques);

    //création de l'arbre de codage
    int huffmanTree [2*MAXCARS-1];

    construit_arbre_codage(huffmanTree, ih, *nb_car_uniques);

    //l'arbre de codage est alors construit
    //on va alors le parcourir pour en déduire le code de chacun des caractères de la chaine

    int occurences[MAXCARS]; //stocke les occurences de chaque car.
    int longueurs[MAXCARS]; //stocke la longueur des code de chaque car.
    char codes[MAXCARS][MAXCARS]; //stocke le code de chaque car.
    //MAXCARS = longueur max d'un code (on peut faire mieux avec nb_car_uniques mais taille connue qu'au runtime)

    calculer_codes(huffmanTree, ih, occurences, longueurs, codes);

    //affichage du tableau des codes
    printf("car : occ | long | bits\n");
    printf("----+-----+------+---------\n");

    for(int i = 0; i <= 127; i++){
        if(huffmanTree[i] != -256){
            printf("'%c' :   %d |    %d | %s\n", i, -occurences[i], longueurs[i], codes[i]);
        }
    }

    //print le codage de la string d'entrée
    int len_code_huffman = 0;
    for(int i = 0; i <= strlen(ma_string)-1; i++){
        printf("%s", codes[(int)ma_string[i]]);

        len_code_huffman += longueurs[(int)ma_string[i]];
    }
    printf("\n");

    //print la conclusion
    //longueur du code binaire :
    int len_code_binaire = strlen(ma_string) * 8; //1 char = 8 bit

    //ratio de compression : longueur du code de huffman / longueur du code binaire
    float ratio_compression = 100 * (float) len_code_huffman / (float) len_code_binaire;

    printf("Longueur du code binaire : %d\n", len_code_binaire);
    printf("Longueur du code de huffman : %d\n", len_code_huffman);
    printf("Ratio de compression : %.2f%%\n", ratio_compression);
    
    return 0;
}