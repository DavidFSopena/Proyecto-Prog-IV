#include <stdio.h>
#include <stdlib.h>
#include "menus.h"
#include "utils.h"


void mostrarMenuPrincipal() {
    printf("\nMENÚ PRINCIPAL CMD\n");
    printf("1. Gestión de colegiales\n");
    printf("2. Gestión de habitaciones\n");
    printf("3. Gestión de incidencias\n");
    printf("4. Gestión de pagos\n");
    printf("5. Reservas y actividades\n");
    printf("6. Llegadas nocturnas\n");
    printf("7. Salidas por vacaciones\n");
    printf("8. Normas y sanciones\n");
    printf("9. Propuestas y comentarios\n");
    printf("0. Cerrar sesión\n");
    printf("Elige una opcion: ");
}

void mostrarMenuGestionColegiales() {
    printf("\nGESTIÓN DE COLEGIALES\n");
    printf("1. Alta de colegial\n");
    printf("2. Baja de colegial\n");
    printf("3. Modificar datos colegial\n");
    printf("4. Consultar por DNI\n");
    printf("5. Buscar colegiales por filtros\n");
    printf("0. Volver\n");
    printf("Elige una opción: ");
}

void mostrarMenuGestionHabitaciones() {
    printf("\nGESTIÓN DE HABITACIONES\n");
    printf("1. Consultar disponibilidad\n");
    printf("2. Asignar habitación a colegial\n");
    printf("3. Resgistrar huésped (hotel - planta A)\n");
    printf("4. Liberar habitación\n");
    printf("5. Registrar solicitud cambio de habitación\n");
    printf("0. Volver\n");
    printf("Elige una opción: ");
}

void mostrarMenuGestionIncidencias() {
    printf("\nGESTIÓN DE INCIDENCIAS\n");
    printf("1. Registrar nueva incidencia\n");
    printf("2. Ver incidencias abiertas\n");
    printf("3. Cambiar estado de incidencia\n");
    printf("4. Consultar por habitación\n");
    printf("0. Volver\n");
    printf("Elige una opción: ");
}

void mostrarMenuGestionPagos() {
    printf("\nGESTIÓN DE PAGOS\n");
    printf("1. Registrar modo de pago\n");
    printf("2. Registrar pago realizado\n");
    printf("3. Consultar estado de pagos por colegial\n");
    printf("4. Ver pagos pendientes\n");
    printf("0. Volver\n");
    printf("Elige una opción: ");
}

void mostrarMenuGestionReservasActividades() {
    printf("\nRESERVAS Y ACTIVIDADES\n");
    printf("1. Reservar sala de trabajo\n");
    printf("2. Consultar reservas salas(por fecha / sala)\n");
    printf("3. Cancelar reserva sala\n");
    printf("4. Registrar inscripción a actividad\n");
    printf("5. Ver inscritos por actividad\n");
    printf("0. Volver\n");
    printf("Elige una opción: ");
}

void mostrarMenuLlegadasNocturnas() {
    printf("\nREGISTRO LLEGADAS NOCTURNAS\n");
    printf("1. Registrar llegada (DNI o habitación)\n");
    printf("2. Consultar histórico por colegial\n");
    printf("0. Volver\n");
    printf("Elige una opción: ");
}

void mostrarMenuSalidasVacaciones() {
    printf("\nREGISTRO SALIDAS POR VACACIONES\n");
    printf("1. Registrar salida (entrega de llave)\n");
    printf("2. Regustar llegada (recogida de llave)\n");
    printf("3. Consultar quién está fuera actualmente\n");
    printf("0. Volver\n");
    printf("Elige una opción: ");
}

void mostrarMenuNormasSanciones() {
    printf("\nNORMAS Y SANCIONES\n");
    printf("1. Registrar sanción\n");
    printf("2. Consultar sanciones por colegial\n");
    printf("3. Ver sanciones recientes\n");
    printf("0. Volver\n");
    printf("Elige una opción: ");
}

void mostrarMenuPropuestasYComentarios() {
    printf("\nPROPUESTAS Y COMENTARIOS\n");
    printf("1. Registrar propuesta/comentario\n");
    printf("2. Ver pendientes\n");
    printf("3. Buscar por colegial\n");
    printf("4. Cambiar estado\n");
    printf("0. Volver\n");
    printf("Elige una opcion: ");
}

void menuPrincipal() {
    int opcion = 0;

    do {
        mostrarMenuPrincipal();

        switch (opcion) {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;
            case 8:
                break;
            case 9:
                menuPropuestasYComentarios();
                break;
            case 10:
                break;
            case 11:
                printf("\nCerrando sesión...\n");
                break;
            default:
                printf("\nOpcion no valida.\n");
        }

    } while (opcion != 11);
}
