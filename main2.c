#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h> 
// ceil, floor : #include <math.h>

#include "include/traces.h" 
#include "include/check.h" 

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

#define MAXCARS 128

typedef struct {
	unsigned int nbElt;
	unsigned char * tree; // todo : j'ai mis * au lieu d'un truc [MAXCARS] ... preconise dans le sujet
	int * data;
} T_indirectHeap;


char *strrev(char * str);

int comparer(int a, int b);
T_indirectHeap * newHeapV3(unsigned int nbMaxElt);
void buildHeapV3(T_indirectHeap * p);
void siftDownV3(T_indirectHeap *p, int k);
void swapV3(T_indirectHeap *p, int i, int j);
void showHeapV3(T_indirectHeap *p);
void showHeap_recV3(T_indirectHeap *p, int root, int indent);
unsigned char removeMaxV3(T_indirectHeap *p);
void siftUpV3(T_indirectHeap *p, int k);

void genere_minimier_viz(unsigned char *tree, int nbElt, char * nom_fichier);
void genere_minimier_viz_rec(unsigned char *tree, int nbElt, int root, char * nom_fichier);

void genere_arbre_codage_viz(int *tree, int root, char * nom_fichier);
void genere_arbre_codage_viz_rec(int *tree, int root, char * nom_fichier);

//todo : foutre les fichiers generer dans un ss-dossier...
//revoir les todo

int main(void){
	printf("******************************************\n");

    //char ma_string[] = "ABBACADABRA";
    //char ma_string[] = "algorithme de huffman pour la compression de chaines";

    char str[500]; //todo : CS
    char *ma_string;
    printf("Veuillez tapper un texte :");
    fgets(str, 500, stdin);
    ma_string = strtok(str, "\n");

    T_indirectHeap * ih = newHeapV3(5*MAXCARS); //todo, arbitraire
    int compteur_char_uniques = 0;

    for (int i = 0; i < MAXCARS; i++)
	{
		ih->tree[i] = 0;
	}

    for (int i = 0; i < 2*MAXCARS-1; i++)
	{
		ih->data[i] = 0;
	}

    for(int i = 0; i < strlen(ma_string); i++){
        ih->data[(int)ma_string[i]]--;

        if(ih->data[(int)ma_string[i]] == -1){
            ih->tree[compteur_char_uniques] = ma_string[i];
            compteur_char_uniques++;
        }
    }
	
    int nb_char_uniques = compteur_char_uniques;
    ih->nbElt = nb_char_uniques;

	buildHeapV3(ih); // fait un maximier mais en prenant compte des occ. et non du code ASCII!!! donc c parfait
    //for(int i = 0; i<ih->nbElt; i++){
    //    printf("%d\n", ih->tree[i]);
    //}
    //showHeapV3(ih);

    int huffmanTree [2*MAXCARS-1];
    for(int i = 0; i < 2*MAXCARS-1; i++){
        huffmanTree[i] = -256;
    }

    //TODO : tout mettre dans une fonction ou on donne juste le X (mais a mettre avec l'arbre de codage)
    //minimier
    char source_fn[50] = "minimier_etape_0.dot";
    char output_fn[50] = "minimier_etape_0.png";

    genere_minimier_viz(ih->tree, ih->nbElt, source_fn);

    char cmd[100];
    sprintf(cmd, "dot %s -T png -o %s", source_fn, output_fn);
    system(cmd);

    //arbre de codage
    char source_fn2[50] = "arbre_codage_etape_0.dot";
    char output_fn2[50] = "arbre_codage_etape_0.png";

    genere_arbre_codage_viz(huffmanTree, 0, source_fn2);

    char cmd2[100];
    sprintf(cmd2, "dot %s -T png -o %s", source_fn2, output_fn2);
    system(cmd2);

    for(int i = 0; i < nb_char_uniques-1; i++){
        //1ere extraction
        unsigned char elt1 = removeMaxV3(ih);
        //showHeapV3(ih);

        //printf("retire :%d\n", elt1);
        int occ_elt1 = ih->data[elt1];

        //2eme extraction
        unsigned char elt2 = removeMaxV3(ih);
        //showHeapV3(ih);

        //printf("retire : %d\n", elt2);
        int occ_elt2 = ih->data[elt2];

        //insertion de 128 et d'occurence occ_elt1+occ_elt2
        //donc ce qu'il faut faire : mettre à jour data : data[128] + occ_elt1+occ_elt2
        //                           inserer 128 dans le tas (on sait pas comment faire)

        ih->data[128+i] = occ_elt1+occ_elt2;
        ih->tree[ih->nbElt] = 128+i;
        ih->nbElt++;
        buildHeapV3(ih);
        //showHeapV3(ih);

        //huffman tree / arbre de codage : comment l'implémenter ? tableau de 255 cases
        huffmanTree[elt1] = -(128+i);
        huffmanTree[elt2] = +(128+i); //todo : variabler ce 128+i

        //TODO : tout mettre dans une fonction ou on donne juste le X (mais a mettre avec l'arbre de codage)
        //minimier
        char source_fn[50] = "";
        char output_fn[50] = "";

        sprintf(source_fn, "minimier_etape_%d.dot", i+1);
        sprintf(output_fn, "minimier_etape_%d.png", i+1);

        genere_minimier_viz(ih->tree, ih->nbElt, source_fn);

        char cmd[100];
        sprintf(cmd, "dot %s -T png -o %s", source_fn, output_fn);
        system(cmd);

        //arbre de codage
        char source_fn2[50] = "";
        char output_fn2[50] = "";

        sprintf(source_fn2, "arbre_codage_etape_%d.dot", i+1);
        sprintf(output_fn2, "arbre_codage_etape_%d.png", i+1);

        genere_arbre_codage_viz(huffmanTree, 128+i, source_fn2);

        char cmd2[100];
        sprintf(cmd2, "dot %s -T png -o %s", source_fn2, output_fn2);
        system(cmd2);
    }

    int occurences[127];
    int longueurs[127];
    char codes[127][10]; // todo : 10 : longueur max du code d'un char... a voir

    char c0 = '0';
    char c1 = '1';
    
    //calcul du code des chars.
    for(int i = 0; i <= 127; i++){
        if(huffmanTree[i] != -256){
            char code_car[10] = ""; // todo : 10 : longueur max du code d'un char... a voir

            int index = i;
            while(huffmanTree[index] != -256){

                if(huffmanTree[index] < 0){
                    strncat(code_car, &c0, 1);
                }else {
                    strncat(code_car, &c1, 1);
                }

                index = abs(huffmanTree[index]);    
            }

            strrev(code_car); //todo : faire un truc plus simple ?

            occurences[i] = ih->data[i];
            longueurs[i] = strlen(code_car);
            strcpy(codes[i], code_car);
        }
    }

    //affichage du tableau des codes
    printf("car : occ | long | bits\n");
    printf("----+-----+------+---------\n");

    for(int i = 0; i <= 127; i++){
        if(huffmanTree[i] != -256){
            printf("'%c' :   %d |    %d | %s\n", i, -occurences[i], longueurs[i], codes[i]);
        }
    }

    //print le code de la string d'entrée ma_string (ABBACADABRA)

    int len_code_huffman = 0;
    for(int i = 0; i <= strlen(ma_string)-1; i++){
        printf("%s", codes[(int)ma_string[i]]);

        len_code_huffman += longueurs[(int)ma_string[i]];
    }
    printf("\n");

    //print la conclusion

    //longueur du code binaire :
    int len_code_binaire = strlen(ma_string) * 8;

    //ratio de compression : longueur du code de huffman / longueur du code binaire
    float ratio_compression = 100 * (float) len_code_huffman / (float) len_code_binaire;

    printf("Longueur du code binaire : %d\n", len_code_binaire);
    printf("Longueur du code de huffman : %d\n", len_code_huffman);
    printf("Ratio de compression : %.2f%%\n", ratio_compression);
    
    return 0;
}

char *strrev(char * str){
    char *p1, *p2;

    if(! str || ! *str){
        return str;
    }

    for(p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
    {
        *p1 ^= *p2;
        *p2 ^= *p1;
        *p1 ^= *p2;
    }
    return str;
}

int comparer(int a, int b){
	return a-b;
}

T_indirectHeap * newHeapV3(unsigned int nbMaxElt){
	T_indirectHeap * pAux;

	CHECK_IF(pAux = malloc(sizeof(T_indirectHeap)), NULL, "erreur malloc");
	CHECK_IF(pAux->tree = malloc(nbMaxElt * sizeof(unsigned char)), NULL, "erreur malloc");
	CHECK_IF(pAux->data = malloc(nbMaxElt * sizeof(int)), NULL, "erreur malloc");

	pAux->nbElt = 0;
	
	return pAux;
}


void buildHeapV3(T_indirectHeap * p){
	int k;
	int n;
	assert(p!=NULL);
	n = p->nbElt; 
	for(k=iLASTINTERNAL(n); k>=0; k--) {
		siftDownV3(p, k);
	}
}

void siftDownV3(T_indirectHeap *p, int k){
	int n; 
	assert(p!=NULL);
	n = p->nbElt; 
	int i; 
	if (!isINTREE(k,n)) return; 
	 
	while ( ! isLEAF(k,n) ) {
		if (isINTREE(iRCHILD(k),n) && (comparer(VALP_ih(p, iRCHILD(k)), VALP_ih(p,iLCHILD(k))) > 0)) i = iRCHILD(k); 
		else i = iLCHILD(k); 
		
		if (comparer(VALP_ih(p,k), VALP_ih(p,i)) < 0) {
			swapV3(p, k, i);
			k = i; 
		} else break;  
	}
}

void swapV3(T_indirectHeap *p, int i, int j) {
	unsigned char aux; 
	assert(p!=NULL);
	aux = p->tree[i]; 
	p->tree[i] = p->tree[j];
	p->tree[j] = aux;
}

void showHeapV3(T_indirectHeap *p) {
	assert(p!=NULL);
	printf("Affichage du tas (nbElt : %d)\n",p->nbElt);
	showHeap_recV3(p, 0, 0); 
}

void showHeap_recV3(T_indirectHeap *p, int root, int indent) {
	// affiche récursivement en mode pseudo graphique 
	// ordre de récurrence  : taille du sous-arbre 
	int i; 
	assert(p!=NULL);
	if (! isINTREE(root,p->nbElt)) return;  
	
	showHeap_recV3(p, iRCHILD(root), indent+1);
	for(i=0;i<indent;i++) {
		printf("\t"); 
	}
	printf("%c(i:%d)\n", VALP(p, root), root);
	showHeap_recV3(p, iLCHILD(root), indent+1);
}


unsigned char removeMaxV3(T_indirectHeap *p) {
	unsigned char aux;
	assert(p!=NULL);
	aux = p->tree[0];
	swapV3(p, 0, p->nbElt-1);
	p->nbElt--;
	siftDownV3(p, 0);
	return aux;
}

void siftUpV3(T_indirectHeap *p, int k) {
	assert(p!=NULL);
	while ( !isROOT(k) && (comparer(VALP_ih(p,k),VALP_ih(p,iPARENT(k))) > 0 ) ) {
		swapV3(p,k,iPARENT(k)); 
		k = iPARENT(k); 
	}
}

void genere_minimier_viz(unsigned char *tree, int nbElt, char * nom_fichier){
    FILE *fp = fopen(nom_fichier, "w");

    fputs("digraph POT_test {\n", fp);
    fputs("node [fontname=\"Arial\", shape=\"circle\", width=0.5];\n", fp);
    fclose(fp);

    genere_minimier_viz_rec(tree, nbElt, 0, nom_fichier);

    fp = fopen(nom_fichier, "a");
    fputs("}\n", fp);
    fclose(fp);
}

void genere_minimier_viz_rec(unsigned char *tree, int nbElt, int root, char * nom_fichier){
    assert(tree!=NULL); //todo : remove
	if (!isINTREE(root, nbElt)) return;

    char label[4];
    if(tree[root] <= 127){
        sprintf(label, "%c", tree[root]);
    }else{
        sprintf(label, "%d", tree[root]);
    }

    FILE *fp = fopen(nom_fichier, "a");
	
    char str[100];
    sprintf(str, "%d [label = \"%s\"]\n", tree[root], label);
    fputs(str, fp);

    if(isINTREE(iLCHILD(root), nbElt)){
        char str2[100];
        sprintf(str2, "%d:sw -> %d\n", tree[root], tree[iLCHILD(root)]);
        fputs(str2, fp);
    }
    if(isINTREE(iRCHILD(root), nbElt)){
        char str3[100];
        sprintf(str3, "%d:se -> %d\n", tree[root], tree[iRCHILD(root)]);
        fputs(str3, fp);
    }

    fclose(fp);

	genere_minimier_viz_rec(tree, nbElt, iLCHILD(root), nom_fichier);
	genere_minimier_viz_rec(tree, nbElt, iRCHILD(root), nom_fichier);
}

void genere_arbre_codage_viz(int *tree, int root, char * nom_fichier){
    FILE *fp = fopen(nom_fichier, "w");

    fputs("digraph POT_test {\n", fp);
    fputs("node [fontname=\"Arial\", shape=\"circle\", width=0.5];\n", fp);
    fclose(fp);

    genere_arbre_codage_viz_rec(tree, root, nom_fichier);

    fp = fopen(nom_fichier, "a");
    fputs("}\n", fp);
    fclose(fp);
}

void genere_arbre_codage_viz_rec(int *tree, int root, char * nom_fichier){
    //on cherche les 2 fils de root puis lancer l'appel rec.

    char label[4];
    if(root <= 127){
        sprintf(label, "%c", root);
    }else{
        sprintf(label, "%d", root);
    }

    FILE *fp = fopen(nom_fichier, "a");
	
    char str[100];
    sprintf(str, "%d [label = \"%s\"]\n", root, label);
    fputs(str, fp);
    fclose(fp);

    int fils_gauche = -256;
    int fils_droite = -256;

    for(int i = 0; i<256; i++){
        if(tree[i] == -root){
            //on a trouvé le fils gauche
            fils_gauche = i;

        }
        if(tree[i] == root){
            //on a trouvé le fils droite
            fils_droite = i;
        }
    }

    if(fils_gauche != -256){
        char str2[100];
        sprintf(str2, "%d:sw -> %d [label = \" 0\"]\n", root, fils_gauche);
        fp = fopen(nom_fichier, "a");
        fputs(str2, fp);

        if(fils_droite != -256){
            char str3[100];
            sprintf(str3, "%d:se -> %d [label = \" 1\"]\n", root, fils_droite);
            fputs(str3, fp);
        }

        genere_arbre_codage_viz_rec(tree, fils_gauche, nom_fichier);

        if(fils_droite != -256){
            genere_arbre_codage_viz_rec(tree, fils_droite, nom_fichier);
        }

        fclose(fp);
    }
}