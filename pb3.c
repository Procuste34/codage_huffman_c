#include "pb3.h"

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

T_indirectHeap * creer_tas(char *str, int *nb_car_uniques){
    //création du tas indirect
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

    *nb_car_uniques = compteur_char_uniques;
    ih->nbElt = *nb_car_uniques;

    //construction du minimier à partir du tas
    //c'est un minimier indirect : les noeuds sont bien les caractères mais ils sont triés selon leur occurences
	buildHeap(ih);

    return ih;
}

void construit_arbre_codage(int *huffmanTree, T_indirectHeap *ih, int nb_car_uniques){
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
}

void calculer_codes(int *huffmanTree, T_indirectHeap *ih, int *occurences, int *longueurs, char codes[][MAXCARS]){
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

            occurences[i] = ih->data[i];
            longueurs[i] = strlen(code_car);
            strcpy(codes[i], code_car);
        }
    }
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

        parcours2_rec(tree, parcours_prefixe, caracteres, 1, j, i, compteur_car, pere);

        //appel rec. à droite
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
