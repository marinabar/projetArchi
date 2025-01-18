// la première est un assembleur qui va transformer un programme écrit en langage 
// assembleur (dans un fichier) en un programme écrit en langage machine (dans un autre fichier)

#include "src/header.h"


void EcrireFichier(FILE * destination, int * num_ligne, char mot1[], char mot2[], int * nb_mots, Etiquette tableau_etiquettes[], int nb_etiquettes, Erreur * erreur) {
    /*
    convertit les mots analysés en valeurs numériques et les écrit dans le fichier de destination
    en entrée :
    - le fichier avec les codes hexadécimaux
    - le numéro de la ligne actuelle
    - le premier mot de l'instruction
    - le deuxième mot de l'instruction si présent
    - le nombre de mots dans l'instruction
    - le tableau des étiquettes contenant le nom et l'adresse
    - le nombre d'étiquettes totales
    - la structure d'erreur
    */

    // convertit le premier mot en instruction le deuxième mot en valeur
    printf("conversion de l'instruction %s et %s à la ligne %d\n", mot1, mot2, *num_ligne);
    printf("contient %d mots\n", *nb_mots);

    int opcode = ObtenirCodeOperation(mot1);
    // en cas de mauvais numéro d'opération entré
    if (opcode == -1) {
        erreur->statut = 1;
        strcpy(erreur->msg_erreur, "erreur de conversion de l'instruction");
        return;
    }
    // écrit dans le fichier sur deux caractères hexadécimaux
    fprintf(destination, "%02x ", opcode);
    // vérifie si la valeur est valide pour l'opération choisie
    int valeur = VerifOp(&opcode, num_ligne, mot2, nb_mots, tableau_etiquettes, nb_etiquettes, erreur);

    if (erreur->statut == 1) {
        return;
    }
    fprintf(destination, "%04x\n", (unsigned int)valeur & 0xFFFF); // cast vers unsigned int pour que hex comprenne complément à deux
}


// assigne une ligne du fichier d'instructions à des mots séparés + renvoie le nombre de mots non étiquette
int SeparerLigne(const int * num_ligne, char chaine[], char * possible_etiquette, char * non_etiquette, char * mot1, char * mot2, Etiquette tableau_etiquettes[], int * nb_etiquettes, int * erreur) {
    // sépare la ligne en 50 caractères max avant et 50 max après ':' non_etiquette est vide s'il n'y a pas d'étiquette dans la ligne
    int n = sscanf(chaine, "%49[^:]:%49[^\n]", possible_etiquette, non_etiquette); 
    int nbmots = 0;
    if (n==1) {
        printf("pas d'étiquette:%s\n", possible_etiquette);
        nbmots = sscanf(possible_etiquette, " %49s %49s", mot1, mot2);
        printf("mot1 :%s mot2 :%s\n", mot1, mot2);
    }
    if (n==2) {
        printf("étiquette :%s\n", possible_etiquette);
        // on re divise le mot en étiquette vs non étiquette
        nbmots = sscanf(non_etiquette, " %49s %49s", mot1, mot2);
        printf("mot1 :%s mot2 :%s\n", mot1, mot2);
    }
    if (n==0) {
        printf("erreur de syntaxe à la ligne %d : ligne vide ou format incorrect\n", *num_ligne);
        return -1;
    }
    if (ContientEspace(mot2)) {
        printf("erreur de syntaxe à la ligne %d : on ne peut pas avoir plus de 2 mots pour une instruction\n", *num_ligne);
        return -1;
        
    }
    return nbmots;
}

int Conversion(int argc, char *argv[], Erreur *erreur) {
    /*
    Déroulement du programme :
    - vérifie si le format d'entrée est correct
    - ouvre le fichier source et crée le fichier pour les codes hexadécimaux
    - lit le fichier une première fois pour enregistrer les étiquettes
    - lit le fichier une deuxième fois pour convertir chaque ligne en hexadécimal
    - écrit les codes hexadécimaux dans le fichier de destination
    - vérifie si le fichier se termine par halt 
    */
    // vérifie si le format d'entrée est correct
    VerifFichierEntree(argc, argv, erreur);
    if (erreur->statut == 1) {
        printf("%s", erreur->msg_erreur);
        return 1;
    }
    // ouvre le fichier source et crée le fichier pour les codes hexadécimaux
    FILE * source = fopen(argv[1], "r");
    if (source == NULL) {
        printf("le fichier n'a pas pu être ouvert\n");
        return 1;
    }
    FILE * destination = fopen("hexa.txt", "w");
    if (destination == NULL) {
        printf("le fichier n'a pas pu être créé\n");
        return 1;
    }
    char possible_etiquette[MAX_LONGUEUR_INSTRUCTION], non_etiquette[MAX_LONGUEUR_INSTRUCTION], mot1[20], mot2[20];
    char ligne[256];
    int num_ligne=0;

    // tableau d'étiquettes
    Etiquette tableau_etiquettes[MAX_NB_INSTRUCTION];
    int nb_etiquettes = 0;
    int nb_mots;

    // repère les étiquettes du tableau
    RemplirTableauEtiq(source, tableau_etiquettes, &nb_etiquettes, erreur);

    // réinitialise le curseur de lecture du fichier
    rewind(source);

    while (fgets(ligne, sizeof(ligne), source)) {
        // passe les lignes vides
        if (ligne[0] == '\n' || ligne[0] == '\0') {
            continue;
        }
        // il faut réinitialiser les variables à chaque ligne
        possible_etiquette[0] = '\0';
        non_etiquette[0] = '\0';
        mot1[0] = '\0';
        mot2[0] = '\0';

        // découpe la ligne de cette manière : étiquette:instruction valeur => possible_etiquette:non_etiquette et non_etiquette=>mot1 mot2
        nb_mots = SeparerLigne(&num_ligne, ligne, possible_etiquette, non_etiquette, mot1, mot2, tableau_etiquettes, &nb_etiquettes, erreur);
        if (erreur->statut) {
            printf("Erreur de lecture de la ligne %d\n", num_ligne);
            printf("%s\n", erreur->msg_erreur);
            break;
        }

        // convertit ces mots en instructions et les écrit dans le nouveau fichier
        EcrireFichier(destination, &num_ligne, mot1, mot2, &nb_mots,tableau_etiquettes, nb_etiquettes, erreur);

        if (erreur->statut) {
            printf("Erreur d'écriture du fichier\n");
            printf("%s\n", erreur->msg_erreur);
            break;
        }

        num_ligne++;
    }
    fclose(source);
    fclose(destination);

    if (erreur->statut) {
        printf("%s\n", erreur->msg_erreur);
        return 1;
    }
    // nb_mots a le nombre de mots de la dernière ligne
    if (nb_mots == 2 && (strcmp("halt", mot2) != 0)) {
        strcpy(erreur->msg_erreur, "le fichier ne se termine pas par halt");
        erreur->statut = 1;
        return 1;
    }
    if (nb_mots == 1 && (strcmp("halt", mot1) != 0)) {
        strcpy(erreur->msg_erreur, "le fichier ne se termine pas par halt");
        erreur->statut = 1;
        return 1;
    }
    return 0;
}