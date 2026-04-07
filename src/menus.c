#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menus.h"
#include "utils.h"
#include "db.h"

static sqlite3 *g_db = NULL;
void menuSetDB(sqlite3 *db) {
	g_db = db;
}

void mostrarMenuPrincipal() {
	printf("\nMENÚ PRINCIPAL CMD\n");
	printf("\n_______________________\n");
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
	printf("\n_______________________\n");
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
	printf("\n_______________________\n");
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
	printf("\n_______________________\n");
	printf("1. Registrar nueva incidencia\n");
	printf("2. Ver incidencias abiertas\n");
	printf("3. Cambiar estado de incidencia\n");
	printf("4. Consultar por habitación\n");
	printf("0. Volver\n");
	printf("Elige una opción: ");
}

void mostrarMenuGestionPagos() {
	printf("\nGESTIÓN DE PAGOS\n");
	printf("\n_______________________\n");
	printf("1. Registrar modo de pago\n");
	printf("2. Registrar pago realizado\n");
	printf("3. Consultar estado de pagos por colegial\n");
	printf("4. Ver pagos pendientes\n");
	printf("0. Volver\n");
	printf("Elige una opción: ");
}

void mostrarMenuGestionReservasActividades() {
	printf("\nRESERVAS Y ACTIVIDADES\n");
	printf("\n_______________________\n");
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
	printf("\n_______________________\n");
	printf("1. Registrar llegada (DNI o habitación)\n");
	printf("2. Consultar histórico por colegial\n");
	printf("0. Volver\n");
	printf("Elige una opción: ");
}

void mostrarMenuSalidasVacaciones() {
	printf("\nREGISTRO SALIDAS POR VACACIONES\n");
	printf("\n_______________________\n");
	printf("1. Registrar salida (entrega de llave)\n");
	printf("2. Regustar llegada (recogida de llave)\n");
	printf("3. Consultar quién está fuera actualmente\n");
	printf("0. Volver\n");
	printf("Elige una opción: ");
}

void mostrarMenuNormasSanciones() {
	printf("\nNORMAS Y SANCIONES\n");
	printf("\n_______________________\n");
	printf("1. Registrar sanción\n");
	printf("2. Consultar sanciones por colegial\n");
	printf("3. Ver sanciones recientes\n");
	printf("0. Volver\n");
	printf("Elige una opción: ");
}

void mostrarMenuPropuestasYComentarios() {
	printf("\nPROPUESTAS Y COMENTARIOS\n");
	printf("\n_______________________\n");
	printf("1. Registrar propuesta/comentario\n");
	printf("2. Ver pendientes\n");
	printf("3. Buscar por colegial\n");
	printf("4. Cambiar estado\n");
	printf("0. Volver\n");
	printf("Elige una opcion: ");
}

/* Genera un ID simple contando filas en una tabla
 * */
static int siguienteId(const char *tabla) {
	static int contador = 100;
	(void) tabla;
	return ++contador;
}

void menuPrincipal() {
	int opcion;

	do {
		mostrarMenuPrincipal();
		opcion = leerEntero();

		switch (opcion) {
		case 1:
			menuGestionColegiales();
			break;
		case 2:
			menuGestionHabitaciones();
			break;
		case 3:
			menuGestionIncidencias();
			break;
		case 4:
			menuGestionPagos();
			break;
		case 5:
			menuGestionReservasActividades();
			break;
		case 6:
			menuLlegadasNocturnas();
			break;
		case 7:
			menuSalidasVacaciones();
			break;
		case 8:
			menuNormasSanciones();
			break;
		case 9:
			menuPropuestasYComentarios();
			break;
		case 0:
			printf("\nCerrando sesion...\n");
			break;
		default:
			printf("\nOpcion no valida\n");
		}

	} while (opcion != 0);
}

void menuGestionColegiales() {
	int opcion;

	do {
		mostrarMenuGestionColegiales();
		opcion = leerEntero();

		switch (opcion) {

		case 1: {/* Alta de colegial */
			char dni[20], nombre[60], apellidos[80];
			char telefono[20], emailPersonal[80], emailDeusto[80];
			char telPadre1[20], emailPadre1[80];
			char telPadre2[20], emailPadre2[80];
			char iban[35], fechaNac[15], curso[5], grado[80], alergias[100];
			char habitacion[10];
			int idPiso;

			printf("\nAlta de colegial\n");
			printf("\n_______________________\n");
			printf("DNI: ");
			leerCadena(dni, sizeof(dni));
			printf("Nombre: ");
			leerCadena(nombre, sizeof(nombre));
			printf("Apellidos: ");
			leerCadena(apellidos, sizeof(apellidos));
			printf("Telefono: ");
			leerCadena(telefono, sizeof(telefono));
			printf("Email personal: ");
			leerCadena(emailPersonal, sizeof(emailPersonal));
			printf("Email @opendeusto: ");
			leerCadena(emailDeusto, sizeof(emailDeusto));
			printf("Telefono padre/madre 1: ");
			leerCadena(telPadre1, sizeof(telPadre1));
			printf("Email padre/madre 1: ");
			leerCadena(emailPadre1, sizeof(emailPadre1));
			printf("Telefono padre/madre 2: ");
			leerCadena(telPadre2, sizeof(telPadre2));
			printf("Email padre/madre 2: ");
			leerCadena(emailPadre2, sizeof(emailPadre2));
			printf("IBAN: ");
			leerCadena(iban, sizeof(iban));
			printf("Fecha nacimiento (YYYY-MM-DD): ");
			leerCadena(fechaNac, sizeof(fechaNac));
			printf("Curso (ej. 1, 2...): ");
			leerCadena(curso, sizeof(curso));
			printf("Grado universitario: ");
			leerCadena(grado, sizeof(grado));
			printf("Alergias (o Enter): ");
			leerCadena(alergias, sizeof(alergias));
			printf("Habitacion (ej. C508): ");
			leerCadena(habitacion, sizeof(habitacion));
			printf("ID piso (numero): ");
			idPiso = leerEntero();

			if (insertarColegial(g_db, dni, nombre, apellidos, telefono,
					emailPersonal, emailDeusto, telPadre1, emailPadre1,
					telPadre2, emailPadre2, iban, fechaNac, curso, grado,
					alergias, habitacion, idPiso)) {
				printf("OK - Colegial registrado correctamente.\n");
			} else {
				printf("ERROR - No se pudo registrar el colegial.\n");
			}
			pausarPantalla();
			break;
		}

		case 2: { /* Baja de colegial */
			char dni[20];
			printf("\nBaja de colegial\n");
			printf("\n_______________________\n");
			printf("DNI del colegial: ");
			leerCadena(dni, sizeof(dni));

			if (bajaColegial(g_db, dni)) {
				printf("OK - Colegial dado de baja correctamente.\n");
			} else {
				printf("ERROR - No se pudo dar de baja al colegial.\n");
			}
			pausarPantalla();
			break;
		}

		case 3: { /* Modificar datos */
			char dni[20];
			int subop;
			printf("\nModificar datos de colegial\n");
			printf("\n_______________________\n");
			printf("DNI del colegial: ");
			leerCadena(dni, sizeof(dni));

			printf("1. Cambiar telefono\n2. Cambiar email personal\nOpcion: ");
			subop = leerEntero();

			if (subop == 1) {
				char nuevo[20];
				printf("Nuevo telefono: ");
				leerCadena(nuevo, sizeof(nuevo));
				if (modificarTelefonoColegial(g_db, dni, nuevo))
					printf("OK - Telefono actualizado.\n");
				else
					printf("ERROR - No se pudo actualizar.\n");
			} else if (subop == 2) {
				char nuevo[80];
				printf("Nuevo email: ");
				leerCadena(nuevo, sizeof(nuevo));
				if (modificarEmailColegial(g_db, dni, nuevo))
					printf("OK - Email actualizado.\n");
				else
					printf("ERROR - No se pudo actualizar.\n");
			} else {
				printf("Opcion no valida.\n");
			}
			pausarPantalla();
			break;
		}

		case 4: { /* Consultar por DNI */
			char dni[20];
			printf("\nConsultar colegial por DNI\n");
			printf("\n_______________________\n");
			printf("DNI: ");
			leerCadena(dni, sizeof(dni));
			buscarColegialPorDni(g_db, dni);
			pausarPantalla();
			break;
		}

		case 5: { /* Buscar por filtros */
			int filtro;
			printf("\nBuscar colegiales por filtro\n");
			printf("\n_______________________\n");
			printf("1. Ver todos\n2. Por torre\n3. Por habitacion\nOpcion: ");
			filtro = leerEntero();

			if (filtro == 1) {
				mostrarColegiales(g_db);
			} else if (filtro == 2) {
				char torre[20];
				printf("Nombre de la torre (Izquierda / Central / Derecha): ");
				leerCadena(torre, sizeof(torre));
				listarColegialesTorre(g_db, torre);
			} else if (filtro == 3) {
				char hab[10];
				printf("ID habitacion (ej. C508): ");
				leerCadena(hab, sizeof(hab));
				buscarColegialPorHabitacion(g_db, hab);
			} else {
				printf("Opcion no valida.\n");
			}
			pausarPantalla();
			break;
		}

		case 0:
			break;

		default:
			printf("Opcion no valida.\n");
		}

	} while (opcion != 0);
}

void menuGestionHabitaciones() {
	int opcion;

	do {
		mostrarMenuGestionHabitaciones();
		opcion = leerEntero();

		switch (opcion) {

		case 1:
			mostrarHabitacionesDisponibles(g_db);
			pausarPantalla();
			break;

		case 2:
			mostrarOcupacionPorTorre(g_db);
			pausarPantalla();
			break;

		case 3: { /* Asignar habitacion a colegial */
			char dni[20], hab[10];
			printf("\nAsignar habitacion\n");
			printf("\n_______________________\n");
			printf("DNI del colegial: ");
			leerCadena(dni, sizeof(dni));
			printf("ID habitacion (ej. C508): ");
			leerCadena(hab, sizeof(hab));

			if (asignarHabitacionColegial(g_db, dni, hab))
				printf("OK - Habitacion asignada.\n");
			else
				printf("ERROR - No se pudo asignar la habitacion.\n");
			pausarPantalla();
			break;
		}

		case 4: { /* Registrar huesped */
			char dni[20], nombre[60], apellidos[80];
			char hab[10], fechaInicio[15], fechaFin[15];
			int idPiso;

			printf("\nRegistrar huesped (hotel)\n");
			printf("\n_______________________\n");
			printf("DNI: ");
			leerCadena(dni, sizeof(dni));
			printf("Nombre: ");
			leerCadena(nombre, sizeof(nombre));
			printf("Apellidos: ");
			leerCadena(apellidos, sizeof(apellidos));
			printf("Habitacion (planta A, ej. A101): ");
			leerCadena(hab, sizeof(hab));
			printf("ID piso: ");
			idPiso = leerEntero();
			printf("Fecha entrada (YYYY-MM-DD): ");
			leerCadena(fechaInicio, sizeof(fechaInicio));
			printf("Fecha salida  (YYYY-MM-DD): ");
			leerCadena(fechaFin, sizeof(fechaFin));

			if (insertarHuesped(g_db, dni, nombre, apellidos, hab, idPiso,
					fechaInicio, fechaFin))
				printf("OK - Huesped registrado.\n");
			else
				printf("ERROR - No se pudo registrar el huesped.\n");
			pausarPantalla();
			break;
		}

		case 5: { /* Dar de baja huesped */
			char dni[20];
			printf("\nDar de baja huesped\n");
			printf("\n_______________________\n");
			printf("DNI del huesped: ");
			leerCadena(dni, sizeof(dni));

			if (darBajaHuesped(g_db, dni))
				printf("OK - Huesped dado de baja.\n");
			else
				printf("ERROR - No se pudo dar de baja.\n");
			pausarPantalla();
			break;
		}

		case 6:
			mostrarHuespedes(g_db);
			pausarPantalla();
			break;

		case 0:
			break;

		default:
			printf("Opcion no valida.\n");
		}

	} while (opcion != 0);
}

void menuGestionIncidencias() {
	int opcion;

	do {
		mostrarMenuGestionIncidencias();
		opcion = leerEntero();

		switch (opcion) {

		case 1: { /* Registrar nueva incidencia */
			char desc[200], estado[15], fechaAp[15], dni[20], hab[10];
			int urgencia, id;

			printf("\nRegistrar incidencia\n");
			printf("\n_______________________\n");
			printf("Descripcion: ");
			leerCadena(desc, sizeof(desc));
			printf("Urgencia (1=Alta, 2=Media, 3=Baja): ");
			urgencia = leerEntero();
			printf("DNI del colegial: ");
			leerCadena(dni, sizeof(dni));
			printf("Habitacion afectada: ");
			leerCadena(hab, sizeof(hab));
			printf("Fecha apertura (YYYY-MM-DD): ");
			leerCadena(fechaAp, sizeof(fechaAp));

			strcpy(estado, "abierta");
			id = siguienteId("incidencia");

			if (insertarIncidencia(g_db, id, desc, urgencia, estado, fechaAp,
					"", dni, hab))
				printf("OK - Incidencia registrada con ID %d.\n", id);
			else
				printf("ERROR - No se pudo registrar la incidencia.\n");
			pausarPantalla();
			break;
		}

		case 2:
			mostrarIncidenciasAbiertas(g_db);
			pausarPantalla();
			break;

		case 3:
			mostrarIncidenciasOrdenadasUrgencia(g_db);
			pausarPantalla();
			break;

		case 4: { /* Cambiar estado */
			int id;
			char nuevoEstado[15], fechaCierre[15];

			printf("\nCambiar estado de incidencia\n");
			printf("\n_______________________\n");
			printf("ID de la incidencia: ");
			id = leerEntero();
			printf("Nuevo estado (abierta / en curso / cerrada): ");
			leerCadena(nuevoEstado, sizeof(nuevoEstado));

			fechaCierre[0] = '\0';
			if (strcmp(nuevoEstado, "cerrada") == 0) {
				printf("Fecha de cierre (YYYY-MM-DD): ");
				leerCadena(fechaCierre, sizeof(fechaCierre));
			}

			if (cambiarEstadoIncidencia(g_db, id, nuevoEstado, fechaCierre))
				printf("OK - Estado actualizado.\n");
			else
				printf("ERROR - No se pudo actualizar el estado.\n");
			pausarPantalla();
			break;
		}

		case 5: { /* Consultar por habitacion */
			char hab[10];
			printf("\nIncidencias por habitacion\n");
			printf("\n_______________________\n");
			printf("ID habitacion: ");
			leerCadena(hab, sizeof(hab));
			mostrarIncidenciasPorHabitacion(g_db, hab);
			pausarPantalla();
			break;
		}

		case 0:
			break;

		default:
			printf("Opcion no valida.\n");
		}

	} while (opcion != 0);
}


void menuGestionPagos() {
	int opcion;

	do {
		mostrarMenuGestionPagos();
		opcion = leerEntero();

		switch (opcion) {

		case 1: { /* Registrar pago */
			char dni[20], modalidad[15], fecha[15];
			int importe, id;

			printf("\nRegistrar pago\n");
			printf("\n_______________________\n");
			printf("DNI del colegial: ");
			leerCadena(dni, sizeof(dni));
			printf("Modalidad (mensual / unico): ");
			leerCadena(modalidad, sizeof(modalidad));
			printf("Fecha (YYYY-MM-DD): ");
			leerCadena(fecha, sizeof(fecha));
			printf("Importe (EUR): ");
			importe = leerEntero();

			id = siguienteId("pago");
			if (insertarPago(g_db, id, dni, modalidad, "pendiente", fecha,
					importe))
				printf("OK - Pago registrado con ID %d.\n", id);
			else
				printf("ERROR - No se pudo registrar el pago.\n");
			pausarPantalla();
			break;
		}

		case 2: { /* Marcar pago realizado */
			int id;
			printf("\nMarcar pago como realizado\n");
			printf("\n_______________________\n");
			printf("ID del pago: ");
			id = leerEntero();

			if (marcarPagoRealizado(g_db, id))
				printf("OK - Pago marcado como realizado.\n");
			else
				printf("ERROR - No se pudo marcar el pago.\n");
			pausarPantalla();
			break;
		}

		case 3: { /* Pagos por colegial */
			char dni[20];
			printf("\nPagos por colegial\n");
			printf("\n_______________________\n");
			printf("DNI del colegial: ");
			leerCadena(dni, sizeof(dni));
			mostrarPagosPorColegial(g_db, dni);
			pausarPantalla();
			break;
		}

		case 4:
			mostrarPagosPendientes(g_db);
			pausarPantalla();
			break;

		case 0:
			break;

		default:
			printf("Opcion no valida\n");
		}

	} while (opcion != 0);
}

void menuGestionReservasActividades() {
	int opcion;

	do {
		mostrarMenuGestionReservasActividades();
		opcion = leerEntero();

		switch (opcion) {

		case 1: { /* Reservar sala */
			char sala[20], dni[20], fecha[15], horaIni[10], horaFin[10];
			int id;

			printf("\nReservar sala de trabajo\n");
			printf("\n_______________________\n");
			printf("ID sala (ej. SALA_A1): ");
			leerCadena(sala, sizeof(sala));
			printf("DNI del colegial: ");
			leerCadena(dni, sizeof(dni));
			printf("Fecha (YYYY-MM-DD): ");
			leerCadena(fecha, sizeof(fecha));
			printf("Hora inicio (HH:MM): ");
			leerCadena(horaIni, sizeof(horaIni));
			printf("Hora fin   (HH:MM): ");
			leerCadena(horaFin, sizeof(horaFin));

			id = siguienteId("reserva");
			if (insertarReserva(g_db, id, sala, dni, fecha, horaIni, horaFin))
				printf("OK - Reserva creada con ID %d.\n", id);
			else
				printf("ERROR - No se pudo crear la reserva.\n");
			pausarPantalla();
			break;
		}

		case 2: { /* Reservas por fecha */
			char fecha[15];
			printf("\nReservas por fecha\n");
			printf("\n_______________________\n");
			printf("Fecha (YYYY-MM-DD): ");
			leerCadena(fecha, sizeof(fecha));
			mostrarReservasPorFecha(g_db, fecha);
			pausarPantalla();
			break;
		}

		case 3: { /* Reservas por sala */
			char sala[20];
			printf("\nReservas por sala\n");
			printf("\n_______________________\n");
			printf("ID sala: ");
			leerCadena(sala, sizeof(sala));
			mostrarReservasPorSala(g_db, sala);
			pausarPantalla();
			break;
		}

		case 4: { /* Registrar actividad */
			char nombre[80], tipo[20], fecha[15], hora[10], lugar[80];
			int id;

			printf("\nRegistrar actividad\n");
			printf("\n_______________________\n");
			printf("Nombre: ");
			leerCadena(nombre, sizeof(nombre));
			printf("Tipo (coloquio / evento): ");
			leerCadena(tipo, sizeof(tipo));
			printf("Fecha (YYYY-MM-DD): ");
			leerCadena(fecha, sizeof(fecha));
			printf("Hora (HH:MM): ");
			leerCadena(hora, sizeof(hora));
			printf("Lugar: ");
			leerCadena(lugar, sizeof(lugar));

			id = siguienteId("actividad");
			if (insertarActividad(g_db, id, nombre, tipo, fecha, hora, lugar,
					""))
				printf("OK - Actividad registrada con ID %d.\n", id);
			else
				printf("ERROR - No se pudo registrar la actividad.\n");
			pausarPantalla();
			break;
		}

		case 5: { /* Inscribir colegial a actividad */
			char dni[20], fecha[15];
			int idAct;

			printf("\nInscribir colegial a actividad\n");
			printf("\n_______________________\n");
			printf("ID de la actividad: ");
			idAct = leerEntero();
			printf("DNI del colegial: ");
			leerCadena(dni, sizeof(dni));
			printf("Fecha inscripcion (YYYY-MM-DD): ");
			leerCadena(fecha, sizeof(fecha));

			if (inscribirColegialActividad(g_db, idAct, dni, fecha))
				printf("OK - Colegial inscrito.\n");
			else
				printf("ERROR - No se pudo inscribir al colegial.\n");
			pausarPantalla();
			break;
		}

		case 6: { /* Ver inscritos */
			int idAct;
			printf("\nInscritos en actividad\n");
			printf("\n_______________________\n");
			printf("ID de la actividad: ");
			idAct = leerEntero();
			mostrarInscritos(g_db, idAct);
			pausarPantalla();
			break;
		}

		case 0:
			break;

		default:
			printf("Opcion no valida\n");
		}

	} while (opcion != 0);
}

/* ────────────────────────────────────────────────────────────
 SUBMENÚ: LLEGADAS NOCTURNAS
 ──────────────────────────────────────────────────────────── */

void menuLlegadasNocturnas() {
	int opcion;

	do {
		mostrarMenuLlegadasNocturnas();
		opcion = leerEntero();

		switch (opcion) {

		case 1: { /* Registrar llegada */
			char dni[20], hab[10], fecha[15], hora[10];
			int id;

			printf("\nRegistrar llegada nocturna\n");
			printf("\n_______________________\n");
			printf("DNI del colegial: ");
			leerCadena(dni, sizeof(dni));
			printf("Habitacion: ");
			leerCadena(hab, sizeof(hab));
			printf("Fecha (YYYY-MM-DD): ");
			leerCadena(fecha, sizeof(fecha));
			printf("Hora (HH:MM): ");
			leerCadena(hora, sizeof(hora));

			id = siguienteId("llegada");
			if (registrarLlegadaNocturna(g_db, id, dni, hab, fecha, hora))
				printf("OK - Llegada nocturna registrada.\n");
			else
				printf("ERROR - No se pudo registrar.\n");
			pausarPantalla();
			break;
		}

		case 2: { /* Por fecha */
			char fecha[15];
			printf("\nLlegadas nocturnas por fecha\n");
			printf("\n_______________________\n");
			printf("Fecha (YYYY-MM-DD): ");
			leerCadena(fecha, sizeof(fecha));
			mostrarLlegadasNocturnasPorFecha(g_db, fecha);
			pausarPantalla();
			break;
		}

		case 3: { /* Historico por colegial */
			char dni[20];
			printf("\nHistorico de llegadas por colegial\n");
			printf("\n_______________________\n");
			printf("DNI del colegial: ");
			leerCadena(dni, sizeof(dni));
			mostrarLlegadasNocturnasPorColegial(g_db, dni);
			pausarPantalla();
			break;
		}

		case 0:
			break;

		default:
			printf("Opcion no válida.\n");
		}

	} while (opcion != 0);
}

void menuSalidasVacaciones() {
	int opcion;

	do {
		mostrarMenuSalidasVacaciones();
		opcion = leerEntero();

		switch (opcion) {

		case 1: { /* Registrar salida */
			char dni[20], fechaSalida[15], tipo[20];
			int id;

			printf("\nRegistrar salida por vacaciones\n");
			printf("\n_______________________\n");
			printf("DNI del colegial: ");
			leerCadena(dni, sizeof(dni));
			printf("Fecha salida (YYYY-MM-DD): ");
			leerCadena(fechaSalida, sizeof(fechaSalida));
			printf("Tipo (navidad / semana_santa / otra): ");
			leerCadena(tipo, sizeof(tipo));

			id = siguienteId("salida");
			if (registrarSalidaVacaciones(g_db, id, dni, fechaSalida, "", tipo))
				printf("OK - Salida registrada. Llave entregada.\n");
			else
				printf("ERROR - No se pudo registrar la salida.\n");
			pausarPantalla();
			break;
		}

		case 2: { /* Registrar regreso */
			int id;
			char fechaRegreso[15];

			printf("\nRegistrar regreso\n");
			printf("\n_______________________\n");
			printf("ID de la salida: ");
			id = leerEntero();
			printf("Fecha regreso (YYYY-MM-DD): ");
			leerCadena(fechaRegreso, sizeof(fechaRegreso));

			if (registrarRegresoVacaciones(g_db, id, fechaRegreso))
				printf("OK - Regreso registrado. Llave devuelta.\n");
			else
				printf("ERROR - No se pudo registrar el regreso.\n");
			pausarPantalla();
			break;
		}

		case 3:
			mostrarColegialFuera(g_db);
			pausarPantalla();
			break;

		case 0:
			break;

		default:
			printf("Opcion no valida.\n");
		}

	} while (opcion != 0);
}


void menuNormasSanciones() {
	int opcion;

	do {
		mostrarMenuNormasSanciones();
		opcion = leerEntero();

		switch (opcion) {

		case 1: { /* Registrar sancion */
			char dni[20], fecha[15], tipo[20], motivo[200];
			int duracion, id;

			printf("\nRegistrar sancion \n");
			printf("\n_______________________\n");
			printf("DNI del colegial: ");
			leerCadena(dni, sizeof(dni));
			printf("Fecha (YYYY-MM-DD): ");
			leerCadena(fecha, sizeof(fecha));
			printf("Tipo (aviso / temporal / permanente): ");
			leerCadena(tipo, sizeof(tipo));
			printf("Motivo: ");
			leerCadena(motivo, sizeof(motivo));

			duracion = 0;
			if (strcmp(tipo, "temporal") == 0) {
				printf("Duracion en dias: ");
				duracion = leerEntero();
			}

			id = siguienteId("sancion");
			if (insertarSancion(g_db, id, dni, fecha, tipo, motivo, duracion))
				printf("OK - Sancion registrada con ID %d.\n", id);
			else
				printf("ERROR - No se pudo registrar la sancion.\n");
			pausarPantalla();
			break;
		}

		case 2: { /* Sanciones por colegial */
			char dni[20];
			printf("\nSanciones por colegial\n");
			printf("\n_______________________\n");
			printf("DNI del colegial: ");
			leerCadena(dni, sizeof(dni));
			mostrarSancionesPorColegial(g_db, dni);
			pausarPantalla();
			break;
		}

		case 3:
			mostrarSancionesRecientes(g_db);
			pausarPantalla();
			break;

		case 0:
			break;

		default:
			printf("Opcion no valida\n");
		}

	} while (opcion != 0);
}

void menuPropuestasYComentarios() {
	int opcion;
	do {
		mostrarMenuPropuestasYComentarios();
		opcion = leerEntero();
		if (opcion != 0)
			printf("Funcionalidad en desarrollo\n");
	} while (opcion != 0);
}

