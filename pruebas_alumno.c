#include "pa2m.h"
#include "src/tp1.h"
#include <string.h>
#define ARCHIVO_PRUEBA_INEXISTENTE "ejemplos/asdkasjhfskladjhfksdfhksdf.csv"

bool contar_pokemon(struct pokemon *p, void *extra)
{
	(void)extra; //para el warning
	if (!p) {
		return false;
	}

	return true;
}

void tp1_leer_archivo_tests()
{
	tp1_t *tp1 = NULL;
	size_t n;
	// 1
	tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_INEXISTENTE);
	pa2m_afirmar(tp1 == NULL, "Abrir un archivo inexistente devuelve NULL");

	// 2
	tp1 = tp1_leer_archivo("");
	pa2m_afirmar(
		tp1 == NULL,
		"Abrir un archivo pasandole un nombre vacio devuelve NULL");

	// 3
	tp1 = tp1_leer_archivo("ejemplos/vacio.csv");
	pa2m_afirmar(tp1 == NULL, "Abrir un archivo Vacio devuelve NULL");
	tp1_destruir(tp1);

	// 4
	tp1 = tp1_leer_archivo("ejemplos/formato.csv");
	n = tp1_con_cada_pokemon(tp1, contar_pokemon, NULL);
	pa2m_afirmar(n == 2, "No se parsean las lineas que contienen ';'");
	tp1_destruir(tp1);

	// 5
	tp1 = tp1_leer_archivo("ejemplos/tipo_incorrecto.csv");
	n = tp1_con_cada_pokemon(tp1, contar_pokemon, NULL);
	pa2m_afirmar(n == 2, "No se parsean los pokemones con tipo incorrecto");
	tp1_destruir(tp1);

	// 6
	tp1 = tp1_leer_archivo("ejemplos/duplicados.csv");
	n = tp1_con_cada_pokemon(tp1, contar_pokemon, NULL);
	pa2m_afirmar(n == 5, "No se parsean los pokemones duplicados");
	tp1_destruir(tp1);

	// 7
	tp1 = tp1_leer_archivo("ejemplos/campos_faltantes.csv");
	n = tp1_con_cada_pokemon(tp1, contar_pokemon, NULL);
	pa2m_afirmar(n == 2,
		     "No se parsean los pokemones con campos faltantes");
	tp1_destruir(tp1);

	// 8
	tp1 = tp1_leer_archivo("ejemplos/campos_extras.csv");
	n = tp1_con_cada_pokemon(tp1, contar_pokemon, NULL);
	pa2m_afirmar(n == 1, "No se parsean los pokemones con campos extras");
	tp1_destruir(tp1);

	// 9
	tp1 = tp1_leer_archivo("ejemplos/muchos_pokemones.csv");
	n = tp1_con_cada_pokemon(tp1, contar_pokemon, NULL);
	pa2m_afirmar(
		n == 200,
		"Se parsean correctamente los 200 pokemones del archivo de prueba (test de estrés)");
	tp1_destruir(tp1);

	// 10
	tp1 = tp1_leer_archivo("ejemplos/largo.csv");
	n = tp1_con_cada_pokemon(tp1, contar_pokemon, NULL);
	pa2m_afirmar(
		n == 15,
		"Se parsean correctamente los pokemones con nombres largos");
	tp1_destruir(tp1);
}

void tp1_cantidad_tests()
{
	// 1
	pa2m_afirmar(tp1_cantidad(NULL) == 0,
		     "tp1_cantidad devuelve 0 cuando tp1 es NULL");
	// 2
	tp1_t *tp1 = tp1_leer_archivo("ejemplos/normal.csv");
	size_t n = tp1_cantidad(tp1);
	pa2m_afirmar(n == 5,
		     "tp1_cantidad devuelve la cantidad correcta de pokemones");
	tp1_destruir(tp1);
}

void tp1_guardar_archivo_tests()
{
	tp1_t *tp1 = NULL;
	tp1_t *tp1_ = NULL;
	const char *nombre_archivo = "salida.txt";

	// 1
	tp1 = tp1_guardar_archivo(NULL, "Un nombre");
	pa2m_afirmar(tp1 == NULL,
		     "tp1_guardar_archivo Devuelve NULL cuando tp1 es NULL");

	// 2
	tp1 = tp1_guardar_archivo(tp1, NULL);
	pa2m_afirmar(
		tp1 == NULL,
		"tp1_guardar_archivo Devuelve NULL cuando  se le pasa un nombre NULL");

	// 3
	tp1 = tp1_leer_archivo("ejemplos/normal.csv");
	tp1_ = tp1_guardar_archivo(tp1, nombre_archivo);
	pa2m_afirmar(
		tp1 == tp1_,
		"tp1_guardar_archivo devuelve el mismo puntero al tp1_t cuando se guarda correctamente");

	//4
	FILE *f = fopen(nombre_archivo, "r");
	pa2m_afirmar(f != NULL,
		     "tp1_guardar_archivo crea el archivo correctamente");
	if (f)
		fclose(f);

	remove(nombre_archivo);
	tp1_destruir(tp1);

	//5
	tp1 = tp1_leer_archivo("ejemplos/vacio.csv");
	tp1_ = tp1_guardar_archivo(tp1, nombre_archivo);
	pa2m_afirmar(
		tp1_ == NULL,
		"tp1_guardar_archivo devuelve NULL si se pasa un tp sin pokemones");
	tp1_destruir(tp1);

	//6
	tp1 = tp1_leer_archivo("ejemplos/muchos_pokemones.csv");
	tp1_ = tp1_guardar_archivo(tp1, nombre_archivo);

	tp1_t *tp1_guardado = tp1_leer_archivo(nombre_archivo);
	size_t n = tp1_con_cada_pokemon(tp1_guardado, contar_pokemon, NULL);
	pa2m_afirmar(
		n == 200,
		"tp1_guardar_archivo crea correctamente el archivo con los 200 pokemones (test de estrés)");
	remove(nombre_archivo);

	tp1_destruir(tp1);
	tp1_destruir(tp1_guardado);
}

void probar_casos_union_interseccion_diferencia(tp1_t *(*funcion)(tp1_t *,
								  tp1_t *),
						const char *nombre_funcion,
						int resultado_esperado)
{
	const char *nombre_archivo = "resultado.csv";
	tp1_t *tp1 = NULL;
	tp1_t *tp2 = NULL;
	tp1_t *tp3 = NULL;

	// 1
	tp2 = tp1_leer_archivo("ejemplos/largo.csv");
	tp3 = funcion(NULL, tp2);
	pa2m_afirmar(tp3 == NULL, "%s devuelve NULL cuando tp1 es NULL",
		     nombre_funcion);
	tp1_destruir(tp2);

	// 2
	tp1 = tp1_leer_archivo("ejemplos/normal.csv");
	tp3 = funcion(tp1, NULL);
	pa2m_afirmar(tp3 == NULL, "%s devuelve NULL cuando tp2 es NULL",
		     nombre_funcion);
	tp1_destruir(tp1);
	// 3
	tp1 = tp1_leer_archivo("ejemplos/vacio.csv");
	tp3 = funcion(tp1, tp2);
	pa2m_afirmar(tp3 == NULL,
		     "%s devuelve NULL cuando tp1 no tiene pokemones",
		     nombre_funcion);
	tp1_destruir(tp1);
	// 4
	tp1 = tp1_leer_archivo("ejemplos/normal.csv");
	tp2 = tp1_leer_archivo("ejemplos/vacio.csv");
	tp3 = funcion(tp1, tp2);
	pa2m_afirmar(tp3 == NULL,
		     "%s devuelve NULL cuando tp2 no tiene pokemones",
		     nombre_funcion);
	tp1_destruir(tp1);
	tp1_destruir(tp2);
	tp1_destruir(tp3);
	//5
	tp1 = tp1_leer_archivo("ejemplos/normal.csv");
	tp2 = tp1_leer_archivo("ejemplos/largo.csv");
	tp3 = funcion(tp1, tp2);

	tp1_guardar_archivo(tp3, nombre_archivo);
	tp1_destruir(tp3);

	tp3 = tp1_leer_archivo(nombre_archivo);
	size_t n = tp1_con_cada_pokemon(tp3, contar_pokemon, NULL);
	pa2m_afirmar((int)n == resultado_esperado,
		     "%s crea el archivo y une correctamente los pokemones",
		     nombre_funcion);

	tp1_destruir(tp1);
	tp1_destruir(tp2);
	tp1_destruir(tp3);
	remove(nombre_archivo);
}

void tp1_union_tests()
{
	//1-2-3-4-5
	probar_casos_union_interseccion_diferencia(tp1_union, "tp1_union", 18);
}

void tp1_interseccion_tests()
{
	//1-2-3-4-5
	probar_casos_union_interseccion_diferencia(tp1_interseccion,
						   "tp1_interseccion", 2);
}

void tp1_diferencia_tests()
{
	//1-2-3-4-5
	probar_casos_union_interseccion_diferencia(tp1_diferencia,
						   "tp1_diferencia", 3);
}

void tp1_buscar_nombre_tests()
{
	tp1_t *tp1 = NULL;
	struct pokemon *p = NULL;

	// 1
	p = tp1_buscar_nombre(NULL, "Pikachu");
	pa2m_afirmar(p == NULL,
		     "tp1_buscar_nombre devuelve NULL si tp1 es NULL");

	// 2
	tp1 = tp1_leer_archivo("ejemplos/normal.csv");
	p = tp1_buscar_nombre(tp1, NULL);
	pa2m_afirmar(p == NULL,
		     "tp1_buscar_nombre devuelve NULL si el nombre es NULL");
	tp1_destruir(tp1);

	// 3
	tp1 = tp1_leer_archivo("ejemplos/vacio.csv");
	p = tp1_buscar_nombre(tp1, "Charmander");
	pa2m_afirmar(
		p == NULL,
		"tp1_buscar_nombre devuelve NULL si tp1 no tiene pokemones");
	tp1_destruir(tp1);
	// 4
	tp1 = tp1_leer_archivo("ejemplos/normal.csv");
	p = tp1_buscar_nombre(tp1, "Bulbasaur");
	pa2m_afirmar(p != NULL && strcmp(p->nombre, "Bulbasaur") == 0,
		     "tp1_buscar_nombre encuentra un pokemon existente");

	// 5
	p = tp1_buscar_nombre(tp1, "Mewtwo");
	pa2m_afirmar(p == NULL,
		     "tp1_buscar_nombre devuelve NULL si el pokemon no existe");
	tp1_destruir(tp1);
}

void tp1_buscar_id_tests()
{
	tp1_t *tp1 = NULL;
	struct pokemon *p = NULL;

	// 1
	p = tp1_buscar_id(NULL, 25);
	pa2m_afirmar(p == NULL, "tp1_buscar_id devuelve NULL si tp1 es NULL");

	// 2
	tp1 = tp1_leer_archivo("ejemplos/vacio.csv");
	p = tp1_buscar_id(tp1, 1);
	pa2m_afirmar(p == NULL,
		     "tp1_buscar_id devuelve NULL si tp1 no tiene pokemones");
	tp1_destruir(tp1);
	// 3
	tp1 = tp1_leer_archivo("ejemplos/normal.csv");
	p = tp1_buscar_id(tp1, 2);
	pa2m_afirmar(p != NULL && p->id == 2,
		     "tp1_buscar_id encuentra un pokemon existente");

	// 4
	p = tp1_buscar_id(tp1, 999);
	pa2m_afirmar(p == NULL,
		     "tp1_buscar_id devuelve NULL si el pokemon no existe");

	// 6) id negativo
	p = tp1_buscar_id(tp1, -5);
	pa2m_afirmar(p == NULL,
		     "tp1_buscar_id devuelve NULL si se pasa id negativo");

	tp1_destruir(tp1);
}

bool contar_hasta_tres(struct pokemon *p, void *extra)
{
	(void)p;
	int *contador = (int *)extra;
	(*contador)++;
	return (*contador < 3); // se detiene después de 3
}

void tp1_con_cada_pokemon_tests()
{
	tp1_t *tp1 = NULL;
	size_t n;

	// 1. TP nulo
	n = tp1_con_cada_pokemon(NULL, contar_pokemon, NULL);
	pa2m_afirmar(n == 0, "tp1_con_cada_pokemon retorna 0 si TP es NULL");

	// 2. Función nula
	tp1 = tp1_leer_archivo("ejemplos/normal.csv");
	n = tp1_con_cada_pokemon(tp1, NULL, NULL);
	pa2m_afirmar(n == 0,
		     "tp1_con_cada_pokemon retorna 0 si la función es NULL");

	// 3. Archivo normal, contar todos
	n = tp1_con_cada_pokemon(tp1, contar_pokemon, NULL);
	pa2m_afirmar(n == 5,
		     "tp1_con_cada_pokemon recorre todos los pokemones");

	// 4. Detenerse después de 3
	int contador = 0;
	n = tp1_con_cada_pokemon(tp1, contar_hasta_tres, &contador);
	pa2m_afirmar(n == 3,
		     "tp1_con_cada_pokemon se detiene después de 3 pokemones");

	tp1_destruir(tp1);
	// 5. Test de estrés (archivo con 200 pokemones)
	tp1 = tp1_leer_archivo("ejemplos/muchos_pokemones.csv");
	n = tp1_con_cada_pokemon(tp1, contar_pokemon, NULL);
	pa2m_afirmar(
		n == 200,
		"tp1_con_cada_pokemon recorre todos los 200 pokemones (test de estrés)");

	// Liberar memoria
	tp1_destruir(tp1);
}

int main()
{
	pa2m_nuevo_grupo("Pruebas de funciones tp1_leer_archivo");
	tp1_leer_archivo_tests();

	pa2m_nuevo_grupo("Pruebas de funcion tp1_cantidad");
	tp1_cantidad_tests();

	pa2m_nuevo_grupo("Pruebas de funcion tp1_guardar_archivo");
	tp1_guardar_archivo_tests();

	pa2m_nuevo_grupo("Pruebas de funcion tp1_union");
	tp1_union_tests();

	pa2m_nuevo_grupo("Pruebas de funcion tp1_interseccion");
	tp1_interseccion_tests();

	pa2m_nuevo_grupo("Pruebas de funcion tp1_diferencia");
	tp1_diferencia_tests();

	pa2m_nuevo_grupo("Pruebas de funcion tp1_buscar_nombre");
	tp1_buscar_nombre_tests();

	pa2m_nuevo_grupo("Pruebas de funcion tp1_buscar_id");
	tp1_buscar_id_tests();

	pa2m_nuevo_grupo("Pruebas de funcion tp1_con_cada_pokemon");
	tp1_con_cada_pokemon_tests();

	return pa2m_mostrar_reporte();
}
