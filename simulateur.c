#include "src/header.h"

int main(int argc, char *argv[]) {
    Erreur erreur;
    erreur.statut = 0;
    Conversion(argc, argv, &erreur);
    if (erreur.statut ==0){
        printf("Conversion réussie\n");
    }
    else{
        printf("Erreur lors de la conversion du fichier code machine en hexadécimal\n");
        return 1;
    }
    Simulateur();
}