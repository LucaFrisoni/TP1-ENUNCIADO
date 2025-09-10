#include "funciones_main.h"
#include "tp1.h"
#include "mis_funciones.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct tp1 {
	struct pokemon *pokemones;
	size_t cantidad;
};

void mensaje_inicial()
{
	printf("\033[1;34m🎉 Bienvenido a TP1! De Algoritmo 2 - Méndez 🎉\033[0m\n"
	       "\033[1;33mEste proyecto fue realizado por LucaFrisoni - Padrón 113497 - gran programador 😎\033[0m\n"
	       "O como diría el código, 'si funciona, es magia; si no, culpa del compilador 🪄'\n\n"
	       "\033[1;32mEn este proyecto vas a poder realizar varias funcionalidades, pero hay que respetar unas reglas básicas:\033[0m\n"
	       "Formato de ejecución:\n"
	       "  \033[1;36m<programa_compilado> <archivo.csv> <operacion>\033[0m\n"
	       "  (📄 <archivo.csv> es el archivo de donde se leen los datos)\n\n"
	       "\033[1;32mTipos de operaciones:\033[0m\n"
	       "  buscar | mostrar | union | diferencia | interseccion\n\n"
	       "Dependiendo de la operación elegida, deberás pasar más parámetros:\n"
	       "  - Caso 'buscar' o 'mostrar': <nombre_o_id> <valor_a_buscar> 🔍\n"
	       "  - Caso 'union', 'diferencia' o 'interseccion': <archivo2.csv> <resultado.csv> 📁\n"
	       "    (el archivo resultado se crea automáticamente, no hace falta que exista)\n\n"
	       "\033[1;35m💡 Tip: Si algo falla, revisá que los parámetros estén correctos y que los archivos existan.\033[0m\n");
}

int validando_params(int argc, char *argv[])
{
	if (argc < 2) {
		mensaje_inicial();
		return 1;
	} else if (argc == 2) {
		printf("Debe pasarse una operacion\n");
		if (validando_formato_csv(argv[1]) == 1) {
			printf("El segundo parámetro debe ser un archivo .csv\n");
		}
		return 1;
	} else if (argc > 2) {
		const char *operacion = argv[2];

		if (validando_formato_csv(argv[1]) == 1) {
			printf("El segundo parámetro debe ser un archivo .csv\n");
			return 1;
		}

		// Verifico que el segundo argumento sea válido
		if (strcmp(operacion, "buscar") != 0 &&
		    strcmp(operacion, "mostrar") != 0 &&
		    strcmp(operacion, "union") != 0 &&
		    strcmp(operacion, "interseccion") != 0 &&
		    strcmp(operacion, "diferencia") != 0) {
			printf("El tercer parametro solo puede ser: buscar | mostrar | union | interseccion | diferencia\n");
			return 1;
		}
		// Validaciones buscar
		if (strcmp(operacion, "buscar") == 0) {
			if (argc < 5) {
				printf("Falta argumento: debe pasar 'nombre' o 'id' y el valor a buscar\n");
				return 1;
			}
			if (argc >= 6) {
				printf("Se pasaron argumentos de más para buscar\n");
				return 1;
			}
			const char *tipo_busqueda = argv[3];
			if (strcmp(tipo_busqueda, "nombre") != 0 &&
			    strcmp(tipo_busqueda, "id") != 0) {
				printf("El cuarto argumento para buscar debe ser 'nombre' o 'id'\n");
				return 1;
			}
		}

		// Validaciones mostrar
		if (strcmp(operacion, "mostrar") == 0) {
			if (argc < 5) {
				printf("Falta argumento: debe pasar 'nombre' o 'id' y el valor a buscar\n");
				return 1;
			}
			const char *tipo_mostrar = argv[3];
			if (strcmp(tipo_mostrar, "nombre") != 0 &&
			    strcmp(tipo_mostrar, "id") != 0) {
				printf("El cuarto argumento para mostrar debe ser 'nombre' o 'id'\n");
				return 1;
			}
			const char *asc_o_desc = argv[4];
			if (strcmp(asc_o_desc, "asc") != 0 &&
			    strcmp(asc_o_desc, "desc") != 0) {
				printf("El ultimo argumento debe indicar si quiere mostrar asc o desc'\n");
				return 1;
			}

			if (argc > 6) {
				printf("Se pasaron argumentos de más para mostrar\n");
				return 1;
			}
		}

		// Validaciones union/interseccion/diferencia
		if (strcmp(operacion, "union") == 0 ||
		    strcmp(operacion, "interseccion") == 0 ||
		    strcmp(operacion, "diferencia") == 0) {
			if (argc < 5) {
				printf("Faltan argumentos: debe pasar 'nombre_archivo2.csv' y 'resultado.csv' como ultimo parametro\n");
				return 1;
			}
			if (argc >= 6) {
				printf("Se pasaron argumentos de más para unir los pokemones de los archivos sin repetir\n");
				return 1;
			}

			if (validando_formato_csv(argv[3]) == 1) {
				printf("El cuarto argumento debe ser un archivo CSV\n");
				return 1;
			}
			if (validando_formato_csv(argv[4]) == 1) {
				printf("El ultimo argumento debe ser resultado.csv'\n");
				return 1;
			}
		}
	}
	return 0;
}

int ejecutando_funciones(int argc, char *argv[])
{
	if (argc > 2) {
		const char *archivo1 = argv[1];
		const char *operacion = argv[2];

		struct pokemon *pk;

		tp1_t *tp1 = tp1_leer_archivo(archivo1);
		if (!tp1) {
			printf("Error leyendo el archivo 1\n");
			return 1;
		}

		if (strcmp(operacion, "buscar") == 0) {
			const char *tipo_busqueda = argv[3];
			const char *valor = argv[4];

			if (strcmp(tipo_busqueda, "nombre") == 0) {
				pk = tp1_buscar_nombre(tp1, valor);
				if (pk) {
					mostrar_pokemon(pk);
				} else {
					printf("No se encontro el pokemon\n");
				}

			} else {
				pk = tp1_buscar_id(tp1, atoi(valor));
				if (pk) {
					mostrar_pokemon(pk);
				} else {
					printf("No se encontro el pokemon\n");
				}
			}

		} else if (strcmp(operacion, "mostrar") == 0) {
			const char *tipo_mostrar = argv[3];
			const char *asc_o_desc = argv[4];

			if (strcmp(tipo_mostrar, "nombre") == 0) {
				if (strcmp(asc_o_desc, "asc") == 0) {
					bubbleSort_pokemones_alfabeticamente_asc_optimizado(
						tp1);
					mostrar_pokemones(tp1);
				} else {
					bubbleSort_pokemones_alfabeticamente_desc_optimizado(
						tp1);
					mostrar_pokemones(tp1);
				}

			} else {
				if (strcmp(asc_o_desc, "asc") == 0) {
					bubbleSort_pokemones_id_asc_optimizado(
						tp1);
					mostrar_pokemones(tp1);
				} else {
					bubbleSort_pokemones_id_desc_optimizado(
						tp1);
					mostrar_pokemones(tp1);
				}
			}

		} else if (strcmp(operacion, "union") == 0) {
			const char *archivo2 = argv[3];
			const char *resultado = argv[4];
			tp1_t *tp2 = tp1_leer_archivo(archivo2);

			if (!tp2) {
				printf("Error leyendo el archivo 2\n");
				return 1;
			}

			tp1_t *tp_resultado = tp1_union(tp1, *tp2);
			tp1_guardar_archivo(tp_resultado, resultado);
			free(tp2);
			free(tp_resultado);
		} else if (strcmp(operacion, "interseccion") == 0) {
			const char *archivo2 = argv[3];
			const char *resultado = argv[4];
			tp1_t *tp2 = tp1_leer_archivo(archivo2);
			if (!tp2) {
				printf("Error leyendo el archivo 2\n");
				return 1;
			}
			tp1_t *tp_resultado = tp1_interseccion(tp1, *tp2);
			tp1_guardar_archivo(tp_resultado, resultado);
			free(tp2);
			free(tp_resultado);
		} else if (strcmp(operacion, "diferencia") == 0) {
			const char *archivo2 = argv[3];
			const char *resultado = argv[4];
			tp1_t *tp2 = tp1_leer_archivo(archivo2);
			tp1_t *tp_resultado = tp1_diferencia(tp1, *tp2);
			if (!tp2) {
				printf("Error leyendo el archivo 2\n");
				return 1;
			}
			tp1_guardar_archivo(tp_resultado, resultado);
			free(tp2);
			free(tp_resultado);
		}

		for (size_t i = 0; i < tp1->cantidad; i++) {
			free(tp1->pokemones[i].nombre);
		}
		free(tp1->pokemones);
		free(tp1);
	}

	return 0;
};
