/* GFX2CRTC - raw2crtc.c 
 * CloudStrife - 20080921
 * DiffusÃ© sous licence libre CeCILL v2
 * Voire LICENCE
 */

#include <stdio.h>
#include <stdlib.h>
#include "libraw2crtc.h"

int main(int argc, char **argv)
{
  FILE *inFile, *outFile;
  unsigned char *inBuffer, *outBuffer;
  unsigned long inSize;
  unsigned long outSize;

  unsigned char r1, r6, r9;
  unsigned short width;
  unsigned short height;
  unsigned char mode;

  if((argc != 6) && (argc != 7))
  {
    printf("Utilisation : %s input_filename output_filename width height mode [registre9]\n",argv[0]);
    printf("Exemple : Convertir une image en 176 sur 224 en mode 0 :\n");
    printf("%s image.raw image.scr 176 224 0\n",argv[0]);
    printf("Exemple : Convertir une image en 256 sur 128 en mode 1 avec R9 = 3 :\n");
    printf("%s image.raw image.scr 256 128 1 3\n",argv[0]);
    exit(0);
  }

  inFile = fopen(argv[1],"rb");

  sscanf(argv[3],"%hud",&width);
  sscanf(argv[4],"%hud",&height);
  sscanf(argv[5],"%hhud",&mode);
  if(mode > 3) printf("mode doit être compris entre 0 et 3");
  mode = mode & 3;
  if(argc == 7) 
  {sscanf(argv[6],"%hhud",&r9);}
  else
  {r9 = 7;}

  if (inFile == NULL)
  {
    printf("Fichier Inexistant\n");
    exit(1);
  }

  fseek(inFile, 0, SEEK_END);
  inSize = ftell(inFile);
  rewind(inFile);

  if (inSize != (unsigned long)(width*height))
  {
    printf("Attention ! Mauvaise taille du fichier d'entré\n");
  }

  inBuffer = (unsigned char*)malloc(inSize);
  if (inBuffer == NULL)
  {
    printf("Allocation inBuffer raté\n");
    exit(3);
  }

  fread(inBuffer, 1, inSize, inFile);
  fclose(inFile);

  outBuffer = raw2crtc(inBuffer, width, height, mode, r9, &outSize, &r1, 0, 0, &r6);

  printf("Taille de l'écran de sortie : %lu\n",outSize);
  printf("Mode = %d  Largeur = %d  Hauteur = %d  R1 = %d  R9 = %d R6 = %d\n",mode,width,height,r1,r9,r6);

  outFile = fopen(argv[2], "wb");
  fwrite(outBuffer, 1, outSize, outFile);
  fclose(outFile);

  free(inBuffer);
  inBuffer = NULL;

  free(outBuffer);
  outBuffer = NULL;

  return 0;
}
