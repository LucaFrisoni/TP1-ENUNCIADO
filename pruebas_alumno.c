#include "pa2m.h"
#include "src/tp1.h"
#include "src/funciones_main.h"
#include "src/mis_funciones.h"

#define ARCHIVO_PRUEBA_INEXISTENTE "ejemplos/asdkasjhfskladjhfksdfhksdf.csv"

struct tp1 {
	struct pokemon *pokemones;
	size_t cantidad;
};

void tp1_leer_archivo_devuelve_null_cuando_el_archivo_no_existe()
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_INEXISTENTE);
	pa2m_afirmar(tp1 == NULL, "Abrir un archivo inexistente devuelve NULL");
}

void resize_buffer_tests()
{
	// Caso 1: capacidad 0 → devuelve NULL
	{
		size_t capacidad = 0;
		char *buffer = malloc(10);
		char *nuevo = resize_buffer(buffer, &capacidad);
		pa2m_afirmar(nuevo == NULL, "Si capacidad es 0 devuelve NULL");
		free(buffer);
	}

	// Caso 2: duplica capacidad
	{
		size_t capacidad = 4;
		char *buffer = malloc(capacidad);
		char *nuevo = resize_buffer(buffer, &capacidad);
		pa2m_afirmar(capacidad == 8,
			     "La capacidad se duplica correctamente");
		free(nuevo);
	}

	// Caso 3: conserva datos
	{
		size_t capacidad = 15;
		char *buffer = malloc(capacidad);
		strcpy(buffer, "Soy una prueba");
		char *nuevo = resize_buffer(buffer, &capacidad);
		pa2m_afirmar(strcmp(nuevo, "Soy una prueba") == 0,
			     "El buffer conserva todos los datos tras resize");
		free(nuevo);
	}
}

void creando_malloc_tests()
{
	// Caso 1: malloc devuelve memoria válida
	{
		void *ptr = creando_maloc(10);
		pa2m_afirmar(ptr != NULL, "Malloc devuelve memoria válida");
		free(ptr);
	}

	// Caso 2: memoria asignada es usable
	{
		int *arr = creando_maloc(4 * sizeof(int));
		arr[0] = 42;
		arr[1] = 99;
		pa2m_afirmar(arr[0] == 42 && arr[1] == 99,
			     "Memoria asignada es usable");
		free(arr);
	}

	// Caso 3: size 0 devuelve NULL
	{
		void *ptr = creando_maloc(0);
		pa2m_afirmar(ptr == NULL, "Si size es 0, devuelve NULL");
	}
}

void archivo_open_tests()
{
	// Caso 1: archivo inexistente devuelve NULL
	{
		FILE *f = archivo_open("archivo_que_no_existe.txt");
		pa2m_afirmar(f == NULL, "Archivo inexistente devuelve NULL");
	}

	// Caso 2: nombre NULL devuelve NULL
	{
		FILE *f = archivo_open(NULL);
		pa2m_afirmar(f == NULL, "Nombre de archivo NULL devuelve NULL");
	}

	// Caso 3: archivo existente devuelve FILE* válido
	{
		const char *nombre = "archivo_test.txt";

		// Creo un archivo temporal
		FILE *tmp = fopen(nombre, "w");
		fclose(tmp);

		FILE *f = archivo_open(nombre);
		pa2m_afirmar(f != NULL,
			     "Archivo existente devuelve FILE* válido");
		fclose(f);

		// Borro el archivo temporal
		remove(nombre);
	}
}

void archivo_crear_tests()
{
	// Caso 1: archivo válido
	{
		const char *nombre = "archivo_test_crear.txt";
		FILE *f = archivo_crear(nombre);
		pa2m_afirmar(f != NULL, "Archivo creado devuelve FILE* válido");
		fclose(f);
		remove(nombre);
	}

	// Caso 2: archivo NULL
	{
		FILE *f = archivo_crear(NULL);
		pa2m_afirmar(f == NULL,
			     "Archivo creado con NULL devuelve NULL");
	}
}

void leer_linea_tests()
{
	// Caso 1: archivo NULL
	{
		size_t capacidad = 10;
		char *linea = leer_linea(NULL, &capacidad);
		pa2m_afirmar(linea == NULL,
			     "Si archivo es NULL, devuelve NULL");
	}

	// Caso 2: capacidad NULL
	{
		FILE *tmp = fopen("archivo_temp.txt", "w");
		fclose(tmp);

		FILE *archivo = fopen("archivo_temp.txt", "r");
		char *linea = leer_linea(archivo, NULL);
		pa2m_afirmar(linea == NULL,
			     "Si capacidad es NULL, devuelve NULL");

		fclose(archivo);
		remove("archivo_temp.txt");
	}

	// Caso 3: capacidad 0
	{
		FILE *tmp = fopen("archivo_temp.txt", "w");
		fclose(tmp);

		FILE *archivo = fopen("archivo_temp.txt", "r");
		size_t capacidad = 0;
		char *linea = leer_linea(archivo, &capacidad);
		pa2m_afirmar(linea == NULL, "Si capacidad <= 0, devuelve NULL");

		fclose(archivo);
		remove("archivo_temp.txt");
	}

	// Caso 4: devuelve línea correcta
	{
		const char *nombre = "archivo_temp.txt";
		FILE *tmp = fopen(nombre, "w");
		fprintf(tmp, "Primera linea\nSegunda linea\n");
		fclose(tmp);

		FILE *archivo = fopen(nombre, "r");
		size_t capacidad = 10;
		char *linea = leer_linea(archivo, &capacidad);

		pa2m_afirmar(strcmp(linea, "Primera linea") == 0,
			     "Devuelve la primera linea correctamente");

		free(linea);
		fclose(archivo);
		remove(nombre);
	}

	// Caso 5: resize funciona
	{
		const char *nombre = "archivo_temp.txt";
		FILE *tmp = fopen(nombre, "w");
		fprintf(tmp, "Linea muy larga que supera el buffer\n");
		fclose(tmp);

		FILE *archivo = fopen(nombre, "r");
		size_t capacidad = 10; // fuerza resize
		char *linea = leer_linea(archivo, &capacidad);

		pa2m_afirmar(
			strcmp(linea, "Linea muy larga que supera el buffer") ==
				0,
			"Devuelve la linea completa correctamente usando resize");

		free(linea);
		fclose(archivo);
		remove(nombre);
	}
}

void escribiendo_linea_escribe_correctamente()
{
	struct pokemon p1 = { 1, "Pikachu", 0, 55, 40, 90 };
	struct pokemon p2 = { 2, "Charmander", 1, 52, 43, 65 };

	tp1_t tp1;
	tp1.cantidad = 2;
	tp1.pokemones = malloc(2 * sizeof(struct pokemon));
	tp1.pokemones[0] = p1;
	tp1.pokemones[1] = p2;

	const char *nombre = "archivo_temp.txt";
	FILE *archivo = fopen(nombre, "w");

	escribiendo_linea(&tp1, archivo);
	fclose(archivo);

	archivo = fopen(nombre, "r");
	char buffer[256];
	if (fgets(buffer, sizeof(buffer), archivo) != NULL) {
		buffer[strcspn(buffer, "\n")] = '\0'; // eliminar \n
		pa2m_afirmar(strcmp(buffer, "1,Pikachu,ELEC,55,40,90") == 0,
			     "Primera linea escrita correctamente");
	}

	if (fgets(buffer, sizeof(buffer), archivo) != NULL) {
		buffer[strcspn(buffer, "\n")] = '\0';
		pa2m_afirmar(strcmp(buffer, "2,Charmander,FUEG,52,43,65") == 0,
			     "Segunda linea escrita correctamente");
	}

	fclose(archivo);
	remove(nombre);
	free(tp1.pokemones);
}

void parse_pokemon_validations_tests()
{
	// Caso 1: línea NULL
	pa2m_afirmar(parse_pokemon_validations(NULL) == true,
		     "Devuelve true si linea es NULL");

	// Caso 2: línea vacía
	pa2m_afirmar(parse_pokemon_validations("") == true,
		     "Devuelve true si linea está vacía");

	// Caso 3: línea con contenido
	pa2m_afirmar(parse_pokemon_validations("Pikachu,0,55,40,90") == false,
		     "Devuelve false si linea tiene contenido");
}

void switch_pokemon_tests()
{
	struct pokemon p = { 0 };

	// Campo 0: id
	bool ok = switch_pokemon(&p, 0, "25");
	pa2m_afirmar(ok && p.id == 25, "Campo 0 asigna correctamente el id");

	// Campo 1: nombre
	ok = switch_pokemon(&p, 1, "Pikachu");
	pa2m_afirmar(ok && strcmp(p.nombre, "Pikachu") == 0,
		     "Campo 1 asigna correctamente el nombre");
	free(p.nombre);
	p.nombre = NULL;

	// Campo 2: tipo válido
	ok = switch_pokemon(&p, 2, "ELEC");
	pa2m_afirmar(ok && p.tipo == TIPO_ELEC,
		     "Campo 2 asigna el tipo correctamente");

	// Campo 2: tipo inválido no cambia
	p.tipo = TIPO_NORM;
	ok = switch_pokemon(&p, 2, "INVALIDO");
	pa2m_afirmar(!ok && p.tipo == TIPO_NORM,
		     "Campo 2 con tipo inválido no modifica el tipo");

	// Campos 3, 4, 5: ataque, defensa, velocidad
	ok = switch_pokemon(&p, 3, "55");
	pa2m_afirmar(ok && p.ataque == 55,
		     "Campo 3 asigna ataque correctamente");

	ok = switch_pokemon(&p, 4, "40");
	pa2m_afirmar(ok && p.defensa == 40,
		     "Campo 4 asigna defensa correctamente");

	ok = switch_pokemon(&p, 5, "90");
	pa2m_afirmar(ok && p.velocidad == 90,
		     "Campo 5 asigna velocidad correctamente");
}

void parsear_pokemon_tests()
{
	// Caso 1: línea con ';' → devuelve NULL
	{
		char linea[] = "1,Pikachu,ELEC,55,40,90;extra";
		struct pokemon *p = parsear_pokemon(linea);
		pa2m_afirmar(p == NULL,
			     "Si la linea contiene ';', devuelve NULL");
		if (p) {
			free(p->nombre);
			free(p);
		}
	}

	// Caso 2: menos de 6 campos → devuelve NULL
	{
		char linea[] = "1,Pikachu,ELEC,55,40"; // solo 5 campos
		struct pokemon *p = parsear_pokemon(linea);
		pa2m_afirmar(p == NULL,
			     "Si hay menos de 6 campos, devuelve NULL");
		if (p) {
			free(p->nombre);
			free(p);
		}
	}

	// Caso 3: más de 6 campos → devuelve NULL
	{
		char linea[] = "1,Pikachu,ELEC,55,40,90,extra"; // 7 campos
		struct pokemon *p = parsear_pokemon(linea);
		pa2m_afirmar(p == NULL,
			     "Si hay más de 6 campos, devuelve NULL");
		if (p) {
			free(p->nombre);
			free(p);
		}
	}

	// Caso 4: línea válida → devuelve struct pokemon
	{
		char linea[] = "1,Pikachu,ELEC,55,40,90";
		struct pokemon *p = parsear_pokemon(linea);
		pa2m_afirmar(p != NULL, "Línea válida devuelve struct pokemon");

		if (p) {
			free(p->nombre);
			free(p);
		}
	}
}

void agregar_pokemon_funciona()
{
	tp1_t tp1;
	tp1.cantidad = 0;
	tp1.pokemones = NULL;

	// Creo el pokemon dinamicamente
	struct pokemon *p = malloc(sizeof(struct pokemon));
	p->id = 1;
	p->tipo = TIPO_ELEC;
	p->ataque = 55;
	p->defensa = 40;
	p->velocidad = 90;
	p->nombre = malloc(strlen("Pikachu") + 1);
	strcpy(p->nombre, "Pikachu");

	agregar_pokemon(&tp1, p);

	pa2m_afirmar(tp1.cantidad == 1 && tp1.pokemones[0].id == 1 &&
			     strcmp(tp1.pokemones[0].nombre, "Pikachu") == 0 &&
			     tp1.pokemones[0].tipo == TIPO_ELEC &&
			     tp1.pokemones[0].ataque == 55 &&
			     tp1.pokemones[0].defensa == 40 &&
			     tp1.pokemones[0].velocidad == 90,
		     "Pokemon agregado correctamente con todos sus campos");

	free(tp1.pokemones[0].nombre);
	free(tp1.pokemones);
}

void bubbleSort_tests()
{
	struct pokemon pokes[3] = { { 3, "Charmander", TIPO_FUEG, 52, 43, 65 },
				    { 1, "Bulbasaur", TIPO_PLAN, 49, 49, 45 },
				    { 2, "Squirtle", TIPO_AGUA, 48, 65, 43 } };

	tp1_t tp1;
	tp1.pokemones = pokes;
	tp1.cantidad = 3;

	// Orden alfabético ascendente
	bubbleSort_pokemones_alfabeticamente_asc_optimizado(&tp1);
	pa2m_afirmar(strcmp(tp1.pokemones[0].nombre, "Bulbasaur") == 0 &&
			     strcmp(tp1.pokemones[1].nombre, "Charmander") ==
				     0 &&
			     strcmp(tp1.pokemones[2].nombre, "Squirtle") == 0,
		     "Ordena alfabéticamente ascendente correctamente");

	// Orden alfabético descendente
	bubbleSort_pokemones_alfabeticamente_desc_optimizado(&tp1);
	pa2m_afirmar(strcmp(tp1.pokemones[0].nombre, "Squirtle") == 0 &&
			     strcmp(tp1.pokemones[1].nombre, "Charmander") ==
				     0 &&
			     strcmp(tp1.pokemones[2].nombre, "Bulbasaur") == 0,
		     "Ordena alfabéticamente descendente correctamente");

	// Orden por id ascendente
	bubbleSort_pokemones_id_asc_optimizado(&tp1);
	pa2m_afirmar(tp1.pokemones[0].id == 1 && tp1.pokemones[1].id == 2 &&
			     tp1.pokemones[2].id == 3,
		     "Ordena por id ascendente correctamente");

	// Orden por id descendente
	bubbleSort_pokemones_id_desc_optimizado(&tp1);
	pa2m_afirmar(tp1.pokemones[0].id == 3 && tp1.pokemones[1].id == 2 &&
			     tp1.pokemones[2].id == 1,
		     "Ordena por id descendente correctamente");
}

void tp1_cantidad_tests()
{
	pa2m_afirmar(tp1_cantidad(NULL) == 0,
		     "tp1_cantidad devuelve 0 cuando tp1 es NULL");

	tp1_t tp1_vacio = { .pokemones = NULL, .cantidad = 0 };
	pa2m_afirmar(tp1_cantidad(&tp1_vacio) == 0,
		     "tp1_cantidad devuelve 0 cuando no hay pokémones");

	struct pokemon pokes[3] = { { 1, "Bulbasaur", TIPO_PLAN, 49, 49, 45 },
				    { 2, "Charmander", TIPO_FUEG, 52, 43, 65 },
				    { 3, "Squirtle", TIPO_AGUA, 48, 65, 43 } };
	tp1_t tp1_lleno = { .pokemones = pokes, .cantidad = 3 };
	pa2m_afirmar(tp1_cantidad(&tp1_lleno) == 3,
		     "tp1_cantidad devuelve la cantidad correcta de pokémones");
}

void tp1_guardar_archivo_tests()
{
	struct pokemon pokes[3] = { { 3, "Charmander", TIPO_FUEG, 52, 43, 65 },
				    { 1, "Bulbasaur", TIPO_PLAN, 49, 49, 45 },
				    { 2, "Squirtle", TIPO_AGUA, 48, 65, 43 } };

	tp1_t tp1;
	tp1.pokemones = pokes;
	tp1.cantidad = 3;

	tp1_t *test = tp1_guardar_archivo(NULL, "Un nombre");
	pa2m_afirmar(test == NULL,
		     "tp1_guardar_archivo Devuelve NULL cuando no tp1 Es NULL");

	tp1_t *test2 = tp1_guardar_archivo(&tp1, NULL);
	pa2m_afirmar(
		test2 == NULL,
		"tp1_guardar_archivo Devuelve NULL cuando no se le pasa un nombre NULL");

	tp1_t *test3 = tp1_guardar_archivo(&tp1, "");
	pa2m_afirmar(
		test3 == NULL,
		"tp1_guardar_archivo Devuelve NULL cuando no se le pasa un nombre");

	tp1_t *test4 = tp1_guardar_archivo(&tp1, "");
	pa2m_afirmar(
		test4 == NULL,
		"tp1_guardar_archivo Devuelve NULL cuando el nombre es vacío");

	const char *nombre_archivo = "salida.txt";
	tp1_t *test5 = tp1_guardar_archivo(&tp1, nombre_archivo);
	pa2m_afirmar(
		test5 == &tp1,
		"tp1_guardar_archivo devuelve el mismo puntero cuando se guarda correctamente");

	FILE *f = fopen(nombre_archivo, "r");
	pa2m_afirmar(f != NULL, "Se crea el archivo de salida correctamente");
	if (f) {
		fclose(f);
		remove(nombre_archivo);
	}
}

void tp1_union_tests()
{
	// Creo pokemonesde preuba
	struct pokemon pokes1[3] = { { 1, "Bulbasaur", TIPO_PLAN, 49, 49, 45 },
				     { 2, "Ivysaur", TIPO_PLAN, 60, 62, 60 },
				     { 3, "Venusaur", TIPO_PLAN, 82, 83, 80 } };

	struct pokemon pokes2[3] = { { 3, "Venusaur", TIPO_PLAN, 82, 83, 80 },
				     { 4, "Charmander", TIPO_FUEG, 52, 43, 65 },
				     { 5, "Charmeleon", TIPO_FUEG, 64, 58,
				       80 } };

	tp1_t tp1 = { pokes1, 3 };
	tp1_t tp2 = { pokes2, 3 };

	// Caso NULL
	tp1_t *res_null = tp1_union(NULL, &tp2);
	pa2m_afirmar(res_null == NULL,
		     "tp1_union devuelve NULL si un_tp es NULL");

	// Caso otro_tp vacío
	tp1_t tp_vacio = { NULL, 0 };
	tp1_t *res_vacio = tp1_union(&tp1, &tp_vacio);
	pa2m_afirmar(res_vacio == NULL,
		     "tp1_union devuelve NULL si otro_tp está vacío");

	// Caso exitoso
	tp1_t *res = tp1_union(&tp1, &tp2);
	pa2m_afirmar(res != NULL,
		     "tp1_union devuelve puntero no NULL en caso exitoso");
	pa2m_afirmar(
		res->cantidad == 5,
		"tp1_union tiene la cantidad correcta de pokemones (sin duplicados)");

	// Verificamos que estén los IDs correctos
	int ids_esperados[] = { 1, 2, 3, 4, 5 };
	bool ok = true;
	for (size_t i = 0; i < 5; i++) {
		if (res->pokemones[i].id != ids_esperados[i]) {
			ok = false;
			break;
		}
	}
	pa2m_afirmar(
		ok,
		"tp1_union contiene todos los pokemones esperados (IDs correctos)");

	// Libero memoria
	tp1_destruir(res);
}

void tp1_interseccion_tests()
{
	struct pokemon pokes1[3] = { { 1, "Bulbasaur", TIPO_PLAN, 49, 49, 45 },
				     { 2, "Ivysaur", TIPO_PLAN, 60, 62, 60 },
				     { 3, "Venusaur", TIPO_PLAN, 82, 83, 80 } };

	struct pokemon pokes2[3] = { { 3, "Venusaur", TIPO_PLAN, 82, 83, 80 },
				     { 4, "Charmander", TIPO_FUEG, 52, 43, 65 },
				     { 5, "Charmeleon", TIPO_FUEG, 64, 58,
				       80 } };

	tp1_t tp1 = { pokes1, 3 };
	tp1_t tp2 = { pokes2, 3 };

	// Caso NULL
	tp1_t *res_null = tp1_interseccion(NULL, &tp2);
	pa2m_afirmar(res_null == NULL,
		     "tp1_interseccion devuelve NULL si un_tp es NULL");

	// Caso otro_tp vacío
	tp1_t tp_vacio = { NULL, 0 };
	tp1_t *res_vacio = tp1_interseccion(&tp1, &tp_vacio);
	pa2m_afirmar(res_vacio == NULL,
		     "tp1_interseccion devuelve NULL si otro_tp está vacío");

	// Caso exitoso
	tp1_t *res = tp1_interseccion(&tp1, &tp2);
	pa2m_afirmar(
		res != NULL,
		"tp1_interseccion devuelve puntero no NULL en caso exitoso");
	pa2m_afirmar(
		res->cantidad == 1,
		"tp1_interseccion tiene la cantidad correcta de pokemones");

	// Verificamos que esté el Pokémon correcto
	pa2m_afirmar(res->pokemones[0].id == 3,
		     "tp1_interseccion contiene el pokemon correcto");

	// Libero memoria
	tp1_destruir(res);
}

void tp1_diferencia_tests()
{
	struct pokemon pokes1[3] = { { 1, "Bulbasaur", TIPO_PLAN, 49, 49, 45 },
				     { 2, "Ivysaur", TIPO_PLAN, 60, 62, 60 },
				     { 3, "Venusaur", TIPO_PLAN, 82, 83, 80 } };

	struct pokemon pokes2[2] = { { 3, "Venusaur", TIPO_PLAN, 82, 83, 80 },
				     { 4, "Charmander", TIPO_FUEG, 52, 43,
				       65 } };

	tp1_t tp1 = { pokes1, 3 };
	tp1_t tp2 = { pokes2, 2 };

	// Caso NULL
	tp1_t *res_null = tp1_diferencia(NULL, &tp2);
	pa2m_afirmar(res_null == NULL,
		     "tp1_diferencia devuelve NULL si un_tp es NULL");

	// Caso otro_tp vacío
	tp1_t tp_vacio = { NULL, 0 };
	tp1_t *res_vacio = tp1_diferencia(&tp1, &tp_vacio);
	pa2m_afirmar(res_vacio == NULL,
		     "tp1_diferencia devuelve NULL si otro_tp está vacío");

	// Caso exitoso
	tp1_t *res = tp1_diferencia(&tp1, &tp2);
	pa2m_afirmar(res != NULL,
		     "tp1_diferencia devuelve puntero no NULL en caso exitoso");
	pa2m_afirmar(res->cantidad == 2,
		     "tp1_diferencia tiene la cantidad correcta de pokemones");

	//Verificacion Ids
	int ids_esperados[] = { 1, 2 };
	bool ok = true;
	for (size_t i = 0; i < 2; i++) {
		if (res->pokemones[i].id != ids_esperados[i]) {
			ok = false;
			break;
		}
	}
	pa2m_afirmar(ok,
		     "tp1_diferencia contiene los pokemones correctos (IDs)");

	tp1_destruir(res);
}

void tp1_buscar_nombre_tests()
{
	struct pokemon pokes[3] = { { 3, "Charmander", TIPO_FUEG, 52, 43, 65 },
				    { 1, "Bulbasaur", TIPO_PLAN, 49, 49, 45 },
				    { 2, "Squirtle", TIPO_AGUA, 48, 65, 43 } };

	tp1_t tp1 = { pokes, 3 };

	// Buscar un nombre existente
	struct pokemon *res = tp1_buscar_nombre(&tp1, "Bulbasaur");
	pa2m_afirmar(res != NULL && res->id == 1,
		     "tp1_buscar_nombre encuentra Bulbasaur correctamente");

	// Buscar nombre que no existe
	res = tp1_buscar_nombre(&tp1, "Pikachu");
	pa2m_afirmar(res == NULL,
		     "tp1_buscar_nombre devuelve NULL si el Pokémon no existe");

	// Buscar con tp NULL
	res = tp1_buscar_nombre(NULL, "Bulbasaur");
	pa2m_afirmar(res == NULL,
		     "tp1_buscar_nombre devuelve NULL si tp es NULL");

	// Buscar con nombre NULL
	res = tp1_buscar_nombre(&tp1, NULL);
	pa2m_afirmar(res == NULL,
		     "tp1_buscar_nombre devuelve NULL si nombre es NULL");
}

void tp1_buscar_id_tests()
{
	struct pokemon pokes[3] = { { 3, "Charmander", TIPO_FUEG, 52, 43, 65 },
				    { 1, "Bulbasaur", TIPO_PLAN, 49, 49, 45 },
				    { 2, "Squirtle", TIPO_AGUA, 48, 65, 43 } };

	tp1_t tp1 = { pokes, 3 };

	// Buscar un ID existente
	struct pokemon *res = tp1_buscar_id(&tp1, 1);
	pa2m_afirmar(res != NULL && res->id == 1,
		     "tp1_buscar_id encuentra Bulbasaur correctamente");

	// Buscar ID que no existe
	res = tp1_buscar_id(&tp1, 99);
	pa2m_afirmar(res == NULL,
		     "tp1_buscar_id devuelve NULL si el ID no existe");

	// Buscar id negativo
	res = tp1_buscar_id(&tp1, -1);
	pa2m_afirmar(res == NULL,
		     "tp1_buscar_id devuelve NULL si el id es negativo");
	// Buscar con tp NULL
	res = tp1_buscar_id(NULL, 1);
	pa2m_afirmar(res == NULL, "tp1_buscar_id devuelve NULL si tp es NULL");

	// Buscar en TP vacío
	tp1_t tp_vacio = { NULL, 0 };
	res = tp1_buscar_id(&tp_vacio, 1);
	pa2m_afirmar(res == NULL,
		     "tp1_buscar_id devuelve NULL si tp está vacío");
}

void test_validando_formato_csv()
{
	pa2m_nuevo_grupo("Pruebas validando_formato_csv");

	pa2m_afirmar(validando_formato_csv(NULL) == 1,
		     "Archivo NULL devuelve error");
	pa2m_afirmar(validando_formato_csv("datos.txt") == 1,
		     "Archivo .txt devuelve error");
	pa2m_afirmar(validando_formato_csv("datos") == 1,
		     "Archivo sin extension devuelve error");
	pa2m_afirmar(validando_formato_csv("archivo.csv") == 0,
		     "Archivo .csv devuelve correcto");
	pa2m_afirmar(validando_formato_csv("ruta/a/archivo.csv") == 0,
		     "Archivo con path y .csv devuelve correcto");
}

int main()
{
	pa2m_nuevo_grupo("Pruebas de funciones tp1_leer_archivo");
	tp1_leer_archivo_devuelve_null_cuando_el_archivo_no_existe();

	resize_buffer_tests();

	creando_malloc_tests();

	archivo_open_tests();

	archivo_crear_tests();

	leer_linea_tests();

	escribiendo_linea_escribe_correctamente();

	parse_pokemon_validations_tests();

	switch_pokemon_tests();

	parsear_pokemon_tests();

	agregar_pokemon_funciona();

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

	pa2m_nuevo_grupo("Pruebas de sorts");
	bubbleSort_tests();

	pa2m_nuevo_grupo("Pruebas de validando params main");
	test_validando_formato_csv();
	return pa2m_mostrar_reporte();
}
