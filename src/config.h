#ifndef CONFIG_H
#define CONFIG_H

#define MAX_CONFIG_VALUE 256

typedef struct {
    char ruta_db[MAX_CONFIG_VALUE];
    char log_file[MAX_CONFIG_VALUE];
    char usuario[MAX_CONFIG_VALUE];
    char password[MAX_CONFIG_VALUE];
} Config;

int cargarConfig(const char *nombreFichero, Config *cfg);
void inicializarConfig(Config *cfg);
void imprimirConfig(const Config *cfg);

#endif

