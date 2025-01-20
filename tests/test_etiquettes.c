// teste les fonctions de gestion du tableau d'étiquettes

#include "header.h"

void test_AjoutEtiquette() {
    Etiquette tableau[10];
    Erreur erreur;
    int nb_etiquettes = 0;
    unsigned int adresse = 6;

    AjoutEtiquette(tableau, "fin", &nb_etiquettes, &adresse, &erreur);
    assert(nb_etiquettes == 1);
    assert(tableau[0].adr == 6);
}

void test_EtiquetteNonValide() {
    Erreur erreur;
    erreur.statut = 0;
    EtiquetteNonValide("fin", &erreur);
    assert(erreur.statut == 0);
    EtiquetteNonValide("un_deux", &erreur);
    assert(erreur.statut == 0);
    EtiquetteNonValide("un deux", &erreur);
    assert(erreur.statut == 1);
    erreur.statut = 0;
    EtiquetteNonValide("1un", &erreur);
    assert(erreur.statut == 1);
    erreur.statut = 0;
    EtiquetteNonValide("fin!", &erreur);
    assert(erreur.statut == 1);
}

void test_TrouverAdresse() {
    Etiquette tableau[2] = {
        {3,"etiq1"},
        {6,"etiq2"}
    };
    Erreur erreur;
    erreur.statut = 0;

    unsigned int adr = TrouverAdresse("etiq1", tableau, 2, &erreur);
    assert(adr == 3);
    
    adr = TrouverAdresse("etiq3", tableau, 2, &erreur);
    assert(erreur.statut == 1);
}

int main() {
    test_AjoutEtiquette();
    printf("AjoutEtiquette a été testé avec succès\n");
    test_EtiquetteNonValide();
    printf("EtiquetteNonValide a été testé avec succès\n");
    test_TrouverAdresse();
    printf("TrouverAdresse a été testé avec succès\n");
    return 0;
}