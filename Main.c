#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#pragma warning(disable : 4996 6031)

enum { 
    MIN_CHAR = 1,
    MAX_CHAR = 50,
    NOM_CHAR_MAX = 30,
    ETU_MIN = 1,
    ETU_MAX = 100,
    JOUR_MIN = 1,
    JOUR_MAX = 40,
    ABSENCES_MAX = 5,
    DEMI_JOURNEE_MAX = 3,
};

typedef struct {
    char nom[NOM_CHAR_MAX];
    int groupe;
} Etudiant;

typedef struct {
    int identifiant; 
    int jour;
    char demi_journee[DEMI_JOURNEE_MAX];
    char justification[MAX_CHAR];
} Absence;

typedef struct {
    Etudiant etudiants[ETU_MAX];
    unsigned int cpt_etu;
} ListEtu;

typedef struct {
    Absence absences[ABSENCES_MAX];
    unsigned int cpt_absence;
} ListAbsences;

/* --------------------------------------------------------------------------------------------------- */

void inscription(ListEtu* listEtu, const char* nom, int groupe) {
    // Vérification des doublons
    for (int i = 0; i < listEtu->cpt_etu; i++) {
        if (strcmp(listEtu->etudiants[i].nom, nom) == 0 && listEtu->etudiants[i].groupe == groupe) {
            printf("Nom incorrect\n");
            return;
        }
    }

    if (listEtu->cpt_etu < ETU_MAX) {
        strncpy(listEtu->etudiants[listEtu->cpt_etu].nom, nom, NOM_CHAR_MAX);
        listEtu->etudiants[listEtu->cpt_etu].nom[NOM_CHAR_MAX - 1] = '\0';
        listEtu->etudiants[listEtu->cpt_etu].groupe = groupe;
        printf("Inscription enregistree (%d)\n", listEtu->cpt_etu + 1);
        listEtu->cpt_etu++;
    }
    else {
        printf("Limite d'étudiants atteinte.\n");
    }
}

/* --------------------------------------------------------------------------------------------------- */

void enregistrer_absences(ListAbsences* listabsence, ListEtu* listetu, int identifiant, int jour, char* demi_journee) {
    // Vérification de l'identifiant
    if (identifiant < ETU_MIN || identifiant > listetu->cpt_etu) {
        printf("Identifiant incorrect\n");
        return;
    }

    // Vérification du jour
    if (jour < JOUR_MIN || jour > JOUR_MAX) {
        printf("Date incorrecte\n");
        return;
    }

    // Vérification de la demi-journée
    if (strcmp(demi_journee, "am") != 0 && strcmp(demi_journee, "pm") != 0) {
        printf("Demi-journee incorrecte\n");
        return;
    }

    // Vérification si l'absence est déjà connue
    for (int i = 0; i < listabsence->cpt_absence; i++) {
        if (listabsence->absences[i].identifiant == identifiant && listabsence->absences[i].jour == jour && strcmp(listabsence->absences[i].demi_journee, demi_journee) == 0) {
            printf("Absence deja connue\n");
            return;
        }
    }

    // Enregistrement de la nouvelle absence
    if (listabsence->cpt_absence < ABSENCES_MAX) {
        listabsence->absences[listabsence->cpt_absence].identifiant = identifiant;
        listabsence->absences[listabsence->cpt_absence].jour = jour;
        strncpy(listabsence->absences[listabsence->cpt_absence].demi_journee, demi_journee, sizeof(listabsence->absences[listabsence->cpt_absence].demi_journee));
        listabsence->absences[listabsence->cpt_absence].demi_journee[sizeof(listabsence->absences[listabsence->cpt_absence].demi_journee) - 1] = '\0'; // S'assurer que la chaîne est bien terminée
        printf("Absence enregistree [%d]\n", listabsence->cpt_absence + 1);
        listabsence->cpt_absence++;
    }
    else {
        printf("Limite d'absences atteinte.\n");
    }
}

/* --------------------------------------------------------------------------------------------------- */

int tri(Etudiant* etudiant1, Etudiant* etudiant2) {
    if (etudiant1->groupe != etudiant2->groupe) {
        return(etudiant1->groupe - etudiant2->groupe);
    } 
    return(strcmp(etudiant1->nom, etudiant2->nom));
}

void listes(ListEtu* listetu, ListAbsences* listeabsences, int jour_courant) {
    // Vérification du jour courant
    if (jour_courant < JOUR_MIN) {
        printf("Date incorrecte\n");
        return;
    }

    // Vérification s'il y a des étudiants inscrits
    if (listetu->cpt_etu == 0) {
        printf("Aucun inscrit\n");
        return;
    }

    // Tableau pour stocker le nombre d'absences pour chaque étudiant
    int absencesCount[ETU_MAX] = { 0 };

    // Calcul des absences pour chaque étudiant
    for (int i = 0; i < listeabsences->cpt_absence; i++) {
        if (listeabsences->absences[i].jour <= jour_courant) {
            absencesCount[listeabsences->absences[i].identifiant - 1]++;
        }
    }

    // Tri des étudiants par groupe croissant et par nom
    qsort(listetu->etudiants, ETU_MAX, sizeof(Etudiant), tri);

    // Affichage des étudiants
    printf("Liste des etudiants :\n");
    for (int i = 0; i < listetu->cpt_etu; i++) {
        printf("(%d) %-15s %3d %2d\n", i + 1, listetu->etudiants[i].nom, listetu->etudiants[i].groupe, absencesCount[i]);
    }
}

/* --------------------------------------------------------------------------------------------------- */

void justificatif(int identifiant, int jour, char justification) {
    if (identifiant < ETU_MIN) {
        printf("Identifiant incorrect");
    }
    else if (jour < JOUR_MIN) {
        printf("Date incorrecte");
    }
    else if (justificatif > MAX_CHAR) {
        printf("Justification trop long, charactère max : 50");
    }
}

/* --------------------------------------------------------------------------------------------------- */

int main() {
    ListEtu* listetu;
    ListAbsences* listabsence;
    char input[MAX_CHAR];
    char nom[NOM_CHAR_MAX];
    int groupe;
    int identifiant; // Identifiant de l'étudiant
    int jour;
    char demi_journee[3];
    char justification[50];

    printf("Choisir une fonction : inscription, absence, listes, justificatif, exit \n");

    while (1) {
        scanf("%s", input);

        // C0 - Sortie
        if (strcmp(input, "exit") == 0) {
            break;  // Sortir de la boucle
        }

        // C1 - Inscription
        else if (strcmp(input, "inscription") == 0) {
            scanf("%s %d", nom, &groupe);
            inscription(&listetu, nom, groupe);
        }

        // C2 - Enregistrement des absences
        else if (strcmp(input, "absence") == 0) {
            scanf("%d %d %s", &identifiant, &jour, demi_journee);
            enregistrer_absences(&listetu, &listabsence, identifiant, jour, demi_journee);
        }

        // C3 - Liste des etudiants
        else if (strcmp(input, "listes") == 0) {
            scanf("%d", &jour);
            listes(&listetu, &listabsence, jour);
        }

        // C4 - Justificatif
        else if (strcmp(input, "justificatif") == 0) {
            scanf("%d %d %s", &identifiant, &jour, justification);
            // justificatif();
        }

        // Commande inconnue
        else {
            printf("Commande inconnue\n");
        }
    }
}
