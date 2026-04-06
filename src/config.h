#ifndef CONFIG_H
#define CONFIG_H

#define MAX_CONFIG_VALUE_256

typedef struct {
	char ruta_db[MAX_CONFIG_VALUE_256];
	char log_file[MAX_CONFIG_VALUE_256];
	char usuario[MAX_CONFIG_VALUE_256];
	char password[MAX_CONFIG_VALUE_256];
} Config ;

int cargarConfig(const char *nombreFichero, Config *cfg);

void inicializarConfig(Config *cfg);

void imprimirConfig(Config *cfg);

#endif

