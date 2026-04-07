#include <stdio.h>
#include <stdlib.h>
#include "menus.h"
#include "utils.h"


void mostrarMenuPrincipal() {
    printf("\nMENÚ PRINCIPAL\n");
    printf("1. Gestión de colegiales\n");
    printf("2. Gestión de habitaciones\n");
    printf("3. Gestión de incidencias\n");
    printf("4. Gestión de pagos\n");
    printf("5. Reservas y actividades\n");
    printf("6. Llegadas nocturnas\n");
    printf("7. Salidas por vacaciones\n");
    printf("8. Normas y sanciones\n");
    printf("9. Propuestas y comentarios\n");
    printf("10. Cambiar mi contraseña\n");
    printf("11. Cerrar sesión\n");
    printf("Elige una opcion: ");
}

void mostrarMenuPropuestasYComentarios() {
    printf("\nPROPUESTAS Y COMENTARIOS\n");
    printf("1. Registrar propuesta/comentario\n");
    printf("2. Ver pendientes\n");
    printf("3. Buscar por colegial\n");
    printf("4. Buscar por categoría\n");
    printf("5. Cambiar estado\n");
    printf("6. Cerrar propuesta\n");
    printf("7. Volver\n");
    printf("Elige una opcion: ");
}

void menuPropuestasYComentarios() {
    int opcion = 0;
    char texto[20];

    do {
        mostrarMenuPropuestasYComentarios();
        scanf("%d", &opcion);
        getchar();

        switch (opcion) {
            case 1:
                printf("\nHas elegido: Registrar propuesta/comentario\n");
                break;
            case 2:
                printf("\nHas elegido: Ver pendientes\n");
                break;
            case 3:
                printf("\nHas elegido: Buscar por colegial\n");
                break;
            case 4:
                printf("\nHas elegido: Buscar por categoría\n");
                break;
            case 5:
                printf("\nHas elegido: Cambiar estado\n");
                break;
            case 6:
                printf("\nHas elegido: Cerrar propuesta\n");
                break;
            case 7:
                printf("\nVolviendo al MENÚ PRINCIPAL...\n");
                break;
            default:
                printf("\nOpcion no valida.\n");
        }

    } while (opcion != 7);
}

void menuPrincipal() {
    int opcion = 0;
    char texto[20];

    do {
        mostrarMenuPrincipal();
        scanf("%d", &opcion);
        getchar();

        switch (opcion) {
            case 1:
                printf("\nHas elegido: Gestión de colegiales\n");
                break;
            case 2:
                printf("\nHas elegido: Gestión de habitaciones\n");
                break;
            case 3:
                printf("\nHas elegido: Gestión de incidencias\n");
                break;
            case 4:
                printf("\nHas elegido: Gestión de pagos\n");
                break;
            case 5:
                printf("\nHas elegido: Reservas y actividades\n");
                break;
            case 6:
                printf("\nHas elegido: Llegadas nocturnas\n");
                break;
            case 7:
                printf("\nHas elegido: Salidas por vacaciones\n");
                break;
            case 8:
                printf("\nHas elegido: Normas y sanciones\n");
                break;
            case 9:
                menuPropuestasYComentarios();
                break;
            case 10:
                printf("\nHas elegido: Cambiar mi contraseña\n");
                break;
            case 11:
                printf("\nCerrando sesión...\n");
                break;
            default:
                printf("\nOpcion no valida.\n");
        }

    } while (opcion != 11);
}
