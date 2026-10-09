#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_SIZE           100
#define BUF_SIZE            100
#define MAX_BODOVI          98  
#define FILE_OPEN_ERR       -1
#define MISSING_ITEM_ERR    -2
#define MAX_BODOVI_EXCEEDED -3
#define EARLY_EXIT_ERR      -4

typedef struct _student {
    char ime[NAME_SIZE];
    char prezime[NAME_SIZE];
    int  broj_bodova;
} student;

FILE    *otvori_datoteku(const char *ime_datoteke, const char *radnja);
int      counter_studenata(FILE *datoteka, int brojac);
void     provjera_brojac(int brojac);
void     read_file_to_struct(student *lista, FILE *datoteka, int broj_studenata);
void     print_struct(student *lista, int broj_studenata);
student *mem_alloc(int broj_studenata);

int main() {
    FILE    *datoteka       = otvori_datoteku("studenti.txt", "r");
    student *lista          = NULL;
    int      broj_studenata = 0;

    broj_studenata = counter_studenata(datoteka, broj_studenata);
    provjera_brojac(broj_studenata);

    lista = mem_alloc(broj_studenata);
    read_file_to_struct(lista, datoteka, broj_studenata);
    print_struct(lista, broj_studenata);

    fclose(datoteka);
    free(lista);

    return 0;
}

FILE *otvori_datoteku(const char *ime_datoteke, const char *radnja) {
    FILE *datoteka = fopen(ime_datoteke, radnja);

    if (datoteka == NULL) {
        printf("Neuspjesno otvaranje datoteke %s!\n", ime_datoteke);
        exit(FILE_OPEN_ERR);
    }

    return datoteka;
}

int counter_studenata(FILE *datoteka, int brojac) {
    char buffer[BUF_SIZE];

    while (fgets(buffer, BUF_SIZE, datoteka) != NULL) {
        if (buffer[0] != '\n') {
            brojac++;
        }
    }

    rewind(datoteka);
    return brojac;
}

void provjera_brojac(int brojac) {
    printf("Broj redova je: %d\n", brojac);
}

student *mem_alloc(int broj_studenata) {
    student *lista = malloc(sizeof(student) * broj_studenata);
    return lista;
}

void read_file_to_struct(student *lista, FILE *datoteka, int broj_studenata) {
    int  i = 0;
    int  check_arg = 0;
    char buffer[BUF_SIZE];

    while (i < broj_studenata) {
        //check u slucaju da je prazan red između imena i fgets dode do kraja datoteke prije loadanja svih studenata u struct
        if (fgets(buffer, BUF_SIZE, datoteka) == NULL) {
            printf("Nisu ucitani svi studenti prije kraja datoteke!\n");
            fclose(datoteka);
            free(lista);
            exit(EARLY_EXIT_ERR);
        }

        //value za provjeru jesu li svi argumenti loadani
        check_arg = sscanf_s(buffer, "%s %s %d", lista[i].ime, (unsigned int)sizeof(lista[i].ime),
            lista[i].prezime, (unsigned int)sizeof(lista[i].prezime),
            &lista[i].broj_bodova);

        // Ne prekida program u slučaju new linea ili EOF koji vraća -1
        if (check_arg <= 0) {
            continue;
        }

        //sva tri argumenta (ime, prezime, bodovi) loadani
        if (check_arg == 3) {
            if (lista[i].broj_bodova > MAX_BODOVI) {
                printf("Broj bodova ne smije biti veci od maksimalnog broja bodova!\n");
                fclose(datoteka);
                free(lista);
                exit(MAX_BODOVI_EXCEEDED);
            }
            else {
                i++;
            }
        }
        else {
            printf("Nedovoljno podataka!\n");
            fclose(datoteka);
            free(lista);
            exit(MISSING_ITEM_ERR);
        }
    }
}

void print_struct(student *lista, int broj_studenata) {
    int i = 0;

    while (i < broj_studenata) {
        printf("Ime i prezime: %s %s, %d, %.2f\n", lista[i].ime, lista[i].prezime,
            lista[i].broj_bodova, (((float)lista[i].broj_bodova / MAX_BODOVI) * 100));
        i++;
    }
}
