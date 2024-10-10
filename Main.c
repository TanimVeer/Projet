#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#pragma warning (disable : 4996 6031)

enum {CHAR_MAX = 30};
enum {MAX_ETU = 100};
enum {MAX_ABSENCES = 5};

// C1

/*
void inscription{
    char prenom[CHAR_MAX];
    int groupe;
    int cpt = 0;
    if (strcmp(input, "inscription") == 0) {
        scanf("%s %d", prenom, &groupe);
        if (strstr(input, inscription) != 0) {
            cpt++;
            printf("Inscription enregistree (%d)", cpt);
        }
        else {
            printf("Nom incorrect);
        }
    }
}
*/


int main() {
    char input[CHAR_MAX];
    do {
        printf("Choisir une fonction : (inscription, absence, etudiants, justificatif, defaillants, exit) : ");
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
