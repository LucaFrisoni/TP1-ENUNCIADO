#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "mis_funciones.h"
struct tp1 {
	struct pokemon *pokemones;
	size_t cantidad;
};
// --------------------------------------------------------------------------------------------------------
void bubbleSort_pokemones_alfabeticamente_asc_optimizado(tp1_t *tp1)
{
	if (!tp1 || tp1->cantidad == 0) {
		return;
	}

	size_t i, j;
	bool hubo_intercambio = true;
	i = 1;

	while (i < tp1->cantidad && hubo_intercambio) {
		hubo_intercambio = false;
		for (j = 0; j < tp1->cantidad - i; j++) {
			if (strcmp(tp1->pokemones[j].nombre,
				   tp1->pokemones[j + 1].nombre) > 0) {
				hubo_intercambio = true;
				struct pokemon pokemon_aux = tp1->pokemones[j];
				tp1->pokemones[j] = tp1->pokemones[j + 1];
				tp1->pokemones[j + 1] = pokemon_aux;
			}
		}
		i++;
	}
}

void bubbleSort_pokemones_alfabeticamente_desc_optimizado(tp1_t *tp1)
{
	if (!tp1 || tp1->cantidad == 0) {
		return;
	}

	size_t i, j;
	bool hubo_intercambio = true;
	i = 1;

	while (i < tp1->cantidad && hubo_intercambio) {
		hubo_intercambio = false;
		for (j = 0; j < tp1->cantidad - i; j++) {
			// Comparamos al revés para descendente
			if (strcmp(tp1->pokemones[j].nombre,
				   tp1->pokemones[j + 1].nombre) < 0) {
				hubo_intercambio = true;
				struct pokemon aux = tp1->pokemones[j];
				tp1->pokemones[j] = tp1->pokemones[j + 1];
				tp1->pokemones[j + 1] = aux;
			}
		}
		i++;
	}
}

void bubbleSort_pokemones_id_asc_optimizado(tp1_t *tp1)
{
	if (!tp1 || tp1->cantidad == 0) {
		return;
	}

	size_t i, j;
	bool hubo_intercambio = true;
	i = 1;

	while (i < tp1->cantidad && hubo_intercambio) {
		hubo_intercambio = false;
		for (j = 0; j < tp1->cantidad - i; j++) {
			if (tp1->pokemones[j].id > tp1->pokemones[j + 1].id) {
				hubo_intercambio = true;
				struct pokemon pokemon_aux = tp1->pokemones[j];
				tp1->pokemones[j] = tp1->pokemones[j + 1];
				tp1->pokemones[j + 1] = pokemon_aux;
			}
		}
		i++;
	}
}

void bubbleSort_pokemones_id_desc_optimizado(tp1_t *tp1)
{
	if (!tp1 || tp1->cantidad == 0) {
		return;
	}

	size_t i, j;
	bool hubo_intercambio = true;
	i = 1;

	while (i < tp1->cantidad && hubo_intercambio) {
		hubo_intercambio = false;
		for (j = 0; j < tp1->cantidad - i; j++) {
			if (tp1->pokemones[j].id < tp1->pokemones[j + 1].id) {
				hubo_intercambio = true;
				struct pokemon pokemon_aux = tp1->pokemones[j];
				tp1->pokemones[j] = tp1->pokemones[j + 1];
				tp1->pokemones[j + 1] = pokemon_aux;
			}
		}
		i++;
	}
}
// --------------------------------------------------------------------------------------------------------
void mostrar_pokemon(struct pokemon *p)
{
	if (!p)
		return;
	printf("Id: %d\n", p->id);
	printf("Nombre: %s\n", p->nombre);
	printf("Tipo: %d\n", p->tipo); // Podés mapear a string si querés
	printf("Ataque: %d\n", p->ataque);
	printf("Defensa: %d\n", p->defensa);
	printf("Velocidad: %d\n\n", p->velocidad);
}

void mostrar_pokemones(tp1_t *tp1)
{
	size_t i;
	if (!tp1) {
		return;
	}

	for (i = 0; i < tp1->cantidad; i++) {
		mostrar_pokemon(&tp1->pokemones[i]);
	}
}
// --------------------------------------------------------------------------------------------------------
char *resize_buffer(char *buffer, size_t *capacidad)
{
	if (!capacidad || *capacidad <= 0) {
		return NULL;
	}

	*capacidad *= 2;
	char *tmp = realloc(buffer, *capacidad);
	if (!tmp) {
		free(buffer);
		return NULL;
	}
	return tmp;
}

void *creando_maloc(size_t size)
{
	if (size == 0) {
		return NULL;
	}

	void *ptr = malloc(size);
	if (!ptr) {
		return NULL;
	}
	return ptr;
}
// --------------------------------------------------------------------------------------------------------
FILE *archivo_open(const char *nombre_archivo)
{
	if (!nombre_archivo) {
		return NULL;
	}
	FILE *archivo = fopen(nombre_archivo, "r");
	if (!archivo) {
		return NULL;
	}

	return archivo;
}

FILE *archivo_crear(const char *nombre_archivo)
{
	if (!nombre_archivo) {
		return NULL;
	}

	FILE *archivo = fopen(nombre_archivo, "w");
	if (!archivo) {
		return NULL;
	}

	return archivo;
}
// --------------------------------------------------------------------------------------------------------
char *leer_linea(FILE *archivo, size_t *capacidad)
{
	size_t longitud = 0;
	bool leyendo = true;
	char *buffer;

	if (!archivo || !capacidad || *capacidad <= 0) {
		return NULL;
	}

	buffer = creando_maloc(*capacidad);

	while (leyendo) {
		if (fgets(&buffer[longitud], (int)(*capacidad - longitud),
			  archivo) == NULL) {
			if (longitud == 0) { //Final del archivo
				free(buffer);
				return NULL;
			} else {
				// Retornar lo que se haya leído antes de EOF
				buffer[longitud] = '\0';
				return buffer;
			}
		}
		//buffer es solo lo que se leyo
		longitud += strlen(buffer + longitud);

		// Línea completa leída
		if (buffer[longitud - 1] == '\n') {
			buffer[longitud - 1] = '\0';
			return buffer;
		}

		// Buffer lleno pero no hay '\n', duplicamos el tamaño
		buffer = resize_buffer(buffer, capacidad);
	}
	return NULL;
}

void escribiendo_linea(tp1_t *tp1, FILE *archivo)
{
	if (!tp1 || !archivo) {
		return;
	}

	for (size_t i = 0; i < tp1->cantidad; i++) {
		struct pokemon *p = &tp1->pokemones[i];
		fprintf(archivo, "%d,%s,%s,%d,%d,%d\n", p->id, p->nombre,
			tipo_a_string(p->tipo), p->ataque, p->defensa,
			p->velocidad);
	}
}
// --------------------------------------------------------------------------------------------------------
bool parse_pokemon_validations(char *linea)
{
	if (!linea || linea[0] == '\0') {
		return true;
	} else {
		return false;
	}
}

bool buscando_duplicados(tp1_t *tp1, struct pokemon *pk)
{
	for (size_t i = 0; i < tp1->cantidad; i++) {
		if (tp1->pokemones[i].id == pk->id) {
			// Es duplicado
			return true;
		}
	}
	return false;
}

bool switch_pokemon(struct pokemon *p, int campo, const char *buffer)
{
	switch (campo) {
	case 0: // ID
		p->id = atoi(buffer);
		return true;

	case 1: // Nombre
		p->nombre = malloc(strlen(buffer) + 1);
		if (!p->nombre) {
			printf("Error al asignar memoria para nombre\n");
			return false;
		}
		strcpy(p->nombre, buffer);
		return true;

	case 2: // Tipo
		if (strcmp(buffer, "ELEC") == 0)
			p->tipo = TIPO_ELEC;
		else if (strcmp(buffer, "FUEG") == 0)
			p->tipo = TIPO_FUEG;
		else if (strcmp(buffer, "PLAN") == 0)
			p->tipo = TIPO_PLAN;
		else if (strcmp(buffer, "AGUA") == 0)
			p->tipo = TIPO_AGUA;
		else if (strcmp(buffer, "NORM") == 0)
			p->tipo = TIPO_NORM;
		else if (strcmp(buffer, "FANT") == 0)
			p->tipo = TIPO_FANT;
		else if (strcmp(buffer, "PSI") == 0)
			p->tipo = TIPO_PSI;
		else if (strcmp(buffer, "LUCH") == 0)
			p->tipo = TIPO_LUCH;
		else
			return false; // tipo inválido
		return true;

	case 3: // Ataque
		p->ataque = atoi(buffer);
		return true;

	case 4: // Defensa
		p->defensa = atoi(buffer);
		return true;

	case 5: // Velocidad
		p->velocidad = atoi(buffer);
		return true;

	default:
		return false; // campo inválido
	}
}

struct pokemon *parsear_pokemon(char *linea)
{
	char c;
	size_t capacidad = 128;
	size_t buf_idx = 0;
	int campo = 0;
	int i = 0;
	char *buffer;
	struct pokemon *p;

	bool leyendo = true;

	if (parse_pokemon_validations(linea)) {
		return NULL;
	}

	buffer = creando_maloc(capacidad);

	p = creando_maloc(sizeof(struct pokemon));
	p->nombre = NULL;

	while (leyendo) {
		c = linea[i];

		if (c == ';') {
			if (p->nombre) {
				free(p->nombre);
			}

			free(buffer);
			free(p);
			return NULL;
		}

		if (c == ',' || c == '\0') {
			buffer[buf_idx] = '\0'; // cierro string

			if (!switch_pokemon(p, campo, buffer)) {
				if (p->nombre) {
					free(p->nombre);
				}
				free(p);
				free(buffer);
				return NULL; // pokemon inválido
			}

			campo++;
			i++;
			buf_idx = 0; // reinicio
		} else {
			// meter char en buffer dinámico
			buffer[buf_idx++] = c;
			i++;

			// si se llena el buffer duplicamos
			if (buf_idx >= capacidad) {
				buffer = resize_buffer(buffer, &capacidad);
			}
		}
		// fin pokemon
		if (c == '\0') {
			leyendo = false;
		}
	}

	free(buffer);

	// faltan campos o hay campos de más
	if (campo != 6) {
		if (p->nombre) {
			free(p->nombre);
		}
		free(p);
		return NULL;
	}

	return p;
}

void agregar_pokemon(tp1_t *tp1, struct pokemon *pk)
{
	if (buscando_duplicados(tp1, pk)) {
		free(pk->nombre);
		free(pk);
		return; //si es duplicado
	}

	struct pokemon *tmp = realloc(
		tp1->pokemones, sizeof(struct pokemon) * (tp1->cantidad + 1));
	if (!tmp) {
		return;
	}
	tp1->pokemones = tmp;

	// Copiamos el pokemon al array
	tp1->pokemones = tmp;
	tp1->pokemones[tp1->cantidad] = *pk;
	tp1->cantidad++;

	free(pk);
}

// Caso: pokemon que ya está en otro tp1
void agregar_pokemon_existente(tp1_t *tp1, struct pokemon *pk)
{
	if (buscando_duplicados(tp1, pk)) {
		return; //si es duplicado
	}

	struct pokemon *tmp = realloc(
		tp1->pokemones, sizeof(struct pokemon) * (tp1->cantidad + 1));
	if (!tmp) {
		return;
	}

	tp1->pokemones = tmp;

	tp1->pokemones[tp1->cantidad].id = pk->id;
	tp1->pokemones[tp1->cantidad].nombre = malloc(strlen(pk->nombre) + 1);
	strcpy(tp1->pokemones[tp1->cantidad].nombre, pk->nombre);
	tp1->pokemones[tp1->cantidad].tipo = pk->tipo;
	tp1->pokemones[tp1->cantidad].ataque = pk->ataque;
	tp1->pokemones[tp1->cantidad].defensa = pk->defensa;
	tp1->pokemones[tp1->cantidad].velocidad = pk->velocidad;

	tp1->cantidad++;
}
// --------------------------------------------------------------------------------------------------------
const char *tipo_a_string(enum tipo_pokemon tipo)
{
	switch (tipo) {
	case TIPO_ELEC:
		return "ELEC";
	case TIPO_FUEG:
		return "FUEG";
	case TIPO_PLAN:
		return "PLAN";
	case TIPO_AGUA:
		return "AGUA";
	case TIPO_NORM:
		return "NORM";
	case TIPO_FANT:
		return "FANT";
	case TIPO_PSI:
		return "PSI";
	case TIPO_LUCH:
		return "LUCH";
	default:
		return "DESCONOCIDO";
	}
}

int validando_formato_csv(const char *archivo)
{
	if (!archivo) {
		return 1;
	}

	const char *ext = strrchr(archivo, '.');
	if (!ext || strcmp(ext, ".csv") != 0) {
		return 1; // error, no es csv
	}

	return 0;
}