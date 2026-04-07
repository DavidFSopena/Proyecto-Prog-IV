#include <stdio.h>
#include "sqlite3.h"
#include "db.h"
#include "config.h"
#include "login.h"
#include "app_support.h"

int iniciarSistema(sqlite3 **db, Config *cfg, const char *rutaConfig) {
    int result;

    if (db == NULL || cfg == NULL || rutaConfig == NULL) {
        return 0;
    }

    if (!cargarConfig(rutaConfig, cfg)) {
        printf("Error al cargar la configuracion\n");
        return 0;
    }

    result = sqlite3_open(cfg->ruta_db, db);
    if (result != SQLITE_OK) {
        printf("Error abriendo la base de datos\n");
        return 0;
    }

    if (!ejecutarSQL(*db, "PRAGMA foreign_keys = ON;")) {
        sqlite3_close(*db);
        return 0;
    }

    if (!crearTablas(*db)) {
        sqlite3_close(*db);
        return 0;
    }

    if (!cargarDatosPrueba(*db)) {
        sqlite3_close(*db);
        return 0;
    }

    return 1;
}

int autenticarSistema(const Config *cfg) {
    if (cfg == NULL) {
        return 0;
    }

    return pedirLogin(cfg);
}

void cerrarSistema(sqlite3 *db) {
    if (db != NULL) {
        sqlite3_close(db);
    }
}
