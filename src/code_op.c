#include "header.h"

// table de toutes les instructions possibles
const char TableInstructions[][15] = {
        "pop", "ipop", "push", "ipush", "push#", 
        "jmp", "jnz", "call", "ret", "read", 
        "write", "op", "rnd", "dup", "halt"
};


int ObtenirCodeOperation(char chaine[]) {
    // obtenir le code d'opération à partir du nom de l'instruction
    if (chaine == NULL || *chaine == '\0') return 0;
    int opcode=-1;
    for (int i = 0; i < 15; i++) {
        if (strcmp(chaine, TableInstructions[i]) == 0) {
            opcode = i;
            break;
        }
    }
    if (opcode==14) {
        // dernier code opération est 99
        opcode = 99;
    }
    return opcode;
}



int VerifOp(int * opcode, int * num_ligne, char mot2[], int * nb_mots, Etiquette tableau_etiquettes[], const int nb_etiquettes, Erreur * erreur) {
    /*
    vérifie la validité de la valeur pour l'opération choisie dnas l'instruction
    en entrée :
    - le code de l'instruction
    - le numéro de la ligne actuelle (pour le calcul de saut)
    - le deuxième mot de l'instruction, qui peut être une référence à une étiquette ou une valeur
    - le nombre de mots dans l'instruction de la ligne
    - le tableau des étiquettes
    - le nombre d'étiquettes
    - la structure d'erreur

    vérifie : 
    - si l'instruction n'est pas ipop, ipush, ret, dup ou halt, il doit y avoir une valeur
    - si le deuxième mot n'est pas un nombre, il doit être une étiquette, c'est possible uniquement pour jmp, jnz et call
    - si la valeur est une étiquette, on trouve l'adresse de l'étiquette et on fait la différence avec l'adresse actuelle
    - si la valeur est un nombre, convertit la chaîne en nombre 
        -> valeur négative autorisée uniquement pour push#, jmp, jnz et call
        -> la valeur pour op doit être comprise entre 0 et 15
        -> ipop, ipush, ret, dup et halt ne peuvent pas avoir de valeur
    
    */
    if (*nb_mots == 1) {
        if (*opcode != 1 && *opcode != 3 && *opcode != 8 && *opcode != 13 && *opcode != 99) {
            sprintf(erreur->msg_erreur, "l'instruction %d nécessite des données", *opcode);
            erreur->statut = 1;
            return 0;
        }
        return 0;
    }
    if (EstNombre(mot2, erreur)==0){
        if (erreur->statut == 1){
            sprintf(erreur->msg_erreur, "erreur de conversion de la valeur de l'instruction %s en étiquette ou nombre", mot2);
            return 0;
        }

        // cas où la valeur est une étiquette
        if (*opcode != 5 && *opcode != 6 && *opcode != 7) {
            sprintf(erreur->msg_erreur, "impossible de définir une adresse comme valeur pour l'instruction (%d)", *opcode);
            erreur->statut = 1;
            return 0;
        }
        // étiquette de texte sert de référence à une autre adresse
        unsigned int adresse_etiquette = TrouverAdresse(mot2, tableau_etiquettes, nb_etiquettes, erreur);
        if (erreur->statut == 1) {
            strcpy(erreur->msg_erreur, "erreur d'accès à cette étiquette: valeur invalide ou inexistante");
            return 0;
        }
        // différence entre l'adresse de l'étiquette et l'adresse de l'instruction actuelle (ligne actuelle)
        return adresse_etiquette - *num_ligne;
    }

    int valeur = ChaineEnNombre(mot2, erreur);
    if (erreur->statut == 1){
        return 0;
    }
    // valeurs négatives autorisées uniquement pour push#
    if (valeur < 0) {
        if (*opcode != 4 && *opcode != 5 && *opcode != 6 && *opcode != 7) {
            sprintf(erreur->msg_erreur, "impossible de définir une valeur négative pour l'instruction (%d)", *opcode);
            erreur->statut = 1;
            return 0;
        }
    }
    // si la valeur n'est pas vide elle ne peut pas correspondre aux instructions 1 3 8 13 ou 99
    if (*opcode == 1 || *opcode == 3 || *opcode == 8 || *opcode == 13 || *opcode == 99) {
        sprintf(erreur->msg_erreur, "l'instruction (%d) n'attend pas de valeur", *opcode);
        erreur->statut = 1;
        return 0;
    }
    // la valeur pour l'instruction op doit être comprise entre 0 et 15
    if (*opcode == 11) {
        if (valeur > 15) {
            sprintf(erreur->msg_erreur, "opération hors limites (%d > 15)", valeur);
            erreur->statut = 1;
            return 0;
        }
    }
    return valeur;
}