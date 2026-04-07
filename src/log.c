#include <stdio.h>
#include <time.h>
#include "log.h"

static void escribirLog(const char *rutaLog, const char *nivel, const char *mensaje) {
    FILE *f;
    time_t t;
    struct tm *tm_info;

    if (rutaLog == NULL || nivel == NULL || mensaje == NULL) {
        return;
    }

    f = fopen(rutaLog, "a");
    if (f == NULL) {
        return;
    }

    time(&t);
    tm_info = localtime(&t);

    if (tm_info == NULL) {
        fclose(f);
        return;
    }

    fprintf(f, "[%04d-%02d-%02d %02d:%02d:%02d] [%s] %s\n",
            tm_info->tm_year + 1900,
            tm_info->tm_mon + 1,
            tm_info->tm_mday,
            tm_info->tm_hour,
            tm_info->tm_min,
            tm_info->tm_sec,
            nivel,
            mensaje);

    fclose(f);
}

void logInfo(const char *rutaLog, const char *mensaje) {
    escribirLog(rutaLog, "INFO", mensaje);
}

void logError(const char *rutaLog, const char *mensaje) {
    escribirLog(rutaLog, "ERROR", mensaje);
}
