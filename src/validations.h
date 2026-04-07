#ifndef VALIDATIONS_H_
#define VALIDATIONS_H_

#ifndef VALIDATIONS_H_
#define VALIDATIONS_H_

int validarTextoVacio(const char *texto);
int validarEntero(const char *texto);
int validarOpcionMenu(int opcion, int min, int max);
int validarLongitud(const char *texto, int max);
int validarDNI(const char *dni);

#endif
