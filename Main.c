#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma warning(disable : 4996 6031)

enum { MAX_CHAR = 50, NOM_CHAR_MAX = 30, ETU_MAX = 100, JOUR_MAX = 40, ABSENCES_MAX = 5 };

typedef struct {
    char nom[NOM_CHAR_MAX];
    int groupe;
} Etudiant;

typedef struct {
    int identifiant; // Ajout de l'identifiant de l'étudiant
    int jour;
    char demi_journee[3];
} Absence;

Etudiant etudiants[ETU_MAX];
Absence absences[ABSENCES_MAX];

int nbEtu = 0;
int nbAbsences = 0;

/* --------------------------------------------------------------------------------------------------- */

void inscription(const char* nom, int groupe) {
    // Vérification des doublons
    for (int i = 0; i < nbEtu; i++) {
        if (strcmp(etudiants[i].nom, nom) == 0 && etudiants[i].groupe == groupe) {
            printf("Nom incorrect\n");
            return;
        }
    }

    if (nbEtu < ETU_MAX) {
        strncpy(etudiants[nbEtu].nom, nom, NOM_CHAR_MAX);
        etudiants[nbEtu].nom[NOM_CHAR_MAX - 1] = '\0';
        etudiants[nbEtu].groupe = groupe;
        printf("Inscription enregistree (%d)\n", nbEtu + 1);
        nbEtu++;
    }
    else {
        printf("Limite d'étudiants atteinte.\n");
    }
}

/* --------------------------------------------------------------------------------------------------- */

void gestion_absences(int identifiant, int jour, char* demi_journee) {
    // Vérification de l'identifiant
    if (identifiant < 1 || identifiant > nbEtu) {
        printf("Identifiant incorrect\n");
        return;
    }

    // Vérification du jour
    if (jour < 1 || jour > 40) {
        printf("Date incorrecte\n");
        return;
    }

    // Vérification de la demi-journée
    if (strcmp(demi_journee, "am") != 0 && strcmp(demi_journee, "pm") != 0) {
        printf("Demi-journee incorrecte\n");
        return;
    }

    // Vérification si l'absence est déjà connue
    for (int i = 0; i < nbAbsences; i++) {
        if (absences[i].identifiant == identifiant && absences[i].jour == jour && strcmp(absences[i].demi_journee, demi_journee) == 0) {
            printf("Absence deja connue\n");
            return;
        }
    }

    // Enregistrement de la nouvelle absence
    if (nbAbsences < ABSENCES_MAX) {
        absences[nbAbsences].identifiant = identifiant;
        absences[nbAbsences].jour = jour;
        strncpy(absences[nbAbsences].demi_journee, demi_journee, sizeof(absences[nbAbsences].demi_journee));
        absences[nbAbsences].demi_journee[sizeof(absences[nbAbsences].demi_journee) - 1] = '\0'; // S'assurer que la chaîne est bien terminée
        printf("Absence enregistree [%d]\n", nbAbsences + 1);
        nbAbsences++;
    }
    else {
        printf("Limite d'absences atteinte.\n");
    }
}

/* --------------------------------------------------------------------------------------------------- */

int main() {
    char input[MAX_CHAR];
    char nom[NOM_CHAR_MAX];
    int groupe;
    int identifiant; // Identifiant de l'étudiant
    int jour;
    char demi_journee[3];

    printf("Choisir une fonction : inscription, absence, exit\n");

    while (1) {
        scanf("%s", input);

        // C1 - Inscription
        if (strcmp(input, "inscription") == 0) {
            scanf("%s %d", nom, &groupe);
            inscription(nom, groupe);
        }

        // C2 - Gestion des absences
        else if (strcmp(input, "absence") == 0) {
            scanf("%d %d %s", &identifiant, &jour, demi_journee);
            gestion_absences(identifiant, jour, demi_journee);
        }

        // C0 - Sortie
        else if (strcmp(input, "exit") == 0) {
            break;  // Sortir de la boucle
        }

        // Commande inconnue
        else {
            printf("Commande inconnue\n");
        }
    }
}
