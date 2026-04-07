#include <stdio.h>
#include <string.h>
#include "login.h"
#include "utils.h"
#include "validations.h"

int comprobarLogin(const Config *cfg, const char *usuario, const char *password) {
    if (cfg == NULL || usuario == NULL || password == NULL) {
        return 0;
    }

    if (strcmp(cfg->usuario, usuario) == 0 &&
        strcmp(cfg->password, password) == 0) {
        return 1;
    }

    return 0;
}

int pedirLogin(const Config *cfg) {
    char usuario[100];
    char password[100];

    if (cfg == NULL) {
        return 0;
    }

    printf("=== LOGIN ===\n");

    /* Limpia lo que haya quedado en el buffer antes de leer texto */
    limpiarBuffer();

    printf("Usuario: ");
    leerCadena(usuario, sizeof(usuario));
    printf("[DEBUG usuario='%s']\n", usuario);


    printf("Password: ");
    leerCadena(password, sizeof(password));
    printf("[DEBUG password='%s']\n", password);

    if (!validarTextoVacio(usuario) || !validarTextoVacio(password)) {
        printf("Error: usuario o password vacios.\n");
        return 0;
    }

    if (comprobarLogin(cfg, usuario, password)) {
        printf("Login correcto.\n");
        return 1;
    } else {
        printf("Usuario o password incorrectos.\n");
        return 0;
    }
}
