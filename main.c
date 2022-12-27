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
	unsigned char * tree; //avant : unsigned char // change... a voir
	int * data;
} T_indirectHeap;


int comparer(int a, int b);
T_indirectHeap * newHeapV3(unsigned int nbMaxElt);
void buildHeapV3(T_indirectHeap * p);
void siftDownV3(T_indirectHeap *p, int k);
void swapV3(T_indirectHeap *p, int i, int j);
void showHeapV3(T_indirectHeap *p);
void showHeap_recV3(T_indirectHeap *p, int root, int indent);
unsigned char removeMaxV3(T_indirectHeap *p);
void siftUpV3(T_indirectHeap *p, int k);

int main(void){
	printf("******************************************\n");
    
	T_indirectHeap * ih = newHeapV3(5*MAXCARS); //todo, arbitraire
    ih->nbElt = 5;	

	for (int i = 0; i < MAXCARS; i++)
	{
		ih->tree[i] = 0;
	}
	ih->tree[0] = 65;
	ih->tree[1] = 66;
	ih->tree[2] = 67;
	ih->tree[3] = 68;
	ih->tree[4] = 82;

	for (int i = 0; i < 2*MAXCARS-1; i++)
	{
		ih->data[i] = 0;
	}
	ih->data[65] = -5;
	ih->data[66] = -3;
	ih->data[67] = -1;
	ih->data[68] = -1;
	ih->data[82] = -1;
    

	buildHeapV3(ih); // fait un maximier mais en prenant compte des occ. et non du code ASCII!!! donc c parfait
    
    showHeapV3(ih);

    
	//todo : finir de reimplementer en V3 (indirect heap), a voir quelles fonctions reimplemnter
	//continuer l'exemple ABRACADRA a la main ici dans le main (retirer les noeuds, ajouter un noeud etc etc) (fonctions pour retirer, inserer etc)
	//gerer le huffmantree, ie l'arbre de codage (liste des -256... ca a l'air ok)

    
    // il y a 4 passes à faire
    // ********************************** 1ERE PASSE **************************************
    printf("*********************************** 1RE PASSE ********************************\n");

	//1ere extraction
	unsigned char elt1 = removeMaxV3(ih);
	showHeapV3(ih);

	printf("retire :%d\n", elt1);
	int occ_elt1 = ih->data[elt1];

	//2eme extraction
	unsigned char elt2 = removeMaxV3(ih);
	showHeapV3(ih);

	printf("retire : %d\n", elt2);
    int occ_elt2 = ih->data[elt2];

	//insertion de 128 et d'occurence occ_elt1+occ_elt2
    //donc ce qu'il faut faire : mettre à jour data : data[128] + occ_elt1+occ_elt2
    //                           inserer 128 dans le tas (on sait pas comment faire)

    ih->data[128] = occ_elt1+occ_elt2;
    ih->tree[ih->nbElt] = 128;
    ih->nbElt++;
    buildHeapV3(ih);
    showHeapV3(ih);


    //huffman tree / arbre de codage : comment l'implémenter ? tableau de 255 cases
    int huffmanTree [2*MAXCARS-1];
    huffmanTree[elt1] = -128;
    huffmanTree[elt2] = +128;

    // ********************************** 2EME PASSE **************************************
    printf("*********************************** 2EME PASSE ********************************\n");

	//1ere extraction
	unsigned char elt3 = removeMaxV3(ih);
	showHeapV3(ih);

	printf("retire :%d\n", elt3);
	int occ_elt3 = ih->data[elt3];

	//2eme extraction
	unsigned char elt4 = removeMaxV3(ih);
	showHeapV3(ih);

	printf("retire : %d\n", elt4);
    int occ_elt4 = ih->data[elt4];

	//insertion de 128 et d'occurence occ_elt1+occ_elt2
    //donc ce qu'il faut faire : mettre à jour data : data[128] + occ_elt1+occ_elt2
    //                           inserer 128 dans le tas (on sait pas comment faire)

    ih->data[129] = occ_elt3+occ_elt4;
    ih->tree[ih->nbElt] = 129;
    ih->nbElt++;
    buildHeapV3(ih);
    showHeapV3(ih);


    //huffman tree / arbre de codage : comment l'implémenter ? tableau de 255 cases
    huffmanTree[elt3] = -129;
    huffmanTree[elt4] = +129;

    // ********************************** 3EME PASSE **************************************
    printf("*********************************** 3EME PASSE ********************************\n");

	//1ere extraction
	unsigned char elt5 = removeMaxV3(ih);
	showHeapV3(ih);

	printf("retire :%d\n", elt5);
	int occ_elt5 = ih->data[elt5];

	//2eme extraction
	unsigned char elt6 = removeMaxV3(ih);
	showHeapV3(ih);

	printf("retire : %d\n", elt6);
    int occ_elt6 = ih->data[elt6];

	//insertion de 128 et d'occurence occ_elt1+occ_elt2
    //donc ce qu'il faut faire : mettre à jour data : data[128] + occ_elt1+occ_elt2
    //                           inserer 128 dans le tas (on sait pas comment faire)

    ih->data[130] = occ_elt5+occ_elt6;
    ih->tree[ih->nbElt] = 130;
    ih->nbElt++;
    buildHeapV3(ih);
    showHeapV3(ih);


    //huffman tree / arbre de codage : comment l'implémenter ? tableau de 255 cases
    huffmanTree[elt5] = -130;
    huffmanTree[elt6] = +130;

    // ********************************** 4EME PASSE **************************************
    printf("*********************************** 4EME PASSE ********************************\n");

	//1ere extraction
	unsigned char elt7 = removeMaxV3(ih);
	showHeapV3(ih);

	printf("retire :%d\n", elt7);
	int occ_elt7 = ih->data[elt7];

	//2eme extraction
	unsigned char elt8 = removeMaxV3(ih);
	showHeapV3(ih);

	printf("retire : %d\n", elt8);
    int occ_elt8 = ih->data[elt8];

	//insertion de 128 et d'occurence occ_elt1+occ_elt2
    //donc ce qu'il faut faire : mettre à jour data : data[128] + occ_elt1+occ_elt2
    //                           inserer 128 dans le tas (on sait pas comment faire)

    ih->data[131] = occ_elt7+occ_elt8;
    ih->tree[ih->nbElt] = 131;
    ih->nbElt++;
    buildHeapV3(ih);
    showHeapV3(ih);


    //huffman tree / arbre de codage : comment l'implémenter ? tableau de 255 cases
    huffmanTree[elt7] = -131;
    huffmanTree[elt8] = +131;

    // check de huffmanTree : OK
    printf("%d\n", huffmanTree[65]);
    printf("%d\n", huffmanTree[66]);
    printf("%d\n", huffmanTree[67]);
    printf("%d\n", huffmanTree[68]);
    printf("%d\n", huffmanTree[82]);
    printf("%d\n", huffmanTree[128]);
    printf("%d\n", huffmanTree[129]);
    printf("%d\n", huffmanTree[130]);	
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







