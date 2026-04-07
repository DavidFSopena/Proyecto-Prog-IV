#include <stdio.h>
#include "sqlite3.h"
#include "db.h"
#include "config.h"
#include "log.h"
#include "login.h"
#include "menus.h"


int main() {

	// BASE DE DATROS
    sqlite3 *db;
    int res;
    Config cfg;

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

    printf("--- Creando tablas ---\n");
    if (!crearTablas(db)) {
        sqlite3_close(db);
        return 1;
    }

    printf("--- Cargando datos de prueba ---\n");
    if (!cargarDatosPrueba(db)) {
        sqlite3_close(db);
        return 1;
    }

    // COLEGIALES
    mostrarColegiales(db);
    buscarColegialPorDni(db, "12345678A");
    buscarColegialPorHabitacion(db, "C518");

    // HABITACIONES
    mostrarHabitacionesDisponibles(db);
    mostrarOcupacionPorTorre(db);

    // INCIDENCIAS
    insertarIncidencia(db, 1, "Bombilla fundida en el bano", 3,
                       "abierta", "2026-04-06", "", "12345678A", "C508");
    insertarIncidencia(db, 2, "Calefaccion no funciona", 1,
                       "abierta", "2026-04-05", "", "87654321B", "C518");
    insertarIncidencia(db, 3, "Gotera en el techo", 2,
                       "abierta", "2026-04-04", "", "54022683F", "I414");

    mostrarIncidenciasAbiertas(db);
    mostrarIncidenciasOrdenadasUrgencia(db);

    cambiarEstadoIncidencia(db, 1, "cerrada", "2026-04-07");
    printf("\nOK - Incidencia 1 cerrada. Estado actualizado:\n");
    mostrarIncidenciasPorHabitacion(db, "C508");

    // PAGOS
    insertarPago(db, 1, "12345678A", "mensual",  "pendiente",  "2026-04-06", 850);
    insertarPago(db, 2, "87654321B", "unico",    "realizado",  "2026-04-06", 9000);
    insertarPago(db, 3, "54022683F", "mensual",  "pendiente",  "2026-04-06", 850);

    mostrarPagosPendientes(db);
    marcarPagoRealizado(db, 1);
    printf("\nOK - Pago 1 marcado como realizado:\n");
    mostrarPagosPendientes(db);
    mostrarPagosPorColegial(db, "12345678A");

    // LLEGADAS NOCTURNAS
    registrarLlegadaNocturna(db, 1, "12345678A", "C508", "2026-04-05", "01:23");
    registrarLlegadaNocturna(db, 2, "87654321B", "C518", "2026-04-06", "02:47");
    mostrarLlegadasNocturnasPorFecha(db, "2026-04-06");
    mostrarLlegadasNocturnasPorColegial(db, "12345678A");

    //SALIDA POR VACACIONES
    registrarSalidaVacaciones(db, 1, "54022683F", "2026-04-10", "", "semana_santa");
    mostrarColegialFuera(db);
    registrarRegresoVacaciones(db, 1, "2026-04-18");
    printf("\nOK - Regreso registrado:\n");
    mostrarColegialFuera(db);

    //SANCIONES
    insertarSancion(db, 1, "87654321B", "2026-04-06",
                   "aviso", "Introduccion de alcohol en el edificio", 0);
    mostrarSancionesPorColegial(db, "87654321B");
    mostrarSancionesRecientes(db);

    // ACTIVIDADESA
    insertarActividad(db, 1, "Coloquio sobre IA", "coloquio",
                     "2026-04-15", "19:00", "Salon de actos", "");
    inscribirColegialActividad(db, 1, "12345678A", "2026-04-07");
    inscribirColegialActividad(db, 1, "54022683F", "2026-04-07");
    mostrarInscritos(db, 1);

    // RESERVA DE LAS SALAS
    insertarReserva(db, 1, "SALA_A1", "12345678A",
                   "2026-04-08", "10:00", "12:00");
    insertarReserva(db, 2, "SALA_A1", "87654321B",
                   "2026-04-08", "16:00", "18:00");
    mostrarReservasPorFecha(db, "2026-04-08");
    mostrarReservasPorSala(db, "SALA_A1");

    // TORRES
    mostrarTorres(db);

    sqlite3_close(db);
    return 0;
}
