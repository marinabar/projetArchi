#include "header.h"


void test_ContientEspace() {
    assert(ContientEspace("hello world") == 1);
    assert(ContientEspace("helloworld") == 0);
    assert(ContientEspace("hello\tworld") == 1);
    assert(ContientEspace("hello\nworld") == 1);
}

void test_ChaineEnNombre() {
    Erreur erreur;
    erreur.statut = 0;
    assert(ChaineEnNombre("123", &erreur) == 123);
    assert(erreur.statut == 0);

    assert(ChaineEnNombre("abc", &erreur) == 0);
    assert(erreur.statut == 1);
    erreur.statut = 0;
    //printf("int max : %d\n", INT_MAX);
    assert(ChaineEnNombre("2147483648", &erreur) == 0);
    assert(erreur.statut == 1);
    erreur.statut = 0;

    assert(ChaineEnNombre("-1", &erreur) == -1);
    assert(erreur.statut == 0);
    erreur.statut = 0;

    assert(ChaineEnNombre("", &erreur) == 0);
    assert(erreur.statut == 1);
}

void test_EstNombre() {
    Erreur erreur;
    erreur.statut = 0;

    assert(EstNombre("18", &erreur) == 1);
    assert(EstNombre("-99", &erreur) == 1);
    assert(EstNombre("abc", &erreur) == 0);
    assert(EstNombre("123a", &erreur) == 0);
    assert(EstNombre("", &erreur) == 0);
    assert(erreur.statut == 1);
}

void test_VerifFichierEntree() {
    Erreur erreur;
    erreur.statut = 0;
    // on simule la liste des arguments en entrée
    char *argv1[] = {"programme.out", "fichier.txt"};
    VerifFichierEntree(2, argv1, &erreur);
    assert(erreur.statut == 0);

    char *argv2[] = {"programme.out"};
    VerifFichierEntree(1, argv2, &erreur);
    assert(erreur.statut == 1);

    erreur.statut = 0;

    char *argv3[] = {"fichier"};
    VerifFichierEntree(2, argv3, &erreur);
    assert(erreur.statut == 1);
    
}

int main() {
    test_ContientEspace();
    printf("ContientEspace a été testée avec succès\n");
    test_ChaineEnNombre();
    printf("ChaineEnNombre a été testée avec succès\n");
    test_EstNombre();
    printf("EstNombre a été testée avec succès\n");
    test_VerifFichierEntree();
    printf("VerifFichierEntree a été testée avec succès\n");
    return 0;
}