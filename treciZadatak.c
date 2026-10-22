#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_SIZE         100
#define BUF_SIZE          100
#define MEM_ALLOC_ERROR   -1
#define EMPTY_LIST        -2
#define SURNAME_NOT_FOUND -3
#define FILE_OPEN_ERROR   -4
#define SUCCES_FUNC       1

typedef struct _osoba {
    char   ime[NAME_SIZE];
    char   prezime[NAME_SIZE];
    int    godina_rodenja;
    struct _osoba *next;
} osoba;

// 2. Zadatak
int    salji_na_pocetak(osoba *head, const char *ime, const char *prezime, int godina_rodenja);
int    salji_na_kraj(osoba *head, const char *ime, const char *prezime, int godina_rodenja);
int    print_list(osoba *head);
osoba *pronadi_element_u_listi(osoba *head, const char *prezime);
int    delete_element(osoba *head, const char *prezime);
int    free_mem(osoba *head);
// 3. Zadatak
int    dodaj_iza(osoba *head, char *trazeno_prezime, char *ime, char *prezime, int godina_rodenja);
int    dodaj_isprid(osoba *head, char *trazeno_prezime, char *ime, char *prezime, int godina_rodenja);
int    sortiraj_listu(osoba *head);
int    upis_u_file(osoba *head);
int    citanje_file_u_listu(osoba *head);

int main() {
    osoba   head = { 0 };
    osoba  *find_prezime       = NULL;
    char    prezime[NAME_SIZE] = { 'K', 'r', 'o', 'l', 'o', '\0' };
    // 2. Zadatak varijable

    // 2. Zadatak
    salji_na_pocetak(&head, "Toni", "Krolo", 2000);
    salji_na_pocetak(&head, "Tony", "Adams", 1966);
    print_list(&head);

    salji_na_kraj(&head, "Tony", "Montana", 1942);
    salji_na_kraj(&head, "Tony", "Soprano", 1959);
    print_list(&head);

    find_prezime = pronadi_element_u_listi(&head, prezime);
    if (find_prezime != NULL) {
        printf("Pronađen: %s %s, godina rođenja: %d\n", find_prezime->ime,
            find_prezime->prezime, find_prezime->godina_rodenja);
    }
    else {
        printf("Prezime %s nije pronađeno!\n", prezime);
    }

    printf("\n\n3. Zadatak\n\n");

    // 3. Zadatak
    dodaj_iza(&head, "Krolo", "Toni", "Kroos", 1990);
    dodaj_isprid(&head, "Krolo", "Tony", "Cipriani", 1968);
    print_list(&head);
    printf("\n");

    sortiraj_listu(&head);
    upis_u_file(&head);

    free_mem(&head);
    citanje_file_u_listu(&head);
    printf("\n\n");
    print_list(&head);

    //delete_element(&head, "Krolo"); // 2. Zadatak

    free_mem(&head);

    return 0;
}

// 2. Zadatak
int salji_na_pocetak(osoba *head, const char *ime, const char *prezime, int godina_rodenja) {
    osoba *new_person = malloc(sizeof(osoba));
    if (new_person == NULL) {
        printf("Neuspjesno alociranje memorije!\n");
        return MEM_ALLOC_ERROR;
    }

    strcpy_s(new_person->ime, sizeof(new_person->ime), ime);
    strcpy_s(new_person->prezime, sizeof(new_person->prezime), prezime);
    new_person->godina_rodenja = godina_rodenja;

    new_person->next = head->next;
    head->next = new_person;
    printf("Ime %s %s dodano na početak liste\n", ime, prezime);

    return SUCCES_FUNC;
}

int print_list(osoba *head) {
    osoba *current_person = head->next;

    if (current_person == NULL) {
        printf("Lista je prazna!\n");
        return EMPTY_LIST;
    }

    while (current_person != NULL) {
        printf("Ime i prezime: %s %s, godina rođenja: %d\n", current_person->ime,
            current_person->prezime, current_person->godina_rodenja);

        current_person = current_person->next;
    }

    return SUCCES_FUNC;
}

int salji_na_kraj(osoba *head, const char *ime, const char *prezime, int godina_rodenja) {
    osoba *new_person = malloc(sizeof(osoba));
    osoba *current_person = head->next;

    if (new_person == NULL) {
        printf("Neuspjesno alociranje memorije!\n");
        return MEM_ALLOC_ERROR;
    }

    strcpy_s(new_person->ime, sizeof(new_person->ime), ime);
    strcpy_s(new_person->prezime, sizeof(new_person->prezime), prezime);
    new_person->godina_rodenja = godina_rodenja;

    new_person->next = NULL;
    if (head->next == NULL) {
        head->next = new_person;
    }
    else {
        while (current_person->next != NULL) {
            current_person = current_person->next;
        }
        current_person->next = new_person;
    }

    printf("Ime %s %s dodano na kraj liste\n", ime, prezime);

    return SUCCES_FUNC;
}

osoba *pronadi_element_u_listi(osoba *head, const char *prezime) {
    osoba *current_person = head->next;

    while (current_person != NULL) {
        if (strcmp(current_person->prezime, prezime) == 0) {
            return current_person;
        }
        current_person = current_person->next;
    }

    printf("Nije pronađeno prezime %s!\n", prezime);
    return NULL;
}

int delete_element(osoba *head, const char *prezime) {
    osoba *current_person = head->next;
    osoba *previous_person = head;

    if (current_person == NULL) {
        printf("Lista je prazna!\n");
        return EMPTY_LIST;
    }

    while (current_person != NULL && strcmp(current_person->prezime, prezime) != 0) {
        previous_person = current_person;
        current_person = current_person->next;
    }

    if (current_person == NULL) {
        printf("Prezime %s nije pronađeno!\n", prezime);
        return SURNAME_NOT_FOUND;
    }

    previous_person->next = current_person->next;
    free(current_person);
    printf("Prezime %s obrisano!\n", prezime);

    return SUCCES_FUNC;
}

int free_mem(osoba *head) {
    osoba *current_person = head->next;
    osoba *next_person    = NULL;

    while (current_person != NULL) {
        next_person = current_person->next;
        free(current_person);
        current_person = next_person;
    }

    head->next = NULL;

    return SUCCES_FUNC;
}

// 3. Zadatak
// A)
int dodaj_iza(osoba *head, char *trazeno_prezime, char *ime, char *prezime, int godina_rodenja) {
    osoba *new_person       = malloc(sizeof(osoba));

    if (new_person == NULL) {
        printf("Alokacija memorije neuspješna!\n");
        return MEM_ALLOC_ERROR;
    }

    osoba *current_person   = pronadi_element_u_listi(head, trazeno_prezime);
 
    if (current_person == NULL) {
        free(new_person);
        new_person = NULL;

        return SURNAME_NOT_FOUND;
    }        

    strcpy_s(new_person->ime, sizeof(new_person->ime), ime);
    strcpy_s(new_person->prezime, sizeof(new_person->prezime), prezime);
    new_person->godina_rodenja = godina_rodenja;

    new_person->next     = current_person->next;
    current_person->next = new_person;

    return SUCCES_FUNC;
}

// B)
int dodaj_isprid(osoba *head, char *trazeno_prezime, char *ime, char *prezime, int godina_rodenja) {
    osoba *new_person       = malloc(sizeof(osoba));

    if (new_person == NULL) {
        printf("Alokacija memorije neuspješna!\n");
        return MEM_ALLOC_ERROR;
    }

    osoba *current_person   = pronadi_element_u_listi(head, trazeno_prezime);

    if (current_person == NULL) {
        free(new_person);
        new_person = NULL;

        return SURNAME_NOT_FOUND;
    }   

    strcpy_s(new_person->ime, sizeof(new_person->ime), ime);
    strcpy_s(new_person->prezime, sizeof(new_person->prezime), prezime);
    new_person->godina_rodenja = godina_rodenja;
    
    osoba *previous_person  = head;    

    while (previous_person->next != current_person) {
        previous_person = previous_person->next;
    }

    previous_person->next = new_person;
    new_person->next = current_person;

    return SUCCES_FUNC;
}

// C)
int sortiraj_listu(osoba *head) {
    osoba *current = head->next;
    osoba *next_person;
    osoba *sorted = NULL;

    while (current != NULL) {
        next_person = current->next;

        osoba **mjesto = &sorted;
        while (*mjesto != NULL &&
               strcmp((*mjesto)->prezime, current->prezime) <= 0) {
            mjesto = &(*mjesto)->next;
        }

        current->next = *mjesto;
        *mjesto = current;
        current = next_person;
    }

    head->next = sorted;
    return SUCCES_FUNC;
}

// D)
int upis_u_file(osoba *head) {
    FILE *lista_person = fopen("treciZadatak_lista_osobe.txt", "w");
    
    if (lista_person == NULL) {
        printf("Datoteka nije uspješno otvorena!");
        return FILE_OPEN_ERROR;
    }

    if (head->next == NULL) {
        printf("Lista je prazna!\n");
        fclose(lista_person);
        return EMPTY_LIST;
    }

    osoba *current_person = head->next;

    while (current_person != NULL ) {
        fprintf(lista_person, "%s %s %d\n", current_person->ime, current_person->prezime, current_person->godina_rodenja);
        current_person = current_person->next;
    }

    fclose(lista_person);
    return SUCCES_FUNC;
}

// E)

int citanje_file_u_listu(osoba *head) {
    FILE *lista_person = fopen("treciZadatak_lista_osobe.txt", "r");
    
    if (lista_person == NULL) {
        printf("Datoteka nije uspješno otvorena!");
        return FILE_OPEN_ERROR;
    }    

    char  buffer[BUF_SIZE];
    char  ime[NAME_SIZE];
    char  prezime[NAME_SIZE];
    int   godina_rodenja;
    int   check_arg;

    while (fgets(buffer, BUF_SIZE, lista_person) != NULL) {
        check_arg = sscanf_s(buffer, "%s %s %d", ime, sizeof(ime), 
        prezime, sizeof(prezime), 
        &godina_rodenja);
        
        if (check_arg == 3) {
            salji_na_kraj(head, ime, prezime, godina_rodenja);
        }
        else continue;
    }

    fclose(lista_person);
    return SUCCES_FUNC;
}

