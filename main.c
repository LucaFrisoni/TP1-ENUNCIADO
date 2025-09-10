#include <stdio.h>
#include <string.h>
#include "src/tp1.h"
#include "src/funciones_main.h"

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