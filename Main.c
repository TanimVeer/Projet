#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#pragma warning(disable : 4996 6031)

enum { MAX_CHAR = 50, NOM_CHAR_MAX = 30, ETU_MAX = 100, JOUR_MAX = 40, ABSENCES_MAX = 5 };

typedef struct {
    char nom[NOM_CHAR_MAX];
    int groupe;
} Etudiant;

typedef struct {
    int jour;
    char demi_journee[3];
    int justifiee;          
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

void gestion_absences(int nbEtu, int jour, char demi_journee) {
    for (int i = 0; i < nbAbsences; i++) {
        if (strcmp(absences[i].demi_journee, demi_journee) == 0 && absences[i].jour == jour && nbEtu == nbEtu) {
            printf("Absence deja connue\n");
            return;
        }
    }

    if (nbAbsences < ABSENCES_MAX) {
        strncpy(absences[nbAbsences].demi_journee, demi_journee, ABSENCES_MAX);
        strncpy(absences[i].jour, jour, JOUR_MAX);
        printf("Absence enregistree [%d]\n", nbAbsences + 1);
        nbAbsences++;
    }
    else {
        printf("Limite d'étudiants atteinte.\n");
    }
}

/* --------------------------------------------------------------------------------------------------- */

int main() {
    char input[MAX_CHAR];
    char nom[NOM_CHAR_MAX];
    int groupe;
    int jour;
    char demi_journee[3];
    printf("Choisir une fonction : inscription, absence, etudiants, justificatif, defaillants, exit\n");

    do {
        scanf("%s", input);

        // C1
        if (strcmp(input, "inscription") == 0) {
            scanf("%s %d", nom, &groupe);
            inscription(nom, groupe);
        }

        //C2
        if (strcmp(input, "absence") == 0) {
            scanf("%d %d %s", &nbEtu, &jour, demi_journee);
            gestion_absences(nbEtu, jour, demi_journee);
        }

        //C3





        else {
            printf("Commande inconnue\n");
        }
    } while (strcmp(input, "exit") != 0); // C0
}
