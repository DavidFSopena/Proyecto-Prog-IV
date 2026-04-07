#ifndef LOGIN_H
#define LOGIN_H

#include "config.h"

int comprobarLogin(const Config *cfg, const char *usuario, const char *password);
int pedirLogin(const Config *cfg);

#endif
