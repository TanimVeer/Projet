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
    CHAR_RESULTAT = 3
};

typedef struct {
    char nom[NOM_CHAR_MAX];
    int groupe;
    int absences_non_justifiees;
    int ordre_etudiants;
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
    int ordre_etudiants[ETU_MAX];
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
        listEtu->etudiants[listEtu->cpt_etu].groupe = groupe;
        listEtu->etudiants[listEtu->cpt_etu].absences_non_justifiees = 0;
        listEtu->etudiants[listEtu->cpt_etu].ordre_etudiants = listEtu->cpt_etu + 1;
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
        strcpy(listabsence->absences[listabsence->cpt_absence].demi_journee, demi_journee);
        listabsence->absences[listabsence->cpt_absence].justifie = 0;
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
        printf("(%d) %s %d %d\n", listetu->etudiants[i].ordre_etudiants, listetu->etudiants[i].nom, listetu->etudiants[i].groupe, absencesParEtu[i]);
    }
}

/* --------------------------------------------------------------------------------------------------- */

void justificatif(ListAbsences* listabsence, int id_absence, int jour, char* justification) {
    size_t len = strlen(justification);
    if (justification[len-1] == '\n') {
        justification[len-1] = '\0';
    }
    if (id_absence < ETU_MIN || id_absence > ETU_MAX) {
        printf("Identifiant incorrect");
        return;
    }
    
    if (jour < JOUR_MIN) {
        printf("Date incorrecte");
        return;
    }

    for (int i = 0; i < listabsence->cpt_absence; i++) {
        if (listabsence->absences[i].identifiant == id_absence && strcmp(listabsence->absences[i].justification, justification) == 0) {
            printf("Justificatif deja connu\n");
            return;
        }
    }

    strncpy(listabsence->absences[id_absence-1].justification, justification, JUSTI_MAX);
    listabsence->absences[id_absence-1].justification_jour = jour;
    listabsence->absences[id_absence-1].justifie = 0;
    printf("Justificatif enregistre\n");
}

/* --------------------------------------------------------------------------------------------------- */

void validations(ListEtu* listetu, ListAbsences* listeabsences) {
    int validation = 0; 

    for (int i = 0; i < listeabsences->cpt_absence; i++) {
        if (listeabsences->absences[i].justifie == 0) { 
            validation = 1; 

            int identifiant = listeabsences->absences[i].identifiant;

            printf("[%d] (%d) %s %d %d/%s (%s)\n",
                listeabsences->absences[i].identifiant,
                listetu->etudiants[i].ordre_etudiants,
                listetu->etudiants[i].nom,
                listetu->etudiants[i].groupe,
                listeabsences->absences[i].jour,
                listeabsences->absences[i].demi_journee,
                listeabsences->absences[i].justification);
        }
    }

    if (!validation) {
        printf("Aucune validation en attente\n"); 
    }
}

/* --------------------------------------------------------------------------------------------------- */

void validation_justificatif(ListAbsences* listeabsences, int id_absence, const char* resultat) {
    if (id_absence < 0 || id_absence >= listeabsences->cpt_absence) {
        printf("Identifiant incorrect\n");
        return;
    }

    if (listeabsences->absences[id_absence].justifie != 0) {
        printf("Validation deja connue\n");
        return;
    }

    if (strcmp(resultat, "ok") != 0 && strcmp(resultat, "ko") != 0) {
        printf("Code incorrect\n");
        return;
    }

    if (strcmp(resultat, "ok") == 0) {
        listeabsences->absences[id_absence].justifie = 1;
        printf("Validation enregistree\n");
    }
    else if (strcmp(resultat, "ko") == 0) {
        listeabsences->absences[id_absence].justifie = -1;
        printf("Validation enregistree\n");
    }
}

/* --------------------------------------------------------------------------------------------------- */

void situation_etudiant(ListEtu* listetu, ListAbsences* listeabsences, int identifiant, int jour) {
    if (identifiant < ETU_MIN || identifiant > listetu->cpt_etu) {
        printf("Identifiant incorrect\n");
        return;
    }

    if (jour < JOUR_MIN) {
        printf("Date incorrecte\n");
        return;
    }

    Etudiant* etudiant = &listetu->etudiants[identifiant];
    printf("(%d) %s %d %d\n", identifiant, etudiant->nom, etudiant->groupe, etudiant->absences_non_justifiees);

    Absence en_attente_justificatif[ABSENCES_MAX];
    Absence en_attente_validation[ABSENCES_MAX];
    Absence justifiees[ABSENCES_MAX];
    Absence non_justifiees[ABSENCES_MAX];

    int nb_attente_justificatif = 0;
    int nb_attente_validation = 0;
    int nb_justifiees = 0;
    int nb_non_justifiees = 0;

    for (int i = 0; i < listeabsences->cpt_absence; i++) {
        if (listeabsences->absences[i].identifiant == identifiant && listeabsences->absences[i].jour < jour) {
            if (listeabsences->absences[i].justifie == 0) {
                if (strlen(listeabsences->absences[i].justification) == 0) {
                    en_attente_justificatif[nb_attente_justificatif++] = listeabsences->absences[i];
                }
                else {
                    en_attente_validation[nb_attente_validation++] = listeabsences->absences[i];
                }
            }
            else if (listeabsences->absences[i].justifie == 1) {
                justifiees[nb_justifiees++] = listeabsences->absences[i];
            }
            else {
                non_justifiees[nb_non_justifiees++] = listeabsences->absences[i];
            }
        }
    }

    if (nb_attente_justificatif > 0) {
        printf("- En attente justificatif\n");
        for (int i = 0; i < nb_attente_justificatif; i++) {
            printf("[%d] %d/%s\n",
                en_attente_justificatif[i].identifiant,
                en_attente_justificatif[i].jour,
                en_attente_justificatif[i].demi_journee);
        }
    }

    if (nb_attente_validation > 0) {
        printf("- En attente validation\n");
        for (int i = 0; i < nb_attente_validation; i++) {
            printf("[%d] %d/%s (%s)\n",
                en_attente_validation[i].identifiant,
                en_attente_validation[i].jour,
                en_attente_validation[i].demi_journee,
                en_attente_validation[i].justification);
        }
    }

    if (nb_justifiees > 0) {
        printf("- Justifiees\n");
        for (int i = 0; i < nb_justifiees; i++) {
            printf("[%d] %d/%s (%s)\n",
                justifiees[i].identifiant,
                justifiees[i].jour,
                justifiees[i].demi_journee,
                justifiees[i].justification);
        }
    }

    if (nb_non_justifiees > 0) {
        printf("- Non-justifiees\n");
        for (int i = 0; i < nb_non_justifiees; i++) {
            printf("[%d] %d/%s\n",
                non_justifiees[i].identifiant,
                non_justifiees[i].jour,
                non_justifiees[i].demi_journee);
        }
    }
}

/* --------------------------------------------------------------------------------------------------- */

void liste_defaillants(ListEtu* listetu, ListAbsences* listeabsences, int jour_courant) {
    if (jour_courant < JOUR_MIN) {
        printf("Date incorrecte\n");
        return;
    }

    int found_defaillant = 0;

    for (int i = 0; i < listetu->cpt_etu; i++) {
        int total_absences = 0;
        int non_justifiees = listetu->etudiants[i].absences_non_justifiees;

        for (int j = 0; j < listeabsences->cpt_absence; j++) {
            if (listeabsences->absences[j].identifiant == i + 1 && listeabsences->absences[j].jour < jour_courant) {
                total_absences++;
            }
        }

        if (non_justifiees >= 5) {
            printf("(%d) %s %d %d\n", i + 1, listetu->etudiants[i].nom, listetu->etudiants[i].groupe, total_absences);
            found_defaillant = 1;
        }
    }

    if (!found_defaillant) {
        printf("Aucun defaillant\n");
    }
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
            validations(&listetu, &listabsence);
        }

        // C6 - Validation/invalidation d’un justificatif
        else if (strcmp(input, "validation") == 0) {
            int id_absence;
            char resultat[CHAR_RESULTAT];
            scanf("%d %s", &id_absence, resultat);
            validation_justificatif(&listabsence, id_absence, resultat);
        }

        // C7 - Situation d’un étudiant
        else if (strcmp(input, "etudiant") == 0) {
            scanf("%d %d", &identifiant, &jour);
            situation_etudiant(&listetu, &listabsence, identifiant, jour);
        }

        // C8 - Liste des étudiants défaillants
        else if (strcmp(input, "defaillants") == 0) {
            scanf("%d", &jour);
            liste_defaillants(&listetu, &listabsence, jour);
        }

        // Commande inconnue
        else {
            printf("Commande inconnue\n");
        }
    }
}
