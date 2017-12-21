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
	int n, i, j, k, l, line, col, padding;	// line - nr de linii, col - nr de coloane; n - nr caractere din stringul nr[]
	struct BGR color;	// canalele ce vor fi citite din fisierul BMP captcha

	//sir de caractere pentru toate tipurile de cifre
	char nr0[30] = "XXXXXX...XX...XX...XXXXXX";
	char nr1[30] = "....X....X....X....X....X";	// nu ne trebuie, dar asa le avem pe toate
	char nr2[30] = "XXXXX....XXXXXXX....XXXXX";
	char nr3[30] = "XXXXX....XXXXXX....XXXXXX";
	char nr4[30] = "X...XX...XXXXXX....X....X";
	char nr5[30] = "XXXXXX....XXXXX....XXXXXX";
	char nr6[30] = "XXXXXX....XXXXXX...XXXXXX";
	char nr7[30] = "XXXXX....X....X....X....X";
	char nr8[30] = "XXXXXX...XXXXXXX...XXXXXX";
	char nr9[30] = "XXXXXX...XXXXXX....XXXXXX";
	char nr[30];		// sir de caractere pentru a verifica cifra

	input = fopen("input.txt", "rt");

	fgets(captcha_name, MAXCHAR, input);	//captcha_name va avea "\n" inainte de "\0"
	if ((strlen(captcha_name) > 0)
	    && (captcha_name[strlen(captcha_name) - 1] == '\n')) {
		captcha_name[strlen(captcha_name) - 1] = '\0';
	}			// s-a eliminat "\n" din captcha_name

	strncpy(output_name, captcha_name, strlen(captcha_name) - 4);	// ".bmp" are 4 caractere
	output_name[strlen(captcha_name) - 4] = '\0';	// sa ne asiguram ca se pune "\0"
	strcat(output_name, "_task2.txt");
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

	for (j = 0; j < col; j++) {
		for (i = 0; i < line; i++) {
			if ((j < col - 4) && (matrice[i][j + 4] == 'X') && (matrice[i][j] == '.'))	// verificam cazul in care este cifra 1
			{
				if (((j + 4 == col - 2)
				     && (matrice[i][j + 3] == '.')
				     && (matrice[i + 1][j + 3] == '.')
				     && (matrice[i + 2][j + 3] == '.')
				     && (matrice[i + 3][j + 3] == '.')
				     && (matrice[i + 4][j + 3] == '.'))
				    || ((j + 4 < col - 1)
					&& (matrice[i][j + 5] == '.')
					&& (matrice[i + 1][j + 5] == '.')
					&& (matrice[i + 2][j + 5] == '.')
					&& (matrice[i + 3][j + 5] == '.')
					&& (matrice[i + 4][j + 5] == '.'))) {
					fprintf(output, "%c", '1');
					for (k = i; k < i + 5; k++)	// eliminam din matrice cifra 1 gasita
					{
						matrice[k][j + 4] = '.';
					}
				}
			} else if (matrice[i][j] == 'X')	// verificam cazul in care este alta cifra decat 1
			{
				// construim un sir de caractere pentru acea cifra
				n = 0;
				for (k = i; k < i + 5; k++) {
					for (l = j; l < j + 5; l++) {
						nr[n] = matrice[k][l];
						n = n + 1;
						matrice[k][l] = '.';	// eliminam din matrice cifra respectiva
					}
				}
				nr[n] = '\0';

				// verificam ce cifra e
				if (strcmp(nr, nr0) == 0) {
					fprintf(output, "%c", '0');
				}
				if (strcmp(nr, nr2) == 0) {
					fprintf(output, "%c", '2');
				}
				if (strcmp(nr, nr3) == 0) {
					fprintf(output, "%c", '3');
				}
				if (strcmp(nr, nr4) == 0) {
					fprintf(output, "%c", '4');
				}
				if (strcmp(nr, nr5) == 0) {
					fprintf(output, "%c", '5');
				}
				if (strcmp(nr, nr6) == 0) {
					fprintf(output, "%c", '6');
				}
				if (strcmp(nr, nr7) == 0) {
					fprintf(output, "%c", '7');
				}
				if (strcmp(nr, nr8) == 0) {
					fprintf(output, "%c", '8');
				}
				if (strcmp(nr, nr9) == 0) {
					fprintf(output, "%c", '9');
				}
			}
		}
	}

	fclose(input);
	fclose(captcha);
	fclose(output);

	return 0;
}
