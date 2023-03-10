#include <stdio.h>

#include "pb3.h"

int main(int argc, char *argv[]){
    int compressage = 0;

    if(argc==3){
        compressage = 1;
    }
    if(argc != 2 && argc != 3){
        printf("erreur : mauvais nombre d'arguments passés!\n");
        return 0.;
    }

    if(compressage){
        FILE *fp = fopen(argv[1], "r");
        if(fp == NULL){
            perror("Erreur lors de l'ouverture du fichier");
            return 0;
        }

        fseek(fp, 0, SEEK_END); //on se déplace à la fin du fichier pour en connaitre sa taille
        long size = ftell(fp); //on recupere la taille du fichier
        char *str;
        CHECK_IF(str = malloc(size + 1), NULL, "erreur malloc");

        fseek(fp, 0, SEEK_SET); //on revient en début de fichier

        fread(str, 1, size, fp); //on lit le fichier en entier
        
        str[size] = '\0';

        fclose(fp);

        int z = 0;
        int *nb_car_uniques = &z;
        T_indirectHeap *ih = creer_tas(str, nb_car_uniques);

        //création de l'arbre de codage
        int huffmanTree[2*MAXCARS];
        for(int i = 0; i < 2*MAXCARS; i++) huffmanTree[i] = -256;

        construit_arbre_codage(huffmanTree, ih, *nb_car_uniques);

        //l'arbre de codage est alors construit

        char codes[MAXCARS][MAXCARS]; //stocke le code de chaque car.
        //MAXCARS = longueur max d'un code (on peut faire mieux avec nb_car_uniques mais taille connue qu'au runtime)

        calculer_codes(huffmanTree, codes);

        //ecrire l'entete
        T_entete *entete = init_entete();

        huffmanTree_to_entete(huffmanTree, *nb_car_uniques, entete);

        FILE *fp_target = fopen(argv[2], "w");

        fputs(entete->parcours_prefixe, fp_target);
        fputs("\n", fp_target);
        fputs(entete->caracteres, fp_target);
        fputs("\n", fp_target);

        //ecrire le codage de la string d'entrée
        for(int i = 0; i <= strlen(str)-1; i++){
            fputs(codes[(int)str[i]], fp_target);
        }

        fclose(fp_target);
        free(str);

    }else { // décompressage
        FILE *fp = fopen(argv[1], "r");
        if(fp == NULL){
            perror("Erreur lors de l'ouverture du fichier");
            return 0;
        }

        fseek(fp, 0, SEEK_END); //on se déplace à la fin du fichier pour en connaitre sa taille
        long size = ftell(fp); //on recupere la taille du fichier
        char *str;
        CHECK_IF(str = malloc(size + 1), NULL, "erreur malloc");

        fseek(fp, 0, SEEK_SET); //on revient en début de fichier

        fread(str, 1, size, fp); //on lit le fichier en entier
        
        str[size] = '\0';

        fclose(fp);

        //on a lu et stocké tout le contenu du fichier (entete+code) dans str
        //on va alors parcourir str pour split cela
        //et recupérer : parcours_prefixe, caracteres et le code séparément
        
        //on parcourt str avec la variable i

        T_entete *entete = init_entete();

        char c = str[0];
        int i = 0;
        while(c != '\n'){
            strncat(entete->parcours_prefixe, &c, 1);

            i++;
            c = str[i];   
        }
        i = i+1;
        c = str[i];

        while(c != '\n'){
            strncat(entete->caracteres, &c, 1);

            i++;
            c = str[i];   
        }

        //on a l'entete, on peut donc en déduire huffmanTree
        int huffmanTree[2*MAXCARS];
        for(int i = 0; i < 2*MAXCARS; i++) huffmanTree[i] = -256;

        entete_to_huffmanTree(huffmanTree, entete);

        char codes[MAXCARS][MAXCARS]; //stocke le code de chaque car.
        //MAXCARS = longueur max d'un code (on peut faire mieux avec nb_car_uniques mais taille connue qu'au runtime)

        calculer_codes(huffmanTree, codes);

        //on a le code de chaque car., on peut donc decoder le texte
        //pour cela, on parcourt le texte codé que l'on coupe en bout dès qu'on reconnait l'un des codes
        //on utilise donc une variable qui stocke le code en cours de lecture

        char code_lu[MAXCARS] = "";
        //MAXCARS = longueur max d'un code (on peut faire mieux avec nb_car_uniques mais taille connue qu'au runtime)

        for(int j = i+1; j<size; j++){ //on reprend le parcours de str (on avait jusqu'ici lu les caractères de 0 jusqu'à i inclu)
            strncat(code_lu, &str[j], 1);

            //on regarde si code_lu est l'un des codes
            for(int k = 0; k <= MAXCARS-1; k++){
                if(huffmanTree[k] != 256){
                    if(strcmp(codes[k], code_lu) == 0){
                        //c'est le cas : on reset code_lu, on print la lettre associée
                        printf("%c", k);
                        sprintf(code_lu, "");
                        break;
                    }
                }
            }
        }
        printf("\n");
    }

    return 0;
}