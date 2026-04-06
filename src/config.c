#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "config.h"

static void trim(char *str) {
	int inicio = 0;
	int fin = strlen(str) -1;
	int i, j;

	while (str[inicio] != '\0' && isspace((unsigned char) str[inicio])) {
		inicio++;
	}

	while (fin >= inicio && isspace ((unsigned char)str[fin])) {
		fin--;
	}

	for (i=inicio,j=0;i<=fin;i++,j++) {
		str[j] = str[i];
	}
	str[j] = '\0';

}

static void copiarValor(char *destino, const char *origen, int max) {
	strncpy(destino, origen, max -1);
	destino[max -1] = '\0';
}

void inicializarConfig(Config *cfg) {
	if (cfg == NULL) {
		return;
	}
    cfg->ruta_db[0] = '\0';
    cfg->log_file[0] = '\0';
    cfg->usuario[0] = '\0';
    cfg->password[0] = '\0';
}

int cargarConfig(const char *nombreFichero, Config *cfg) {
    FILE *f;
    char linea[512];
    char *igual;
    char clave[128];
    char valor[256];

    if (nombreFichero == NULL || cfg == NULL) {
        return 0;
    }

    inicializarConfig(cfg);

    f = fopen(nombreFichero, "r");
    if (f == NULL) {
        return 0;
    }

    while (fgets(linea, sizeof(linea), f) != NULL) {
        trim(linea);

        if (linea[0] == '\0' || linea[0] == '#') {
            continue;
        }

        igual = strchr(linea, '=');
        if (igual == NULL) {
            continue;
        }

        *igual = '\0';

        strcpy(clave, linea);
        strcpy(valor, igual + 1);

        trim(clave);
        trim(valor);

        if (strcmp(clave, "ruta_db") == 0) {
            copiarValor(cfg->ruta_db, valor, MAX_CONFIG_VALUE);
        } else if (strcmp(clave, "log_file") == 0) {
            copiarValor(cfg->log_file, valor, MAX_CONFIG_VALUE);
        } else if (strcmp(clave, "usuario") == 0) {
            copiarValor(cfg->usuario, valor, MAX_CONFIG_VALUE);
        } else if (strcmp(clave, "password") == 0) {
            copiarValor(cfg->password, valor, MAX_CONFIG_VALUE);
        }
    }

    fclose(f);
    return 1;
}

void imprimirConfig(const Config *cfg) {
    if (cfg == NULL) {
        return;
    }

    printf("=== CONFIG CARGADA ===\n");
    printf("ruta_db : %s\n", cfg->ruta_db);
    printf("log_file: %s\n", cfg->log_file);
    printf("usuario : %s\n", cfg->usuario);
    printf("password: %s\n", cfg->password);
}



