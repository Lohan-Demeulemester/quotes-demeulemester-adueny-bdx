//
// Created by lohan on 17/03/2025.
//

#include "fonctions_lauris.h"
void bmp8_loadIImage(const char * filename, t_bmp8 * img)
{ t_bmp8 * bmp8 = bmp8_loadImage(filename);


  // Debut de fonction load
    t_bmp8 *readBMP8(const char *filename) {
        FILE *file = fopen(filename, "rb");
        if (!file) {
            perror("Erreur lors de l'ouverture du fichier");
            return NULL;
        }

        // lecture en-tete
        unsigned char header[54];
        if (fread(header, sizeof(unsigned char), 54, file) != 54) {
            perror("Erreur lors de la lecture de l'en-tête du fichier");
            fclose(file);
            return NULL;
        }

        // Vérif du format BMP ( je suis pas sur de celle là)
        if (header[0] != 'B' || header[1] != 'M') {
            fprintf(stderr, "Fichier non valide : ce n'est pas un fichier BMP.\n");
            fclose(file);
            }
            return NULL;










            // Debut de fonction save

            int l_image;
            int h_image;
            uint8_t *pixels;
            t_bmp8;

            int bmp8_saveImage(const char *filename, t_bmp8 *image) {
                FILE *file = fopen(filename, "wb");
                if (!file) {
                    fprintf(stderr, "Erreur : impossible d'ouvrir le fichier %s pour l'écriture.\n", filename);
                    return -1;
                    int width = image->l_image;
                    int h_image = image->h_image;
                    unsigned char *pixels = image->pixels;


     int largeur;
    int hauteur;
    unsigned char *pixels;
    t_bmp8;

void free_image(t_bmp8 *image) {
  if (image->pixels != NULL) {
    free(image->pixels);
    image->pixels = NULL;
    }
}