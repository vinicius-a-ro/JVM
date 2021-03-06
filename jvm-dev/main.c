/*
 * main.c
 *
 *  Created on: 22/12/2010
 *      Author: joaofreitas
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reader/classLoader.h"
#include "classViewer/classViewer.h"
#include "structures/mnemonics.h"
#include "engine/executeEngine.h"
char file_name[200];
int main(int argc, char **argv) {
	classFileFormat *classFile;
	int output_type; /*Saida igual a 0 -> não mostra, saida igual a 1 -> tela*/

	output_type = 0;
	if (argc > 1) {
		strcpy(file_name, argv[1]);
		printf("JVM - Executando arquivo: %s\n", argv[1]);
		if (argc > 2) {
			printf("Arquivo gerado com sucesso apos passar o par�metro %s\n", argv[2]);
			if (strcmp(argv[2], "v") == 0) {
				output_type = 1;
			}
		}
	}
	else {
		printf("\nDigite o nome do classFile a ser executado:\n");
		fflush(stdout);
		scanf("%s", file_name);
	}

	classFile = loadClassFile(file_name);
	if (output_type) {
		inspectClassFile(classFile);
	} else {
		exec(classFile, file_name);
	}
	system("pause");

	return EXIT_SUCCESS;
}
