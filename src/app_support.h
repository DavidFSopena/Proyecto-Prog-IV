#ifndef APP_SUPPORT_H
#define APP_SUPPORT_H

#include "sqlite3.h"
#include "config.h"

int iniciarSistema(sqlite3 **db, Config *cfg, const char *rutaConfig);
int autenticarSistema(const Config *cfg);
void cerrarSistema(sqlite3 *db);

#endif
