// bmp24.c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bmp24.h"
#include <math.h>

#define BITMAP_MAGIC 0x4D42
#define BITMAP_OFFSET 0x0A
#define BITMAP_WIDTH 0x12
#define BITMAP_HEIGHT 0x16
#define BITMAP_DEPTH 0x1C

// Allocation mémoire pour data
t_pixel** bmp24_allocateDataPixels(int width, int height) {
    t_pixel **pixels = (t_pixel **)malloc(height * sizeof(t_pixel *));
    if (!pixels) {
        perror("Erreur d'allocation pour les lignes");
        return NULL;
    }
    for (int i = 0; i < height; ++i) {
        pixels[i] = (t_pixel *)malloc(width * sizeof(t_pixel));
        if (!pixels[i]) {
            perror("Erreur d'allocation pour une ligne");
            for (int j = 0; j < i; ++j) free(pixels[j]);
            free(pixels);
            return NULL;
        }
    }
    return pixels;
}

// Libération de data
void bmp24_freeDataPixels(t_pixel **pixels, int height) {
    if (!pixels) return;
    for (int i = 0; i < height; ++i) {
        free(pixels[i]);
    }
    free(pixels);
}

// Allocation d'une image entière
t_bmp24* bmp24_allocate(int width, int height, int colorDepth) {
    t_bmp24 *img = (t_bmp24*)malloc(sizeof(t_bmp24));
    if (!img) {
        perror("Erreur d'allocation pour t_bmp24");
        return NULL;
    }
    img->width = width;
    img->height = height;
    img->colorDepth = colorDepth;
    img->data = bmp24_allocateDataPixels(width, height);
    if (!img->data) {
        free(img);
        return NULL;
    }
    return img;
}

// Libération d'une image entière
void bmp24_free(t_bmp24 *img) {
    if (!img) return;
    bmp24_freeDataPixels(img->data, img->height);
    free(img);
}

// Charger une image 24 bits
t_bmp24* bmp24_loadImage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Erreur d'ouverture du fichier");
        return NULL;
    }

    uint16_t magic;
    fread(&magic, sizeof(uint16_t), 1, file);
    if (magic != BITMAP_MAGIC) {
        printf("Erreur : fichier non BMP\n");
        fclose(file);
        return NULL;
    }
    fseek(file, 0, SEEK_SET);

    t_bmp24 *img = (t_bmp24*)malloc(sizeof(t_bmp24));
    if (!img) {
        perror("Erreur d'allocation");
        fclose(file);
        return NULL;
    }

    fread(&(img->header), sizeof(t_bmp_header), 1, file);
    fread(&(img->header_info), sizeof(t_bmp_info), 1, file);

    img->width = img->header_info.width;
    img->height = img->header_info.height;
    img->colorDepth = img->header_info.bits;

    if (img->colorDepth != 24) {
        printf("Erreur : profondeur de couleur différente de 24 bits.\n");
        free(img);
        fclose(file);
        return NULL;
    }

    img->data = bmp24_allocateDataPixels(img->width, img->height);
    if (!img->data) {
        free(img);
        fclose(file);
        return NULL;
    }

    fseek(file, img->header.offset, SEEK_SET);

    for (int i = img->height - 1; i >= 0; --i) { // BMP stocke les pixels à l'envers
        for (int j = 0; j < img->width; ++j) {
            uint8_t colors[3];
            fread(colors, sizeof(uint8_t), 3, file);
            img->data[i][j].blue = colors[0];
            img->data[i][j].green = colors[1];
            img->data[i][j].red = colors[2];
        }
    }

    fclose(file);
    return img;
}

// Sauvegarder une image 24 bits
void bmp24_saveImage(const char *filename, t_bmp24 *img) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Erreur de création du fichier");
        return;
    }

    fwrite(&(img->header), sizeof(t_bmp_header), 1, file);
    fwrite(&(img->header_info), sizeof(t_bmp_info), 1, file);

    fseek(file, img->header.offset, SEEK_SET);

    for (int i = img->height - 1; i >= 0; --i) {
        for (int j = 0; j < img->width; ++j) {
            uint8_t colors[3] = {img->data[i][j].blue, img->data[i][j].green, img->data[i][j].red};
            fwrite(colors, sizeof(uint8_t), 3, file);
        }
    }

    fclose(file);
}

// Négatif
void bmp24_negative(t_bmp24 *img) {
    for (int y = 0; y < img->height; ++y) {
        for (int x = 0; x < img->width; ++x) {
            img->data[y][x].red = 255 - img->data[y][x].red;
            img->data[y][x].green = 255 - img->data[y][x].green;
            img->data[y][x].blue = 255 - img->data[y][x].blue;
        }
    }
}

// Conversion en niveaux de gris
void bmp24_grayscale(t_bmp24 *img) {
    for (int y = 0; y < img->height; ++y) {
        for (int x = 0; x < img->width; ++x) {
            uint8_t gray = (uint8_t)(((int)img->data[y][x].red + img->data[y][x].green + img->data[y][x].blue) / 3);
            img->data[y][x].red = gray;
            img->data[y][x].green = gray;
            img->data[y][x].blue = gray;
        }
    }
}

// Ajustement de la luminosité
void bmp24_brightness(t_bmp24 *img, int value) {
    for (int y = 0; y < img->height; ++y) {
        for (int x = 0; x < img->width; ++x) {
            int r = img->data[y][x].red + value;
            int g = img->data[y][x].green + value;
            int b = img->data[y][x].blue + value;
            img->data[y][x].red = (r > 255) ? 255 : (r < 0) ? 0 : r;
            img->data[y][x].green = (g > 255) ? 255 : (g < 0) ? 0 : g;
            img->data[y][x].blue = (b > 255) ? 255 : (b < 0) ? 0 : b;
        }
    }
}
