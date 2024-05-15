#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "general.h"
#include "patricia_trie.h"

void cree_racine_Patricia(ABR *racine)
{
	int i;
	*racine = (ABR)malloc(sizeof(struct noeud));
	for ( i = 0; i < NALPHABET; i++)
	{
		(*racine)->cle[i] = NULL;
		(*racine)->fin[i] = 0;
		(*racine)->fils[i] = NULL;
	}
}

void affiche_Patricia(ABR racine)
{
	//printf("== debut affichage ==\n");
	char mot_courant[TAILLE_MAX];
	int est_fin;
	
	mot_courant[0] = '\0';
	est_fin = FALSE;
	affiche_rec(racine, mot_courant, est_fin);
	//printf(" --- fin affichage --- \n");
}

void affiche_rec(ABR racine, char *partie_haute, int est_fin)
{
	int i;
	char mot_courant[TAILLE_MAX];
	char temp[TAILLE_MAX];
	
	if (est_fin == TRUE)
		printf(" %s\n", partie_haute);
	if (racine == NULL)
		return;
	for (i = 0; i < NALPHABET; i++)
	{
		if (racine->cle[i] != NULL)
		{
			//printf(" @@@@@ %s\n", racine->cle[i]);
			strcpy(temp, partie_haute);
			strcpy(mot_courant, strcat(temp, racine->cle[i]));
			affiche_rec(racine->fils[i], mot_courant, racine->fin[i]);
		}
	}
	return;
}

void insert_Patricia(ABR racine, char *valeur)
{
	int i, j, indice;
	int position, pos_bis;
	ABR branche;
	char temp[TAILLE_MAX], reste[TAILLE_MAX], commun[TAILLE_MAX];
	
	//printf("debut insert pour >%s<\n", valeur);
	position = valeur[0] - DEBUT;
	//printf(" position : %d <\n", position);
	if(racine->cle[position] == NULL)
	{
		racine->cle[position] = malloc(TAILLE_MAX*sizeof(char));
		strcpy(racine->cle[position], valeur);
		racine->fin[position] = TRUE;
		return;
	}
	else
	{
		if (strcmp(racine->cle[position], valeur) == 0)
		{
			racine->fin[position] = TRUE;
			return;
		}
		indice = 0; 
		while (racine->cle[position][indice] == valeur[indice]) indice++;
		//printf("indice =%d\n", indice);
		if(racine->cle[position][indice] == '\0')
		{
			/* la valeur dépasse la chaine gardée */
			j = indice; 
			while (valeur[j] != '\0') 
			{
				temp[j - indice] = valeur[j];
				j++;
			}
			temp[j - indice] = '\0';
			/* insertion du morceaux qui dépasse */
			if(racine->fils[position] == NULL)
				cree_racine_Patricia(&racine->fils[position]);
			insert_Patricia(racine->fils[position], temp);
			return;
		}
		
		else
		{
			/* la valeur à insérer et la chaine gardée ont une partie en commun */
			/* calcul de la partie commune */
			for (i = 0; i < indice; i++)
				commun[i] = racine->cle[position][i];
			commun[indice] = '\0';
			/* calcul de temp : valeur = commun + temp */
			j = indice; 
			while (valeur[j] != '\0') 
			{
				temp[j - indice] = valeur[j];
				j++;
			}
			temp[j - indice] = '\0';
			/* calcul de reste : racine->cle[position] = commun + reste */
			j = indice; 
			while (racine->cle[position][j] != '\0') 
			{
				reste[j - indice] = racine->cle[position][j];
				j++;
			}
			reste[j - indice] = '\0';
			//printf( " commun : %s **  reste : %s ** temp :%s \n", commun, reste, temp);
			
			/* on crée un noeud supplémenatire avec 2 élements : temp et reste */
			/* on prend soin de ne rien perdre */
			branche =  racine->fils[position];
			cree_racine_Patricia(&racine->fils[position]);
			pos_bis = reste[0] - DEBUT;
			racine->fils[position]->cle[pos_bis] = malloc(TAILLE_MAX*sizeof(char));
			strcpy(racine->fils[position]->cle[pos_bis], reste);
			racine->fils[position]->fils[pos_bis] = branche;
			racine->fils[position]->fin[pos_bis] = racine->fin[position];
			racine->fin[position] = FALSE;
			strcpy(racine->cle[position], commun);
			if (strlen(temp) != 0)
			{
				insert_Patricia(racine->fils[position], temp);				
			}
			else
			{
				racine->fin[position] = TRUE;
			}
			return;
		}
		return;
	}
	printf(" fin insert ");
}






void nettoyage(char *ligne, char *mot_propre) {
    int i, j;

    i = 0;
    j = 0;
    while (ligne[i] != '\0')
    {
        if ((ligne[i] >= DEBUT) && (ligne[i] <= DEBUT + NALPHABET))
        {
            mot_propre[j] = ligne[i];
            j++;
        }
        i++;
    }
    mot_propre[j] = '\0';
}

void intersect(ABR noeud1, ABR noeud2) {
    int i;
        if (noeud1 == NULL)
            return;

        for (i = 0; i < NALPHABET; i++) {
            if (noeud1->cle[i] != NULL && noeud2->cle[i] != NULL) {
                printf("%s\n", noeud1->cle[i]);
                intersect(noeud1->fils[i], noeud2->fils[i]);
            }
        }
}

int main() {

    ABR patricia1, patricia2;
    FILE *fichier1, *fichier2;
    char ligne[TAILLE_MAX], mot[TAILLE_MAX];
    char f1[50], f2[50];


    /*Fichier 1 */
    printf("Fichier 1 : ");
    scanf("%s",f1);
    /* Fichier 2 */
    printf("Fichier 2 : ");
    scanf("%s",f2);



    fichier1 = fopen(f1, "r");

    if (fichier1 == NULL) {
        printf("erreur ouverture fichier %s.\n", f1);
        return 2;
    }

    fichier2 = fopen(f2, "r");

    if (fichier2 == NULL) {
        printf("erreur ouverture fichier %s.\n", f2);
        fclose(fichier1);
        return 2;
    }

    cree_racine_Patricia(&patricia1);
    cree_racine_Patricia(&patricia2);

    while (!feof(fichier1) && (fgets(ligne, sizeof ligne, fichier1) != NULL)) {
        nettoyage(ligne, mot);
        if (strlen(mot) != 0)
            insert_Patricia(patricia1, mot);
    }
    fclose(fichier1);

    while (!feof(fichier2) && (fgets(ligne, sizeof ligne, fichier2) != NULL)) {
        nettoyage(ligne, mot);
        if (strlen(mot) != 0)
            insert_Patricia(patricia2, mot);
    }
    fclose(fichier2);

    /* Parcours de l'arbre 1 et vérification de la présence dans l'arbre 2 */

    printf("Intersection des contenus de %s et %s :\n", f1, f2);
    intersect(patricia1, patricia2);

    return 0;
}
