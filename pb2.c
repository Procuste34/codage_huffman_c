#include "pb2.h"

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

void genere_minimier_viz(unsigned char *tree, int nbElt, char * nom_fichier){
    //ouverture du fichier en w pour écraser tout le contenu
    FILE *fp = fopen(nom_fichier, "w");

    //formatage nécessaire pour graphviz
    fputs("digraph POT_test {\n", fp);
    fputs("node [fontname=\"Arial\", shape=\"circle\", width=0.5];\n", fp);
    fclose(fp); //fermeture du fichier

    //ecrit dans le fichier
    genere_minimier_viz_rec(tree, nbElt, 0, nom_fichier);

    //ouverture en append pour ne pas écraser ce qu'on a déjà écrit
    fp = fopen(nom_fichier, "a");
    fputs("}\n", fp);
    fclose(fp);
}

void genere_minimier_viz_rec(unsigned char *tree, int nbElt, int root, char * nom_fichier){
    assert(tree!=NULL);
	if (!isINTREE(root, nbElt)) return;

    //si le car. est affichable en tant que %c, on l'affiche ainsi, sinon on affiche son code ASCII
    char label[4];
    if(tree[root] <= 127){
        sprintf(label, "%c", tree[root]);
    }else{
        sprintf(label, "%d", tree[root]);
    }

    FILE *fp = fopen(nom_fichier, "a");
	
    char str[2*3+20]; // 2*3 chiffres à écrire + qqs caractères de formatage 
    sprintf(str, "%d [label = \"%s\"]\n", tree[root], label);
    fputs(str, fp);

    //on procède de façon similaire à la fonction showHeap, récursivement

    //on traite le ss-arbre issu du fils gauche (si existe)
    if(isINTREE(iLCHILD(root), nbElt)){
        char str2[2*3+20]; // 2*3 chiffres à écrire + qqs caractères de formatage
        sprintf(str2, "%d:sw -> %d\n", tree[root], tree[iLCHILD(root)]);
        fputs(str2, fp);
    }

    //puis celui issu du fils droit (si existe)
    if(isINTREE(iRCHILD(root), nbElt)){
        char str3[2*3+20]; // 2*3 chiffres à écrire + qqs caractères de formatage
        sprintf(str3, "%d:se -> %d\n", tree[root], tree[iRCHILD(root)]);
        fputs(str3, fp);
    }

    fclose(fp);

	genere_minimier_viz_rec(tree, nbElt, iLCHILD(root), nom_fichier);
	genere_minimier_viz_rec(tree, nbElt, iRCHILD(root), nom_fichier);
}

void genere_arbre_codage_viz(int *tree, int root, char * nom_fichier){
    FILE *fp = fopen(nom_fichier, "w");

    //formatage
    fputs("digraph POT_test {\n", fp);
    fputs("node [fontname=\"Arial\", shape=\"circle\", width=0.5];\n", fp);
    fclose(fp);

    genere_arbre_codage_viz_rec(tree, root, nom_fichier);

    fp = fopen(nom_fichier, "a");
    fputs("}\n", fp);
    fclose(fp);
}

void genere_arbre_codage_viz_rec(int *tree, int root, char * nom_fichier){
    //on cherche les 2 fils de root puis on lance l'appel rec.

    //si le car. est affichable en tant que %c, on l'affiche ainsi, sinon on affiche son code ASCII
    char label[4];
    if(root <= 127){
        sprintf(label, "%c", root);
    }else{
        sprintf(label, "%d", root);
    }

    FILE *fp = fopen(nom_fichier, "a");
	
    char str[2*3+20];
    sprintf(str, "%d [label = \"%s\"]\n", root, label);
    fputs(str, fp);
    fclose(fp);

    //la aussi, on travaille récursivement pour afficher l'arbre de codage
    //on lance l'appel recursif sur le ss-arbre issu du fils gauche, puis sur celui issu du fils droit

    //on parcourt tree pour trouver le fils_gauche et le fils_droit (s'ils existents)

    int fils_gauche = -256;
    int fils_droit = -256;

    for(int i = 0; i<256; i++){
        if(tree[i] == -root){
            //on a trouvé le fils gauche
            fils_gauche = i;

        }
        if(tree[i] == root){
            //on a trouvé le fils droit
            fils_droit = i;
        }
    }

    if(fils_gauche != -256){
        char str2[2*3+30];
        sprintf(str2, "%d:sw -> %d [label = \" 0\"]\n", root, fils_gauche);
        fp = fopen(nom_fichier, "a");
        fputs(str2, fp);

        if(fils_droit != -256){
            char str3[2*3+30];
            sprintf(str3, "%d:se -> %d [label = \" 1\"]\n", root, fils_droit);
            fputs(str3, fp);
        }

        genere_arbre_codage_viz_rec(tree, fils_gauche, nom_fichier);

        if(fils_droit != -256){
            genere_arbre_codage_viz_rec(tree, fils_droit, nom_fichier);
        }

        fclose(fp);
    }
}

void genere_fichier_viz(T_indirectHeap *p, int *tree, int top_noeud_tree, int etape){
    //cette fonction combine les fonctions de génération de viz. du minimier et de l'arbre de codage
    //en plus de générer les fichiers dot, elle génère les fichiers png associés

    //minimier
    char source_fn[50] = ""; //taille max attendue du nom du fichier
    char output_fn[50] = "";

    sprintf(source_fn, "fichiers_viz/minimier_etape_%d.dot", etape);
    sprintf(output_fn, "fichiers_viz/minimier_etape_%d.png", etape);

    genere_minimier_viz(p->tree, p->nbElt, source_fn);

    char cmd[2*50 + 20]; //taille max attendue de la commande
    sprintf(cmd, "dot %s -T png -o %s", source_fn, output_fn);
    system(cmd);

    //arbre de codage
    char source_fn2[50] = "";
    char output_fn2[50] = "";

    sprintf(source_fn2, "fichiers_viz/arbre_codage_etape_%d.dot", etape);
    sprintf(output_fn2, "fichiers_viz/arbre_codage_etape_%d.png", etape);

    genere_arbre_codage_viz(tree, top_noeud_tree, source_fn2);

    char cmd2[2*50 + 20];
    sprintf(cmd2, "dot %s -T png -o %s", source_fn2, output_fn2);
    system(cmd2);
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

    //première visualisation (etape 0)
    genere_fichier_viz(ih, huffmanTree, 0, 0);

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

        //visualisation (post etape i+1)
        genere_fichier_viz(ih, huffmanTree, n, i+1);
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