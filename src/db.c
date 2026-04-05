#include <stdio.h>
#include <string.h>
#include "db.h"

int ejecutarSQL(sqlite3 *db, char sql[]) {
    sqlite3_stmt *stmt;
    int result;

    result = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error preparando SQL:\n%s\n", sql);
        return 0;
    }

    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (result != SQLITE_DONE) {
        printf("Error ejecutando SQL:\n%s\n", sql);
        return 0;
    }

    return 1;
}

int crearTablas(sqlite3 *db) {
    char sqlTorre[] =
        "CREATE TABLE IF NOT EXISTS torre ("
        "id_torre INTEGER PRIMARY KEY, "
        "nombre TEXT NOT NULL)";

    char sqlPiso[] =
        "CREATE TABLE IF NOT EXISTS piso ("
        "id_piso INTEGER PRIMARY KEY, "
        "numero_piso INTEGER NOT NULL, "
        "id_torre INTEGER NOT NULL, "
        "FOREIGN KEY(id_torre) REFERENCES torre(id_torre))";

    char sqlPersona[] =
        "CREATE TABLE IF NOT EXISTS persona ("
        "dni TEXT PRIMARY KEY, "
        "nombre TEXT NOT NULL, "
        "apellidos TEXT NOT NULL, "
        "telefono TEXT, "
        "email TEXT)";

    char sqlHabitacion[] =
        "CREATE TABLE IF NOT EXISTS habitacion ("
        "id_habitacion TEXT PRIMARY KEY, "
        "id_piso INTEGER NOT NULL, "
        "FOREIGN KEY(id_piso) REFERENCES piso(id_piso))";

    char sqlColegial[] =
        "CREATE TABLE IF NOT EXISTS colegial ("
        "dni TEXT PRIMARY KEY, "
        "id_habitacion TEXT, "
        "id_piso INTEGER NOT NULL, "
        "FOREIGN KEY(dni) REFERENCES persona(dni), "
        "FOREIGN KEY(id_habitacion) REFERENCES habitacion(id_habitacion), "
        "FOREIGN KEY(id_piso) REFERENCES piso(id_piso))";

    if (!ejecutarSQL(db, sqlTorre)) {
        return 0;
    }

    if (!ejecutarSQL(db, sqlPiso)) {
        return 0;
    }

    if (!ejecutarSQL(db, sqlPersona)) {
        return 0;
    }

    if (!ejecutarSQL(db, sqlHabitacion)) {
        return 0;
    }

    if (!ejecutarSQL(db, sqlColegial)) {
        return 0;
    }

    return 1;
}

int insertarDatosPrueba(sqlite3 *db) {
    sqlite3_stmt *stmt;
    int result;

    char sqlInsertTorre[] = "INSERT INTO torre(id_torre, nombre) VALUES(?, ?)";
    int idTorre = 1;
    char nombreTorre[] = "Izquierda";

    result = sqlite3_prepare_v2(db, sqlInsertTorre, strlen(sqlInsertTorre) + 1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error preparando INSERT torre\n");
        return 0;
    }
    sqlite3_bind_int(stmt, 1, idTorre);
    sqlite3_bind_text(stmt, 2, nombreTorre, strlen(nombreTorre), SQLITE_STATIC);
    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (result != SQLITE_DONE) {
        printf("Error insertando torre\n");
        return 0;
    }

    char sqlInsertPiso[] = "INSERT INTO piso(id_piso, numero_piso, id_torre) VALUES(?, ?, ?)";
    int idPiso = 1;
    int numeroPiso = 1;

    result = sqlite3_prepare_v2(db, sqlInsertPiso, strlen(sqlInsertPiso) + 1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error preparando INSERT piso\n");
        return 0;
    }
    sqlite3_bind_int(stmt, 1, idPiso);
    sqlite3_bind_int(stmt, 2, numeroPiso);
    sqlite3_bind_int(stmt, 3, idTorre);
    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (result != SQLITE_DONE) {
        printf("Error insertando piso\n");
        return 0;
    }

    char sqlInsertPersona[] = "INSERT INTO persona(dni, nombre, apellidos, telefono, email) VALUES(?, ?, ?, ?, ?)";

    char dniDavid[] = "12345678A";
    char nombreDavid[] = "David";
    char apellidosDavid[] = "Fernandez-Sopena Hernandez";
    char telefonoDavid[] = "600123123";
    char emailDavid[] = "david@opendeusto.es";

    result = sqlite3_prepare_v2(db, sqlInsertPersona, strlen(sqlInsertPersona) + 1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error preparando INSERT persona David\n");
        return 0;
    }
    sqlite3_bind_text(stmt, 1, dniDavid, strlen(dniDavid), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, nombreDavid, strlen(nombreDavid), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, apellidosDavid, strlen(apellidosDavid), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, telefonoDavid, strlen(telefonoDavid), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, emailDavid, strlen(emailDavid), SQLITE_STATIC);
    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (result != SQLITE_DONE) {
        printf("Error insertando persona David\n");
        return 0;
    }

    char dniJavi[] = "87654321B";
    char nombreJavi[] = "Javier";
    char apellidosJavi[] = "Montero Villoslada";
    char telefonoJavi[] = "600999999";
    char emailJavi[] = "javier@opendeusto.es";

    result = sqlite3_prepare_v2(db, sqlInsertPersona, strlen(sqlInsertPersona) + 1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error preparando INSERT persona Javi\n");
        return 0;
    }
    sqlite3_bind_text(stmt, 1, dniJavi, strlen(dniJavi), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, nombreJavi, strlen(nombreJavi), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, apellidosJavi, strlen(apellidosJavi), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, telefonoJavi, strlen(telefonoJavi), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, emailJavi, strlen(emailJavi), SQLITE_STATIC);
    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (result != SQLITE_DONE) {
        printf("Error insertando persona Javi\n");
        return 0;
    }

    char dniLucia[] = "12348765C";
    char nombreLucia[] = "Lucia";
    char apellidosLucia[] = "Salvatierra Saenz";
    char telefonoLucia[] = "600111111";
    char emailLucia[] = "lucia@opendeusto.es";

    result = sqlite3_prepare_v2(db, sqlInsertPersona, strlen(sqlInsertPersona) + 1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error preparando INSERT persona Lucia\n");
        return 0;
    }
    sqlite3_bind_text(stmt, 1, dniLucia, strlen(dniLucia), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, nombreLucia, strlen(nombreLucia), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, apellidosLucia, strlen(apellidosLucia), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, telefonoLucia, strlen(telefonoLucia), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, emailLucia, strlen(emailLucia), SQLITE_STATIC);
    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (result != SQLITE_DONE) {
        printf("Error insertando persona Lucia\n");
        return 0;
    }

    char sqlInsertHabitacion[] = "INSERT INTO habitacion(id_habitacion, id_piso) VALUES(?, ?)";

    char habDavid[] = "C508";
    result = sqlite3_prepare_v2(db, sqlInsertHabitacion, strlen(sqlInsertHabitacion) + 1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error preparando INSERT habitacion David\n");
        return 0;
    }
    sqlite3_bind_text(stmt, 1, habDavid, strlen(habDavid), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, idPiso);
    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (result != SQLITE_DONE) {
        printf("Error insertando habitacion David\n");
        return 0;
    }

    char habJavi[] = "C518";
    result = sqlite3_prepare_v2(db, sqlInsertHabitacion, strlen(sqlInsertHabitacion) + 1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error preparando INSERT habitacion Javi\n");
        return 0;
    }
    sqlite3_bind_text(stmt, 1, habJavi, strlen(habJavi), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, idPiso);
    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (result != SQLITE_DONE) {
        printf("Error insertando habitacion Javi\n");
        return 0;
    }

    char habLucia[] = "I414";
    result = sqlite3_prepare_v2(db, sqlInsertHabitacion, strlen(sqlInsertHabitacion) + 1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error preparando INSERT habitacion Lucia\n");
        return 0;
    }
    sqlite3_bind_text(stmt, 1, habLucia, strlen(habLucia), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, idPiso);
    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (result != SQLITE_DONE) {
        printf("Error insertando habitacion Lucia\n");
        return 0;
    }

    char sqlInsertColegial[] = "INSERT INTO colegial(dni, id_habitacion, id_piso) VALUES(?, ?, ?)";

    result = sqlite3_prepare_v2(db, sqlInsertColegial, strlen(sqlInsertColegial) + 1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error preparando INSERT colegial David\n");
        return 0;
    }
    sqlite3_bind_text(stmt, 1, dniDavid, strlen(dniDavid), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, habDavid, strlen(habDavid), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, idPiso);
    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (result != SQLITE_DONE) {
        printf("Error insertando colegial David\n");
        return 0;
    }

    result = sqlite3_prepare_v2(db, sqlInsertColegial, strlen(sqlInsertColegial) + 1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error preparando INSERT colegial Javi\n");
        return 0;
    }
    sqlite3_bind_text(stmt, 1, dniJavi, strlen(dniJavi), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, habJavi, strlen(habJavi), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, idPiso);
    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (result != SQLITE_DONE) {
        printf("Error insertando colegial Javi\n");
        return 0;
    }

    result = sqlite3_prepare_v2(db, sqlInsertColegial, strlen(sqlInsertColegial) + 1, &stmt, NULL);
    if (result != SQLITE_OK) {
        printf("Error preparando INSERT colegial Lucia\n");
        return 0;
    }
    sqlite3_bind_text(stmt, 1, dniLucia, strlen(dniLucia), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, habLucia, strlen(habLucia), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, idPiso);
    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (result != SQLITE_DONE) {
        printf("Error insertando colegial Lucia\n");
        return 0;
    }

    return 1;
}

int mostrarColegiales(sqlite3 *db) {
    sqlite3_stmt *stmt;
    int result;

    char sqlSelect[] =
        "SELECT p.dni, p.nombre, p.apellidos, p.telefono, p.email, h.id_habitacion "
        "FROM persona p, colegial c, habitacion h "
        "WHERE p.dni = c.dni AND c.id_habitacion = h.id_habitacion";

    result = sqlite3_prepare_v2(db, sqlSelect, strlen(sqlSelect) + 1, &stmt, NULL);

    if (result != SQLITE_OK) {
        printf("Error preparando SELECT: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    printf("Mostrando colegiales:\n");

    do {
        result = sqlite3_step(stmt);

        if (result == SQLITE_ROW) {
            printf("DNI: %s | Nombre: %s | Apellidos: %s | Telefono: %s | Email: %s | Habitacion: %s\n",
                sqlite3_column_text(stmt, 0),
                sqlite3_column_text(stmt, 1),
                sqlite3_column_text(stmt, 2),
                sqlite3_column_text(stmt, 3),
                sqlite3_column_text(stmt, 4),
                sqlite3_column_text(stmt, 5));
        }

    } while (result == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return 1;
}
