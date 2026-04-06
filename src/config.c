#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "config.h"

static void trim(char *str) {
	int inicio = 0;
	int fin = strlen(str) -1;
	int i, j;

	while (str[inicio] != '\0' && isspace((unsigned char) str[inicio])) {
		inicio++;
	}

	while (fin >= inicio && isspace ((unsigned char)str[fin])) {
		fin--;
	}

	for (i=inicio,j=0;i<=fin;i++,j++) {
		str[j] = str[i];
	}
	str[j] = '\0';
}
