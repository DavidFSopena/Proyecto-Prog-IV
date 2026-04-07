#ifndef MENUS_H
#define MENUS_H

#include "sqlite3.h"

void pantallaInicio(sqlite3 *db);
void menuPrincipal(sqlite3 *db);

void menuGestionColegiales(sqlite3 *db);
void menuGestionHabitaciones(sqlite3 *db);
void menuGestionIncidencias(sqlite3 *db);
void menuGestionPagos(sqlite3 *db);
void menuReservasActividades(sqlite3 *db);
void menuLlegadasNocturnas(sqlite3 *db);
void menuSalidasVacaciones(sqlite3 *db);
void menuNormasSanciones(sqlite3 *db);
void menuPropuestasComentarios(sqlite3 *db);
void menuCambiarContrasena();

#endif
