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

void listes(int jour_courant) {
    // Vérification du jour courant
    if (jour_courant < 1) {
        printf("Date incorrecte\n");
        return;
    }

    // Vérification s'il y a des étudiants inscrits
    if (nbEtu == 0) {
        printf("Aucun inscrit\n");
        return;
    }

    // Tableau pour stocker le nombre d'absences pour chaque étudiant
    int absencesCount[ETU_MAX] = { 0 };

    // Calcul des absences pour chaque étudiant
    for (int i = 0; i < nbAbsences; i++) {
        if (absences[i].jour <= jour_courant) {
            absencesCount[absences[i].identifiant - 1]++; // On utilise identifiant - 1 pour accéder au bon index
        }
    }

    // Tri des étudiants par groupe croissant et par nom
    for (int i = 0; i < nbEtu - 1; i++) {
        for (int j = i + 1; j < nbEtu; j++) {
            if (etudiants[i].groupe > etudiants[j].groupe ||
                (etudiants[i].groupe == etudiants[j].groupe && strcmp(etudiants[i].nom, etudiants[j].nom) > 0)) {
                Etudiant temp = etudiants[i];
                etudiants[i] = etudiants[j];
                etudiants[j] = temp;
            }
        }
    }

    // Affichage des étudiants
    printf("Liste des etudiants :\n");
    for (int i = 0; i < nbEtu; i++) {
        printf("(%d) %-15s %3d %2d\n", i + 1, etudiants[i].nom, etudiants[i].groupe, absencesCount[i]);
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

    printf("Choisir une fonction : inscription, absence, listes, exit\n");

    while (1) {
        scanf("%s", input);

        // C0 - Sortie
        if (strcmp(input, "exit") == 0) {
            break;  // Sortir de la boucle
        }

        // C1 - Inscription
        else if (strcmp(input, "inscription") == 0) {
            scanf("%s %d", nom, &groupe);
            inscription(nom, groupe);
        }

        // C2 - Gestion des absences
        else if (strcmp(input, "absence") == 0) {
            scanf("%d %d %s", &identifiant, &jour, demi_journee);
            gestion_absences(identifiant, jour, demi_journee);
        }

        // C3 - Liste des etudiants
        else if (strcmp(input, "listes") == 0) {
            scanf("%d", &jour);
            listes(jour);
        }

        // C4 - Justificatif
        else if (strcmp(input, "justificatif") == 0) {
            scanf("%d", ??, &jour, justification);
            listes(jour);
        }

        // Commande inconnue
        else {
            printf("Commande inconnue\n");
        }
    }
}
