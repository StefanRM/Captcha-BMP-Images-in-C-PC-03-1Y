#include "bmp_header.h"
#include <stdio.h>
#include <string.h>

#define MAXCHAR 30		// lungimea maxima a numelor fisierelor
#define MAX 150			// lungimea maxima a liniilor/coloanelor din matrice
#define MAXCOLORS 50		// lungimea maxima a nr. de numere/culori din fisier

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
	char matrice[MAX][MAX], matrice_aux[MAX][MAX];	// matrice - matricea de pixeli, matrice_aux - matrice de pixeli auxiliara
	int n, i, j, k, l, q, r, line, col, padding, aux = 0, nr_colors, nr_numbers = 0, i_pozy = -1, j_pozy = -1;	// nr_numbers - cate numere au fost gasite in matrice; nr_colors - nr de elemente din vectorul colors; line - nr de linii; col - nr de coloane; n - nr caractere din stringul nr[]
	struct BGR color, white;	// color - canalele ce vor fi citite din fisierul BMP captcha
	struct BGR colors[MAXCOLORS];	// vector cu cele 3 canale BGR pentru fiecare cifra din BMP (in ordinea cifrelor)
	struct BGR matrice_colors[MAX][MAX];	// matrice de pixeli, avand cele 3 canale BGR
	int vector_vals[MAXCOLORS], nr_vals;	// vector_vals - vector cu cifrele de eliminat din BMP; nr_vals - nr de cifre de eliminat
	char collector[MAXCHAR];	// pentru a 'colecta' fiecare linii din fisierul input

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

	// pixelul alb
	white.B = 255;
	white.G = 255;
	white.R = 255;

	input = fopen("input.txt", "rt");

	fgets(captcha_name, MAXCHAR, input);	//captcha_name va avea '\n' inainte de '\0'
	if ((strlen(captcha_name) > 0)
	    && (captcha_name[strlen(captcha_name) - 1] == '\n')) {
		captcha_name[strlen(captcha_name) - 1] = '\0';
	}			// s-a eliminat '\n' din captcha_name

	strncpy(output_name, captcha_name, strlen(captcha_name) - 4);	// ".bmp" are 4 caractere
	output_name[strlen(captcha_name) - 4] = '\0';	// sa ne asiguram ca se pune '\0'
	strcat(output_name, "_task3.bmp");
	output = fopen(output_name, "wb");

	fgets(collector, MAXCHAR, input);	// se citeste al doilea rand din input, cel cu culorile BGR, dar nu il vom folosi

	fgets(collector, MAXCHAR, input);	// se citeste array-ul cu numere de eliminat intr-un sir de caractere
	if ((strlen(collector) > 0)
	    && (collector[strlen(collector) - 1] == '\n')) {
		collector[strlen(collector) - 1] = '\0';
	}			// s-a eliminat '\n' din collector

	nr_vals = 0;

	i = 0;

	// se extrag din sirul de caractere numerele care vor fi introduse in vector_vals
	if (collector[i] != '\0') {
		while (i <= strlen(collector)) {
			vector_vals[nr_vals] = collector[i] - '0';	// diferenta de cod ASCII astfel incat sa se obtine valoarea cifrei
			nr_vals = nr_vals + 1;
			i = i + 2;
		}
	}
	// citim fisierul BMP
	captcha = fopen(captcha_name, "rb");
	readBMP(captcha, &captcha_fileheader, &captcha_infoheader);
	writeBMP(output, &captcha_fileheader, &captcha_infoheader);

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

			// construim in acelasi timp matricea de pixeli cu cele 3 canale BGR
			matrice_colors[i][j].B = color.B;
			matrice_colors[i][j].G = color.G;
			matrice_colors[i][j].R = color.R;
		}
		fseek(captcha, padding, SEEK_CUR);	// se ignora padding-ul de dupa fiecare linie
	}

	nr_colors = 0;

	// se face o copie a matricei de pixeli
	for (i = 0; i < line; i++) {
		for (j = 0; j < col; j++) {
			matrice_aux[i][j] = matrice[i][j];
		}
	}

	// construim vectorul de culori
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
					for (k = i; k < i + 5; k++)	// eliminam din matrice cifra 1
					{
						matrice[k][j + 4] = '.';
					}
					colors[nr_colors].B =
					    matrice_colors[i][j + 4].B;
					colors[nr_colors].G =
					    matrice_colors[i][j + 4].G;
					colors[nr_colors].R =
					    matrice_colors[i][j + 4].R;
					nr_colors = nr_colors + 1;
				}
			} else if (matrice[i][j] == 'X')	// verificam cazul in care este alta cifra decat 1
			{
				// construim un sir de caractere pentru restul cifrelor
				n = 0;
				for (k = i; k < i + 5; k++) {
					for (l = j; l < j + 5; l++) {
						nr[n] = matrice[k][l];
						n = n + 1;
						matrice[k][l] = '.';	// eliminam din matrice cifra 1
					}
				}
				nr[n] = '\0';

				colors[nr_colors].B = matrice_colors[i][j].B;
				colors[nr_colors].G = matrice_colors[i][j].G;
				colors[nr_colors].R = matrice_colors[i][j].R;
				nr_colors = nr_colors + 1;
			}
		}
		fseek(captcha, padding, SEEK_CUR);	// se ignora padding-ul de dupa fiecare linie
	}

	// refacem matricea de pixeli folosind copia din matrice_aux
	for (i = 0; i < line; i++) {
		for (j = 0; j < col; j++) {
			matrice[i][j] = matrice_aux[i][j];
		}
	}

	// afisam matricea de pixeli
	for (i = 0; i < line; i++) {
		for (j = 0; j < col; j++) {
			printf("%c", matrice[i][j]);
		}
		printf("\n");
	}

	// eliminam cifrele
	// cautam in matricea de pixeli cifrele
	for (j = 0; j < col; j++) {
		for (i = 0; i < line; i++) {
			if ((i < line - 4) && (j < col - 4) && (matrice[i][j + 4] == 'X') && (matrice[i][j] == '.') && (matrice[i + 1][j + 4] == 'X') && (matrice[i + 2][j + 4] == 'X') && (matrice[i + 3][j + 4] == 'X') && (matrice[i + 4][j + 4] == 'X'))	// verificam cazul in care este cifra 1
			{
				if (((j + 4 == col - 1)
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
					nr_numbers = nr_numbers + 1;	// incrementam numarul de cifre gasite in matricea de pixeli
					for (l = 0; l < nr_vals; l++)	// verificam daca cifra 1 se afla in vectorul de cifre de eliminat
					{
						if (vector_vals[l] == 1) {
							break;
						}
					}

					if (l < nr_vals) {
						for (k = i; k < i + 5; k++) {
							matrice[k][j + 4] = '.';
						}
						matrice[i][j] = 'Y';	// indice in matricea de pixeli unde incepea cifra eliminata
						for (q = nr_numbers - 1; q < nr_colors - 1; q++)	// eliminam culoarea din vectorul de culori
						{
							colors[q] =
							    colors[q + 1];
						}
						nr_colors = nr_colors - 1;
						nr_numbers = nr_numbers - 1;	// decrementam numarul de cifre gasite in matrice, ptr ca a fost eliminata cifra
					}
				}
			} else if ((matrice[i][j] == 'X') && (i < line - 4) && (j < col - 4))	// verificam cazul in care este alta cifra decat 1
			{
				// construim un sir de caractere pentru restul cifrelor
				n = 0;
				for (k = i; k < i + 5; k++) {
					for (l = j; l < j + 5; l++) {
						nr[n] = matrice[k][l];
						n = n + 1;
					}
				}
				nr[n] = '\0';

				// verificam daca ce cifra este si apoi incrementam numarul de cifre gasite in matricea de pixeli
				if (strcmp(nr, nr0) == 0) {
					nr_numbers = nr_numbers + 1;
				}
				if (strcmp(nr, nr2) == 0) {
					nr_numbers = nr_numbers + 1;
				}
				if (strcmp(nr, nr3) == 0) {
					nr_numbers = nr_numbers + 1;
				}
				if (strcmp(nr, nr4) == 0) {
					nr_numbers = nr_numbers + 1;
				}
				if (strcmp(nr, nr5) == 0) {
					nr_numbers = nr_numbers + 1;
				}
				if (strcmp(nr, nr6) == 0) {
					nr_numbers = nr_numbers + 1;
				}
				if (strcmp(nr, nr7) == 0) {
					nr_numbers = nr_numbers + 1;
				}
				if (strcmp(nr, nr8) == 0) {
					nr_numbers = nr_numbers + 1;
				}
				if (strcmp(nr, nr9) == 0) {
					nr_numbers = nr_numbers + 1;
				}
				// verificam daca cifra se afla in vectorul de cifre de eliminat
				for (r = 0; r < nr_vals; r++) {
					if (strcmp(nr, nr0) == 0) {
						if (vector_vals[r] == 0) {
							break;
						}
					}
					if (strcmp(nr, nr2) == 0) {
						if (vector_vals[r] == 2) {
							break;
						}
					}
					if (strcmp(nr, nr3) == 0) {
						if (vector_vals[r] == 3) {
							break;
						}
					}
					if (strcmp(nr, nr4) == 0) {
						if (vector_vals[r] == 4) {
							break;
						}
					}
					if (strcmp(nr, nr5) == 0) {
						if (vector_vals[r] == 5) {
							break;
						}
					}
					if (strcmp(nr, nr6) == 0) {
						if (vector_vals[r] == 6) {
							break;
						}
					}
					if (strcmp(nr, nr7) == 0) {
						if (vector_vals[r] == 7) {
							break;
						}
					}
					if (strcmp(nr, nr8) == 0) {
						if (vector_vals[r] == 8) {
							break;
						}
					}
					if (strcmp(nr, nr9) == 0) {
						if (vector_vals[r] == 9) {
							break;
						}
					}
				}

				if (r < nr_vals) {
					for (k = i; k < i + 5; k++) {
						for (l = j; l < j + 5; l++) {
							matrice[k][l] = '.';	// eliminam din matrice cifra 1
						}
					}
					matrice[i][j] = 'Y';	// indice in matricea de pixeli unde incepea cifra eliminata
					for (q = nr_numbers - 1; q < nr_colors - 1; q++)	// eliminam culoarea din vectorul de culori
					{
						colors[q] = colors[q + 1];
					}
					nr_colors = nr_colors - 1;
					nr_numbers = nr_numbers - 1;	// decrementam numarul de cifre gasite in matrice, ptr ca a fost eliminata cifra
				}

			}
		}
	}

	printf("\n");

	// afisam matricea dupa ce s-au eliminat cifrele de eliminat si s-au marcat pozitiile cu Y
	for (i = 0; i < line; i++) {
		for (j = 0; j < col; j++) {
			printf("%c", matrice[i][j]);
		}
		printf("\n");
	}

	// mutam cifrele in pozitiile cifrelor eliminate anterior
	for (r = 0; r < 20; r++)	// parcurgem algoritmul de mai multe ori ca sa fim siguri ca s-au mutat toate cifrele
	{
		i_pozy = -1, j_pozy = -1;	// pozitiile Y in matricea de pixeli; cautam aparitiile lor

		for (j = 0; j < col; j++) {
			for (i = 0; i < line; i++) {
				if (matrice[i][j] == 'Y') {
					i_pozy = i;
					j_pozy = j;
				} else if ((i < line - 4) && (j < col - 4) && (matrice[i][j + 4] == 'X') && (matrice[i][j] == '.') && (matrice[i + 1][j + 4] == 'X') && (matrice[i + 2][j + 4] == 'X') && (matrice[i + 3][j + 4] == 'X') && (matrice[i + 4][j + 4] == 'X') && (i_pozy >= 0) && (j_pozy >= 0))	// verificam cazul in care este cifra 1
				{
					if (((j + 4 == col - 1)
					     && (matrice[i][j + 3] == '.')
					     && (matrice[i + 1][j + 3] == '.')
					     && (matrice[i + 2][j + 3] == '.')
					     && (matrice[i + 3][j + 3] == '.')
					     && (matrice[i + 4][j + 3] == '.'))
					    || ((j + 4 < col - 1)
						&& (matrice[i][j + 5] == '.')
						&& (matrice[i + 1][j + 5] ==
						    '.')
						&& (matrice[i + 2][j + 5] ==
						    '.')
						&& (matrice[i + 3][j + 5] ==
						    '.')
						&& (matrice[i + 4][j + 5] ==
						    '.'))) {
						// suprapunem cifra peste locul anterior
						for (k = i_pozy; k < i_pozy + 5;
						     k++) {
							matrice[k][j_pozy + 4] =
							    'X';
						}

						matrice[i_pozy][j_pozy] = '.';	// stergem Y din matricea de pixeli

						for (k = i; k < i + 5; k++)	// stergem cifra din matricea de pixeli
						{
							matrice[k][j + 4] = '.';
						}
						matrice[i][j] = 'Y';	// punem in locul cifrei sterse Y ca sa marcam pozitia

						//retinem coordonatele ptr Y
						i_pozy = i;
						j_pozy = j;
					}
				} else if ((matrice[i][j] == 'X') && (i < line - 4) && (j < col - 4) && (i_pozy >= 0) && (j_pozy >= 0))	// verificam cazul in care este alta cifra decat 1
				{
					// construim un sir de caractere pentru restul cifrelor
					n = 0;
					for (k = i; k < i + 5; k++) {
						for (l = j; l < j + 5; l++) {
							nr[n] = matrice[k][l];
							n = n + 1;
						}
					}
					nr[n] = '\0';

					if ((strcmp(nr, nr0) == 0) || (strcmp(nr, nr2) == 0) || (strcmp(nr, nr3) == 0) || (strcmp(nr, nr4) == 0) || (strcmp(nr, nr5) == 0) || (strcmp(nr, nr6) == 0) || (strcmp(nr, nr7) == 0) || (strcmp(nr, nr8) == 0) || (strcmp(nr, nr9) == 0))	// verificam daca e cifra
					{
						for (k = i; k < i + 5; k++)	// stergem cifra din matricea de pixeli
						{
							for (l = j; l < j + 5;
							     l++) {
								matrice[k][l] =
								    '.';
							}
						}

						matrice[i][j] = 'Y';	// punem in locul cifrei sterse Y ca sa marcam pozitia

						n = 0;

						for (k = i_pozy; k < i_pozy + 5; k++)	// suprapunem cifra peste locul anterior
						{
							for (l = j_pozy;
							     l < j_pozy + 5;
							     l++) {
								matrice[k][l] =
								    nr[n];
								n = n + 1;
							}
						}

						//retinem coordonatele ptr Y
						i_pozy = i;
						j_pozy = j;
					}

				}
			}
		}
	}

	for (i = 0; i < line; i++)	// stergem Y din matricea de pixeli
	{
		for (j = 0; j < col; j++) {
			if (matrice[i][j] == 'Y') {
				matrice[i][j] = '.';
			}
		}
	}

	printf("\n");

	// afisam matricea dupa ce s-au mutat cifrele
	for (i = 0; i < line; i++) {
		for (j = 0; j < col; j++) {
			printf("%c", matrice[i][j]);
		}
		printf("\n");
	}

	// facem din nou o copie noii matrici de pixeli
	for (i = 0; i < line; i++) {
		for (j = 0; j < col; j++) {
			matrice_aux[i][j] = matrice[i][j];
		}
	}

	// facem alba matricea de culori (fiecare canal BGR e alb)
	for (i = 0; i < line; i++) {
		for (j = 0; j < col; j++) {
			matrice_colors[i][j].B = white.B;
			matrice_colors[i][j].G = white.G;
			matrice_colors[i][j].R = white.R;
		}
	}

	nr_numbers = 0;		// resetam nr de cifre gasite in matrice

	// coloram corespunzator matricea de culori, verificand fiecare cifra
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
					nr_numbers = nr_numbers + 1;
					for (k = i; k < i + 5; k++)	// eliminam din matrice cifra 1
					{
						matrice[k][j + 4] = '.';

						// adaugam culoarea in matrice pe poziitile corespunzatoare
						matrice_colors[k][j + 4].B =
						    colors[nr_numbers - 1].B;
						matrice_colors[k][j + 4].G =
						    colors[nr_numbers - 1].G;
						matrice_colors[k][j + 4].R =
						    colors[nr_numbers - 1].R;
					}
				}
			} else if (matrice[i][j] == 'X')	// verificam cazul in care este alta cifra decat 1
			{
				nr_numbers = nr_numbers + 1;
				for (k = i; k < i + 5; k++) {
					for (l = j; l < j + 5; l++) {
						if (matrice[k][l] == 'X')	// adaugam culoarea in matrice pe poziitile corespunzatoare
						{
							matrice_colors[k][l].B =
							    colors[nr_numbers -
								   1].B;
							matrice_colors[k][l].G =
							    colors[nr_numbers -
								   1].G;
							matrice_colors[k][l].R =
							    colors[nr_numbers -
								   1].R;
						}
						matrice[k][l] = '.';	// eliminam din matrice cifra
					}
				}
			}
		}
	}

	// refacem matricea de pixeli folosind copia
	for (i = 0; i < line; i++) {
		for (j = 0; j < col; j++) {
			matrice[i][j] = matrice_aux[i][j];
		}
	}

	// scriem datele in fisierul BMP output, folosind matricea de pixeli si matricea de culori
	for (i = line - 1; i >= 0; i--) {
		for (j = 0; j < col; j++) {
			if (matrice[i][j] == '.') {
				fwrite(&white.B, 1, 1, output);
				fwrite(&white.G, 1, 1, output);
				fwrite(&white.R, 1, 1, output);
			} else {
				fwrite(&matrice_colors[i][j].B, 1, 1, output);
				fwrite(&matrice_colors[i][j].G, 1, 1, output);
				fwrite(&matrice_colors[i][j].R, 1, 1, output);
			}
		}

		fwrite(&aux, padding, 1, output);	// se scrie padding 0 in output
	}

	fclose(input);
	fclose(captcha);
	fclose(output);

	return 0;
}
