#ifndef TP1_ORDENAMIENTO_H
#define TP1_ORDENAMIENTO_H
#include "tp1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Funciones de ordenamiento (Bubble Sort optimizado)

/**
 * @brief Ordena los pokemones alfabéticamente en orden ascendente (A-Z).
 * @param tp1 Puntero a la estructura que contiene los pokemones.
 */
void bubbleSort_pokemones_alfabeticamente_asc_optimizado(tp1_t *tp1);

/**
 * @brief Ordena los pokemones alfabéticamente en orden descendente (Z-A).
 * @param tp1 Puntero a la estructura que contiene los pokemones.
 */
void bubbleSort_pokemones_alfabeticamente_desc_optimizado(tp1_t *tp1);

/**
 * @brief Ordena los pokemones por ID en orden ascendente.
 * @param tp1 Puntero a la estructura que contiene los pokemones.
 */
void bubbleSort_pokemones_id_asc_optimizado(tp1_t *tp1);

/**
 * @brief Ordena los pokemones por ID en orden descendente.
 * @param tp1 Puntero a la estructura que contiene los pokemones.
 */
void bubbleSort_pokemones_id_desc_optimizado(tp1_t *tp1);

// --------------------------------------------------------------------------------------------------------
// Funciones de visualización

/**
 * @brief Muestra por consola la información de un pokemon.
 * @param p Puntero al pokemon a mostrar.
 */
void mostrar_pokemon(struct pokemon *p);

/**
 * @brief Muestra por consola todos los pokemones de tp1.
 * @param tp1 Puntero a la estructura con los pokemones.
 */
void mostrar_pokemones(tp1_t *tp1);

// --------------------------------------------------------------------------------------------------------
// Manejo de memoria

/**
 * @brief Duplica la capacidad de un buffer dinámico usando realloc.
 * @param buffer El buffer actual.
 * @param capacidad Puntero a la capacidad actual (se actualiza al doble).
 * @return El nuevo buffer redimensionado, o termina el programa en caso de error.
 */
char *resize_buffer(char *buffer, size_t *capacidad);

/**
 * @brief Reserva memoria dinámica con malloc y valida errores.
 * @param size Cantidad de bytes a reservar.
 * @param mensaje_error Mensaje a imprimir si malloc falla.
 * @return Puntero a la memoria reservada, o termina el programa en caso de error.
 */
void *creando_maloc(size_t size, const char *mensaje_error);

// --------------------------------------------------------------------------------------------------------
// Manejo de archivos

/**
 * @brief Abre un archivo en modo lectura.
 * @param nombre_archivo Nombre del archivo.
 * @return Puntero al archivo o NULL si no se pudo abrir.
 */
FILE *archivo_open(const char *nombre_archivo);

/**
 * @brief Crea un archivo en modo escritura.
 * @param nombre_archivo Nombre del archivo.
 * @return Puntero al archivo o termina el programa en caso de error.
 */
FILE *archivo_crear(const char *nombre_archivo);

/**
 * @brief Lee una línea completa desde un archivo, expandiendo el buffer dinámicamente si es necesario.
 * @param archivo Archivo del cual leer.
 * @param capacidad Capacidad inicial del buffer (se puede modificar si se expande).
 * @return String leído sin salto de línea, o NULL en caso de EOF.
 */
char *leer_linea(FILE *archivo, size_t *capacidad);

/**
 * @brief Escribe todos los pokemones de tp1 en un archivo en formato CSV.
 * @param tp1 Puntero a la estructura con pokemones.
 * @param archivo Archivo de salida.
 */
void escribiendo_linea(tp1_t *tp1, FILE *archivo);

// --------------------------------------------------------------------------------------------------------
// Parsing y validaciones

/**
 * @brief Valida si una línea es vacía o inválida.
 * @param linea String con la línea.
 * @return true si es inválida, false si es válida.
 */
bool parse_pokemon_validations(char *linea);

/**
 * @brief Asigna un valor de un campo leído a un pokemon.
 * @param p Puntero al pokemon a modificar.
 * @param campo Número del campo (0=id, 1=nombre, 2=tipo, 3=ataque, 4=defensa, 5=velocidad).
 * @param buffer String con el valor a asignar.
 */
void switch_pokemon(struct pokemon *p, int campo, const char *buffer);

/**
 * @brief Convierte una línea CSV en un struct pokemon.
 * @param linea Línea en formato "id,nombre,tipo,ataque,defensa,velocidad".
 * @return Puntero a pokemon parseado, o NULL en caso de error.
 */
struct pokemon *parsear_pokemon(char *linea);

/**
 * @brief Agrega un pokemon al array dinámico de tp1.
 * @param tp1 Puntero a la estructura principal.
 * @param pk Puntero al pokemon a agregar.
 */
void agregar_pokemon(tp1_t *tp1, struct pokemon *pk);

// --------------------------------------------------------------------------------------------------------
// Utilidades

/**
 * @brief Convierte un enum tipo_pokemon en string.
 * @param tipo Enumeración del tipo.
 * @return String con el nombre del tipo, o "DESCONOCIDO" si no es válido.
 */
const char *tipo_a_string(enum tipo_pokemon tipo);

/**
 * @brief Valida que un archivo tenga extensión .csv.
 * 
 * Esta función comprueba que el puntero al nombre de archivo no sea NULL
 * y que el archivo tenga la extensión ".csv". Devuelve 0 si todo es correcto
 * y 1 en caso de error.
 * 
 * @param archivo Puntero al nombre del archivo a validar.
 * @return int 0 si el archivo es válido, 1 si es NULL o no tiene extensión .csv.
 */
int validando_formato_csv(const char *archivo);

#endif // TP1_ORDENAMIENTO_H