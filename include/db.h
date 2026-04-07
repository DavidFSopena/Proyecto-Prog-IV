#ifndef DB_H_
#define DB_H_

#include "sqlite3.h"

// Inicio bases
int ejecutarSQL(sqlite3 *db, char sql[]);

// Iniciar tablas y datos (los 3 colegiales que he puesto somos los que hemos hecho este proyecto)
int crearTablas(sqlite3 *db);
int cargarDatosPrueba(sqlite3 *db);

// Torres y pisos
int insertarTorre(sqlite3 *db, int idTorre, char nombre[]);
int insertarPiso(sqlite3 *db, int idPiso, int numeroPiso, int idTorre);
int mostrarTorres(sqlite3 *db);

// Habitaciones
int insertarHabitacion(sqlite3 *db, char idHabitacion[], int idPiso);
int habitacionDisponible(sqlite3 *db, char idHabitacion[]);
int asignarHabitacionColegial(sqlite3 *db, char dni[], char idHabitacion[]);
int mostrarHabitacionesDisponibles(sqlite3 *db);
int mostrarOcupacionPorTorre(sqlite3 *db);

// Personas
int insertarPersona(sqlite3 *db, char dni[], char nombre[], char apellidos[],
                    char telefono[], char email[]);
// Colegiales
int insertarColegial(sqlite3 *db,
                     char dni[], char nombre[], char apellidos[],
                     char telefono[], char emailPersonal[],
                     char emailDeusto[], char telefonoPadre1[],
                     char emailPadre1[], char telefonoPadre2[],
                     char emailPadre2[], char iban[],
                     char fechaNacimiento[], char curso[],
                     char grado[], char alergias[],
                     char idHabitacion[], int idPiso);int bajaColegial(sqlite3 *db, char dni[]);
int bajaColegial(sqlite3 *db, char dni[]);
int modificarTelefonoColegial(sqlite3 *db, char dni[], char nuevoTelefono[]);
int modificarEmailColegial(sqlite3 *db, char dni[], char nuevoEmail[]);
int mostrarColegiales(sqlite3 *db);
int buscarColegialPorDni(sqlite3 *db, char dni[]);
int buscarColegialPorHabitacion(sqlite3 *db, char idHabitacion[]);
int listarColegialesTorre(sqlite3 *db, char nombreTorre[]);
int colegialTieneIncidenciasAbiertas(sqlite3 *db, char dni[]);
int colegialTienePagosPendientes(sqlite3 *db, char dni[]);

// Huespedes
int insertarHuesped(sqlite3 *db,
                    char dni[], char nombre[], char apellidos[],
                    char idHabitacion[], int idPiso,
                    char fechaInicio[], char fechaFin[]);
int mostrarHuespedes(sqlite3 *db);
int darBajaHuesped(sqlite3 *db, char dni[]);

// Incidencias
int insertarIncidencia(sqlite3 *db,
                       int idIncidencia, char descripcion[],
                       int urgencia, char estado[],
                       char fechaApertura[], char fechaCierre[],
                       char dniColegial[], char idHabitacion[]);
int cambiarEstadoIncidencia(sqlite3 *db, int idIncidencia,
                            char nuevoEstado[], char fechaCierre[]);
int mostrarIncidenciasAbiertas(sqlite3 *db);
int mostrarIncidenciasOrdenadasUrgencia(sqlite3 *db);
int mostrarIncidenciasPorHabitacion(sqlite3 *db, char idHabitacion[]);

// Pagos
int insertarPago(sqlite3 *db,
                 int idPago, char dniColegial[], char modalidad[],
                 char estado[], char fecha[], int importe);
int marcarPagoRealizado(sqlite3 *db, int idPago);
int mostrarPagosPendientes(sqlite3 *db);
int mostrarPagosPorColegial(sqlite3 *db, char dniColegial[]);

// LLegadas nocturnas
int registrarLlegadaNocturna(sqlite3 *db,
                             int idLlegada, char dniColegial[],
                             char idHabitacion[], char fecha[], char hora[]);
int mostrarLlegadasNocturnasPorFecha(sqlite3 *db, char fecha[]);
int mostrarLlegadasNocturnasPorColegial(sqlite3 *db, char dniColegial[]);

// Salida de vacaciones
int registrarSalidaVacaciones(sqlite3 *db,
                              int idSalida, char dniColegial[],
                              char fechaSalida[], char fechaRegreso[],
                              char tipo[]);
int registrarRegresoVacaciones(sqlite3 *db, int idSalida, char fechaRegreso[]);
int mostrarColegialFuera(sqlite3 *db);


// Sanciones
int insertarSancion(sqlite3 *db,
                    int idSancion, char dniColegial[], char fecha[],
                    char tipo[], char motivo[], int duracionDias);
int mostrarSancionesPorColegial(sqlite3 *db, char dniColegial[]);
int mostrarSancionesRecientes(sqlite3 *db);

// Actividades
int insertarActividad(sqlite3 *db,
                      int idActividad, char nombre[], char tipo[],
                      char fecha[], char hora[], char lugar[],
                      char dniEmpleado[]);
int inscribirColegialActividad(sqlite3 *db,
                               int idActividad, char dniColegial[],
                               char fechaInscripcion[]);
int mostrarInscritos(sqlite3 *db, int idActividad);

// Reserva salas

int insertarReserva(sqlite3 *db,
                    int idReserva, char idSala[],
                    char dniColegial[], char fecha[],
                    char horaInicio[], char horaFin[]);
int mostrarReservasPorFecha(sqlite3 *db, char fecha[]);
int mostrarReservasPorSala(sqlite3 *db, char idSala[]);

#endif
