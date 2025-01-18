
#include "header.h"


// ajoute une entrée au tableau avec toutes les étiquettes
void AjoutEtiquette(Etiquette tableau_etiquettes[], char nom_etiquette[], int * nb_etiquettes, const unsigned int * adresse, Erreur * erreur) {
    if (tableau_etiquettes==NULL) {
        strcpy(erreur->msg_erreur, "erreur d'accès au tableau des étiquettes");
        erreur->statut = 1;
        return;
    }
    strcpy(tableau_etiquettes[*nb_etiquettes].nom, nom_etiquette); // copie l'étiquette dans le tableau en dernière position
    tableau_etiquettes[*nb_etiquettes].adr = *adresse;
    (*nb_etiquettes)++;
    printf("étiquette %s numéro %d ajoutée avec succès au tableau\n", nom_etiquette, *nb_etiquettes);
    return;
}

// trouve l'adresse associée à un nom d'étiquette dans le tableau des étiquettes
unsigned int TrouverAdresse(const char nom_etiquette[], const Etiquette tableau_etiquettes[], const int nb_etiquettes, Erreur * erreur) {
    if (tableau_etiquettes == NULL || nom_etiquette == NULL) {
        strcpy(erreur->msg_erreur, "étiquette ou tableau invalide");
        erreur->statut = 1;
        return 0;
    }

    for (int i = 0; i < nb_etiquettes; i++) {
        if (strcmp(tableau_etiquettes[i].nom, nom_etiquette) == 0) {
            return tableau_etiquettes[i].adr;
        }
    }

    // étiquette non trouvée dans le tableau
    erreur->statut = 1;
    return 0;
}

// vérifie si l'étiquette est dans le bon format
int EtiquetteNonValide(const char chaine[], Erreur * erreur){
    // ne commence pas par un chiffre contient seulement des caractères dans un certain intervalle ASCII, pas d'espace, pas de caractères spéciaux

    return 0;
}

// ajoute une entrée au tableau avec toutes les étiquettes
void RemplirTableauEtiq(FILE * source, Etiquette tableau_etiquettes[], int *nb_etiquettes, Erreur * erreur) {
    /*
    en entrée : 
    - le fichier avec les instructions
    - le tableau des étiquettes, au début vide
    - pointeur vers le nombre d'étiquettes, au début 0
    - la structure d'erreur

    remplit le tableau des étiquettes avec les étiquettes trouvées dans le fichier
    */
    if (source==NULL) {
        strcpy(erreur->msg_erreur, "Erreur de lecture du fichier");
        erreur->statut = 1;
        return;
    }
    char ligne[256];
    char possible_etiquette[MAX_LONGUEUR_INSTRUCTION];
    char autre[MAX_LONGUEUR_INSTRUCTION];
    int num_ligne=0;

    while (fgets(ligne, sizeof(ligne), source)) {
        if (ligne[0]=='\n' || ligne[0]=='\0') {
            continue;
        }
        possible_etiquette[0]='\0';
        autre[0]='\0';
        // sépare la ligne en string avant et après le ':' si le ':' est présent
        // 49 correspond à la longueur max de l'instruction
        int n = sscanf(ligne, "%49[^:]:%49[^\n]", possible_etiquette, autre);
        if (n==2) {
            printf("étiquette :%s\n", possible_etiquette);
            // vérifie si l'étiquette est valide
            EtiquetteNonValide(possible_etiquette, erreur);
            if (erreur->statut==1) {
                sprintf(erreur->msg_erreur, "erreur de syntaxe pour l'étiquette à la ligne %d", possible_etiquette);
                return;
            }
            // ajoute l'étiquette au tableau des étiquettes, l'adresse est l'adresse relative du fichier d'instructions
            unsigned int temp_adresse = (unsigned int)num_ligne;
            AjoutEtiquette(tableau_etiquettes, possible_etiquette, nb_etiquettes, &temp_adresse, erreur);
        }  
        num_ligne++;
    }
    return;
}