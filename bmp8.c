// bmp8.c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bmp8.h"
#include <math.h>

#define HEADER_SIZE 54
#define COLOR_TABLE_SIZE 1024

// Chargement d'une image 8 bits
t_bmp8* bmp8_loadImage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");
        return NULL;
    }

    t_bmp8 *img = (t_bmp8*)malloc(sizeof(t_bmp8));
    if (!img) {
        perror("Erreur d'allocation mémoire pour l'image");
        fclose(file);
        return NULL;
    }

    fread(img->header, sizeof(unsigned char), HEADER_SIZE, file);

    img->width = *(unsigned int *)(img->header + 18);
    img->height = *(unsigned int *)(img->header + 22);
    img->colorDepth = *(unsigned short *)(img->header + 28);  // ATTENTION ici: c'est 2 octets, pas 4
    img->dataSize = img->width * img->height;

    if (img->colorDepth != 8) {
        printf("Erreur : profondeur de couleur différente de 8 bits.\n");
        free(img);
        fclose(file);
        return NULL;
    }

    fread(img->colorTable, sizeof(unsigned char), COLOR_TABLE_SIZE, file);

    img->data = (unsigned char*)malloc(img->dataSize * sizeof(unsigned char));
    if (!img->data) {
        perror("Erreur d'allocation mémoire pour les données de l'image");
        free(img);
        fclose(file);
        return NULL;
    }

    fread(img->data, sizeof(unsigned char), img->dataSize, file);

    fclose(file);
    return img;
}

// Sauvegarde d'une image 8 bits
void bmp8_saveImage(const char *filename, t_bmp8 *img) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Erreur lors de la création du fichier");
        return;
    }

    fwrite(img->header, sizeof(unsigned char), HEADER_SIZE, file);
    fwrite(img->colorTable, sizeof(unsigned char), COLOR_TABLE_SIZE, file);
    fwrite(img->data, sizeof(unsigned char), img->dataSize, file);

    fclose(file);
}

// Libération de la mémoire
void bmp8_free(t_bmp8 *img) {
    if (img) {
        free(img->data);
        free(img);
    }
}

// Affichage des infos
void bmp8_printInfo(t_bmp8 *img) {
    printf("\n--- Informations sur l'image ---\n");
    printf("Largeur : %d pixels\n", img->width);
    printf("Hauteur : %d pixels\n", img->height);
    printf("Profondeur de couleur : %d bits\n", img->colorDepth);
    printf("Taille des données : %d octets\n", img->dataSize);
}

// Appliquer le négatif
void bmp8_negative(t_bmp8 *img) {
    for (unsigned int i = 0; i < img->dataSize; ++i) {
        img->data[i] = 255 - img->data[i];
    }
}

// Modifier la luminosité
void bmp8_brightness(t_bmp8 *img, int value) {
    for (unsigned int i = 0; i < img->dataSize; ++i) {
        int pixel = img->data[i] + value;
        if (pixel > 255) pixel = 255;
        if (pixel < 0) pixel = 0;
        img->data[i] = (unsigned char)pixel;
    }
}

// Seuil/binarisation
void bmp8_threshold(t_bmp8 *img, int threshold) {
    for (unsigned int i = 0; i < img->dataSize; ++i) {
        img->data[i] = (img->data[i] >= threshold) ? 255 : 0;
    }
}

// Application de filtre de convolution
void bmp8_applyFilter(t_bmp8 *img, float **kernel, int kernelSize) {
    int offset = kernelSize / 2;
    unsigned char *temp = (unsigned char*)malloc(img->dataSize * sizeof(unsigned char));
    if (!temp) {
        perror("Erreur d'allocation mémoire temporaire");
        return;
    }

    for (unsigned int y = offset; y < img->height - offset; ++y) {
        for (unsigned int x = offset; x < img->width - offset; ++x) {
            float sum = 0.0;
            for (int i = -offset; i <= offset; ++i) {
                for (int j = -offset; j <= offset; ++j) {
                    int xi = x + j;
                    int yi = y + i;
                    sum += img->data[yi * img->width + xi] * kernel[i + offset][j + offset];
                }
            }
            if (sum > 255) sum = 255;
            if (sum < 0) sum = 0;
            temp[y * img->width + x] = (unsigned char)round(sum);
        }
    }

    for (unsigned int i = 0; i < img->dataSize; ++i) {
        img->data[i] = temp[i];
    }

    free(temp);
}
