#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE                100
#define MEM_ALLOC_ERROR         -1
#define FILE_OPEN_ERROR         -2
#define EMPTY_FILE_ERROR        -3
#define FORMAT_ERROR            -4
#define OP_ERROR                -5
#define DIVIDE_BY_ZERO          -6
#define MISSING_RESULT_ERROR    -7
#define EMPTY_STACK_ERROR       -8
#define SUCCESS_FUNC             1

typedef struct _stack {
    float          broj;
    struct _stack *next;
} stack;

int push(stack *head, float element);       
int pop(stack *head, float *element);
int izracunaj_postfiks(stack *head, const char *ime_datoteke, float *rezultat);
int izvrsi_operaciju(char operator, float clan1, float clan2, float *rezultat);
int free_mem(stack *head);

int main() {
    stack head     = { 0 };
    float rezultat = 0;

    izracunaj_postfiks(&head, "petiZadatakPostfiks.txt", &rezultat);
    printf("Rezulat je: %f\n\n", rezultat);

    free_mem(&head);
    return 0;
}

int push(stack *head, float element) {
    stack *new_cvor = malloc(sizeof(stack));

    if (new_cvor == NULL) {
        printf("Neuspješna alokacija memorije!\n\n");
        return MEM_ALLOC_ERROR;
    }

    new_cvor->broj = element;
    new_cvor->next = head->next;
    head->next     = new_cvor;
    
    return SUCCESS_FUNC;
}

int pop(stack *head, float *element) {
    if (head->next == NULL) {
        printf("Stack je prazan!\n\n");
        return EMPTY_STACK_ERROR;
    }

    stack *temp = head->next;
    *element    = temp->broj;
    head->next  = temp->next;
    
    free(temp);

    return SUCCESS_FUNC;
}

int izvrsi_operaciju(char operator, float clan1, float clan2, float *rezultat) {
    switch (operator) {
        case '+':
            *rezultat = clan1 + clan2;
            break;
        case '-':
            *rezultat = clan1 - clan2;
            break;
        case '*':
            *rezultat = clan1 * clan2;
            break;
        case '/':
            if (clan2 == 0) {
                printf("Pogreška: Dijeljenje s nulom!\n");
                return DIVIDE_BY_ZERO;
            }
            *rezultat = clan1 / clan2;
            break;
        default:
            printf("Nepoznat operator: %c\n", operator);
            return OP_ERROR;
    }
    return SUCCESS_FUNC;
}

int izracunaj_postfiks(stack *head, const char *ime_datoteke, float *rezultat) {
    FILE *file = fopen(ime_datoteke, "r");
     
    if (file == NULL) {
        printf("Greška pri otvaranju datoteke!\n\n");
        return FILE_OPEN_ERROR;
    }

    char buffer[BUF_SIZE];
    
    if (fgets(buffer, BUF_SIZE, file) == NULL) {
        printf("Datoteka je prazna!\n");
        fclose(file);
        return EMPTY_FILE_ERROR;
    }
    fclose(file);

    char *token    = strtok(buffer, " \t\n");
    float element;
    float clan1;
    float clan2;
    float racun;

    while (token != NULL) {
        if (sscanf(token, "%f", &element) == 1 && !(strlen(token) == 1 && (token[0] == '+' || token[0] == '-' || token[0] == '*' || token[0] == '/'))) {
            push(head, element);
        }
        else {
            if (pop(head, &clan2) != SUCCESS_FUNC || pop(head, &clan1) != SUCCESS_FUNC) {
                printf("Neispravan izraz!\n\n");
                free_mem(head);
                return FORMAT_ERROR;
            } 

            if (izvrsi_operaciju(token[0], clan1, clan2, &racun) != SUCCESS_FUNC) {
                printf("Greška pri izvršavanju operacije!\n\n");
                free_mem(head);
                return OP_ERROR;
            }
            push(head, racun);
        }
        token = strtok(NULL, " \t\n"); 
    }

    float glavni_racun;

    if (pop(head, &glavni_racun) != SUCCESS_FUNC) {
        printf("Greška, nedostaje rezultat!\n\n");
        free_mem(head);
        return MISSING_RESULT_ERROR;
    }

    if (head->next != NULL) {
        printf("Greška u izrazu!\n\n");
        free_mem(head);
        return FORMAT_ERROR;
    }

    *rezultat = glavni_racun;
    return SUCCESS_FUNC;
}

int free_mem(stack *head) {
    stack *current   = head->next;
    stack *next      = NULL;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    head->next = NULL;
    return SUCCESS_FUNC;
}