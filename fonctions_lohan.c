#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "fonctions_lohan.h" // Ajout pour les types uint8_t, uint16_t, uint32_t

#define HEADER_SIZE 54
#define COLOR_TABLE_SIZE 1024

// Fonction pour charger une image BMP en niveaux de gris
t_bmp8* bmp8_loadImage(const char* filename) {
    FILE *file = fopen(filename, "rb");
    if (file) {
        perror("Erreur fopen");
        printf("Vérifie que le fichier %s existe et que le programme y a accès.\n", filename);
        return NULL;
    }

    t_bmp8 *img = (t_bmp8*)malloc(sizeof(t_bmp8));
    if (!img) {
        printf("Erreur : Échec d'allocation mémoire.\n");
        fclose(file);
        return NULL;
    }

    fread(img->header, sizeof(unsigned char), HEADER_SIZE, file);

    img->width = *(unsigned int*)(img->header + 18);
    img->height = *(unsigned int*)(img->header + 22);
    img->colorDepth = *(unsigned int*)(img->header + 28);
    img->dataSize = img->width * img->height;

    if (img->colorDepth <= 8) {
        fread(img->colorTable, sizeof(unsigned char), COLOR_TABLE_SIZE, file);
    }

    img->data = (unsigned char*)malloc(img->dataSize * sizeof(unsigned char));
    if (!img->data) {
        printf("Erreur : Échec d'allocation mémoire pour les données d'image.\n");
        free(img);
        fclose(file);
        return NULL;
    }
    fread(img->data, sizeof(unsigned char), img->dataSize, file);
    fclose(file);

    return img;
}

// Fonction pour sauvegarder une image BMP en niveaux de gris
void bmp8_saveImage(const char* filename, t_bmp8 *img) {
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
void bmp8_printInfo(t_bmp8 *img) {
    printf("\n--- Informations sur l'image ---\n");
    printf("Largeur : %d pixels\n", img->width);
    printf("Hauteur : %d pixels\n", img->height);
    printf("Profondeur de couleur : %d bits\n", img->colorDepth);
    printf("Taille des données : %d octets\n", img->dataSize);
}

// Fonction d'affichage du menu
void afficherMenu() {
    printf("\n--- Menu ---\n");
    printf("1. Charger une image\n");
    printf("2. Afficher les informations de l'image\n");
    printf("3. Sauvegarder l'image\n");
    printf("4. Quitter\n");
    printf("Votre choix : ");
}
