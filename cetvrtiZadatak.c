#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE          100
#define MEM_ALLOC_ERROR   -1
#define FILE_OPEN_ERROR   -2
#define SUCCESS_FUNC       1

typedef struct _poly {
    int eksponent;
    int koeficijent;
    struct _poly *next;
} polinom;

int sortiraj_unos(polinom *head, int eksponent, int koeficijent);
int citaj_polinom_iz_file(polinom *head, int poly_line);
int zbroj_polinoma(polinom *head_p, polinom *head_p1, polinom *head_p2);
int umnozak_polinoma(polinom *head_p, polinom *head_p1, polinom *head_p2);
int print_poly(polinom *head);
int free_mem(polinom *head);

int main() {
    polinom head_p  = { 0 };    
    polinom head_p1 = { 0 };
    polinom head_p2 = { 0 };
    
    citaj_polinom_iz_file(&head_p1, 1);
    printf("Prvi polinom: ");
    print_poly(&head_p1);
    printf("\n");

    citaj_polinom_iz_file(&head_p2, 2);
    printf("Drugi polinom: ");
    print_poly(&head_p2);
    printf("\n\n");

    zbroj_polinoma(&head_p, &head_p1, &head_p2);
    print_poly(&head_p1); printf("    +    "); print_poly(&head_p2); printf("    =    ");
    print_poly(&head_p); printf("\n\n");
    free_mem(&head_p);

    umnozak_polinoma(&head_p, &head_p1, &head_p2);
    print_poly(&head_p1); printf("    *    "); print_poly(&head_p2); printf("    =    ");
    print_poly(&head_p); printf("\n\n");    

    free_mem(&head_p);
    free_mem(&head_p1);
    free_mem(&head_p2);

    return 0;
}

int sortiraj_unos(polinom *head, int eksponent, int koeficijent) {
    if (koeficijent == 0) {
        return SUCCESS_FUNC;
    }

    polinom *current_poly = head;

    while (current_poly->next != NULL && current_poly->next->eksponent > eksponent) {
        current_poly = current_poly->next;
    }

    if (current_poly->next != NULL && current_poly->next->eksponent == eksponent) {
        current_poly->next->koeficijent += koeficijent;

        if (current_poly->next->koeficijent == 0) {
            polinom *temp       = current_poly->next;
            current_poly->next  = temp->next;
            free(temp);
        }

        return SUCCESS_FUNC;
    }

    polinom *new_poly = malloc(sizeof(polinom));
    if (new_poly == NULL) {
        printf("Neuspješna alokacija memorije!\n\n");
        return MEM_ALLOC_ERROR;
    }

    new_poly->eksponent     = eksponent;
    new_poly->koeficijent   = koeficijent;
    new_poly->next          = current_poly->next;
    current_poly->next      = new_poly;

    return SUCCESS_FUNC;
}

int citaj_polinom_iz_file(polinom *head, int poly_line) {
    FILE   *file_polinomi = fopen("polinomi.txt", "r");
    char    buffer[BUF_SIZE];
    int     koeficijent;
    int     eksponent;      
    int     current_poly_line = 0;

    if (file_polinomi == NULL) {
        printf("Otvaranje datoteke neuspješno!\n");
        return FILE_OPEN_ERROR;
    }

    while (fgets(buffer, BUF_SIZE, file_polinomi) != NULL) {
        current_poly_line++;

        if (current_poly_line == poly_line) {
            if (buffer[0] == '\n' || buffer[0] == '\0') {
                fclose(file_polinomi);
                return SUCCESS_FUNC;
            }

            char *misto_u_stringu = buffer;
            int   pomaK_u_stringu;

            while (sscanf_s(misto_u_stringu, "%d %d%n", 
                &koeficijent, &eksponent, &pomaK_u_stringu) == 2) {
                    sortiraj_unos(head, eksponent, koeficijent);
                    misto_u_stringu += pomaK_u_stringu;
            }
        }
    }

    fclose(file_polinomi);
    return SUCCESS_FUNC;
}

int zbroj_polinoma(polinom *head_p, polinom *head_p1, polinom *head_p2) {
    polinom *p1 = head_p1->next;
    polinom *p2 = head_p2->next;

    while (p1 != NULL) {
        sortiraj_unos(head_p, p1->eksponent, p1->koeficijent);
        p1 = p1->next;
    }

    while (p2 != NULL) {
        sortiraj_unos(head_p, p2->eksponent, p2->koeficijent);
        p2 = p2->next;
    }

    return SUCCESS_FUNC;
}

int umnozak_polinoma(polinom *head_p, polinom *head_p1, polinom *head_p2) {
    polinom *p1 = head_p1->next;
    int      new_koef;   
    int      new_eksp;

    while (p1 != NULL) {
        polinom *p2 = head_p2->next;
        while (p2 != NULL) {
            new_koef = p1->koeficijent * p2->koeficijent;
            new_eksp = p1->eksponent + p2->eksponent;
            
            sortiraj_unos(head_p, new_eksp, new_koef);
            
            p2 = p2->next;
        }
        p1 = p1->next;
    }

    return SUCCESS_FUNC;
}

int print_poly(polinom *head) {
    polinom *current_poly = head->next;
    if (current_poly == NULL) {
        printf("0\n");
        return SUCCESS_FUNC;
    }

    int prvi_clan   = 1;
    while (current_poly != NULL) {
        int koeficijent = current_poly->koeficijent;
        if (!prvi_clan && current_poly->koeficijent > 0) {
            printf(" + ");
        } else if (current_poly->koeficijent < 0) {
            printf(" - ");
            koeficijent = current_poly->koeficijent * -1;
        }

        if (current_poly->eksponent == 0) {
            printf("%d", koeficijent);
        } else if (current_poly->eksponent == 1) {
            printf("%dx", koeficijent);
        } else {
            printf("%dx^%d", koeficijent, current_poly->eksponent);
        }

        prvi_clan = 0;
        current_poly = current_poly->next;
    }

    return SUCCESS_FUNC;
}

int free_mem(polinom *head) {
    polinom *current_poly   = head->next;
    polinom *next_poly      = NULL;

    while (current_poly != NULL) {
        next_poly = current_poly->next;
        free(current_poly);
        current_poly = next_poly;
    }

    head->next = NULL;
    return SUCCESS_FUNC;
}