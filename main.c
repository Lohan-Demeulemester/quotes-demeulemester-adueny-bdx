// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp8.h"
#include "bmp24.h"
#include "bmp8.c"
#include "bmp24.c"

void afficherMenuPrincipal() {
    printf("\n=== Menu Principal ===\n");
    printf("1. Charger une image 8 bits\n");
    printf("2. Charger une image 24 bits\n");
    printf("3. Quitter\n");
    printf("Votre choix : ");
}

void afficherMenuBMP8() {
    printf("\n--- Menu Image 8 bits ---\n");
    printf("1. Afficher les infos\n");
    printf("2. Appliquer un négatif\n");
    printf("3. Modifier la luminosité\n");
    printf("4. Appliquer un seuillage (binarisation)\n");
    printf("5. Sauvegarder l'image\n");
    printf("6. Retour\n");
    printf("Votre choix : ");
}

void afficherMenuBMP24() {
    printf("\n--- Menu Image 24 bits ---\n");
    printf("1. Afficher les infos\n");
    printf("2. Appliquer un négatif\n");
    printf("3. Convertir en niveaux de gris\n");
    printf("4. Modifier la luminosité\n");
    printf("5. Sauvegarder l'image\n");
    printf("6. Retour\n");
    printf("Votre choix : ");
}

int main() {
    int choixPrincipal = 0;
    t_bmp8 *image8 = NULL;
    t_bmp24 *image24 = NULL;
    char filename[256];
    int sousChoix, value, threshold;

    do {
        afficherMenuPrincipal();
        scanf("%d", &choixPrincipal);
        getchar(); // Pour consommer le '\n'

        switch (choixPrincipal) {
            case 1: // Charger 8 bits
                if (image8) bmp8_free(image8);
                printf("Entrez le chemin du fichier BMP 8 bits : ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = 0; // Supprimer \n
                image8 = bmp8_loadImage(filename);
                if (!image8) break;

                do {
                    afficherMenuBMP8();
                    scanf("%d", &sousChoix);
                    getchar();

                    switch (sousChoix) {
                        case 1:
                            bmp8_printInfo(image8);
                            break;
                        case 2:
                            bmp8_negative(image8);
                            printf("Négatif appliqué.\n");
                            break;
                        case 3:
                            printf("Entrez la valeur de luminosité (+ ou -) : ");
                            scanf("%d", &value);
                            bmp8_brightness(image8, value);
                            printf("Luminosité modifiée.\n");
                            break;
                        case 4:
                            printf("Entrez le seuil (0-255) : ");
                            scanf("%d", &threshold);
                            bmp8_threshold(image8, threshold);
                            printf("Seuillage appliqué.\n");
                            break;
                        case 5:
                            printf("Nom du fichier de sauvegarde : ");
                            getchar();
                            fgets(filename, sizeof(filename), stdin);
                            filename[strcspn(filename, "\n")] = 0;
                            bmp8_saveImage(filename, image8);
                            printf("Image sauvegardée.\n");
                            break;
                        case 6:
                            printf("Retour au menu principal.\n");
                            break;
                        default:
                            printf("Option invalide.\n");
                    }
                } while (sousChoix != 6);

                break;

            case 2: // Charger 24 bits
                if (image24) bmp24_free(image24);
                printf("Entrez le chemin du fichier BMP 24 bits : ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = 0;
                image24 = bmp24_loadImage(filename);
                if (!image24) break;

                do {
                    afficherMenuBMP24();
                    scanf("%d", &sousChoix);
                    getchar();

                    switch (sousChoix) {
                        case 1:
                            printf("\n--- Informations sur l'image ---\n");
                            printf("Largeur : %d pixels\n", image24->width);
                            printf("Hauteur : %d pixels\n", image24->height);
                            printf("Profondeur de couleur : %d bits\n", image24->colorDepth);
                            break;
                        case 2:
                            bmp24_negative(image24);
                            printf("Négatif appliqué.\n");
                            break;
                        case 3:
                            bmp24_grayscale(image24);
                            printf("Image convertie en niveaux de gris.\n");
                            break;
                        case 4:
                            printf("Entrez la valeur de luminosité (+ ou -) : ");
                            scanf("%d", &value);
                            bmp24_brightness(image24, value);
                            printf("Luminosité modifiée.\n");
                            break;
                        case 5:
                            printf("Nom du fichier de sauvegarde : ");
                            getchar();
                            fgets(filename, sizeof(filename), stdin);
                            filename[strcspn(filename, "\n")] = 0;
                            bmp24_saveImage(filename, image24);
                            printf("Image sauvegardée.\n");
                            break;
                        case 6:
                            printf("Retour au menu principal.\n");
                            break;
                        default:
                            printf("Option invalide.\n");
                    }
                } while (sousChoix != 6);

                break;

            case 3:
                printf("Fermeture du programme.\n");
                break;

            default:
                printf("Option invalide.\n");
        }

    } while (choixPrincipal != 3);

    // Nettoyage mémoire
    if (image8) bmp8_free(image8);
    if (image24) bmp24_free(image24);

    return 0;
}
