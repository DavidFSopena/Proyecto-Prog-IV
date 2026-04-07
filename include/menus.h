#ifndef MENUS_H
#define MENUS_H

#include "sqlite3.h"
void menuSetDB(sqlite3 *db);

void menuPrincipal();
void menuGestionColegiales();
void menuGestionHabitaciones();
void menuGestionIncidencias();
void menuGestionPagos();
void menuGestionReservasActividades();
void menuLlegadasNocturnas();
void menuSalidasVacaciones();
void menuNormasSanciones();
void menuPropuestasYComentarios();
void mostrarMenuPrincipal();
void mostrarMenuGestionColegiales();
void mostrarMenuGestionHabitaciones();
void mostrarMenuGestionIncidencias();
void mostrarMenuGestionPagos();
void mostrarMenuGestionReservasActividades();
void mostrarMenuLlegadasNocturnas();
void mostrarMenuSalidasVacaciones();
void mostrarMenuNormasSanciones();
void mostrarMenuPropuestasYComentarios();

#endif
