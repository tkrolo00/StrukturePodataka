#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_SIZE       100
#define BUF_SIZE        100
#define MEM_ALLOC_ERROR -1
#define EMPTY_LIST      -2
#define SUCCES_FUNC     1

typedef struct _osoba {
    char   ime[NAME_SIZE];
    char   prezime[NAME_SIZE];
    int    godina_rodenja;
    struct _osoba *next;
} osoba;

int    salji_na_pocetak(osoba **head, const char *ime, const char *prezime, int godina_rodenja);
int    salji_na_kraj(osoba **head, const char *ime, const char *prezime, int godina_rodenja);
int    print_list(osoba *head);
osoba *pronadi_element_u_listi(osoba *head, const char *prezime);
int    delete_element(osoba **head, const char *prezime);
int    free_mem(osoba **head);

int main() {
    osoba *head = NULL;
    osoba *find_prezime = NULL;
    char   prezime[NAME_SIZE] = { 'K', 'r', 'o', 'l', 'o', '\0' };

    salji_na_pocetak(&head, "Toni", "Krolo", 2000);
    salji_na_pocetak(&head, "Tony", "Adams", 1966);
    print_list(head);

    salji_na_kraj(&head, "Tony", "Montana", 1942);
    print_list(head);

    find_prezime = pronadi_element_u_listi(head, prezime);
    if (find_prezime != NULL) {
        printf("Pronađen: %s %s, godina rođenja: %d\n", find_prezime->ime,
            find_prezime->prezime, find_prezime->godina_rodenja);
    }
    else {
        printf("Prezime %s nije pronađeno!\n", prezime);
    }

    delete_element(&head, "Krolo");

    free_mem(&head);

    return 0;
}

int salji_na_pocetak(osoba **head, const char *ime, const char *prezime, int godina_rodenja) {
    osoba *new_list = malloc(sizeof(osoba));
    if (new_list == NULL) {
        printf("Neuspjesno alociranje memorije!\n");
        return MEM_ALLOC_ERROR;
    }

    strcpy_s(new_list->ime, sizeof(new_list->ime), ime);
    strcpy_s(new_list->prezime, sizeof(new_list->prezime), prezime);
    new_list->godina_rodenja = godina_rodenja;

    new_list->next = *head;
    *head = new_list;
    printf("Ime %s %s dodano na početak liste\n", ime, prezime);

    return SUCCES_FUNC;
}

int print_list(osoba *head) {
    osoba *current_list = head;

    if (head == NULL) {
        printf("Lista je prazna!\n");
        return EMPTY_LIST;
    }

    while (current_list != NULL) {
        printf("Ime i prezime: %s %s, godina rođenja: %d\n", current_list->ime,
            current_list->prezime, current_list->godina_rodenja);

        current_list = current_list->next;
    }

    return SUCCES_FUNC;
}

int salji_na_kraj(osoba **head, const char *ime, const char *prezime, int godina_rodenja) {
    osoba *new_list = malloc(sizeof(osoba));
    osoba *current_list = *head;

    if (new_list == NULL) {
        printf("Neuspjesno alociranje memorije!\n");
        exit(MEM_ALLOC_ERROR);
    }

    strcpy_s(new_list->ime, sizeof(new_list->ime), ime);
    strcpy_s(new_list->prezime, sizeof(new_list->prezime), prezime);
    new_list->godina_rodenja = godina_rodenja;

    new_list->next = NULL;
    if (*head == NULL) {
        *head = new_list;
    }
    else {
        while (current_list->next != NULL) {
            current_list = current_list->next;
        }
        current_list->next = new_list;
    }

    printf("Ime %s %s dodano na kraj liste\n", ime, prezime);

    return SUCCES_FUNC;
}

osoba *pronadi_element_u_listi(osoba *head, const char *prezime) {
    osoba *current_list = head;

    while (current_list != NULL) {
        if (strcmp(current_list->prezime, prezime) == 0) {
            return current_list;
        }
        current_list = current_list->next;
    }

    printf("Nije pronađeno prezime %s!\n", prezime);
    return NULL;
}

int delete_element(osoba **head, const char *prezime) {
    osoba *current_list = *head;
    osoba *previous_list = NULL;

    if (*head == NULL) {
        printf("Lista je prazna!\n");
        return EMPTY_LIST;
    }

    if (strcmp(current_list->prezime, prezime) == 0) {
        *head = current_list->next;
        free(current_list);
        printf("Prezime %s je obrisano!\n", prezime);
        return SUCCES_FUNC;
    }

    while (current_list != NULL && strcmp(current_list->prezime, prezime) != 0) {
        previous_list = current_list;
        current_list = current_list->next;
    }

    if (current_list == NULL) {
        printf("Prezime %s nije pronađeno!\n", prezime);
        return SUCCES_FUNC;
    }

    previous_list->next = current_list->next;
    free(current_list);
    printf("Prezime %s obrisano!\n", prezime);

    return SUCCES_FUNC;
}

int free_mem(osoba **head) {
    osoba *current_list = *head;
    osoba *next_list = NULL;

    while (current_list != NULL) {
        next_list = current_list->next;
        free(current_list);
        current_list = next_list;
    }

    *head = NULL;

    return SUCCES_FUNC;
}