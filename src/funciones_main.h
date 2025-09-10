#ifndef FUNCIONES_MAIN_H_
#define FUNCIONES_MAIN_H_

#include <stdio.h>
/**
 * @brief Imprime un mensaje dando reglas para usar la aplicacion de consola.
 */
void mensaje_inicial();

/**
 * @brief Valida los parámetros pasados por consola (argc y argv).
 *
 * Revisa que la cantidad y el contenido de los argumentos sean correctos
 * según la operación solicitada:
 * - buscar: requiere "nombre|id" y un valor
 * - mostrar: requiere "nombre|id" y "asc|desc"
 * - union/interseccion/diferencia: requieren archivo2.csv y resultado.csv
 *
 * @param argc Cantidad de parámetros pasados al main.
 * @param argv Vector con los parámetros de ejecución.
 * @return devuelve 0 en caso de exito 1 si no pasa una validacion
 */
int validando_params(int argc, char *argv[]);

/**
 * @brief Ejecuta la operación solicitada según los parámetros de entrada.
 *
 * Carga el archivo CSV inicial y aplica la acción indicada:
 * - buscar: busca un pokemon por nombre o id.
 * - mostrar: ordena por nombre o id, ascendente o descendente, y muestra.
 * - union/interseccion/diferencia: combina dos archivos de pokemones y
 *   guarda el resultado en resultado.csv.
 *
 * @param argc Cantidad de parámetros pasados al main.
 * @param argv Vector con los parámetros de ejecución.
 * @return devuelve 0 en caso de exito 1 si hay un error
 */
int ejecutando_funciones(int argc, char *argv[]);

#endif