#ifndef HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

#define HEADER_H
#define MAX_NB_INSTRUCTION 500
#define MAX_LONGUEUR_ETIQUETTE 30
#define MAX_LONGUEUR_INSTRUCTION 50

// constantes pour le simulateur de ma ùachine à pile
//nous avons choisi de réserver 1000 adresses pour la pile et 4000 adresses pour le reste

#define TAILLE_TOTALE 5000
#define TAILLE_PILE 1000
#define TAILLE_PROGRAMME 2000 //on a donc le droit à 1000 lignes de code. (1000 adresses pour le premier élément du tableau, les 1000 autres pour le deuxième)
#define TAILLE_MEMOIRE 2000

// structure pour sauvegarder les informations sur une étiquette dans le fichier d'instructions
typedef struct {
    int adr;
    char nom[MAX_LONGUEUR_ETIQUETTE];
} Etiquette;

typedef struct {
    int statut;
    char msg_erreur[100];
} Erreur;


extern const char TableInstructions[][15];

// prototypes des fonctions sur les opérations dans les instructions
int ObtenirCodeOperation(char chaine[]);
int VerifOp(int * opcode, int * num_ligne, char mot2[], int * nb_mots, Etiquette tableau_etiquettes[], const int nb_etiquettes, Erreur * erreur);

// prototypes des fonctions pour des vérifications et opérations élémentaires
void VerifFichierEntree(int argc, char *argv[], Erreur *erreur);
int ContientEspace(const char chaine[]);
int ChaineEnNombre(const char chaine[], Erreur * erreur);
int EstNombre(const char chaine[], Erreur * erreur);

// prototypes des fonctions pour la gestion des étiquettes
unsigned int TrouverAdresse(const char nom_etiquette[], const Etiquette tableau_etiquettes[], const int nb_etiquettes, Erreur * erreur);
void RemplirTableauEtiq(FILE * source, Etiquette tableau_etiquettes[], int *nb_etiquettes, Erreur * erreur);
void AjoutEtiquette(Etiquette tableau_etiquettes[], char nom_etiquette[], int * nb_etiquettes, const unsigned int * adresse, Erreur * erreur);
void EtiquetteNonValide(const char chaine[], Erreur * erreur);

//prototype des fonctions des deux programmes principaux
int Conversion(int argc, char *argv[], Erreur *erreur);
int Simulateur();



#endif