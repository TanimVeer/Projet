#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#pragma warning (disable : 4996 6031)

enum { NOM_CHAR_MAX = 30, ETU_MAX = 100, ABSENCES_MAX = 5, JUSTIFICATIF_CHAR_MAX = 50 };
char input [NOM_CHAR_MAX];

// C1
void inscription(){
    char nom[NOM_CHAR_MAX];
    int groupe;
    static int cpt = 0;

    scanf("%s %d", nom, &groupe);
    if (strstr(input, inscription) != 0) {
        cpt++;
        printf("Inscription enregistree (%d)\n", cpt);
    }
    else if (nom > NOM_CHAR_MAX) {
        printf("Nom incorrect\n");
    }
    else if (strstr(input, inscription) != 1) {
        printf("Erreur\n");
    }
}



int main() {
    do {
        printf("Choisir une fonction : inscription, absence, etudiants, justificatif, defaillants, exit\n");
        scanf("%s", &input);

        if (strcmp(input, "inscription") == 0) {
            inscription();
        }
        /* else if (strcmp(input, "absence") == 0) {
            absences();
        }
        else if (strcmp(input, "etudiants") == 0) {
            etudiants();
        }
        else if (strcmp(input, "justificatif") == 0) {
            justificatif();
        }
        else if (strcmp(input, "defaillants") == 0) {
            defaillants();
        } */
        else {
            printf("Commande inconnue\n");
        }
    } while (strcmp(input, "exit") != 0); // C0
}
