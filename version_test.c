//
// Created by lohan on 24/03/2025.
//
//
// Created by lohan on 17/03/2025.
//

#include "fonctions_lohan.h"
#include <stdio.h>
#include <stdlib.h>
#define HEADER_SIZE 54
#define COLOR_TABLE_SIZE 1024


// Fonction pour charger une image BMP en niveaux de gris
t_bmp8* bmp8_loadImage(const char* filename) {
    FILE *file = fopen(filename, "rb");
    if (file) {
        printf("Erreur : Impossible d'ouvrir le fichier %s\n", filename);
        return NULL;
    }

    t_bmp8 *img = (t_bmp8*)malloc(sizeof(t_bmp8));
    fread(img->header, sizeof(unsigned char), HEADER_SIZE, file);

    img->width = *(unsigned int*)&img->header[18];
    img->height = *(unsigned int*)&img->header[22];
    img->colorDepth = *(unsigned int*)&img->header[28];
    img->dataSize = *(unsigned int*)&img->header[34];

    if (img->colorDepth <= 8) {
        fread(img->colorTable, sizeof(unsigned char), COLOR_TABLE_SIZE, file);
    }

    img->data = (unsigned char*)malloc(img->dataSize * sizeof(unsigned char));
    fread(img->data, sizeof(unsigned char), img->dataSize, file);
    fclose(file);

    return img;
}

// Fonction pour sauvegarder une image BMP en niveaux de gris
void bmp8_saveImage(const char* filename, t_bmp8* img) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Erreur : Impossible de créer le fichier %s\n", filename);
        return;
    }

    fwrite(img->header, sizeof(unsigned char), HEADER_SIZE, file);
    if (img->colorDepth <= 8) {
        fwrite(img->colorTable, sizeof(unsigned char), COLOR_TABLE_SIZE, file);
    }
    fwrite(img->data, sizeof(unsigned char), img->dataSize, file);
    fclose(file);
}

// Fonction pour libérer la mémoire d'une image
void bmp8_free(t_bmp8* img) {
    free(img->data);
    free(img);
}

// Fonction pour afficher les informations d'une image
void bmp8_printInfo(t_bmp8* img) {
    printf("Image Info:\n");
    printf("Width: %d\n", img->width);
    printf("Height: %d\n", img->height);
    printf("Color Depth: %d\n", img->colorDepth);
    printf("Data Size: %d bytes\n", img->dataSize);
}
void afficherMenu() {
    printf("\n--- Menu ---\n");
    printf("1. Charger une image\n");
    printf("2. Afficher les informations de l'image\n");
    printf("3. Sauvegarder l'image\n");
    printf("4. Quitter\n");
    printf("Votre choix : ");
}
