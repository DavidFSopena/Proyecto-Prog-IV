#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

void limpiarBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

void pausarPantalla(void) {
    printf("Pulsa ENTER para continuar...");
    limpiarBuffer();
}

void leerCadena(char *destino, int tam) {
    if (destino == NULL || tam <= 0) {
        return;
    }

    if (fgets(destino, tam, stdin) != NULL) {
        destino[strcspn(destino, "\n")] = '\0';
    } else {
        destino[0] = '\0';
    }
}

int leerEntero(void) {
    char buffer[100];
    int numero;

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }

    numero = atoi(buffer);
    return numero;
}
