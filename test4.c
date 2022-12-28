#include <stdio.h>
#include <assert.h>

#define iLCHILD(i) 			(2*i)+1
#define iRCHILD(i) 			(2*i)+2
#define isINTREE(i,n)		(i<n)

void genere_minimier_viz(int *tree, int nbElt, char * nom_fichier);
void genere_minimier_viz_rec(int *tree, int nbElt, int root, char * nom_fichier);

int main(void){
    int p[] = {68, 82, 67, 66, 65};
    int nbElt = 5;

    genere_minimier_viz(p, nbElt, "1.dot");

    return 0.;
}

void genere_minimier_viz(int *tree, int nbElt, char * nom_fichier){
    FILE *fp = fopen(nom_fichier, "w");

    fputs("digraph POT_test {\n", fp);
    fputs("node [fontname=\"Arial\", shape=\"circle\", width=0.5];\n", fp);
    fclose(fp);

    genere_minimier_viz_rec(tree, nbElt, 0, nom_fichier);

    fp = fopen(nom_fichier, "a");
    fputs("}\n", fp);
    fclose(fp);
}

void genere_minimier_viz_rec(int *tree, int nbElt, int root, char * nom_fichier){
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
        sprintf(str2, "%d:sw -> %d [label = \" 0\"]\n", tree[root], tree[iLCHILD(root)]);
        fputs(str2, fp);
    }
    if(isINTREE(iRCHILD(root), nbElt)){
        char str3[100];
        sprintf(str3, "%d:se -> %d [label = \" 1\"]\n", tree[root], tree[iRCHILD(root)]);
        fputs(str3, fp);
    }

    fclose(fp);

	genere_minimier_viz_rec(tree, nbElt, iLCHILD(root), nom_fichier);
	genere_minimier_viz_rec(tree, nbElt, iRCHILD(root), nom_fichier);
}