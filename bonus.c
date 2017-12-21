#include "bmp_header.h"
#include <stdio.h>
#include <string.h>

#define MAXCHAR 30		// lungimea maxima a numelor fisierelor
#define MAX 150			// lungimea maxima a liniilor/coloanelor din matrice

struct BGR			// canalele pentru fiecare pixel
{
	int B;
	int G;
	int R;
};

//citirea header-ului si infoheader-ului din fisierul BMP
void readBMP(FILE * captcha, struct bmp_fileheader *captcha_fileheader,
	     struct bmp_infoheader *captcha_infoheader)
{
	fread(&captcha_fileheader->fileMarker1, sizeof(unsigned char), 1,
	      captcha);
	fread(&captcha_fileheader->fileMarker2, sizeof(unsigned char), 1,
	      captcha);
	fread(&captcha_fileheader->bfSize, sizeof(unsigned int), 1, captcha);
	fread(&captcha_fileheader->unused1, sizeof(unsigned short), 1, captcha);
	fread(&captcha_fileheader->unused2, sizeof(unsigned short), 1, captcha);
	fread(&captcha_fileheader->imageDataOffset, sizeof(unsigned int), 1,
	      captcha);

	fread(&captcha_infoheader->biSize, sizeof(unsigned int), 1, captcha);
	fread(&captcha_infoheader->width, sizeof(signed int), 1, captcha);
	fread(&captcha_infoheader->height, sizeof(signed int), 1, captcha);
	fread(&captcha_infoheader->planes, sizeof(unsigned short), 1, captcha);
	fread(&captcha_infoheader->bitPix, sizeof(unsigned short), 1, captcha);
	fread(&captcha_infoheader->biCompression, sizeof(unsigned int), 1,
	      captcha);
	fread(&captcha_infoheader->biSizeImage, sizeof(unsigned int), 1,
	      captcha);
	fread(&captcha_infoheader->biXPelsPerMeter, sizeof(int), 1, captcha);
	fread(&captcha_infoheader->biYPelsPerMeter, sizeof(int), 1, captcha);
	fread(&captcha_infoheader->biClrUsed, sizeof(unsigned int), 1, captcha);
	fread(&captcha_infoheader->biClrImportant, sizeof(unsigned int), 1,
	      captcha);
	return;
}

// scrierea header-ului si infoheader-ului intr-un fisier BMP
void writeBMP(FILE * output, struct bmp_fileheader *captcha_fileheader,
	      struct bmp_infoheader *captcha_infoheader)
{
	fwrite(&captcha_fileheader->fileMarker1, sizeof(unsigned char), 1,
	       output);
	fwrite(&captcha_fileheader->fileMarker2, sizeof(unsigned char), 1,
	       output);
	fwrite(&captcha_fileheader->bfSize, sizeof(unsigned int), 1, output);
	fwrite(&captcha_fileheader->unused1, sizeof(unsigned short), 1, output);
	fwrite(&captcha_fileheader->unused2, sizeof(unsigned short), 1, output);
	fwrite(&captcha_fileheader->imageDataOffset, sizeof(unsigned int), 1,
	       output);

	fwrite(&captcha_infoheader->biSize, sizeof(unsigned int), 1, output);
	fwrite(&captcha_infoheader->width, sizeof(signed int), 1, output);
	fwrite(&captcha_infoheader->height, sizeof(signed int), 1, output);
	fwrite(&captcha_infoheader->planes, sizeof(unsigned short), 1, output);
	fwrite(&captcha_infoheader->bitPix, sizeof(unsigned short), 1, output);
	fwrite(&captcha_infoheader->biCompression, sizeof(unsigned int), 1,
	       output);
	fwrite(&captcha_infoheader->biSizeImage, sizeof(unsigned int), 1,
	       output);
	fwrite(&captcha_infoheader->biXPelsPerMeter, sizeof(int), 1, output);
	fwrite(&captcha_infoheader->biYPelsPerMeter, sizeof(int), 1, output);
	fwrite(&captcha_infoheader->biClrUsed, sizeof(unsigned int), 1, output);
	fwrite(&captcha_infoheader->biClrImportant, sizeof(unsigned int), 1,
	       output);
	return;
}

int main(void)
{
	FILE *input, *captcha, *output;
	char captcha_name[MAXCHAR], output_name[MAXCHAR];
	struct bmp_fileheader captcha_fileheader;
	struct bmp_infoheader captcha_infoheader;
	char matrice[MAX][MAX];	// matricea de pixeli
	int n, i, j, k, l, line, col, padding = 0, aux = 0, dif, found;	// dif - diferenta de pixeli fata de original, found - numar gasit; line - nr de linii, col - nr de coloane; n - nr caractere din stringul nr[]
	struct BGR color;	// canalele ce vor fi citite din fisierul BMP captcha
	char collector[MAXCHAR];	// pentru a 'colecta' fiecare linie din fisierul input
	int vec_nr[MAX], nr_vec;	// vec_nr - vector cu numerele de afisat in ordine inversa, nr_vec - nr de elemente ale vectorului vec_nr

	//sir de caractere pentru toate tipurile de cifre
	char nr0[30] = "XXXXXX...XX...XX...XXXXXX";
	char nr1[30] = "X....X....X....X....X....";
	char nr2[30] = "XXXXXX....XXXXX....XXXXXX";
	char nr3[30] = "XXXXXX....XXXXXX....XXXXX";
	char nr4[30] = "X...XX...XXXXXXX....X....";
	char nr5[30] = "XXXXX....XXXXXXX....XXXXX";
	char nr6[30] = "XXXXX....XXXXXXX...XXXXXX";
	char nr7[30] = "XXXXXX....X....X....X....";
	char nr8[30] = "XXXXXX...XXXXXXX...XXXXXX";
	char nr9[30] = "XXXXXX...XXXXXXX....XXXXX";
	char nr[30];		// sir de caractere pentru a verifica cifra

	input = fopen("input.txt", "rt");

	fgets(captcha_name, MAXCHAR, input);	//captcha_name va avea "\n" inainte de "\0"
	fgets(collector, MAXCHAR, input);	// se citeste al doilea rand din input, cel cu culorile BGR, dar nu il vom folosi
	fgets(collector, MAXCHAR, input);	// se citeste array-ul cu numere de eliminat intr-un sir de caractere
	fgets(captcha_name, MAXCHAR, input);	//captcha_name va avea "\n" inainte de "\0"
	if ((strlen(captcha_name) > 0)
	    && (captcha_name[strlen(captcha_name) - 1] == '\n')) {
		captcha_name[strlen(captcha_name) - 1] = '\0';
	}			// s-a eliminat "\n" din captcha_name

	strncpy(output_name, captcha_name, strlen(captcha_name) - 4);	// ".bmp" are 4 caractere
	output_name[strlen(captcha_name) - 4] = '\0';	// sa ne asiguram ca se pune "\0"

	strcat(output_name, "_bonus.txt");
	output = fopen(output_name, "wt");

	// citim fisierul BMP
	captcha = fopen(captcha_name, "rb");
	readBMP(captcha, &captcha_fileheader, &captcha_infoheader);

	line = captcha_infoheader.height;
	col = captcha_infoheader.width;

	// o linie trebuie sa aiba multiplu de 4 bytes, iar acesta se completeaza cu padding-ul
	if (((captcha_infoheader.width * 3) % 4) != 0) {
		padding = 4 - ((captcha_infoheader.width * 3) % 4);
	}
	// construim matricea cu datele din fisierul BMP captcha
	for (i = line - 1; i >= 0; i--) {
		for (j = 0; j < col; j++) {
			fread(&color.B, 1, 1, captcha);
			fread(&color.G, 1, 1, captcha);
			fread(&color.R, 1, 1, captcha);

			if (color.B == 255 && color.G == 255 && color.R == 255) {
				matrice[i][j] = '.';
			} else {
				matrice[i][j] = 'X';
			}
		}
		fseek(captcha, padding, SEEK_CUR);	// se ignora padding-ul de dupa fiecare linie
	}

	// afisam matricea de pixeli
	for (i = 0; i < line; i++) {
		for (j = 0; j < col; j++) {
			printf("%c", matrice[i][j]);
		}
		printf("\n");
	}

	nr_vec = 0;

	for (j = col - 1; j >= 0; j--) {
		for (i = 0; i < line; i++) {
			// construim un sir de caractere pentru determinarea cifrei
			n = 0;
			for (k = i; k < i + 5; k++) {
				for (l = j; l > j - 5; l--) {
					nr[n] = matrice[k][l];
					n = n + 1;
				}
			}
			nr[n] = '\0';

			found = 0;	// initial nu am gasit cifra

			// cautam cifra in cazul in care sirul de caractere e o copie exacta a cifrei
			if (found == 0) {
				if (strcmp(nr, nr0) == 0) {
					vec_nr[nr_vec] = 0;
					nr_vec = nr_vec + 1;
					for (k = i; k < i + 5; k++) {
						for (l = j; l > j - 5; l--) {
							matrice[k][l] = '.';	// eliminam din matrice cifra
						}
					}
					found = 1;
				}
			}

			if (found == 0) {
				if (strcmp(nr, nr1) == 0) {
					vec_nr[nr_vec] = 1;
					nr_vec = nr_vec + 1;
					for (k = i; k < i + 5; k++) {
						for (l = j; l > j - 5; l--) {
							matrice[k][l] = '.';	// eliminam din matrice cifra
						}
					}
					found = 1;
				}
			}

			if (found == 0) {
				if (strcmp(nr, nr2) == 0) {
					vec_nr[nr_vec] = 2;
					nr_vec = nr_vec + 1;
					for (k = i; k < i + 5; k++) {
						for (l = j; l > j - 5; l--) {
							matrice[k][l] = '.';	// eliminam din matrice cifra
						}
					}
					found = 1;
				}
			}

			if (found == 0) {
				if (strcmp(nr, nr3) == 0) {
					vec_nr[nr_vec] = 3;
					nr_vec = nr_vec + 1;
					for (k = i; k < i + 5; k++) {
						for (l = j; l > j - 5; l--) {
							matrice[k][l] = '.';	// eliminam din matrice cifra
						}
					}
					found = 1;
				}
			}

			if (found == 0) {
				if (strcmp(nr, nr4) == 0) {
					vec_nr[nr_vec] = 4;
					nr_vec = nr_vec + 1;
					for (k = i; k < i + 5; k++) {
						for (l = j; l > j - 5; l--) {
							matrice[k][l] = '.';	// eliminam din matrice cifra
						}
					}
					found = 1;
				}
			}

			if (found == 0) {
				if (strcmp(nr, nr5) == 0) {
					vec_nr[nr_vec] = 5;
					nr_vec = nr_vec + 1;
					for (k = i; k < i + 5; k++) {
						for (l = j; l > j - 5; l--) {
							matrice[k][l] = '.';	// eliminam din matrice cifra
						}
					}
					found = 1;
				}
			}

			if (found == 0) {
				if (strcmp(nr, nr6) == 0) {
					vec_nr[nr_vec] = 6;
					nr_vec = nr_vec + 1;
					for (k = i; k < i + 5; k++) {
						for (l = j; l > j - 5; l--) {
							matrice[k][l] = '.';	// eliminam din matrice cifra
						}
					}
					found = 1;
				}
			}

			if (found == 0) {
				if (strcmp(nr, nr7) == 0) {
					vec_nr[nr_vec] = 7;
					nr_vec = nr_vec + 1;
					for (k = i; k < i + 5; k++) {
						for (l = j; l > j - 5; l--) {
							matrice[k][l] = '.';	// eliminam din matrice cifra
						}
					}
					found = 1;
				}
			}

			if (found == 0) {
				if (strcmp(nr, nr8) == 0) {
					vec_nr[nr_vec] = 8;
					nr_vec = nr_vec + 1;
					for (k = i; k < i + 5; k++) {
						for (l = j; l > j - 5; l--) {
							matrice[k][l] = '.';	// eliminam din matrice cifra
						}
					}
					found = 1;
				}
			}

			if (found == 0) {
				if (strcmp(nr, nr9) == 0) {
					vec_nr[nr_vec] = 9;
					nr_vec = nr_vec + 1;
					for (k = i; k < i + 5; k++) {
						for (l = j; l > j - 5; l--) {
							matrice[k][l] = '.';	// eliminam din matrice cifra
						}
					}
					found = 1;
				}
			}
			// cautam cifra in cazul in care sirul de caractere nu e o copie exacta a cifrei
			// se calculeaza o diferenta dif: daca aceasta e 1 atunci respecta cerinta
			if (found == 0) {
				dif = 0;
				for (k = 0; k < n; k++) {
					if (nr[k] != nr0[k]) {
						dif = dif + 1;
					}
				}

				if (dif == 1) {
					vec_nr[nr_vec] = 0;
					nr_vec = nr_vec + 1;
					for (k = i; k < i + 5; k++) {
						for (l = j; l > j - 5; l--) {
							matrice[k][l] = '.';	// eliminam din matrice cifra
						}
					}
					found = 1;
				}
			}

			if (found == 0) {
				dif = 0;
				for (k = 0; k < n; k++) {
					if (nr[k] != nr1[k]) {
						dif = dif + 1;
					}
				}

				if (dif == 1) {
					vec_nr[nr_vec] = 1;
					nr_vec = nr_vec + 1;
					for (k = i; k < i + 5; k++) {
						for (l = j; l > j - 5; l--) {
							matrice[k][l] = '.';	// eliminam din matrice cifra
						}
					}
					found = 1;
				}
			}

			if (found == 0) {
				dif = 0;
				for (k = 0; k < n; k++) {
					if (nr[k] != nr2[k]) {
						dif = dif + 1;
					}
				}

				if (dif == 1) {
					vec_nr[nr_vec] = 2;
					nr_vec = nr_vec + 1;
					for (k = i; k < i + 5; k++) {
						for (l = j; l > j - 5; l--) {
							matrice[k][l] = '.';	// eliminam din matrice cifra
						}
					}
					found = 1;
				}
			}

			if (found == 0) {
				dif = 0;
				for (k = 0; k < n; k++) {
					if (nr[k] != nr3[k]) {
						dif = dif + 1;
					}
				}

				if (dif == 1) {
					vec_nr[nr_vec] = 3;
					nr_vec = nr_vec + 1;
					for (k = i; k < i + 5; k++) {
						for (l = j; l > j - 5; l--) {
							matrice[k][l] = '.';	// eliminam din matrice cifra
						}
					}
					found = 1;
				}
			}

			if (found == 0) {
				dif = 0;
				for (k = 0; k < n; k++) {
					if (nr[k] != nr4[k]) {
						dif = dif + 1;
					}
				}

				if (dif == 1) {
					vec_nr[nr_vec] = 4;
					nr_vec = nr_vec + 1;
					for (k = i; k < i + 5; k++) {
						for (l = j; l > j - 5; l--) {
							matrice[k][l] = '.';	// eliminam din matrice cifra
						}
					}
					found = 1;
				}
			}

			if (found == 0) {
				dif = 0;
				for (k = 0; k < n; k++) {
					if (nr[k] != nr5[k]) {
						dif = dif + 1;
					}
				}

				if (dif == 1) {
					vec_nr[nr_vec] = 5;
					nr_vec = nr_vec + 1;
					for (k = i; k < i + 5; k++) {
						for (l = j; l > j - 5; l--) {
							matrice[k][l] = '.';	// eliminam din matrice cifra
						}
					}
					found = 1;
				}
			}

			if (found == 0) {
				dif = 0;
				for (k = 0; k < n; k++) {
					if (nr[k] != nr6[k]) {
						dif = dif + 1;
					}
				}

				if (dif == 1) {
					vec_nr[nr_vec] = 6;
					nr_vec = nr_vec + 1;
					for (k = i; k < i + 5; k++) {
						for (l = j; l > j - 5; l--) {
							matrice[k][l] = '.';	// eliminam din matrice cifra
						}
					}
					found = 1;
				}
			}

			if (found == 0) {
				dif = 0;
				for (k = 0; k < n; k++) {
					if (nr[k] != nr7[k]) {
						dif = dif + 1;
					}
				}

				if (dif == 1) {
					vec_nr[nr_vec] = 7;
					nr_vec = nr_vec + 1;
					for (k = i; k < i + 5; k++) {
						for (l = j; l > j - 5; l--) {
							matrice[k][l] = '.';	// eliminam din matrice cifra
						}
					}
					found = 1;
				}
			}

			if (found == 0) {
				dif = 0;
				for (k = 0; k < n; k++) {
					if (nr[k] != nr8[k]) {
						dif = dif + 1;
					}
				}

				if (dif == 1) {
					vec_nr[nr_vec] = 8;
					nr_vec = nr_vec + 1;
					for (k = i; k < i + 5; k++) {
						for (l = j; l > j - 5; l--) {
							matrice[k][l] = '.';	// eliminam din matrice cifra
						}
					}
					found = 1;
				}
			}

			if (found == 0) {
				dif = 0;
				for (k = 0; k < n; k++) {
					if (nr[k] != nr9[k]) {
						dif = dif + 1;
					}
				}

				if (dif == 1) {
					vec_nr[nr_vec] = 9;
					nr_vec = nr_vec + 1;
					for (k = i; k < i + 5; k++) {
						for (l = j; l > j - 5; l--) {
							matrice[k][l] = '.';	// eliminam din matrice cifra
						}
					}
					found = 1;
				}
			}
		}
	}

	printf("\n");

	// afisam numerele gasite in output, numerele sunt in ordine inversa in vector
	for (i = nr_vec - 1; i >= 0; i--) {
		fprintf(output, "%d", vec_nr[i]);
	}

	fclose(input);
	fclose(captcha);
	fclose(output);

	return 0;
}
