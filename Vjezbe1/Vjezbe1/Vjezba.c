#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define max_str (50)
#define max_ln (1024)

typedef struct Student {
	char ime[max_str];
	char prezime[max_str];
	double bodovi;
} Stud;

int main() {

	int brojRedaka = 0;

	FILE* fp = NULL; //fp je filepointer (pokazivac na file)
	char buffer[max_ln] = { 0 };
	fp = fopen("studenti.txt", "r"); //r citanje, w pisanje (ako vec nesto postoji brise sve i pise ispocetka), a nastavlja pisat na postojeci text

	if (fp == NULL) {
		printf("file nije otvoren");
		return -1; //koristimo negativne brojeve ako kod nije uspjesan
	}

	while (feof(fp) == NULL) {
		fgets(buffer, max_ln, fp);
		brojRedaka++;
	}

	printf("%d", brojRedaka);

	fclose(fp); //uvik kad otvorimo neki file, treba ga odma zatvorit sa fopen
	return 0;
}