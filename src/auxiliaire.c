#include "header.h"

void VerifFichierEntree(int argc, char *argv[], Erreur *erreur) {
    // vérifie si le format d'entrée est correc
    if (argc != 2) {
        erreur->statut = 1;
        strcpy(erreur->msg_erreur, "assurez-vous d'avoir entré exactement un nom de fichier\n");
        return;
    }
    char *test = strstr(argv[1], ".txt");
    if (strstr(test, ".txt") == NULL) {
        erreur->statut = 1;
        strcpy(erreur->msg_erreur, "assurez-vous d'avoir entré un nom de fichier se terminant par .txt\n");
        return;
    }
    return;
}


int ContientEspace(const char chaine[]) {
    // vérifie si la chaîne contient un espace
    while (*chaine !='\0') {
        if (*chaine ==' ') return 1;
        chaine++; // incrémente le pointeur pour montrer le caractère suivant
    }
    return 0;
}


int ChaineEnNombre(const char chaine[], Erreur * erreur) {
    // convertit la chaîne en nombre et vérifie si elle est dans les limites d'un int
    if (chaine == NULL || *chaine == '\0') {
        strcpy(erreur->msg_erreur, "chaîne vide ou invalide");
        erreur->statut = 1;
        return 0;
    }
    char *endptr;
    long valeur = strtol(chaine, &endptr, 10); // convertit la chaîne en entier long en base 10
    if (*endptr != '\0') {
        // la conversion s'est arrêtée avant la fin de la chaîne
        erreur->statut = 1;
        strcpy(erreur->msg_erreur, "erreur de conversion numérique");
        return 0;
    }
    // vérifie si la valeur est dans les limites d'un int
    if (valeur > INT_MAX || valeur < INT_MIN) {
        // nombre hors limites
        erreur->statut = 1;
        strcpy(erreur->msg_erreur, "valeur hors limites pour un nombre de deux octets");
        return 0;
    }
    return (int)valeur;
}


int EstNombre(const char chaine[], Erreur * erreur) {
    // vérifie si la chaîne est un nombre
    if (chaine == NULL || *chaine == '\0') {
        erreur->statut = 1;
        return 0;
    }
    if (!(chaine[0] >= '0' && chaine[0] <= '9') && chaine[0] != '-') {
        return 0;
    }
     // le reste peut être des lettres des chiffres un underscore
    for (int i = 1; chaine[i]; i++) {
        if (!(chaine[i] >= '0' && chaine[i] <= '9')) return 0;
    }
    return 1;
}