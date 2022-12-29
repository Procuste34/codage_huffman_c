#include <stdio.h>

#include "pb3.h"

//todo : renommer les fonctions (parcours_rec)

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
        //todo : faire des checks d'erreur (comme openai) si jamais fichier pas ok...

        FILE *fp = fopen(argv[1], "r");

        fseek(fp, 0, SEEK_END); //on se déplace à la fin du fichier pour en connaitre sa taille
        long size = ftell(fp); //on recupere la taille du fichier
        char *str;
        CHECK_IF(str = malloc(size + 1), NULL, "erreur malloc");

        fseek(fp, 0, SEEK_SET); //on revient en début de fichier

        fread(str, 1, size, fp); //on lit le fichier en entier
        
        str[size] = '\0';

        fclose(fp);

        T_indirectHeap * ih = newHeap();
    
        //initialisation de tree (le tas) et de data (occurences)
        for (int i = 0; i < MAXCARS; i++) ih->tree[i] = 0;
        for (int i = 0; i < 2*MAXCARS-1; i++) ih->data[i] = 0;

        //comptage des occurences
        int compteur_char_uniques = 0;
        for(int i = 0; i < strlen(str); i++){
            ih->data[(int) str[i]]--; //maximier truqué en minimier...

            //ajout des caractères uniques au tas
            if(ih->data[(int) str[i]] == -1){
                ih->tree[compteur_char_uniques] = str[i];
                compteur_char_uniques++;
            }
        }
        
        int nb_car_uniques = compteur_char_uniques;
        ih->nbElt = nb_car_uniques;

        //construction du minimier à partir du tas
        //c'est un minimier indirect : les noeuds sont bien les caractères mais ils sont triés selon leur occurences
        buildHeap(ih);

        //création de l'arbre de codage
        int huffmanTree [2*MAXCARS-1];
        for(int i = 0; i < 2*MAXCARS-1; i++) huffmanTree[i] = -256;

        for(int i = 0; i < nb_car_uniques-1; i++){
            //1ere extraction
            unsigned char elt1 = removeMax(ih);
            int occ_elt1 = ih->data[elt1];

            //2eme extraction
            unsigned char elt2 = removeMax(ih);
            int occ_elt2 = ih->data[elt2];

            int n = 128+i;
            //mettre à jour data[n] à occ_elt1+occ_elt2
            ih->data[n] = occ_elt1+occ_elt2;

            //inserer n dans le tas
            ih->tree[ih->nbElt] = n;
            ih->nbElt++;

            //on reconstruit le tas pour avoir un maximier
            buildHeap(ih);

            //on met alors à jour l'arbre de codage
            huffmanTree[elt1] = -n;
            huffmanTree[elt2] = n;
        }

        //l'arbre de codage est alors construit

        char codes[MAXCARS][MAXCARS]; //stocke le code de chaque car.
        //MAXCARS = longueur max d'un code (on peut faire mieux avec nb_car_uniques mais taille connue qu'au runtime)

        char c0 = '0';
        char c1 = '1';
        
        //calcul du code de chacun des caractères ayant une occ > 0
        //pour chaque car., on remonte l'arbre tant qu'on rencontre un noeud (différent de -256)
        //il faudra enfin renverse la string obtenue pour avoir le code du car.
        for(int i = 0; i <= 127; i++){
            if(huffmanTree[i] != -256){
                char code_car[MAXCARS] = ""; //MAXCARS = longueur max d'un code

                int index = i;
                while(huffmanTree[index] != -256){

                    if(huffmanTree[index] < 0){
                        strncat(code_car, &c0, 1); //strncat set à ajoute un car. à la string code_car
                    }else {
                        strncat(code_car, &c1, 1);
                    }

                    index = abs(huffmanTree[index]);    
                }

                reverse_string(code_car);

                strcpy(codes[i], code_car);
            }
        }

        //ecrire l'entete
        T_entete *entete;

        CHECK_IF(entete = malloc(sizeof(T_entete)), NULL, "erreur malloc");
        CHECK_IF(entete->parcours_prefixe = malloc(100 * sizeof(char)), NULL, "erreur malloc");
        CHECK_IF(entete->caracteres = malloc(100 * sizeof(char)), NULL, "erreur malloc");

        entete = parcours_rec(huffmanTree, 128+nb_car_uniques-2, entete);

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

        T_entete *entete;

        CHECK_IF(entete = malloc(sizeof(T_entete)), NULL, "erreur malloc");
        CHECK_IF(entete->parcours_prefixe = malloc(100 * sizeof(char)), NULL, "erreur malloc"); //todo : arbitraire
        CHECK_IF(entete->caracteres = malloc(100 * sizeof(char)), NULL, "erreur malloc");

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
        int huffmanTree[256];
        for(int i = 0; i < 256; i++){
            huffmanTree[i] = -256;
        }

        parcours2(huffmanTree, entete->parcours_prefixe, entete->caracteres);

        char codes[MAXCARS][MAXCARS]; //stocke le code de chaque car.
        //MAXCARS = longueur max d'un code (on peut faire mieux avec nb_car_uniques mais taille connue qu'au runtime)

        char c0 = '0';
        char c1 = '1';

        //calcul du code de chacun des caractères ayant une occ > 0
        //pour chaque car., on remonte l'arbre tant qu'on rencontre un noeud (différent de -256)
        //il faudra enfin renverse la string obtenue pour avoir le code du car.
        for(int i = 0; i <= 127; i++){
            if(huffmanTree[i] != -256){
                char code_car[MAXCARS] = ""; //MAXCARS = longueur max d'un code

                int index = i;
                while(huffmanTree[index] != -256){

                    if(huffmanTree[index] < 0){
                        strncat(code_car, &c0, 1); //strncat set à ajoute un car. à la string code_car
                    }else {
                        strncat(code_car, &c1, 1);
                    }

                    index = abs(huffmanTree[index]);    
                }

                reverse_string(code_car);

                strcpy(codes[i], code_car);
            }
        }

        //on a le code de chaque car., on peut donc decoder le texte
        //pour cela, on parcourt le texte codé que l'on coupe en bout dès qu'on reconnait l'un des codes
        //on utilise donc une variable qui stocke le code en cours de lecture
        char code_lu[100] = "";
        for(int j = i+1; j<size; j++){ //on reprend le parcours de str
            strncat(code_lu, &str[j], 1);

            //on regarde si code_lu est l'un des codes
            for(int k = 0; k <= 127; k++){
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