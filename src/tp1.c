#include "tp1.h"
#include "mis_funciones.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct tp1 {
	struct pokemon *pokemones;
	size_t cantidad;
};

// --------------------------------------------------------------------------------------------------------
tp1_t *tp1_leer_archivo(const char *nombre)
{
	tp1_t *tp1;
	FILE *archivo;
	char *linea;
	size_t capacidad = 128;
	struct pokemon *pk;

	archivo = archivo_open(nombre);
	if (!archivo) {
		return NULL;
	}

	tp1 = creando_maloc(sizeof(tp1_t),
			    "Error asignando memoria para el struct tp1");
	tp1->cantidad = 0;
	tp1->pokemones = NULL;

	while ((linea = leer_linea(archivo, &capacidad))) {
		pk = parsear_pokemon(linea);
		if (pk) {
			agregar_pokemon(tp1, pk);
		}
		free(linea);
	}

	fclose(archivo);
	return tp1;
}
// --------------------------------------------------------------------------------------------------------
size_t tp1_cantidad(tp1_t *tp1)
{
	if (!tp1) {
		return 0;
	} else {
		return tp1->cantidad;
	}
}
// --------------------------------------------------------------------------------------------------------
tp1_t *tp1_guardar_archivo(tp1_t *tp1, const char *nombre)
{
	FILE *archivo;

	if (!tp1 || !nombre || strlen(nombre) == 0) {
		return NULL;
	}

	archivo = archivo_crear(nombre);
	escribiendo_linea(tp1, archivo);

	fclose(archivo);

	return tp1;
}
// --------------------------------------------------------------------------------------------------------
tp1_t *tp1_union(tp1_t *un_tp, tp1_t *otro_tp)
{
	tp1_t *tp3;
	size_t i, j, k;
	bool existe;

	if (!un_tp || otro_tp->cantidad == 0 || un_tp->cantidad == 0) {
		return NULL;
	}

	tp3 = creando_maloc(sizeof(tp1_t),
			    "Error en creando malloc tp1 union\n");

	tp3->cantidad = 0;
	tp3->pokemones = NULL;

	// Copiamos todos los pokemones de un_tp
	for (i = 0; i < un_tp->cantidad; i++) {
		agregar_pokemon(tp3, &un_tp->pokemones[i]);
	}

	// Agregamos de otro_tp los que no están
	for (j = 0; j < otro_tp->cantidad; j++) {
		existe = false;
		for (k = 0; k < un_tp->cantidad; k++) {
			if (un_tp->pokemones[k].id ==
			    otro_tp->pokemones[j].id) {
				existe = true;
				break;
			}
		}
		if (!existe) {
			agregar_pokemon(tp3, &otro_tp->pokemones[j]);
		}
	}

	return tp3;
}
// --------------------------------------------------------------------------------------------------------
tp1_t *tp1_interseccion(tp1_t *un_tp, tp1_t *otro_tp)
{
	if (!un_tp || otro_tp->cantidad == 0 || un_tp->cantidad == 0) {
		return NULL;
	}

	tp1_t *tp3 = creando_maloc(sizeof(tp1_t),
				   "Error creando malloc para interseccion");
	tp3->cantidad = 0;
	tp3->pokemones = NULL;

	// Determino cuál es el TP más pequeño para iterar menos
	tp1_t *tp_pequeno = un_tp;
	tp1_t *tp_grande = otro_tp;
	if (un_tp->cantidad > otro_tp->cantidad) {
		tp_pequeno = otro_tp;
		tp_grande = un_tp;
	}

	for (size_t i = 0; i < tp_pequeno->cantidad; i++) {
		for (size_t j = 0; j < tp_grande->cantidad; j++) {
			if (tp_pequeno->pokemones[i].id ==
			    tp_grande->pokemones[j].id) {
				agregar_pokemon(tp3, &tp_pequeno->pokemones[i]);
				break;
			}
		}
	}

	return tp3;
}
// --------------------------------------------------------------------------------------------------------
tp1_t *tp1_diferencia(tp1_t *un_tp, tp1_t *otro_tp)
{
	bool encontrado;

	if (!un_tp || otro_tp->cantidad == 0 || un_tp->cantidad == 0) {
		return NULL;
	}

	tp1_t *tp3 = creando_maloc(sizeof(tp1_t),
				   "Error creando malloc para diferencia");
	tp3->cantidad = 0;
	tp3->pokemones = NULL;

	for (size_t i = 0; i < un_tp->cantidad; i++) {
		encontrado = false;

		// Reviso si está en el segundo tp1
		for (size_t j = 0; j < otro_tp->cantidad; j++) {
			if (un_tp->pokemones[i].id ==
			    otro_tp->pokemones[j].id) {
				encontrado = true;
				break;
			}
		}

		if (!encontrado) {
			agregar_pokemon(tp3, &un_tp->pokemones[i]);
		}
	}

	return tp3;
}
// --------------------------------------------------------------------------------------------------------
struct pokemon *tp1_buscar_nombre(tp1_t *tp, const char *nombre)
{
	if (!tp || !nombre) {
		return NULL;
	}

	size_t inf = 0;
	size_t sup = tp->cantidad - 1;
	bubbleSort_pokemones_alfabeticamente_asc_optimizado(tp);

	while (inf <= sup) {
		size_t mitad = (inf + sup) / 2;
		int cmp = strcmp(tp->pokemones[mitad].nombre, nombre);

		if (cmp == 0) {
			return &tp->pokemones[mitad]; // Lo encontro
		} else if (cmp < 0) {
			inf = mitad + 1;
		} else {
			if (mitad == 0)
				break; // Evita underflow en size_t
			sup = mitad - 1;
		}
	}
	return NULL; // No se encontró
}
// --------------------------------------------------------------------------------------------------------
struct pokemon *tp1_buscar_id(tp1_t *tp, int id)
{
	if (!tp || tp->cantidad == 0) {
		return NULL;
	}

	if (id < 0) {
		return NULL;
	}

	bubbleSort_pokemones_id_asc_optimizado(tp);

	size_t inf = 0;
	size_t sup = tp->cantidad - 1;

	while (inf <= sup) {
		size_t mitad = (inf + sup) / 2;

		if (tp->pokemones[mitad].id == id) {
			return &tp->pokemones[mitad]; // Lo encontro
		}

		else if (tp->pokemones[mitad].id < id) {
			inf = mitad + 1;
		}

		else {
			sup = mitad - 1;
		}
	}

	return NULL; // No se encontró
}
// --------------------------------------------------------------------------------------------------------
size_t tp1_con_cada_pokemon(tp1_t *un_tp, bool (*f)(struct pokemon *, void *),
			    void *extra)
{
	size_t i;
	bubbleSort_pokemones_id_asc_optimizado(un_tp);
	for (i = 0; i < un_tp->cantidad; i++) {
		if (!f(&un_tp->pokemones[i], extra)) {
			break;
		}
	}
	return i;
}
// --------------------------------------------------------------------------------------------------------
void tp1_destruir(tp1_t *tp1)
{
	for (size_t i = 0; i < tp1->cantidad; i++) {
		free(tp1->pokemones[i].nombre);
	}
	free(tp1->pokemones);
	free(tp1);
}