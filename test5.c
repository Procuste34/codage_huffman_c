#include <stdio.h>
#include <assert.h>

void genere_arbre_codage_viz(int *tree, int root, char * nom_fichier);
void genere_arbre_codage_viz_rec(int *tree, int root, char * nom_fichier);

int main(void){

    int huffmanTree[256];
    for(int i = 0; i < 256; i++){
        huffmanTree[i] = -256;
    }

    /*
    huffmanTree[65] = -131;
    huffmanTree[66] = -130;
    huffmanTree[67] = -129;
    huffmanTree[68] = -128;
    huffmanTree[82] = 128;
    huffmanTree[128] = 129;
    huffmanTree[129] = 130;
    huffmanTree[130] = 131;
    */

    genere_arbre_codage_viz(huffmanTree, 0, "arbre.dot");

    return 0.;
}

void genere_arbre_codage_viz(int *tree, int root, char * nom_fichier){
    genere_arbre_codage_viz_rec(tree, root, nom_fichier);
}

void genere_arbre_codage_viz_rec(int *tree, int root, char * nom_fichier){
    //on cherche les 2 fils de root puis lancer l'appel rec.

    char label[4];
    if(root <= 127){
        sprintf(label, "%c", root);
    }else{
        sprintf(label, "%d", root);
    }

    printf("%d [label = \"%s\"]\n", root, label);

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
        printf("%d:sw -> %d [label = \" 0\"]\n", root, fils_gauche);

        if(fils_droite != -256){
            printf("%d:se -> %d [label = \" 1\"]\n", root, fils_droite);
        }

        genere_arbre_codage_viz_rec(tree, fils_gauche, nom_fichier);

        if(fils_droite != -256){
            genere_arbre_codage_viz_rec(tree, fils_droite, nom_fichier);
        }
    }
	
}