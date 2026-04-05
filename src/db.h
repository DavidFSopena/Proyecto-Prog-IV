#ifndef DB_H_
#define DB_H_

#include "sqlite3.h"

int ejecutarSQL(sqlite3 *db, char sql[]);
int crearTablas(sqlite3 *db);
int insertarDatosPrueba(sqlite3 *db);
int mostrarColegiales(sqlite3 *db);

#endif /* DB_H_ */
