#include <stdio.h>
#include "sqlite3.h"
#include "db.h"
#include "config.h"
#include "log.h"
#include "login.h"
#include "menus.h"

int main() {
    sqlite3 *db;
    int res;
    Config cfg;

    if (!cargarConfig("config/admin.conf", &cfg)) {
        printf("ERROR - No se pudo cargar la configuracion\n");
        return 1;
    }

    if (!pedirLogin(&cfg)) {
        printf("ERROR - Acceso denegado\n");
        return 1;
    }

    res = sqlite3_open(cfg.ruta_db, &db);
    if (res != SQLITE_OK) {
        printf("ERROR - No se pudo abrir la base de datos: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    if (!ejecutarSQL(db, "PRAGMA foreign_keys = ON;")) {
        sqlite3_close(db);
        return 1;
    }

    if (!crearTablas(db)) {
        sqlite3_close(db);
        return 1;
    }

    if (!cargarDatosPrueba(db)) {
        sqlite3_close(db);
        return 1;
    }

    menuSetDB(db);
    menuPrincipal();

    sqlite3_close(db);
    return 0;
}
