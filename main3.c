#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "include/traces.h"
#include "include/check.h"

typedef struct{
    char *parcours_prefixe;
    char *caracteres;
} T_entete;

//todo : renommer les fonctions (parcours_rec)
//passer en minimier

#define MAXCARS 128
#define MAX_STRING_LENGTH 500

#define HEAP_ALLOCATION_OFFSET 5

#define iPARENT(i) 			(i-1)/2
#define iLCHILD(i) 			(2*i)+1
#define iRCHILD(i) 			(2*i)+2
#define iLASTINTERNAL(n)	n/2 -1
#define isINTERNAL(i,n) 	(2*i<(n-1))
#define isLEAF(i,n) 			(2*i>=(n-1))
#define isINTREE(i,n)		(i<n)
#define isROOT(i)				(i==0)
#define nbINTERNALS(n) 		n/2
#define nbLEAVES(n) 			ceil((double)n/2)
#define VALP(pHeap, i)		pHeap->tree[i]		
#define VAL(heap, i)			heap.tree[i]

#define VALP_ih(pHeap, i)		pHeap->data[pHeap->tree[i]]		
#define VAL_ih(heap, i)			heap.data[heap.tree[i]]	

typedef struct {
	unsigned int nbElt;
	unsigned char * tree;
	int * data;
} T_indirectHeap;

void reverse_string(char* str);
int comparer(int a, int b);

T_indirectHeap * newHeap();
void buildHeap(T_indirectHeap *p);
void siftDown(T_indirectHeap *p, int k);
void siftUp(T_indirectHeap *p, int k);
void swap(T_indirectHeap *p, int i, int j);
void showHeap(T_indirectHeap *p);
void showHeap_rec(T_indirectHeap *p, int root, int indent);
unsigned char removeMax(T_indirectHeap *p);

T_entete * parcours_rec(int *tree, int root, T_entete *entete);

void parcours2(int *tree, char *parcours_prefixe, char *caracteres);
void parcours2_rec(int *tree, char *parcours_prefixe, char *caracteres, int appel_fg, int *j, int *i, int *compteur_car, int pere);

int main(int argc, char *argv[]){
    int compressage = 0;

    if(argc==2){
        printf("décompressage\n");
        //printf("fichier à décompresser : %s\n", argv[1]);
    }
    if(argc==3){
        compressage = 1;
        printf("compressage\n");
        //printf("fichier à compresser : %s\n", argv[1]);
        //printf("fichier target : %s\n", argv[2]);
    }
    if(argc != 2 && argc != 3){
        printf("erreur\n");
        return 0.;
    }

    if(compressage){
        //todo : faire des checks d'erreur (comme openai) si jamais fichier pas ok...

        FILE *fp = fopen(argv[1], "r");

        fseek(fp, 0, SEEK_END); //on se déplace à la fin du fichier pour en connaitre sa taille
        long size = ftell(fp); //on recupere la taille du fichier
        char *str = malloc(size + 1); //todo : checkif

        fseek(fp, 0, SEEK_SET); //on revient en début de fichier

        fread(str, 1, size, fp); //on lit le fichier en entier
        
        str[size] = '\0';

        fclose(fp);

        ////// go :

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
        char *str = malloc(size + 1); //todo : checkif

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

        //printf("VOICI MON PARCOURS PREFIXE : %s\n", entete->parcours_prefixe);
        //printf("VOICI MON CARACTERES : %s\n", entete->caracteres);

        //on a l'entete, on peut donc en déduire huffmanTree
        int huffmanTree[256];
        for(int i = 0; i < 256; i++){
            huffmanTree[i] = -256;
        }

        //ici
        parcours2(huffmanTree, entete->parcours_prefixe, entete->caracteres);

        //printf("****** HUFFMAN TREE ****\n");
        //for(int i = 0; i < 256; i++){
        //    if(huffmanTree[i] != -256){
        //        printf("%d : %d\n", i, huffmanTree[i]);
        //    }
        //}
        //printf("*********************\n");

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

void reverse_string(char* str){
    //on parcourt la string de gauche à droite et de droite à gauche en même temps, jusqu'à arriver au milieu

    int length = strlen(str);
    int i;
    for (i = 0; i < length / 2; i++){
        char temp = str[i];
        str[i] = str[length - i - 1];
        str[length - i - 1] = temp;
    }
}

int comparer(int a, int b){
	return a-b;
}

T_indirectHeap * newHeap(){
	T_indirectHeap * pAux;

	CHECK_IF(pAux = malloc(sizeof(T_indirectHeap)), NULL, "erreur malloc");
	CHECK_IF(pAux->tree = malloc(MAXCARS * sizeof(unsigned char)), NULL, "erreur malloc");
	CHECK_IF(pAux->data = malloc((2*MAXCARS-1) * sizeof(int)), NULL, "erreur malloc");

	pAux->nbElt = 0;
	
	return pAux;
}

void buildHeap(T_indirectHeap * p){
	int k;
	int n;
	assert(p!=NULL);
	n = p->nbElt; 
	for(k=iLASTINTERNAL(n); k>=0; k--) {
		siftDown(p, k);
	}
}

void siftDown(T_indirectHeap *p, int k){
	int n; 
	assert(p!=NULL);
	n = p->nbElt; 
	int i; 
	if (!isINTREE(k,n)) return; 
	 
	while ( ! isLEAF(k,n) ) {
		if (isINTREE(iRCHILD(k),n) && (comparer(VALP_ih(p, iRCHILD(k)), VALP_ih(p, iLCHILD(k))) > 0)) i = iRCHILD(k); 
		else i = iLCHILD(k); 
		
		if (comparer(VALP_ih(p,k), VALP_ih(p,i)) < 0) {
			swap(p, k, i);
			k = i; 
		} else break;  
	}
}

void siftUp(T_indirectHeap *p, int k) {
	assert(p!=NULL);
	while ( !isROOT(k) && (comparer(VALP_ih(p, k),VALP_ih(p, iPARENT(k))) > 0 ) ) {
		swap(p, k, iPARENT(k)); 
		k = iPARENT(k); 
	}
}

void swap(T_indirectHeap *p, int i, int j) {
	unsigned char aux; 
	assert(p!=NULL);
	aux = p->tree[i]; 
	p->tree[i] = p->tree[j];
	p->tree[j] = aux;
}

void showHeap(T_indirectHeap *p) {
	assert(p!=NULL);
	printf("Affichage du tas (nbElt : %d)\n",p->nbElt);
	showHeap_rec(p, 0, 0); 
}

void showHeap_rec(T_indirectHeap *p, int root, int indent) {
	// affiche récursivement en mode pseudo graphique 
	// ordre de récurrence  : taille du sous-arbre

	int i; 
	assert(p!=NULL);
	if (! isINTREE(root,p->nbElt)) return;  
	
	showHeap_rec(p, iRCHILD(root), indent+1);
	for(i=0;i<indent;i++) {
		printf("\t"); 
	}
	printf("%c(i:%d)\n", VALP(p, root), root);
	showHeap_rec(p, iLCHILD(root), indent+1);
}

unsigned char removeMax(T_indirectHeap *p) {
	unsigned char aux;
	assert(p!=NULL);
	aux = p->tree[0];
	swap(p, 0, p->nbElt-1);
	p->nbElt--;
	siftDown(p, 0);
	return aux;
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

void parcours2(int *tree, char *parcours_prefixe, char *caracteres){
    int j_base = 128;
    int i_base = 0;
    int compteur_car_base = 0;

    int *j = &j_base;
    int *i = &i_base;
    int *compteur_car = &compteur_car_base;
    parcours2_rec(tree, parcours_prefixe, caracteres, 0, j, i, compteur_car, -1);
}


void parcours2_rec(int *tree, char *parcours_prefixe, char *caracteres, int appel_fg, int *j, int *i, int *compteur_car, int pere){
    if(parcours_prefixe[*i] == '0'){ //noeud interne
        if(pere != -1){
            if(appel_fg){
                tree[*j] = -pere;
            }else{
                tree[*j] = +pere;
            }
        }

        (*i)++;
        //appel rec. a gauche
        int pere = *j;
        (*j)++;

        //tree[*j] = -pere;
        parcours2_rec(tree, parcours_prefixe, caracteres, 1, j, i, compteur_car, pere);

        //appel rec. à droite
        //tree[*j] = +pere;
        parcours2_rec(tree, parcours_prefixe, caracteres, 0, j, i, compteur_car, pere);

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
