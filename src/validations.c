#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "validations.h"

int validarTextoVacio(const char *texto) {
    if (texto == NULL) {
        return 0;
    }

    while (*texto != '\0') {
        if (!isspace((unsigned char)*texto)) {
            return 1;
        }
        texto++;
    }

    return 0;
}

int validarEntero(const char *texto) {
    int i = 0;

    if (texto == NULL || texto[0] == '\0') {
        return 0;
    }

    if (texto[0] == '-' || texto[0] == '+') {
        i++;
    }

    if (texto[i] == '\0') {
        return 0;
    }

    for (; texto[i] != '\0'; i++) {
        if (!isdigit((unsigned char)texto[i])) {
            return 0;
        }
    }

    return 1;
}

int validarOpcionMenu(int opcion, int min, int max) {
    return (opcion >= min && opcion <= max);
}

int validarLongitud(const char *texto, int max) {
    if (texto == NULL) {
        return 0;
    }

    return strlen(texto) <= (size_t)max;
}

int validarDNI(const char *dni) {
    int i;
    const char letras[] = "TRWAGMYFPDXBNJZSQVHLCKE";
    int numero;
    char letraCorrecta;

    if (dni == NULL || strlen(dni) != 9) {
        return 0;
    }

    for (i = 0; i < 8; i++) {
        if (!isdigit((unsigned char)dni[i])) {
            return 0;
        }
    }

    if (!isalpha((unsigned char)dni[8])) {
        return 0;
    }

    numero = 0;
    for (i = 0; i < 8; i++) {
        numero = numero * 10 + (dni[i] - '0');
    }

    letraCorrecta = letras[numero % 23];

    return toupper((unsigned char)dni[8]) == letraCorrecta;
}
