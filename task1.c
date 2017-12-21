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
	int b, g, r;		// culorile ce vor fi citite: b-blue, g-green, r-red
	struct bmp_fileheader captcha_fileheader;
	struct bmp_infoheader captcha_infoheader;
	char matrice[MAX][MAX];	// matricea de pixeli
	int i, j, line, col, padding, aux = 0;	// line - nr de linii, col - nr de coloane; aux - ptr scrierea padding-ului 0 in output;
	struct BGR color;	// canalele ce vor fi citite din fisierul BMP captcha
	struct BGR white;

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
	strcat(output_name, "_task1.bmp");
	output = fopen(output_name, "wb");

	fscanf(input, "%d%d%d", &b, &g, &r);	// citim culorile

	// citim fisierul BMP
	captcha = fopen(captcha_name, "rb");
	readBMP(captcha, &captcha_fileheader, &captcha_infoheader);

	// scriem header-ul si infoheader-ul in fisierul output
	writeBMP(output, &captcha_fileheader, &captcha_infoheader);

	line = captcha_infoheader.height;
	col = captcha_infoheader.width;

	// o linie trebuie sa aiba multiplu de 4 bytes, iar acesta se completeaza cu padding-ul
	if (((captcha_infoheader.width * 3) % 4) != 0) {
		padding = 4 - ((captcha_infoheader.width * 3) % 4);
	}
	// construim matricea cu datele din fisierul BMP captcha
	// scriem datele in fisierul output
	for (i = line - 1; i >= 0; i--) {
		for (j = 0; j < col; j++) {
			// citim cele 3 canale pentru fiecare pixel
			fread(&color.B, 1, 1, captcha);
			fread(&color.G, 1, 1, captcha);
			fread(&color.R, 1, 1, captcha);

			if ((color.B == 255) && (color.G == 255) && (color.R == 255))	//pixel alb
			{
				matrice[i][j] = '.';

				fwrite(&white.B, 1, 1, output);
				fwrite(&white.G, 1, 1, output);
				fwrite(&white.R, 1, 1, output);
			} else {
				matrice[i][j] = 'X';

				fwrite(&b, 1, 1, output);
				fwrite(&g, 1, 1, output);
				fwrite(&r, 1, 1, output);
			}
		}

		fseek(captcha, padding, SEEK_CUR);	// se ignora padding-ul de dupa fiecare linie
		fwrite(&aux, padding, 1, output);	// se scrie padding 0 in output
	}

	// afisam matricea de pixeli
	for (i = 0; i < line; i++) {
		for (j = 0; j < col; j++) {
			printf("%c", matrice[i][j]);
		}
		printf("\n");
	}

	fclose(input);
	fclose(captcha);
	fclose(output);

	return 0;
}
