#include <stdio.h>
#include "sqlite3.h"
#include "db.h"

int main() {
    sqlite3 *db;
    int result;

    result = sqlite3_open("data/cmd.db", &db);
    if (result != SQLITE_OK) {
        printf("Error abriendo la base de datos\n");
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

    if (!insertarDatosPrueba(db)) {
        sqlite3_close(db);
        return 1;
    }

    if (!mostrarColegiales(db)) {
        sqlite3_close(db);
        return 1;
    }

    sqlite3_close(db);
    return 0;
}
