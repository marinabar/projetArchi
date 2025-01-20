#include "src/header.h"

int main(int argc, char *argv[]) {
    Erreur erreur;
    erreur.statut = 0;
    int reussite_conversion = Conversion(argc, argv, &erreur);
    if (erreur.statut ==0){
        printf("Conversion réussie\n");
    }
    int reussite_simulation = Simulateur();
}