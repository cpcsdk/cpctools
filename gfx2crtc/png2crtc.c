/* GFX2CRTC - png2crtc.c
 * CloudStrife - 20080921
 * Diffusé sous licence libre CeCILL v2
 * Voire LICENCE
 */

#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "libraw2crtc.h"

#define ERROR 1

int main(int argc, char **argv)
{
  FILE *inFile, *outFile;
  unsigned char *inBuffer, *outBuffer;
  unsigned long outSize;

  unsigned char r1, r9, r12, r13;
  png_uint_32 width;
  png_uint_32 height;
  int bitdepth;
  int colorType;
  unsigned char mode;
  unsigned char forcemode = 0;
  unsigned char forcer12 = 0, forcer13 = 0;

  unsigned char header[8];
  unsigned char is_png;

  unsigned int y;
  png_structp png_ptr;
  png_infop info_ptr;
  png_infop end_info;

  png_bytep * ptrRow;

  if((argc != 3) && (argc != 4) && (argc != 5) && (argc != 6) && (argc != 7))
  {
    printf("Utilisation : %s input_filename output_filename [registre9] [mode] [r12] [r13]\n",argv[0]);
    exit(0);
  }

  inFile = fopen(argv[1],"rb");

  if(argc >= 4) 
  {sscanf(argv[3],"%hhud",&r9);}
  else
  {r9 = 7;}
  if(argc >= 5)
  {
    sscanf(argv[4],"%hhud",&mode);
    forcemode = 1;
    if(mode > 3) puts("mode doit être compris entre 0 et 3");
    mode = mode & 3;
  }

  // Registre r12 et r13 par défaut :
  r12 = 0x0C;
  r13 = 0x00;

  if(argc >= 6)
  {
    sscanf(argv[5],"%hhud",&r12);
    forcer12 = 1;
  }
  if(argc >= 7)
  {
    sscanf(argv[6],"%hhud",&r13);
    forcer13 = 1;
  }

  if (inFile == NULL)
  {
    printf("Fichier Inexistant\n");
    exit(1);
  }

  fread(header, 1, 8, inFile);
  is_png = !png_sig_cmp(header, 0, 8);
  if (!is_png)
  {
    printf("Ce n'est pas un png\n");
    exit(2);
  }

  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, (png_voidp)NULL, NULL, NULL);
  if (!png_ptr) return (ERROR);

  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
  {
    png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
    return (ERROR);
  }

  end_info = png_create_info_struct(png_ptr);
  if (!end_info)
  {
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
    return (ERROR);
  }

  if (setjmp(png_jmpbuf(png_ptr)))
  {
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    fclose(inFile);
    return (ERROR);
  }

  png_init_io(png_ptr, inFile);
  png_set_sig_bytes(png_ptr, 8);

  png_read_info(png_ptr, info_ptr);

  png_get_IHDR(png_ptr, info_ptr, &width, &height, &bitdepth, &colorType, NULL, NULL, NULL);

  if(!((colorType == PNG_COLOR_TYPE_GRAY) || (colorType == PNG_COLOR_TYPE_PALETTE)))
  {
    puts("Ce PNG n'est pas dans un format exploitable (niveaux de gris ou palette)");
    return (ERROR);
  }

  if(forcemode == 0)
  {
    switch(bitdepth)
    {
      case 1:
      {
        mode = 2;
        break;
      }
      case 2:
      {
        mode = 1;
        break;
      }
      case 4:
      {
        mode = 0;
        break;
      }
      default:
      {
        puts("Ce PNG n'est pas dans un format exploitable (bitdepth = 1, 2 ou 4)");
        return (ERROR);
      }
    }
  }

  png_set_packing(png_ptr); /* Convertir en mode 1 pixel par octets */
  png_read_update_info(png_ptr, info_ptr);

  inBuffer = (unsigned char*)malloc(width*height);
  if (inBuffer == NULL)
  {
    printf("Allocation inBuffer raté\n");
    exit(3);
  }

  ptrRow = (png_bytep*)malloc(sizeof(png_bytep)*height);
  for(y = 0; y < height; y++)
  {
    ptrRow[y] = (inBuffer + width*y);
  }

  png_read_image(png_ptr, ptrRow);

  outBuffer = raw2crtc(inBuffer, width, height, mode, r9, &outSize, &r1, r12, r13);

  printf("Taille de l'écran de sortie : %lu\n",outSize);
  printf("Mode = %d  Largeur = %d  Hauteur = %d  R1 = %d  R9 = %d\n",mode,(int)width,(int)height,r1,r9);

  outFile = fopen(argv[2], "wb");
  fwrite(outBuffer, 1, outSize, outFile);
  fclose(outFile);

  png_read_end(png_ptr, end_info);
  png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);

  free(inBuffer);
  inBuffer = NULL;

  free(outBuffer);
  outBuffer = NULL;

  return 0;
}
