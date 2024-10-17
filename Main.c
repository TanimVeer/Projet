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
    JUSTI_MAX = 50,
};

typedef struct {
    char nom[NOM_CHAR_MAX];
    int groupe;
    int absences_non_justifiees;
} Etudiant;

typedef struct {
    int identifiant; 
    int jour;
    char demi_journee[DEMI_JOURNEE_MAX];
    char justification[MAX_CHAR];
    int justification_jour; 
    int justifie; 
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
    for (int i = 0; i < listEtu->cpt_etu; i++) {
        if (strcmp(listEtu->etudiants[i].nom, nom) == 0 && listEtu->etudiants[i].groupe == groupe) {
            printf("Nom incorrect\n");
            return;
        }
    }

    if (listEtu->cpt_etu < ETU_MAX) {
        strcpy(listEtu->etudiants[listEtu->cpt_etu].nom, nom);
        listEtu->etudiants[listEtu->cpt_etu].nom[NOM_CHAR_MAX] = '\0';
        listEtu->etudiants[listEtu->cpt_etu].groupe = groupe;
        listEtu->etudiants[listEtu->cpt_etu].absences_non_justifiees = 0;
        printf("Inscription enregistree (%d)\n", listEtu->cpt_etu + 1);
        listEtu->cpt_etu++;
    }

    else {
        printf("Limite d'étudiants atteinte.\n");
    }
}

/* --------------------------------------------------------------------------------------------------- */

void enregistrer_absences(ListAbsences* listabsence, ListEtu* listetu, int identifiant, int jour, char* demi_journee) {
    if (identifiant < ETU_MIN || identifiant > listetu->cpt_etu) {
        printf("Identifiant incorrect\n");
        return;
    }

    if (jour < JOUR_MIN || jour > JOUR_MAX) {
        printf("Date incorrecte\n");
        return;
    }

    if (strcmp(demi_journee, "am") != 0 && strcmp(demi_journee, "pm") != 0) {
        printf("Demi-journee incorrecte\n");
        return;
    }

    for (int i = 0; i < listabsence->cpt_absence; i++) {
        if (listabsence->absences[i].identifiant == identifiant && listabsence->absences[i].jour == jour && strcmp(listabsence->absences[i].demi_journee, demi_journee) == 0) {
            printf("Absence deja connue\n");
            return;
        }
    }

    if (listabsence->cpt_absence < ABSENCES_MAX) {
        listabsence->absences[listabsence->cpt_absence].identifiant = identifiant;
        listabsence->absences[listabsence->cpt_absence].jour = jour;
        strncpy(listabsence->absences[listabsence->cpt_absence].demi_journee, demi_journee, DEMI_JOURNEE_MAX);
        listabsence->cpt_absence++;
        listetu->etudiants[identifiant].absences_non_justifiees++;
        printf("Absence enregistree [%d]\n", listabsence->cpt_absence);
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
    if (jour_courant < JOUR_MIN) {
        printf("Date incorrecte\n");
        return;
    }

    if (listetu->cpt_etu == 0) {
        printf("Aucun inscrit\n");
        return;
    }

    int absencesParEtu[ETU_MAX] = { 0 };

    for (int i = 0; i < listeabsences->cpt_absence; i++) {
        if (listeabsences->absences[i].jour <= jour_courant) {
            absencesParEtu[listeabsences->absences[i].identifiant - 1]++;
        }
    }

    qsort(listetu->etudiants, listetu->cpt_etu, sizeof(Etudiant), tri);

    for (int i = 0; i < listetu->cpt_etu; i++) {
        printf("(%d) %s %d %d\n", i+1, listetu->etudiants[i].nom, listetu->etudiants[i].groupe, absencesParEtu[i]);
    }
}

/* --------------------------------------------------------------------------------------------------- */

void justificatif(ListAbsences* listabsence, int identifiant, int jour, char* justification) {
    if (identifiant < ETU_MIN || identifiant > ETU_MAX) {
        printf("Identifiant incorrect");
        return;
    }
    
    if (jour < JOUR_MIN || jour > JOUR_MAX) {
        printf("Date incorrecte");
        return;
    }

    for (int i = 0; i < listabsence->cpt_absence; i++) {
        if (listabsence->absences[i].identifiant == identifiant && strcmp(listabsence->absences[i].justification, justification) == 0) {
            printf("Justificatif deja connu\n");
            return;
        }
    }

    strcpy(listabsence->absences[identifiant].justification, justification);
    listabsence->absences[identifiant].justification_jour = jour;
    printf("Justificatif enregistre\n");
}

/* --------------------------------------------------------------------------------------------------- */

void validations() {
//    printf("[%d] (%d) %s %d %d/%s (%s)", ?? );
}

/* --------------------------------------------------------------------------------------------------- */

int main() {
    ListEtu listetu;
    listetu.cpt_etu = 0;
    ListAbsences listabsence;
    listabsence.cpt_absence = 0;
    char input[MAX_CHAR];
    char nom[NOM_CHAR_MAX];
    int groupe;
    int identifiant; 
    int jour;
    char demi_journee[DEMI_JOURNEE_MAX];
    char justification[JUSTI_MAX];

    while (1) {
        scanf("%s", input);

        // C0 - Sortie
        if (strcmp(input, "exit") == 0) {
            break;
        }

        // C1 - Inscription
        else if (strcmp(input, "inscription") == 0) {
            scanf("%s %d", nom, &groupe);
            inscription(&listetu, nom, groupe);
        }

        // C2 - Enregistrement des absences
        else if (strcmp(input, "absence") == 0) {
            scanf("%d %d %s", &identifiant, &jour, demi_journee);
            enregistrer_absences(&listabsence, &listetu, identifiant, jour, demi_journee);
        }

        // C3 - Liste des etudiants
        else if (strcmp(input, "etudiants") == 0) {
            scanf("%d", &jour);
            listes(&listetu, &listabsence, jour);
        }

        // C4 - Justificatif
        else if (strcmp(input, "justificatif") == 0) {
            scanf("%d %d ", &identifiant, &jour);
            fgets(justification, JUSTI_MAX, stdin);
            justificatif(&listabsence, identifiant, jour, justification);
        }

        // C5 - Absences en attente de validation
        else if (strcmp(input, "validations") == 0) {
            validations();
        }

        // Commande inconnue
        else {
            printf("Commande inconnue\n");
        }
    }
}
