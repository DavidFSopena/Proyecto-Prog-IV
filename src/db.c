#include <stdio.h>
#include <string.h>
#include "db.h"

// GENERAL

int ejecutarSQL(sqlite3 *db, char sql[]) {
    sqlite3_stmt *stmt;
    int res;

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) {
        printf("ERROR - Preparando SQL: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (res != SQLITE_DONE) {
        printf("ERROR - Ejecutando SQL: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    return 1;
}

// prepara un statement y enlaza el texto en posición pos
static int bindTexto(sqlite3_stmt *stmt, int pos, const char *val) {
    return sqlite3_bind_text(stmt, pos, val, strlen(val), SQLITE_STATIC);
}

// CREAR TABLAS
int crearTablas(sqlite3 *db) {

    /* Torres del colegio mayor */
    char sqlTorre[] =
        "CREATE TABLE IF NOT EXISTS torre ("
        "id_torre   INTEGER PRIMARY KEY, "
        "nombre     TEXT NOT NULL)";

    /* Pisos dentro de cada torre */
    char sqlPiso[] =
        "CREATE TABLE IF NOT EXISTS piso ("
        "id_piso      INTEGER PRIMARY KEY, "
        "numero_piso  INTEGER NOT NULL, "
        "id_torre     INTEGER NOT NULL, "
        "FOREIGN KEY(id_torre) REFERENCES torre(id_torre))";

    /* Datos personales compartidos por colegiales, huéspedes y empleados */
    char sqlPersona[] =
        "CREATE TABLE IF NOT EXISTS persona ("
        "dni           TEXT PRIMARY KEY, "
        "nombre        TEXT NOT NULL, "
        "apellidos     TEXT NOT NULL, "
        "telefono      TEXT, "
        "email         TEXT)";

    /* Habitaciones individuales */
    char sqlHabitacion[] =
        "CREATE TABLE IF NOT EXISTS habitacion ("
        "id_habitacion TEXT PRIMARY KEY, "
        "id_piso       INTEGER NOT NULL, "
        "ocupada       INTEGER NOT NULL DEFAULT 0, "
        "FOREIGN KEY(id_piso) REFERENCES piso(id_piso))";

    /* Tabla de colegiales, extiende persona con datos específicos del colegio */
    char sqlColegial[] =
        "CREATE TABLE IF NOT EXISTS colegial ("
        "dni              TEXT PRIMARY KEY, "
        "id_habitacion    TEXT, "
        "id_piso          INTEGER NOT NULL, "
        "email_deusto     TEXT, "
        "telefono_padre1  TEXT, "
        "email_padre1     TEXT, "
        "telefono_padre2  TEXT, "
        "email_padre2     TEXT, "
        "iban             TEXT, "
        "fecha_nacimiento TEXT, "
        "curso            TEXT, "
        "grado            TEXT, "
        "alergias         TEXT, "
        "FOREIGN KEY(dni)           REFERENCES persona(dni), "
        "FOREIGN KEY(id_habitacion) REFERENCES habitacion(id_habitacion), "
        "FOREIGN KEY(id_piso)       REFERENCES piso(id_piso))";

    /* Huéspedes de hotel (planta A) */
    char sqlHuesped[] =
        "CREATE TABLE IF NOT EXISTS huesped ("
        "dni           TEXT PRIMARY KEY, "
        "id_habitacion TEXT NOT NULL, "
        "id_piso       INTEGER NOT NULL, "
        "fecha_inicio  TEXT NOT NULL, "
        "fecha_fin     TEXT NOT NULL, "
        "FOREIGN KEY(dni)           REFERENCES persona(dni), "
        "FOREIGN KEY(id_habitacion) REFERENCES habitacion(id_habitacion), "
        "FOREIGN KEY(id_piso)       REFERENCES piso(id_piso))";

    /* Incidencias notificadas por los colegiales */
    char sqlIncidencia[] =
        "CREATE TABLE IF NOT EXISTS incidencia ("
        "id_incidencia  INTEGER PRIMARY KEY, "
        "descripcion    TEXT NOT NULL, "
        "urgencia       INTEGER NOT NULL, "  // 1->alta, 2->media, 3->baja
        "estado         TEXT NOT NULL, "     // abierta / en curso / cerrada
        "fecha_apertura TEXT NOT NULL, "
        "fecha_cierre   TEXT, "
        "dni_colegial   TEXT NOT NULL, "
        "id_habitacion  TEXT NOT NULL, "
        "FOREIGN KEY(dni_colegial)  REFERENCES colegial(dni), "
        "FOREIGN KEY(id_habitacion) REFERENCES habitacion(id_habitacion))";

    /* Pagos de los colegiales */
    char sqlPago[] =
        "CREATE TABLE IF NOT EXISTS pago ("
        "id_pago      INTEGER PRIMARY KEY, "
        "dni_colegial TEXT NOT NULL, "
        "modalidad    TEXT NOT NULL, "   // mensual / unico
        "estado       TEXT NOT NULL, "   // pendiente / realizado
        "fecha        TEXT NOT NULL, "
        "importe      INTEGER NOT NULL, "
        "FOREIGN KEY(dni_colegial) REFERENCES colegial(dni))";

    /* Llegadas nocturnas */
    char sqlLlegadaNocturna[] =
        "CREATE TABLE IF NOT EXISTS llegada_nocturna ("
        "id_llegada    INTEGER PRIMARY KEY, "
        "dni_colegial  TEXT NOT NULL, "
        "id_habitacion TEXT NOT NULL, "
        "fecha         TEXT NOT NULL, "
        "hora          TEXT NOT NULL, "
        "FOREIGN KEY(dni_colegial)  REFERENCES colegial(dni), "
        "FOREIGN KEY(id_habitacion) REFERENCES habitacion(id_habitacion))";

    /* Salidas por vacaciones */
    char sqlSalidaVacaciones[] =
        "CREATE TABLE IF NOT EXISTS salida_vacaciones ("
        "id_salida    INTEGER PRIMARY KEY, "
        "dni_colegial TEXT NOT NULL, "
        "fecha_salida TEXT NOT NULL, "
        "fecha_regreso TEXT, "
        "tipo         TEXT NOT NULL, "   // navidad / semana santa / otra
        "FOREIGN KEY(dni_colegial) REFERENCES colegial(dni))";

    /* Sanciones por incumplimiento de normas */
    char sqlSancion[] =
        "CREATE TABLE IF NOT EXISTS sancion ("
        "id_sancion    INTEGER PRIMARY KEY, "
        "dni_colegial  TEXT NOT NULL, "
        "fecha         TEXT NOT NULL, "
        "tipo          TEXT NOT NULL, "   // aviso / temporal / permanente
        "motivo        TEXT NOT NULL, "
        "duracion_dias INTEGER, "
        "FOREIGN KEY(dni_colegial) REFERENCES colegial(dni))";

    /* Actividades organizadas por el colegio */
    char sqlActividad[] =
        "CREATE TABLE IF NOT EXISTS actividad ("
        "id_actividad  INTEGER PRIMARY KEY, "
        "nombre        TEXT NOT NULL, "
        "tipo          TEXT NOT NULL, "   // coloquio / evento
        "fecha         TEXT NOT NULL, "
        "hora          TEXT NOT NULL, "
        "lugar         TEXT NOT NULL, "
        "dni_empleado  TEXT)";

    /* Inscripciones de colegiales a actividades */
    char sqlInscripcion[] =
        "CREATE TABLE IF NOT EXISTS inscripcion_colegial ("
        "id_actividad      INTEGER NOT NULL, "
        "dni_colegial      TEXT NOT NULL, "
        "fecha_inscripcion TEXT NOT NULL, "
        "PRIMARY KEY(id_actividad, dni_colegial), "
        "FOREIGN KEY(id_actividad) REFERENCES actividad(id_actividad), "
        "FOREIGN KEY(dni_colegial) REFERENCES colegial(dni))";

    /* Reservas de salas de trabajo */
    char sqlReserva[] =
        "CREATE TABLE IF NOT EXISTS reserva_sala ("
        "id_reserva   INTEGER PRIMARY KEY, "
        "id_sala      TEXT NOT NULL, "
        "dni_colegial TEXT NOT NULL, "
        "fecha        TEXT NOT NULL, "
        "hora_inicio  TEXT NOT NULL, "
        "hora_fin     TEXT NOT NULL, "
        "FOREIGN KEY(dni_colegial) REFERENCES colegial(dni))";

    if (!ejecutarSQL(db, sqlTorre))           return 0;
    if (!ejecutarSQL(db, sqlPiso))            return 0;
    if (!ejecutarSQL(db, sqlPersona))         return 0;
    if (!ejecutarSQL(db, sqlHabitacion))      return 0;
    if (!ejecutarSQL(db, sqlColegial))        return 0;
    if (!ejecutarSQL(db, sqlHuesped))         return 0;
    if (!ejecutarSQL(db, sqlIncidencia))      return 0;
    if (!ejecutarSQL(db, sqlPago))            return 0;
    if (!ejecutarSQL(db, sqlLlegadaNocturna)) return 0;
    if (!ejecutarSQL(db, sqlSalidaVacaciones))return 0;
    if (!ejecutarSQL(db, sqlSancion))         return 0;
    if (!ejecutarSQL(db, sqlActividad))       return 0;
    if (!ejecutarSQL(db, sqlInscripcion))     return 0;
    if (!ejecutarSQL(db, sqlReserva))         return 0;

    return 1;
}

// TORRES Y SUS PISOS

int insertarTorre(sqlite3 *db, int idTorre, char nombre[]) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] = "INSERT INTO torre(id_torre, nombre) VALUES(?, ?)";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) { printf("ERROR - Preparando torre\n"); return 0; }

    sqlite3_bind_int(stmt, 1, idTorre);
    bindTexto(stmt, 2, nombre);
    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (res != SQLITE_DONE) { printf("ERROR - Insertando torre\n"); return 0; }
    return 1;
}

int insertarPiso(sqlite3 *db, int idPiso, int numeroPiso, int idTorre) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] = "INSERT INTO piso(id_piso, numero_piso, id_torre) VALUES(?, ?, ?)";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) { printf("ERROR - Preparando piso\n"); return 0; }

    sqlite3_bind_int(stmt, 1, idPiso);
    sqlite3_bind_int(stmt, 2, numeroPiso);
    sqlite3_bind_int(stmt, 3, idTorre);
    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (res != SQLITE_DONE) { printf("ERROR - Insertando piso\n"); return 0; }
    return 1;
}

int mostrarTorres(sqlite3 *db) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] = "SELECT id_torre, nombre FROM torre ORDER BY id_torre";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) { printf("ERROR - Preparando SELECT torres\n"); return 0; }

    printf("\n--- Torres ---\n");
    do {
        res = sqlite3_step(stmt);
        if (res == SQLITE_ROW)
            printf("ID: %d | Nombre: %s\n",
                sqlite3_column_int(stmt, 0),
                sqlite3_column_text(stmt, 1));
    } while (res == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return 1;
}

// HABITACIONES

int insertarHabitacion(sqlite3 *db, char idHabitacion[], int idPiso) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] = "INSERT INTO habitacion(id_habitacion, id_piso, ocupada) VALUES(?, ?, 0)";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) { printf("ERROR - Preparando habitacion\n"); return 0; }

    bindTexto(stmt, 1, idHabitacion);
    sqlite3_bind_int(stmt, 2, idPiso);
    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (res != SQLITE_DONE) { printf("ERROR - Insertando habitacion %s\n", idHabitacion); return 0; }
    return 1;
}

int habitacionDisponible(sqlite3 *db, char idHabitacion[]) {
    sqlite3_stmt *stmt;
    int res, ocupada = 1;
    char sql[] = "SELECT ocupada FROM habitacion WHERE id_habitacion = ?";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;

    bindTexto(stmt, 1, idHabitacion);
    res = sqlite3_step(stmt);
    if (res == SQLITE_ROW)
        ocupada = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);
    return (ocupada == 0) ? 1 : 0;
}

int asignarHabitacionColegial(sqlite3 *db, char dni[], char idHabitacion[]) {
    sqlite3_stmt *stmt;
    int res;

    if (!habitacionDisponible(db, idHabitacion)) {
        printf("AVISO - La habitacion %s no esta disponible\n", idHabitacion);
        return 0;
    }

    /* Actualizar colegial */
    char sqlColegial[] = "UPDATE colegial SET id_habitacion = ? WHERE dni = ?";
    res = sqlite3_prepare_v2(db, sqlColegial, strlen(sqlColegial) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;
    bindTexto(stmt, 1, idHabitacion);
    bindTexto(stmt, 2, dni);
    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (res != SQLITE_DONE) return 0;

    /* Marcar habitacion como ocupada */
    char sqlHab[] = "UPDATE habitacion SET ocupada = 1 WHERE id_habitacion = ?";
    res = sqlite3_prepare_v2(db, sqlHab, strlen(sqlHab) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;
    bindTexto(stmt, 1, idHabitacion);
    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (res != SQLITE_DONE) return 0;

    return 1;
}

int mostrarHabitacionesDisponibles(sqlite3 *db) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "SELECT h.id_habitacion, p.numero_piso, t.nombre "
        "FROM habitacion h "
        "JOIN piso p ON h.id_piso = p.id_piso "
        "JOIN torre t ON p.id_torre = t.id_torre "
        "WHERE h.ocupada = 0 "
        "ORDER BY t.nombre, p.numero_piso";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) { printf("ERROR - Preparando habitaciones disponibles\n"); return 0; }

    printf("\n--- Habitaciones disponibles ---\n");
    printf("%-12s  %-8s  %-12s\n", "Habitacion", "Planta", "Torre");
    printf("--------------------------------------\n");
    do {
        res = sqlite3_step(stmt);
        if (res == SQLITE_ROW)
            printf("%-12s  %-8d  %-12s\n", sqlite3_column_text(stmt, 0), sqlite3_column_int(stmt, 1), sqlite3_column_text(stmt, 2));
    } while (res == SQLITE_ROW);
    sqlite3_finalize(stmt);
    return 1;
}

int mostrarOcupacionPorTorre(sqlite3 *db) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "SELECT t.nombre, "
        "COUNT(h.id_habitacion) AS total, "
        "SUM(h.ocupada) AS ocupadas, "
        "COUNT(h.id_habitacion) - SUM(h.ocupada) AS libres "
        "FROM torre t "
        "JOIN piso p ON t.id_torre = p.id_torre "
        "JOIN habitacion h ON p.id_piso = h.id_piso "
        "GROUP BY t.nombre";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) { printf("ERROR - Preparando ocupacion\n"); return 0; }

    printf("\n--- Ocupacion por torre ---\n");
    printf("%-12s  %6s  %7s  %6s\n", "Torre", "Total", "Ocupadas", "Libres");
    printf("---------------------------------------\n");
    do {
        res = sqlite3_step(stmt);
        if (res == SQLITE_ROW)
            printf("%-12s  %6d  %8d  %6d\n",
                sqlite3_column_text(stmt, 0),
                sqlite3_column_int(stmt, 1),
                sqlite3_column_int(stmt, 2),
                sqlite3_column_int(stmt, 3));
    } while (res == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return 1;
}

// PERSONAS

int insertarPersona(sqlite3 *db,
                    char dni[], char nombre[], char apellidos[],
                    char telefono[], char email[]) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "INSERT INTO persona(dni, nombre, apellidos, telefono, email) "
        "VALUES(?, ?, ?, ?, ?)";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) { printf("ERROR - Preparando persona\n"); return 0; }

    bindTexto(stmt, 1, dni);
    bindTexto(stmt, 2, nombre);
    bindTexto(stmt, 3, apellidos);
    bindTexto(stmt, 4, telefono);
    bindTexto(stmt, 5, email);
    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (res != SQLITE_DONE) { printf("ERROR - Insertando persona %s\n", dni); return 0; }
    return 1;
}

// COLEGIALES

int insertarColegial(sqlite3 *db,
                     char dni[], char nombre[], char apellidos[],
                     char telefono[], char emailPersonal[],
                     char emailDeusto[], char telefonoPadre1[],
                     char emailPadre1[], char telefonoPadre2[],
                     char emailPadre2[], char iban[],
                     char fechaNacimiento[], char curso[],
                     char grado[], char alergias[],
                     char idHabitacion[], int idPiso) {
    sqlite3_stmt *stmt;
    int res;

    /* Primero inserto en persona */
    if (!insertarPersona(db, dni, nombre, apellidos, telefono, emailPersonal))
        return 0;

    char sql[] =
        "INSERT INTO colegial("
        "dni, id_habitacion, id_piso, email_deusto, "
        "telefono_padre1, email_padre1, telefono_padre2, email_padre2, "
        "iban, fecha_nacimiento, curso, grado, alergias) "
        "VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) { printf("ERROR - Preparando colegial\n"); return 0; }

    bindTexto(stmt,  1, dni);
    bindTexto(stmt,  2, idHabitacion);
    sqlite3_bind_int(stmt, 3, idPiso);
    bindTexto(stmt,  4, emailDeusto);
    bindTexto(stmt,  5, telefonoPadre1);
    bindTexto(stmt,  6, emailPadre1);
    bindTexto(stmt,  7, telefonoPadre2);
    bindTexto(stmt,  8, emailPadre2);
    bindTexto(stmt,  9, iban);
    bindTexto(stmt, 10, fechaNacimiento);
    bindTexto(stmt, 11, curso);
    bindTexto(stmt, 12, grado);
    bindTexto(stmt, 13, alergias);
    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (res != SQLITE_DONE) { printf("ERROR - Insertando colegial %s\n", dni); return 0; }

    /* Marcar habitacion como ocupada */
    char sqlHab[] = "UPDATE habitacion SET ocupada = 1 WHERE id_habitacion = ?";
    res = sqlite3_prepare_v2(db, sqlHab, strlen(sqlHab) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;
    bindTexto(stmt, 1, idHabitacion);
    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return (res == SQLITE_DONE) ? 1 : 0;
}

int bajaColegial(sqlite3 *db, char dni[]) {
    sqlite3_stmt *stmt;
    int res;

    if (colegialTieneIncidenciasAbiertas(db, dni)) {
        printf("[AVISO] El colegial %s tiene incidencias abiertas. Resuelvelas antes de dar de baja.\n", dni);
        return 0;
    }
    if (colegialTienePagosPendientes(db, dni)) {
        printf("[AVISO] El colegial %s tiene pagos pendientes. Saldalos antes de dar de baja.\n", dni);
        return 0;
    }

    /* Liberar habitacion */
    char sqlHab[] =
        "UPDATE habitacion SET ocupada = 0 "
        "WHERE id_habitacion = (SELECT id_habitacion FROM colegial WHERE dni = ?)";
    res = sqlite3_prepare_v2(db, sqlHab, strlen(sqlHab) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;
    bindTexto(stmt, 1, dni);
    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (res != SQLITE_DONE) return 0;

    /* Eliminar colegial */
    char sqlCol[] = "DELETE FROM colegial WHERE dni = ?";
    res = sqlite3_prepare_v2(db, sqlCol, strlen(sqlCol) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;
    bindTexto(stmt, 1, dni);
    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (res != SQLITE_DONE) return 0;

    /* Eliminar persona */
    char sqlPer[] = "DELETE FROM persona WHERE dni = ?";
    res = sqlite3_prepare_v2(db, sqlPer, strlen(sqlPer) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;
    bindTexto(stmt, 1, dni);
    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return (res == SQLITE_DONE) ? 1 : 0;
}

int modificarTelefonoColegial(sqlite3 *db, char dni[], char nuevoTelefono[]) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] = "UPDATE persona SET telefono = ? WHERE dni = ?";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;
    bindTexto(stmt, 1, nuevoTelefono);
    bindTexto(stmt, 2, dni);
    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (res == SQLITE_DONE) ? 1 : 0;
}

int modificarEmailColegial(sqlite3 *db, char dni[], char nuevoEmail[]) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] = "UPDATE persona SET email = ? WHERE dni = ?";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;
    bindTexto(stmt, 1, nuevoEmail);
    bindTexto(stmt, 2, dni);
    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (res == SQLITE_DONE) ? 1 : 0;
}

int mostrarColegiales(sqlite3 *db) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "SELECT p.dni, p.nombre, p.apellidos, p.telefono, p.email, "
        "c.id_habitacion, c.curso, c.grado "
        "FROM persona p "
        "JOIN colegial c ON p.dni = c.dni "
        "ORDER BY p.apellidos, p.nombre";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) {
        printf("ERROR - Preparando SELECT colegiales\n");
        return 0;
    }

    printf("\n--- Colegiales ---\n");

    do {
        res = sqlite3_step(stmt);
        if (res == SQLITE_ROW) {
            printf("DNI: %-12s | %s %s | Tel: %-10s | Email: %-30s | Hab: %-5s | Curso: %s | Grado: %s\n",
                sqlite3_column_text(stmt, 0),
                sqlite3_column_text(stmt, 1),
                sqlite3_column_text(stmt, 2),
                sqlite3_column_text(stmt, 3),
                sqlite3_column_text(stmt, 4),
                sqlite3_column_text(stmt, 5),
                sqlite3_column_text(stmt, 6),
                sqlite3_column_text(stmt, 7));
        }
    } while (res == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return 1;
}

int buscarColegialPorDni(sqlite3 *db, char dni[]) {
    sqlite3_stmt *stmt;
    int res, encontrado = 0;
    char sql[] =
        "SELECT p.dni, p.nombre, p.apellidos, p.telefono, p.email, "
        "c.id_habitacion, c.email_deusto, c.curso, c.grado, c.alergias, "
        "c.fecha_nacimiento, c.iban "
        "FROM persona p "
        "JOIN colegial c ON p.dni = c.dni "
        "WHERE p.dni = ?";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) { printf("ERROR - Preparando busqueda DNI\n"); return 0; }

    bindTexto(stmt, 1, dni);
    res = sqlite3_step(stmt);

    if (res == SQLITE_ROW) {
        encontrado = 1;
        printf("\n--- Ficha colegial ---\n");
        printf("DNI:             %s\n", sqlite3_column_text(stmt, 0));
        printf("Nombre:          %s\n", sqlite3_column_text(stmt, 1));
        printf("Apellidos:       %s\n", sqlite3_column_text(stmt, 2));
        printf("Telefono:        %s\n", sqlite3_column_text(stmt, 3));
        printf("Email personal:  %s\n", sqlite3_column_text(stmt, 4));
        printf("Email Deusto:    %s\n", sqlite3_column_text(stmt, 6));
        printf("Habitacion:      %s\n", sqlite3_column_text(stmt, 5));
        printf("Curso:           %s\n", sqlite3_column_text(stmt, 7));
        printf("Grado:           %s\n", sqlite3_column_text(stmt, 8));
        printf("Alergias:        %s\n", sqlite3_column_text(stmt, 9));
        printf("Fecha nacimiento:    %s\n", sqlite3_column_text(stmt, 10));
        printf("IBAN:            %s\n", sqlite3_column_text(stmt, 11));
    } else {
        printf("INFO - No se encontro colegial con DNI %s\n", dni);
    }

    sqlite3_finalize(stmt);
    return encontrado;
}

int buscarColegialPorHabitacion(sqlite3 *db, char idHabitacion[]) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "SELECT p.dni, p.nombre, p.apellidos, c.id_habitacion "
        "FROM persona p "
        "JOIN colegial c ON p.dni = c.dni "
        "WHERE c.id_habitacion = ?";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;

    bindTexto(stmt, 1, idHabitacion);
    res = sqlite3_step(stmt);

    if (res == SQLITE_ROW)
        printf("Habitacion %s -> %s %s (DNI: %s)\n",
            sqlite3_column_text(stmt, 3),
            sqlite3_column_text(stmt, 1),
            sqlite3_column_text(stmt, 2),
            sqlite3_column_text(stmt, 0));
    else
        printf("INFO - Habitacion %s sin colegial asignado\n", idHabitacion);

    sqlite3_finalize(stmt);
    return 1;
}

int listarColegialesTorre(sqlite3 *db, char nombreTorre[]) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "SELECT p.dni, p.nombre, p.apellidos, c.id_habitacion "
        "FROM persona p "
        "JOIN colegial c ON p.dni = c.dni "
        "JOIN piso pi ON c.id_piso = pi.id_piso "
        "JOIN torre t ON pi.id_torre = t.id_torre "
        "WHERE t.nombre = ? "
        "ORDER BY c.id_habitacion";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;

    bindTexto(stmt, 1, nombreTorre);
    printf("\n--- Colegiales en torre %s ---\n", nombreTorre);

    do {
        res = sqlite3_step(stmt);
        if (res == SQLITE_ROW) {
            printf("DNI: %-12s | %-20s %-20s | Hab: %s\n",
                sqlite3_column_text(stmt, 0),
                sqlite3_column_text(stmt, 1),
                sqlite3_column_text(stmt, 2),
                sqlite3_column_text(stmt, 3));
        }
    } while (res == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return 1;
}

int colegialTieneIncidenciasAbiertas(sqlite3 *db, char dni[]) {
    sqlite3_stmt *stmt;
    int res, count = 0;
    char sql[] =
        "SELECT COUNT(*) FROM incidencia "
        "WHERE dni_colegial = ? AND estado != 'cerrada'";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;

    bindTexto(stmt, 1, dni);
    res = sqlite3_step(stmt);
    if (res == SQLITE_ROW)
        count = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);
    return (count > 0) ? 1 : 0;
}

int colegialTienePagosPendientes(sqlite3 *db, char dni[]) {
    sqlite3_stmt *stmt;
    int res, count = 0;
    char sql[] =
        "SELECT COUNT(*) FROM pago "
        "WHERE dni_colegial = ? AND estado = 'pendiente'";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;

    bindTexto(stmt, 1, dni);
    res = sqlite3_step(stmt);
    if (res == SQLITE_ROW)
        count = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);
    return (count > 0) ? 1 : 0;
}

// HUESPEDES

int insertarHuesped(sqlite3 *db,
                    char dni[], char nombre[], char apellidos[],
                    char idHabitacion[], int idPiso,
                    char fechaInicio[], char fechaFin[]) {
    sqlite3_stmt *stmt;
    int res;

    if (!insertarPersona(db, dni, nombre, apellidos, "", ""))
        return 0;

    if (!habitacionDisponible(db, idHabitacion)) {
        printf("AVISO - Habitacion %s no disponible\n", idHabitacion);
        return 0;
    }

    char sql[] =
        "INSERT INTO huesped(dni, id_habitacion, id_piso, fecha_inicio, fecha_fin) "
        "VALUES(?, ?, ?, ?, ?)";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;

    bindTexto(stmt, 1, dni);
    bindTexto(stmt, 2, idHabitacion);
    sqlite3_bind_int(stmt, 3, idPiso);
    bindTexto(stmt, 4, fechaInicio);
    bindTexto(stmt, 5, fechaFin);
    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (res != SQLITE_DONE) { printf("ERROR - Insertando huesped\n"); return 0; }

    char sqlHab[] = "UPDATE habitacion SET ocupada = 1 WHERE id_habitacion = ?";
    res = sqlite3_prepare_v2(db, sqlHab, strlen(sqlHab) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;
    bindTexto(stmt, 1, idHabitacion);
    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (res == SQLITE_DONE) ? 1 : 0;
}

int mostrarHuespedes(sqlite3 *db) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "SELECT p.dni, p.nombre, p.apellidos, h.id_habitacion, "
        "h.fecha_inicio, h.fecha_fin "
        "FROM persona p JOIN huesped h ON p.dni = h.dni "
        "ORDER BY h.fecha_inicio";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;

    printf("\n--- Huespedes ---\n");
    do {
        res = sqlite3_step(stmt);
        if (res == SQLITE_ROW)
            printf("DNI: %-12s | %-25s | Hab: %-6s | %s -> %s\n",
                sqlite3_column_text(stmt, 0),
                sqlite3_column_text(stmt, 1),
                sqlite3_column_text(stmt, 3),
                sqlite3_column_text(stmt, 4),
                sqlite3_column_text(stmt, 5));
    } while (res == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return 1;
}

int darBajaHuesped(sqlite3 *db, char dni[]) {
    sqlite3_stmt *stmt;
    int res;

    char sqlHab[] =
        "UPDATE habitacion SET ocupada = 0 "
        "WHERE id_habitacion = (SELECT id_habitacion FROM huesped WHERE dni = ?)";
    res = sqlite3_prepare_v2(db, sqlHab, strlen(sqlHab) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;
    bindTexto(stmt, 1, dni);
    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (res != SQLITE_DONE) return 0;

    char sqlHues[] = "DELETE FROM huesped WHERE dni = ?";
    res = sqlite3_prepare_v2(db, sqlHues, strlen(sqlHues) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;
    bindTexto(stmt, 1, dni);
    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (res != SQLITE_DONE) return 0;

    char sqlPer[] = "DELETE FROM persona WHERE dni = ?";
    res = sqlite3_prepare_v2(db, sqlPer, strlen(sqlPer) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;
    bindTexto(stmt, 1, dni);
    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (res == SQLITE_DONE) ? 1 : 0;
}

//INCIDENCIAS
int insertarIncidencia(sqlite3 *db,
                       int idIncidencia, char descripcion[],
                       int urgencia, char estado[],
                       char fechaApertura[], char fechaCierre[],
                       char dniColegial[], char idHabitacion[]) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "INSERT INTO incidencia("
        "id_incidencia, descripcion, urgencia, estado, "
        "fecha_apertura, fecha_cierre, dni_colegial, id_habitacion) "
        "VALUES(?, ?, ?, ?, ?, ?, ?, ?)";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) { printf("ERROR - Preparando incidencia\n"); return 0; }

    sqlite3_bind_int(stmt, 1, idIncidencia);
    bindTexto(stmt, 2, descripcion);
    sqlite3_bind_int(stmt, 3, urgencia);
    bindTexto(stmt, 4, estado);
    bindTexto(stmt, 5, fechaApertura);
    /* fecha_cierre puede ser cadena vacia; guardamos NULL si es asi */
    if (fechaCierre != NULL && strlen(fechaCierre) > 0)
        bindTexto(stmt, 6, fechaCierre);
    else
        sqlite3_bind_null(stmt, 6);
    bindTexto(stmt, 7, dniColegial);
    bindTexto(stmt, 8, idHabitacion);
    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (res != SQLITE_DONE) { printf("ERROR - Insertando incidencia\n"); return 0; }
    return 1;
}

int cambiarEstadoIncidencia(sqlite3 *db, int idIncidencia,
                            char nuevoEstado[], char fechaCierre[]) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "UPDATE incidencia SET estado = ?, fecha_cierre = ? "
        "WHERE id_incidencia = ?";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;

    bindTexto(stmt, 1, nuevoEstado);
    if (fechaCierre != NULL && strlen(fechaCierre) > 0)
        bindTexto(stmt, 2, fechaCierre);
    else
        sqlite3_bind_null(stmt, 2);
    sqlite3_bind_int(stmt, 3, idIncidencia);
    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (res == SQLITE_DONE) ? 1 : 0;
}

int mostrarIncidenciasAbiertas(sqlite3 *db) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "SELECT id_incidencia, descripcion, urgencia, estado, "
        "fecha_apertura, dni_colegial, id_habitacion "
        "FROM incidencia "
        "WHERE estado != 'cerrada' "
        "ORDER BY urgencia ASC, fecha_apertura ASC";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) { printf("ERROR - Preparando incidencias abiertas\n"); return 0; }

    printf("\n--- Incidencias abiertas ---\n");
    do {
        res = sqlite3_step(stmt);
        if (res == SQLITE_ROW) {
            int urg = sqlite3_column_int(stmt, 2);
            const char *urgStr = (urg == 1) ? "ALTA" : (urg == 2) ? "MEDIA" : "BAJA";
            printf("[%d] %s | Urgencia: %-5s | Estado: %-8s | Apertura: %s | DNI: %s | Hab: %s\n",
                sqlite3_column_int(stmt, 0),
                sqlite3_column_text(stmt, 1),
                urgStr,
                sqlite3_column_text(stmt, 3),
                sqlite3_column_text(stmt, 4),
                sqlite3_column_text(stmt, 5),
                sqlite3_column_text(stmt, 6));
        }
    } while (res == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return 1;
}

int mostrarIncidenciasPorHabitacion(sqlite3 *db, char idHabitacion[]) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "SELECT id_incidencia, descripcion, urgencia, estado, "
        "fecha_apertura, fecha_cierre "
        "FROM incidencia "
        "WHERE id_habitacion = ? "
        "ORDER BY fecha_apertura DESC";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;

    bindTexto(stmt, 1, idHabitacion);
    printf("\n--- Historial incidencias habitacion %s ---\n", idHabitacion);
    do {
        res = sqlite3_step(stmt);
        if (res == SQLITE_ROW)
            printf("[%d] %s | Urgencia: %d | %s | Apertura: %s | Cierre: %s\n",
                sqlite3_column_int(stmt, 0),
                sqlite3_column_text(stmt, 1),
                sqlite3_column_int(stmt, 2),
                sqlite3_column_text(stmt, 3),
                sqlite3_column_text(stmt, 4),
                sqlite3_column_text(stmt, 5) ? (char*)sqlite3_column_text(stmt, 5) : "-");
    } while (res == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return 1;
}

int mostrarIncidenciasOrdenadasUrgencia(sqlite3 *db) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "SELECT id_incidencia, descripcion, urgencia, estado, "
        "fecha_apertura, id_habitacion "
        "FROM incidencia "
        "ORDER BY urgencia ASC, fecha_apertura ASC";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;

    printf("\n--- Todas las incidencias por urgencia ---\n");
    do {
        res = sqlite3_step(stmt);
        if (res == SQLITE_ROW) {
            int urg = sqlite3_column_int(stmt, 2);
            const char *urgStr = (urg == 1) ? "ALTA" : (urg == 2) ? "MEDIA" : "BAJA";
            printf("[%d] %-35s | %-5s | %-8s | %s | Hab: %s\n",
                sqlite3_column_int(stmt, 0),
                sqlite3_column_text(stmt, 1),
                urgStr,
                sqlite3_column_text(stmt, 3),
                sqlite3_column_text(stmt, 4),
                sqlite3_column_text(stmt, 5));
        }
    } while (res == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return 1;
}

// PAGOS

int insertarPago(sqlite3 *db,
                 int idPago, char dniColegial[], char modalidad[],
                 char estado[], char fecha[], int importe) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "INSERT INTO pago(id_pago, dni_colegial, modalidad, estado, fecha, importe) "
        "VALUES(?, ?, ?, ?, ?, ?)";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) { printf("ERROR - Preparando pago\n"); return 0; }

    sqlite3_bind_int(stmt, 1, idPago);
    bindTexto(stmt, 2, dniColegial);
    bindTexto(stmt, 3, modalidad);
    bindTexto(stmt, 4, estado);
    bindTexto(stmt, 5, fecha);
    sqlite3_bind_int(stmt, 6, importe);
    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (res != SQLITE_DONE) { printf("ERROR - Insertando pago\n"); return 0; }
    return 1;
}

int marcarPagoRealizado(sqlite3 *db, int idPago) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] = "UPDATE pago SET estado = 'realizado' WHERE id_pago = ?";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;

    sqlite3_bind_int(stmt, 1, idPago);
    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (res == SQLITE_DONE) ? 1 : 0;
}

int mostrarPagosPendientes(sqlite3 *db) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "SELECT id_pago, dni_colegial, modalidad, estado, fecha, importe "
        "FROM pago "
        "WHERE estado = 'pendiente' "
        "ORDER BY fecha ASC";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) { printf("ERROR - Preparando pagos pendientes\n"); return 0; }

    printf("\n--- Pagos pendientes ---\n");
    do {
        res = sqlite3_step(stmt);
        if (res == SQLITE_ROW)
            printf("ID: %d | DNI: %-12s | %-8s | %-10s | Fecha: %s | Importe: %d EUR\n",
                sqlite3_column_int(stmt, 0),
                sqlite3_column_text(stmt, 1),
                sqlite3_column_text(stmt, 2),
                sqlite3_column_text(stmt, 3),
                sqlite3_column_text(stmt, 4),
                sqlite3_column_int(stmt, 5));
    } while (res == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return 1;
}

int mostrarPagosPorColegial(sqlite3 *db, char dni[]) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "SELECT id_pago, modalidad, estado, fecha, importe "
        "FROM pago "
        "WHERE dni_colegial = ? "
        "ORDER BY fecha DESC";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;

    bindTexto(stmt, 1, dni);
    printf("\n--- Pagos del colegial %s ---\n", dni);
    do {
        res = sqlite3_step(stmt);
        if (res == SQLITE_ROW)
            printf("ID: %d | %-8s | %-10s | %s | %d EUR\n",
                sqlite3_column_int(stmt, 0),
                sqlite3_column_text(stmt, 1),
                sqlite3_column_text(stmt, 2),
                sqlite3_column_text(stmt, 3),
                sqlite3_column_int(stmt, 4));
    } while (res == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return 1;
}

// LLEGADAS NOCTURANS
int registrarLlegadaNocturna(sqlite3 *db,
                             int idLlegada, char dniColegial[],
                             char idHabitacion[], char fecha[],
                             char hora[]) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "INSERT INTO llegada_nocturna("
        "id_llegada, dni_colegial, id_habitacion, fecha, hora) "
        "VALUES(?, ?, ?, ?, ?)";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) { printf("ERROR - Preparando llegada nocturna\n"); return 0; }

    sqlite3_bind_int(stmt, 1, idLlegada);
    bindTexto(stmt, 2, dniColegial);
    bindTexto(stmt, 3, idHabitacion);
    bindTexto(stmt, 4, fecha);
    bindTexto(stmt, 5, hora);
    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (res != SQLITE_DONE) { printf("ERROR - Insertando llegada nocturna\n"); return 0; }
    return 1;
}

int mostrarLlegadasNocturnasPorColegial(sqlite3 *db, char dni[]) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "SELECT id_llegada, id_habitacion, fecha, hora "
        "FROM llegada_nocturna "
        "WHERE dni_colegial = ? "
        "ORDER BY fecha DESC, hora DESC";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;

    bindTexto(stmt, 1, dni);
    printf("\n--- Llegadas nocturnas del colegial %s ---\n", dni);
    do {
        res = sqlite3_step(stmt);
        if (res == SQLITE_ROW)
            printf("[%d] Habitacion: %s | Fecha: %s | Hora: %s\n",
                sqlite3_column_int(stmt, 0),
                sqlite3_column_text(stmt, 1),
                sqlite3_column_text(stmt, 2),
                sqlite3_column_text(stmt, 3));
    } while (res == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return 1;
}

int mostrarLlegadasNocturnasPorFecha(sqlite3 *db, char fecha[]) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "SELECT ln.id_llegada, p.nombre, p.apellidos, "
        "ln.id_habitacion, ln.hora "
        "FROM llegada_nocturna ln "
        "JOIN persona p ON ln.dni_colegial = p.dni "
        "WHERE ln.fecha = ? "
        "ORDER BY ln.hora";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;

    bindTexto(stmt, 1, fecha);
    printf("\n--- Llegadas nocturnas el %s ---\n", fecha);
    do {
        res = sqlite3_step(stmt);
        if (res == SQLITE_ROW)
            printf("[%d] %-20s | Hab: %s | Hora: %s\n",
                sqlite3_column_int(stmt, 0),
                sqlite3_column_text(stmt, 1),
                sqlite3_column_text(stmt, 3),
                sqlite3_column_text(stmt, 4));
    } while (res == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return 1;
}

// SALIDAS POR VACACIONES

int registrarSalidaVacaciones(sqlite3 *db,
                              int idSalida, char dniColegial[],
                              char fechaSalida[], char fechaRegreso[],
                              char tipo[]) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "INSERT INTO salida_vacaciones("
        "id_salida, dni_colegial, fecha_salida, fecha_regreso, tipo) "
        "VALUES(?, ?, ?, ?, ?)";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) { printf("ERROR - Preparando salida vacaciones\n"); return 0; }

    sqlite3_bind_int(stmt, 1, idSalida);
    bindTexto(stmt, 2, dniColegial);
    bindTexto(stmt, 3, fechaSalida);
    if (fechaRegreso != NULL && strlen(fechaRegreso) > 0)
        bindTexto(stmt, 4, fechaRegreso);
    else
        sqlite3_bind_null(stmt, 4);
    bindTexto(stmt, 5, tipo);
    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (res != SQLITE_DONE) { printf(" ERROR - Insertando salida vacaciones\n"); return 0; }
    return 1;
}

int registrarRegresoVacaciones(sqlite3 *db, int idSalida, char fechaRegreso[]) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "UPDATE salida_vacaciones SET fecha_regreso = ? WHERE id_salida = ?";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;

    bindTexto(stmt, 1, fechaRegreso);
    sqlite3_bind_int(stmt, 2, idSalida);
    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return (res == SQLITE_DONE) ? 1 : 0;
}

int mostrarColegialFuera(sqlite3 *db) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "SELECT p.nombre, p.apellidos, sv.fecha_salida, sv.tipo "
        "FROM salida_vacaciones sv "
        "JOIN persona p ON sv.dni_colegial = p.dni "
        "WHERE sv.fecha_regreso IS NULL "
        "ORDER BY sv.fecha_salida";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;

    printf("\n--- Colegiales fuera actualmente ---\n");
    do {
        res = sqlite3_step(stmt);
        if (res == SQLITE_ROW)
            printf("%-25s | Salida: %s | Tipo: %s\n",
                sqlite3_column_text(stmt, 0),
                sqlite3_column_text(stmt, 2),
                sqlite3_column_text(stmt, 3));
    } while (res == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return 1;
}

// SANCIONES

int insertarSancion(sqlite3 *db,
                    int idSancion, char dniColegial[],
                    char fecha[], char tipo[],
                    char motivo[], int duracionDias) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "INSERT INTO sancion("
        "id_sancion, dni_colegial, fecha, tipo, motivo, duracion_dias) "
        "VALUES(?, ?, ?, ?, ?, ?)";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) { printf("ERROR - Preparando sancion\n"); return 0; }

    sqlite3_bind_int(stmt, 1, idSancion);
    bindTexto(stmt, 2, dniColegial);
    bindTexto(stmt, 3, fecha);
    bindTexto(stmt, 4, tipo);
    bindTexto(stmt, 5, motivo);
    if (duracionDias > 0)
        sqlite3_bind_int(stmt, 6, duracionDias);
    else
        sqlite3_bind_null(stmt, 6);
    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (res != SQLITE_DONE) { printf("ERROR - Insertando sancion\n"); return 0; }
    return 1;
}

int mostrarSancionesPorColegial(sqlite3 *db, char dni[]) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "SELECT id_sancion, fecha, tipo, motivo, duracion_dias "
        "FROM sancion "
        "WHERE dni_colegial = ? "
        "ORDER BY fecha DESC";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;

    bindTexto(stmt, 1, dni);
    printf("\n--- Sanciones del colegial %s ---\n", dni);

    do {
        res = sqlite3_step(stmt);
        if (res == SQLITE_ROW) {
            int dur = sqlite3_column_type(stmt, 4) == SQLITE_NULL ? 0 : sqlite3_column_int(stmt, 4);

            printf("[%d] %s | %-10s | %s | ",
                sqlite3_column_int(stmt, 0),
                sqlite3_column_text(stmt, 1),
                sqlite3_column_text(stmt, 2),
                sqlite3_column_text(stmt, 3));

            if (dur > 0) {
                printf("Duracion: %d dias\n", dur);
            } else {
                printf("Duracion: -\n");
            }
        }
    } while (res == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return 1;
}

int mostrarSancionesRecientes(sqlite3 *db) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "SELECT s.id_sancion, p.nombre, p.apellidos, "
        "s.fecha, s.tipo, s.motivo "
        "FROM sancion s "
        "JOIN persona p ON s.dni_colegial = p.dni "
        "ORDER BY s.fecha DESC "
        "LIMIT 20";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;

    printf("\n--- Sanciones recientes ---\n");
    do {
        res = sqlite3_step(stmt);
        if (res == SQLITE_ROW)
            printf("[%d] %-20s | %s | %-10s | %s\n",
                sqlite3_column_int(stmt, 0),
                sqlite3_column_text(stmt, 1),
                sqlite3_column_text(stmt, 3),
                sqlite3_column_text(stmt, 4),
                sqlite3_column_text(stmt, 5));
    } while (res == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return 1;
}

// ACTIVIDADES

int insertarActividad(sqlite3 *db,
                      int idActividad, char nombre[],
                      char tipo[], char fecha[],
                      char hora[], char lugar[],
                      char dniEmpleado[]) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "INSERT INTO actividad("
        "id_actividad, nombre, tipo, fecha, hora, lugar, dni_empleado) "
        "VALUES(?, ?, ?, ?, ?, ?, ?)";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) { printf("ERROR - Preparando actividad\n"); return 0; }

    sqlite3_bind_int(stmt, 1, idActividad);
    bindTexto(stmt, 2, nombre);
    bindTexto(stmt, 3, tipo);
    bindTexto(stmt, 4, fecha);
    bindTexto(stmt, 5, hora);
    bindTexto(stmt, 6, lugar);
    bindTexto(stmt, 7, dniEmpleado);
    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (res != SQLITE_DONE) { printf("ERROR - Insertando actividad\n"); return 0; }
    return 1;
}

int inscribirColegialActividad(sqlite3 *db,
                               int idActividad, char dniColegial[],
                               char fechaInscripcion[]) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "INSERT  INTO inscripcion_colegial("
        "id_actividad, dni_colegial, fecha_inscripcion) "
        "VALUES(?, ?, ?)";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) { printf("ERROR - Preparando inscripcion\n"); return 0; }

    sqlite3_bind_int(stmt, 1, idActividad);
    bindTexto(stmt, 2, dniColegial);
    bindTexto(stmt, 3, fechaInscripcion);
    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (res != SQLITE_DONE) { printf("ERROR - Insertando inscripcion\n"); return 0; }
    return 1;
}

int mostrarInscritos(sqlite3 *db, int idActividad) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "SELECT p.nombre, p.apellidos, i.fecha_inscripcion "
        "FROM inscripcion_colegial i "
        "JOIN persona p ON i.dni_colegial = p.dni "
        "WHERE i.id_actividad = ? "
        "ORDER BY p.apellidos";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;

    sqlite3_bind_int(stmt, 1, idActividad);
    printf("\n--- Inscritos en actividad %d ---\n", idActividad);
    do {
        res = sqlite3_step(stmt);
        if (res == SQLITE_ROW)
            printf("%-25s | Inscrito: %s\n",
                sqlite3_column_text(stmt, 0),
                sqlite3_column_text(stmt, 2));
    } while (res == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return 1;
}

// RESERVA SALAS

int insertarReserva(sqlite3 *db,
                    int idReserva, char idSala[],
                    char dniColegial[], char fecha[],
                    char horaInicio[], char horaFin[]) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "INSERT INTO reserva_sala("
        "id_reserva, id_sala, dni_colegial, fecha, hora_inicio, hora_fin) "
        "VALUES(?, ?, ?, ?, ?, ?)";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) { printf("ERROR - Preparando reserva\n"); return 0; }

    sqlite3_bind_int(stmt, 1, idReserva);
    bindTexto(stmt, 2, idSala);
    bindTexto(stmt, 3, dniColegial);
    bindTexto(stmt, 4, fecha);
    bindTexto(stmt, 5, horaInicio);
    bindTexto(stmt, 6, horaFin);
    res = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (res != SQLITE_DONE) { printf("ERROR - Insertando reserva\n"); return 0; }
    return 1;
}

int mostrarReservasPorFecha(sqlite3 *db, char fecha[]) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "SELECT r.id_reserva, r.id_sala, p.nombre, p.apellidos, "
        "r.hora_inicio, r.hora_fin "
        "FROM reserva_sala r "
        "JOIN persona p ON r.dni_colegial = p.dni "
        "WHERE r.fecha = ? "
        "ORDER BY r.id_sala, r.hora_inicio";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;

    bindTexto(stmt, 1, fecha);
    printf("\n--- Reservas el %s ---\n", fecha);
    do {
        res = sqlite3_step(stmt);
        if (res == SQLITE_ROW)
            printf("[%d] Sala: %-8s | %-20s | %s - %s\n",
                sqlite3_column_int(stmt, 0),
                sqlite3_column_text(stmt, 1),
                sqlite3_column_text(stmt, 2),
                sqlite3_column_text(stmt, 4),
                sqlite3_column_text(stmt, 5));
    } while (res == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return 1;
}

int mostrarReservasPorSala(sqlite3 *db, char idSala[]) {
    sqlite3_stmt *stmt;
    int res;
    char sql[] =
        "SELECT r.id_reserva, p.nombre, p.apellidos, "
        "r.fecha, r.hora_inicio, r.hora_fin "
        "FROM reserva_sala r "
        "JOIN persona p ON r.dni_colegial = p.dni "
        "WHERE r.id_sala = ? "
        "ORDER BY r.fecha, r.hora_inicio";

    res = sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL);
    if (res != SQLITE_OK) return 0;

    bindTexto(stmt, 1, idSala);
    printf("\n--- Reservas sala %s ---\n", idSala);
    do {
        res = sqlite3_step(stmt);
        if (res == SQLITE_ROW)
            printf("[%d] %-20s | %s %s - %s\n",
                sqlite3_column_int(stmt, 0),
                sqlite3_column_text(stmt, 1),
                sqlite3_column_text(stmt, 3),
                sqlite3_column_text(stmt, 4),
                sqlite3_column_text(stmt, 5));
    } while (res == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return 1;
}

// DATOS INICIALES DE PRUEBA

int cargarDatosPrueba(sqlite3 *db) {

    /* Torres */
    if (!insertarTorre(db, 1, "Izquierda"))  return 0;
    if (!insertarTorre(db, 2, "Central"))    return 0;
    if (!insertarTorre(db, 3, "Derecha"))    return 0;

    /* Pisos (uno por torre para la demo) */
    if (!insertarPiso(db, 1, 5, 1)) return 0;  /* Torre Izquierda, planta 5 */
    if (!insertarPiso(db, 2, 4, 2)) return 0;  /* Torre Izquierda, planta 4 */
    if (!insertarPiso(db, 3, 5, 3)) return 0;  /* Torre Central,   planta 5 */

    /* Habitaciones */
    if (!insertarHabitacion(db, "C508", 1)) return 0;
    if (!insertarHabitacion(db, "C518", 1)) return 0;
    if (!insertarHabitacion(db, "I414", 2)) return 0;
    if (!insertarHabitacion(db, "C501", 3)) return 0;
    if (!insertarHabitacion(db, "C502", 3)) return 0;

    /* Colegiales */
    if (!insertarColegial(db,
            "12345678A", "David", "Fernandez-Sopena Hernandez",
            "600123123", "david@gmail.com",
            "david@opendeusto.es", "611000001", "padre.david@gmail.com",
            "622000002", "madre.david@gmail.com",
            "ES91 2100 0418 4502 0005 1332",
            "2006-11-18", "2", "Ingenieria Informatica + Ciencia de Datos e IA", "huevo",
            "C508", 1)) return 0;

    if (!insertarColegial(db,
            "87654321B", "Javier", "Montero Villoslada",
            "600999999", "javi@gmail.com",
            "montero.javier@opendeusto.es", "611000003", "padre.javi@gmail.com",
            "622000004", "madre.javi@gmail.com",
            "ES80 2310 0001 1800 0001 2345",
            "2005-8-01", "2", "Ingenieria Informatica + Ciencia de Datos e IA", "",
            "C518", 1)) return 0;

    if (!insertarColegial(db,
            "54022683F", "Lucia", "Salvatierra Saenz",
            "600500500", "lucia@gmail.com",
            "lucia.salvatierra@opendeusto.es", "611000005", "padre.lucia@gmail.com",
            "622000006", "madre.lucia@gmail.com",
            "ES76 0049 1500 0527 6004 3011",
            "2005-02-20", "2", "Ingenieria Informatica + Ciencia de Datos e IA", "frutos secos",
            "I414", 2)) return 0;

    return 1;
}
