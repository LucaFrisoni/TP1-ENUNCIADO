#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "src/tp1.h"

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
	       "  - Caso 'buscar': <nombre_o_id> <valor_a_buscar> 🔍\n"
	       "  - Caso 'mostrar': <nombre_o_id> 🔍\n"
	       "  - Caso 'union', 'diferencia' o 'interseccion': <archivo2.csv> <resultado.csv> 📁\n"
	       "    (el archivo resultado se crea automáticamente, no hace falta que exista)\n\n"
	       "\033[1;35m💡 Tip: Si algo falla, revisá que los parámetros estén correctos y que los archivos existan.\033[0m\n");
}

int validando_formato_csv(const char *archivo)
{
	if (!archivo)
		return 1;

	const char *ext = strrchr(archivo, '.');
	if (!ext || strcmp(ext, ".csv") != 0)
		return 1; // error, no es csv

	return 0;
}

bool validar_operacion(const char *op)
{
	return strcmp(op, "buscar") == 0 || strcmp(op, "mostrar") == 0 ||
	       strcmp(op, "union") == 0 || strcmp(op, "interseccion") == 0 ||
	       strcmp(op, "diferencia") == 0;
}

int validar_buscar(int argc, char *argv[])
{
	if (argc < 5) {
		printf("Falta argumento: debe pasar 'nombre' o 'id' y el valor a buscar\n");
		return 1;
	}
	if (argc > 5) {
		printf("Se pasaron argumentos de más para buscar\n");
		return 1;
	}
	if (strcmp(argv[3], "nombre") != 0 && strcmp(argv[3], "id") != 0) {
		printf("El cuarto argumento para buscar debe ser 'nombre' o 'id'\n");
		return 1;
	}
	return 0;
}

int validar_mostrar(int argc, char *argv[])
{
	if (argc < 4) {
		printf("Falta argumento: debe pasar 'nombre' o 'id'\n");
		return 1;
	}
	if (argc > 4) {
		printf("Se pasaron argumentos de más para mostrar\n");
		return 1;
	}
	if (strcmp(argv[3], "nombre") != 0 && strcmp(argv[3], "id") != 0) {
		printf("El cuarto argumento para mostrar debe ser 'nombre' o 'id'\n");
		return 1;
	}

	return 0;
}

int validar_op_con_archivos(int argc, char *argv[])
{
	if (argc != 5) {
		printf("Debe pasar 'nombre_archivo2.csv' y 'resultado.csv'\n");
		return 1;
	}
	if (validando_formato_csv(argv[3]) == 1) {
		printf("El cuarto argumento debe ser un archivo CSV\n");
		return 1;
	}
	if (validando_formato_csv(argv[4]) == 1) {
		printf("El último argumento debe ser resultado.csv\n");
		return 1;
	}
	return 0;
}

int validando_params(int argc, char *argv[])
{
	if (argc < 2) {
		mensaje_inicial();
		return 1;
	}

	if (validando_formato_csv(argv[1]) == 1) {
		printf("El segundo parámetro debe ser un archivo .csv\n");
		return 1;
	}

	if (argc == 2) {
		printf("Debe pasarse una operación\n");
		return 1;
	}

	const char *operacion = argv[2];
	if (!validar_operacion(operacion)) {
		printf("El tercer parámetro solo puede ser: buscar | mostrar | union | interseccion | diferencia\n");
		return 1;
	}

	if (strcmp(operacion, "buscar") == 0)
		return validar_buscar(argc, argv);
	if (strcmp(operacion, "mostrar") == 0)
		return validar_mostrar(argc, argv);
	if (strcmp(operacion, "union") == 0 ||
	    strcmp(operacion, "interseccion") == 0 ||
	    strcmp(operacion, "diferencia") == 0) {
		return validar_op_con_archivos(argc, argv);
	}

	return 0;
}

bool mostrar_pokemon(struct pokemon *p, void *extra)
{
	if (!p)
		return false;
	printf("Id: %d\n", p->id);
	printf("Nombre: %s\n", p->nombre);
	printf("Tipo: %d\n", p->tipo);
	printf("Ataque: %d\n", p->ataque);
	printf("Defensa: %d\n", p->defensa);
	printf("Velocidad: %d\n\n", p->velocidad);

	(void)extra; // para evitar warnings de parámetro no usado
	return true;
}

void ejecutar_buscar(tp1_t *tp1, const char *tipo_busqueda, const char *valor)
{
	struct pokemon *pk = NULL;

	if (strcmp(tipo_busqueda, "nombre") == 0) {
		pk = tp1_buscar_nombre(tp1, valor);
	} else {
		pk = tp1_buscar_id(tp1, atoi(valor));
	}

	if (pk) {
		mostrar_pokemon(pk, NULL);
	} else {
		printf("No se encontro el pokemon\n");
	}
}

void ejecutar_union(tp1_t *tp1, const char *archivo2, const char *resultado)
{
	tp1_t *tp2 = tp1_leer_archivo(archivo2);
	if (!tp1 || !tp2) {
		printf("Error leyendo archivos\n");
		if (tp2)
			tp1_destruir(tp2);
		return;
	}
	tp1_t *tp_resultado = tp1_union(tp1, tp2);
	tp1_guardar_archivo(tp_resultado, resultado);

	tp1_destruir(tp2);
	tp1_destruir(tp_resultado);
}

void ejecutar_interseccion(tp1_t *tp1, const char *archivo2,
			   const char *resultado)
{
	tp1_t *tp2 = tp1_leer_archivo(archivo2);

	if (!tp1 || !tp2) {
		printf("Error leyendo archivos\n");
		if (tp2)
			tp1_destruir(tp2);
		return;
	}

	tp1_t *tp_resultado = tp1_interseccion(tp1, tp2);
	tp1_guardar_archivo(tp_resultado, resultado);

	tp1_destruir(tp2);
	tp1_destruir(tp_resultado);
}

void ejecutar_diferencia(tp1_t *tp1, const char *archivo2,
			 const char *resultado)
{
	tp1_t *tp2 = tp1_leer_archivo(archivo2);

	if (!tp1 || !tp2) {
		printf("Error leyendo archivos\n");
		if (tp2)
			tp1_destruir(tp2);
		return;
	}

	tp1_t *tp_resultado = tp1_diferencia(tp1, tp2);
	tp1_guardar_archivo(tp_resultado, resultado);

	tp1_destruir(tp2);
	tp1_destruir(tp_resultado);
}

int ejecutando_funciones(int argc, char *argv[])
{
	if (argc > 2) {
		const char *archivo1 = argv[1];
		const char *operacion = argv[2];

		tp1_t *tp1 = tp1_leer_archivo(archivo1);
		if (!tp1) {
			printf("Error leyendo el archivo 1\n");
			return 1;
		}

		if (strcmp(operacion, "buscar") == 0) {
			ejecutar_buscar(tp1, argv[3], argv[4]);
		} else if (strcmp(operacion, "mostrar") == 0) {
			tp1_con_cada_pokemon(tp1, mostrar_pokemon, argv[3]);
		} else if (strcmp(operacion, "union") == 0) {
			ejecutar_union(tp1, argv[3], argv[4]);
		} else if (strcmp(operacion, "interseccion") == 0) {
			ejecutar_interseccion(tp1, argv[3], argv[4]);
		} else if (strcmp(operacion, "diferencia") == 0) {
			ejecutar_diferencia(tp1, argv[3], argv[4]);
		}

		tp1_destruir(tp1);
	}

	return 0;
}

int main(int argc, char *argv[])
{
	int val = validando_params(argc, argv);
	if (val == 1) {
		return 0;
	} else {
		ejecutando_funciones(argc, argv);
	}
	return 0;
}
//