/*
 * Copyright (C) 2006 by HECHT Franck
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/* --------------------------------------------------------------------------

               Gestion de fichiers de configuration: C_IniFile
                                 Version 3.0

   --------------------------------------------------------------------------
   Fichier  :  c_inifile.c

   Auteur   :  HECHT Franck - (c) 2006
   Mail     :  franhec@gmail.com
   Creation :  22/05/2006 - 10:31

   But      :  Gestion de fichiers de configuration.
   --------------------------------------------------------------------------
   Dernieres modifications:
   -------------------------------------------------------------------------- */


#include "c_inifile.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>


/* macros =================================================================== */
/* constants ================================================================ */

/*
 * Nom du fichier de configuration temporaire.
 */
#define TMP_CONF_FILENAME "__tmp__.ini"


/* types ==================================================================== */
/* structures =============================================================== */
/* private variables ======================================================== */

/*
 * Variable contenant le nom du fichier de confiruation.
 */
char * s_conf_filename = NULL;


/* private functions ======================================================== */

/* --------------------------------------------------------------------------
   c_inifile_str_dup
   --------------------------------------------------------------------------
   Copie d'une chaine de caracteres par allocation dynamique.
   --------------------------------------------------------------------------
   E: Position du caractere a changer de casse.
   S: Pointeur sur la copie de la chaine.
      NULL en cas d'erreur.
   -------------------------------------------------------------------------- */
static char * c_inifile_str_dup (char const * const s_str)
{
   char *s_dup = NULL;


   if (s_str != NULL)
   {
      /* Calcul de la longueur de la chaine et allocation. */
      size_t size = strlen (s_str) + 1;
      s_dup = (char*)malloc (size);

      if (s_dup != NULL)
      {
         /* Copie de la chaine. */
         memcpy (s_dup, s_str, size);
      }
   }


   return s_dup;
}


/* --------------------------------------------------------------------------
   c_inifile_get
   --------------------------------------------------------------------------
   Recherche d'une valeur.
   --------------------------------------------------------------------------
   E: Nom de la section ou chercher.
   E: Nom de la cle dans la section.
   E: Adresse d'une variable d'erreur de type: C_Inifile_error.
      L'argument peut etre NULL.
   S: Chaine de caracteres contenant la valeur demandee.
      En case d'erreur la fonction renvoie NULL.

      En cas d'erreur, si l'adresse d'une variable d'erreur a ete fournie,
      la variable peut prendre l'une des valeurs suivantes:

      - C_INIFILE_MODULE_NO_INIT_ERR
      - C_INIFILE_ARG_ERR
      - C_INIFILE_OPEN_ERR
      - C_INIFILE_READ_ERR
      - C_INIFILE_DYN_ALLOC_ERR
      - C_INIFILE_NO_SECTION_ERR
      - C_INIFILE_NO_KEY_ERR
   -------------------------------------------------------------------------- */
static char * c_inifile_get (char const * const s_section,
                             char const * const s_key,
                             C_Inifile_error * p_err)
{
   FILE     *  p_file      = NULL;
   char     *  s_ret       = NULL;
   char     *  p_tmp       = NULL;
   char        s_line        [BUFSIZ];
   C_Bool      b_section   = false;
   C_Bool      b_key       = false;


   /*
    * Verification de l'initialisation du module.
    */
   if (! s_conf_filename)
   {
      /*
       * Erreur: module non initialise.
       */
      if (p_err)
      {
         *p_err = C_INIFILE_MODULE_NO_INIT_ERR;
      }

      return NULL;
   }


   /*
    * Verification de la validite des arguments.
    */
   if (! s_section || ! s_key)
   {
      /*
       * Erreur: arguments invalides ou non fournits.
       */
      if (p_err)
      {
         *p_err = C_INIFILE_ARG_ERR;
      }

      return NULL;
   }


   /*
    * Ouverture du fichier de configuration.
    */
   p_file = fopen (s_conf_filename, "r");

   if (! p_file)
   {
      /*
       * Erreur: impossible d'ouvrir le fichier.
       */
      if (p_err)
      {
         *p_err = C_INIFILE_OPEN_ERR;
      }

      return NULL;
   }


   /*
    * Parcours du fichier.
    */
   while (fgets (s_line, BUFSIZ, p_file))
   {
      /*
       * Si la ligne est un commentaire on passe a l'iteration suivante.
       */
      if (s_line [0] == '#'  ||
          s_line [0] == '\n' ||
          s_line [0] == '\r' ||
          s_line [0] == '\0' ||
          s_line [0] == ' ')
      {
         continue;
      }


      /*
       * On enleve le saut de ligne.
       */
      {
         char * p = strchr (s_line, '\n');
	 char* p2 = strchr(s_line,'\r');
	 if (p2!=NULL)p=p2;

         if (p)
         {
            *p = 0;
         }
      }


      /*
       * Recherche de la section.
       */
      if (! b_section)
      {
         char * p = strstr (s_line, s_section);

         if (p)
         {
            /*
             * La section a ete trouvee.
             */
            b_section = true;
         }
      }
      else
      {
         /*
          * Si le premier caractere de la ligne dans la recherche de la cle
          * est un crochet de debut de section, c'est qu'on est plus dans la
          * section trouvee donc, la cle n'existe pas dans cette section.
          */
         if (s_line [0] == '[')
         {
            /*
             * On sort de la boucle.
             */
            break;
         }


         /*
          * Recherche de la cle.
          */
         p_tmp = strtok (s_line, "=");

         if (p_tmp)
         {
            /*
             * Si la comparaison reussie, on a trouve la cle.
             */
            if (strcmp (p_tmp, s_key) == 0)
            {
               p_tmp = strtok (NULL, "=");

               if (p_tmp)
               {
                  s_ret = (char*)malloc (strlen (p_tmp) + 1);

                  if (! s_ret)
                  {
                     /*
                      * Erreur: impossible d'allouer l'espace requis.
                      */
                     if (p_err)
                     {
                        *p_err = C_INIFILE_DYN_ALLOC_ERR;
                     }
                  }
                  else
                  {
                     strcpy (s_ret, p_tmp);

                     if (p_err)
                     {
                        *p_err = C_INIFILE_NO_ERROR;
                     }

                     b_key = true;
                  }
               }
            }
         }
      }
   }


   /*
    * Test si une erreur s'est produite avant la fin du fichier.
    */
   if (ferror (p_file))
   {
      /*
       * Erreur: erreur de lecture du fichier.
       */
      if (p_err)
      {
         *p_err = C_INIFILE_READ_ERR;
      }
   }


   /*
    * On verifie que tout s'est bien passe.
    */
   if (b_section == false)
   {
      if (p_err)
      {
         *p_err = C_INIFILE_NO_SECTION_ERR;
      }
   }

   if (b_key == false)
   {
      if (p_err)
      {
         *p_err = C_INIFILE_NO_KEY_ERR;
      }
   }


   /*
    * Fermeture du fichier.
    */
   if (p_file)
   {
      fclose (p_file);
   }


   return s_ret;
}


/* --------------------------------------------------------------------------
   c_inifile_set
   --------------------------------------------------------------------------
   Enregistrement de donnees.
   --------------------------------------------------------------------------
   E: Nom de la section ou enregistrer.
      Si la section n'existe pas elle est cree.
   E: Nom de la cle dans la section.
      Si la cle n'existe pas elle est cree sinon la veleur sera remplacee.
   E: Valeur a enregistrer.
   E: Adresse d'une variable d'erreur de type: C_Inifile_error.
      L'argument peut etre NULL.
   S: true  = enregistrement effectue avec succes.
      false = erreur.

      En cas d'erreur, si l'adresse d'une variable d'erreur a ete fournie,
      la variable peut prendre l'une des valeurs suivantes:

      - C_INIFILE_MODULE_NO_INIT_ERR
      - C_INIFILE_ARG_ERR
      - C_INIFILE_OPEN_ERR
      - C_INIFILE_READ_ERR
   -------------------------------------------------------------------------- */
C_Bool c_inifile_set (char const * const s_section,
                      char const * const s_key,
                      char const * const s_value,
                      C_Inifile_error * p_err)
{
   FILE     *  p_src       = NULL;
   FILE     *  p_dest      = NULL;
   char     *  p_tmp       = NULL;
   char        s_line        [BUFSIZ];
   char     *  s_tmp_line  = NULL;
   C_Bool      b_section   = false;
   C_Bool      b_key       = false;
   C_Bool      b_ret       = false;
   C_Bool      b_copy      = false;
   int         count       = 0;


   /*
    * Verification de l'initialisation du module.
    */
   if (! s_conf_filename)
   {
      /*
       * Erreur: module non initialise.
       */
      if (p_err)
      {
         *p_err = C_INIFILE_MODULE_NO_INIT_ERR;
      }

      return false;
   }


   /*
    * Verification de la validite des arguments.
    */
   if (! s_section || ! s_key || ! s_value)
   {
      /*
       * Erreur: arguments invalides ou non fournits.
       */
      if (p_err)
      {
         *p_err = C_INIFILE_ARG_ERR;
      }

      return false;
   }


   /*
    * Ouverture du fichier de configuration source.
    */
   p_src = fopen (s_conf_filename, "r");

   if (! p_src)
   {
      /*
       * Erreur: impossible d'ouvrir le fichier.
       */
      if (p_err)
      {
         *p_err = C_INIFILE_OPEN_ERR;
      }

      return false;
   }


   /*
    * Creation et ouverture d'un fichier de configuration temporaire.
    */
   p_dest = fopen (TMP_CONF_FILENAME, "w");

   if (! p_dest)
   {
      /*
       * Erreur: impossible d'ouvrir le fichier temporaire.
       */
      if (p_err)
      {
         *p_err = C_INIFILE_OPEN_ERR;
      }

	  fclose(p_src);
      return false;
   }


   /*
    * Parcours du fichier.
    */
   while (fgets (s_line, BUFSIZ, p_src))
   {
      /*
       * Si la variable 'count' reste sur zero a la fin de boucle,
       * cela signifie que le fichier est vide.
       */
      count++;


      /*
       * Si la variable 'b_copy' est sur 'true', cela signifie que
       * l'enregistrement a ete fait et qu'il faut simplement recopier
       * le restant du fichier source dans le fichier temporaire sans
       * faire aucune autre modification.
       */
      if (b_copy)
      {
         fprintf (p_dest, "%s", s_line);
         continue;
      }


      /*
       * Si la ligne est un commentaire on copie la ligne dans le fichier
       * temporaire sans modifications.
       */
      if (s_line [0] == '#')
      {
         fprintf (p_dest, "%s", s_line);
         continue;
      }


      /*
       * On enleve le saut de ligne.
       */
      {
         char * p = strchr (s_line, '\n');
	 char* p2 = strchr(p,'\r');
	 if (p2!=NULL)p=p2;

         if (p)
         {
            *p = 0;
         }
      }


      /*
       * Recherche de la section.
       */
      if (! b_section)
      {
         fprintf (p_dest, "%s\n", s_line);

         if (s_line [0] == '[')
         {
            s_tmp_line = c_inifile_str_dup (s_line);

            p_tmp = strtok (s_tmp_line, "[]");

            if (p_tmp)
            {
               if (strcmp (p_tmp, s_section) == 0)
               {
                  /*
                   * La section a ete trouvee.
                   */
                  free (s_tmp_line);
                  b_section = true;

                  continue;
               }
            }

            free (s_tmp_line);
         }
      }
      else
      {
         /*
          * Si le premier caractere de la ligne dans la recherche de la cle
          * est un saut de ligne, c'est qu'on est a la fin de la section
          * trouvee donc, la cle n'existe pas dans cette section.
          *
          * On cree la cle avec sa valeur.
          */
         if (s_line [0] == '\0')
         {
            fprintf (p_dest, "%s=%s\n\n", s_key, s_value);

            b_copy = true;
            b_key = true;
            b_ret = true;

            continue;
         }


         /*
          * Recherche de la cle.
          */
         s_tmp_line = c_inifile_str_dup (s_line);
         p_tmp = strtok (s_tmp_line, "=");

         if (p_tmp)
         {
            /*
             * Si la comparaison reussie, on a trouve la cle.
             */
            if (strcmp (p_tmp, s_key) == 0)
            {
               fprintf (p_dest, "%s=%s\n", s_key, s_value);

               b_copy = true;
               b_key = true;
               b_ret = true;

               free (s_tmp_line);

               continue;
            }
            else
            {
               fprintf (p_dest, "%s\n", s_line);
            }
         }

         free (s_tmp_line);
      }
   }


   /*
    * Test si une erreur s'est produite avant la fin du fichier.
    */
   if (ferror (p_src))
   {
      /*
       * Erreur: erreur de lecture du fichier.
       */
      if (p_err)
      {
         *p_err = C_INIFILE_READ_ERR;
      }
   }


   /*
    * Si aucune section n'a ete trouvee ou si le fichier de configuration
    * source est vide, on cree une nouvelle section d'apres le nom passe en
    * parametre puis on cree la cle et sa valeur.
    */
   if (! b_section || count == 0)
   {
      fprintf (p_dest, "\n[%s]\n%s=%s\n", s_section, s_key, s_value);
      b_ret = true;
   }
   else if (feof (p_src) && ! b_key && b_section == true)
   {
      fprintf (p_dest, "%s=%s\n", s_key, s_value);
      b_ret = true;
   }


   /*
    * Fermeture des fichiers.
    */
   if (p_src)
   {
      fclose (p_src);
   }

   if (p_dest)
   {
      fclose (p_dest);
   }


   /*
    * Finalisation des fichiers.
    */
   remove (s_conf_filename);
   rename (TMP_CONF_FILENAME, s_conf_filename);


   return b_ret;
}


/* internal public functions ================================================ */

/* --------------------------------------------------------------------------
   c_inifile_init
   --------------------------------------------------------------------------
   Initialise le module.
   --------------------------------------------------------------------------
   E: Nom du fichier de configuration.
      Si le fichier n'existe pas, il est cree.
   E: Adresse d'une variable d'erreur de type: C_Inifile_error.
      L'argument peut etre NULL.
   S: true  : Initialisation reussie.
      false : Echec.

      En cas d'echec d'initialisation, si l'adresse d'une variable d'erreur
      a ete fournie, la variable peut prendre l'une des valeurs suivantes:

      - C_INIFILE_DYN_ALLOC_ERR
      - C_INIFILE_MODULE_INIT_ERR
      - C_INIFILE_ARG_ERR
   -------------------------------------------------------------------------- */
C_Bool c_inifile_init (char const * const s_filename, C_Inifile_error * p_err)
{
   C_Bool      b_ret    = false;
   FILE     *  p_file   = NULL;


   /*
    * Test de l'argument pour la continuite.
    */
   if (s_filename)
   {
      if (! s_conf_filename)
      {
         /*
          * Allocation d'un espace pour contenir le chemin du fichier.
          */
         s_conf_filename = (char*)malloc (strlen (s_filename) + 1);

         if (s_conf_filename)
         {
            /*
             * Copie de la chaine
             */
            strcpy (s_conf_filename, s_filename);


            /*
             * Test d'ouverture du fichier.
             * Si l'ouverte echoue, on cree le fichier.
             */
            p_file = fopen (s_conf_filename, "r");

            if (! p_file)
            {
               p_file = fopen (s_conf_filename, "w");

               if (p_file)
               {
                  fclose (p_file);
               }
            }
            else
            {
               fclose (p_file);
            }


            /*
             * Initialisation reussie.
             */
            b_ret = true;

            if (p_err)
            {
               *p_err = C_INIFILE_NO_ERROR;
            }
         }
         else
         {
            /*
             * Erreur: impossible d'allouer l'espace requis.
             */
            b_ret = false;

            if (p_err)
            {
               *p_err = C_INIFILE_DYN_ALLOC_ERR;
            }
         }
      }
      else
      {
         /*
          * Erreur: le module est deja initialise.
          */
         b_ret = false;

         if (p_err)
         {
            *p_err = C_INIFILE_MODULE_INIT_ERR;
         }
      }
   }
   else
   {
      /*
       * Erreur: argument invalide.
       */
      b_ret = false;

      if (p_err)
      {
         *p_err = C_INIFILE_ARG_ERR;
      }
   }


   return b_ret;
}


/* --------------------------------------------------------------------------
   c_inifile_close
   --------------------------------------------------------------------------
   Fermer le module.
   --------------------------------------------------------------------------
   E:
   S:
   -------------------------------------------------------------------------- */
void c_inifile_close (void)
{
   if (s_conf_filename)
   {
      free (s_conf_filename);
      s_conf_filename = NULL;
   }
}


/* --------------------------------------------------------------------------
   c_inifile_get_init
   --------------------------------------------------------------------------
   Determine si le module est charge ou non.
   --------------------------------------------------------------------------
   E:
   S: true  = Le module est charge.
      false = Le module n'est pas charge.
   -------------------------------------------------------------------------- */
C_Bool c_inifile_get_init (void)
{
   if (s_conf_filename)
   {
      return true;
   }
   else
   {
      return false;
   }
}


/* --------------------------------------------------------------------------
   c_inifile_get_string
   --------------------------------------------------------------------------
   Recherche une donnee de type: chaine de caracteres.
   La chaine retournee par la fonction doit etre liberee avec free.
   --------------------------------------------------------------------------
   E: Section de la cle.
   E: Nom de la cle.
   E: Adresse d'une variable d'erreur de type: C_Inifile_error.
      L'argument peut etre NULL.
   S: Donnee de type: chaine avec zero de fin.
      NULL en cas d'erreurs.

      En cas d'erreur, si l'adresse d'une variable d'erreur a ete fournie,
      la variable peut prendre l'une des valeurs suivantes:

      - C_INIFILE_MODULE_NO_INIT_ERR
      - C_INIFILE_ARG_ERR
      - C_INIFILE_OPEN_ERR
      - C_INIFILE_READ_ERR
      - C_INIFILE_DYN_ALLOC_ERR
      - C_INIFILE_NO_SECTION_ERR
      - C_INIFILE_NO_KEY_ERR
   -------------------------------------------------------------------------- */
char * c_inifile_get_string (char const * const s_section,
                             char const * const s_key,
                             C_Inifile_error * p_err)
{
   char * s_str = NULL;


   /*
    * Recherche d'une donne de type chaine.
    */
   s_str = c_inifile_get (s_section, s_key, p_err);


   return s_str;
}


/* --------------------------------------------------------------------------
   c_inifile_get_integer
   --------------------------------------------------------------------------
   Recherche une donnee de type: entier (signed).
   --------------------------------------------------------------------------
   E: Section de la cle.
   E: Nom de la cle.
   E: Adresse d'une variable d'erreur de type: C_Inifile_error.
      L'argument peut etre NULL mais il est conseille d'en fournir une car
      c'est la seule facon de savoir si une erreur a ete provoquee ou non.
   S: Donnee de type: entier (signed).

      En cas d'erreur, si l'adresse d'une variable d'erreur a ete fournie,
      la variable peut prendre l'une des valeurs suivantes:

      - C_INIFILE_MODULE_NO_INIT_ERR
      - C_INIFILE_ARG_ERR
      - C_INIFILE_OPEN_ERR
      - C_INIFILE_READ_ERR
      - C_INIFILE_DYN_ALLOC_ERR
      - C_INIFILE_NO_SECTION_ERR
      - C_INIFILE_NO_KEY_ERR
      - C_INIFILE_CONVERSION_ERR
   -------------------------------------------------------------------------- */
int c_inifile_get_integer (char const * const s_section,
                           char const * const s_key,
                           C_Inifile_error * p_err)
{
   char  *  s_str = NULL;
   int      i_ret = 0;


   /*
    * Recupere la donnee trouvee sous forme de chaine.
    */
   s_str = c_inifile_get (s_section, s_key, p_err);

   if (! *p_err && s_str != NULL)
   {
      /*
       * Conversion de la donnee.
       */
      i_ret = (int) strtol (s_str, (char **)NULL, 10);

      if (i_ret == 0 && errno == ERANGE)
      {
         *p_err = C_INIFILE_CONVERSION_ERR;
      }


      /*
       * Liberation de la chaine.
       */
      free (s_str);
      s_str = NULL;
   }


   return i_ret;
}


/* --------------------------------------------------------------------------
   c_inifile_get_uinteger
   --------------------------------------------------------------------------
   Recherche une donnee de type: entier (unsigned).
   --------------------------------------------------------------------------
   E: Section de la cle.
   E: Nom de la cle.
   E: Adresse d'une variable d'erreur de type: C_Inifile_error.
      L'argument peut etre NULL mais il est conseille d'en fournir une car
      c'est la seule facon de savoir si une erreur a ete provoquee ou non.
   S: Donnee de type: entier (unsigned).

      En cas d'erreur, si l'adresse d'une variable d'erreur a ete fournie,
      la variable peut prendre l'une des valeurs suivantes:

      - C_INIFILE_MODULE_NO_INIT_ERR
      - C_INIFILE_ARG_ERR
      - C_INIFILE_OPEN_ERR
      - C_INIFILE_READ_ERR
      - C_INIFILE_DYN_ALLOC_ERR
      - C_INIFILE_NO_SECTION_ERR
      - C_INIFILE_NO_KEY_ERR
      - C_INIFILE_CONVERSION_ERR
   -------------------------------------------------------------------------- */
unsigned int c_inifile_get_uinteger (char const * const s_section,
                                     char const * const s_key,
                                     C_Inifile_error * p_err)
{
   char           *  s_str    = NULL;
   unsigned int      ui_ret   = 0;


   /*
    * Recupere la donnee trouvee sous forme de chaine.
    */
   s_str = c_inifile_get (s_section, s_key, p_err);

   if (! *p_err && s_str != NULL)
   {
      /*
       * Conversion de la donnee.
       */
      ui_ret = (unsigned int) strtol (s_str, (char **)NULL, 10);

      if (ui_ret == 0 && errno == ERANGE)
      {
         *p_err = C_INIFILE_CONVERSION_ERR;
      }


      /*
       * Liberation de la chaine.
       */
      free (s_str);
      s_str = NULL;
   }


   return ui_ret;
}


/* --------------------------------------------------------------------------
   c_inifile_get_long
   --------------------------------------------------------------------------
   Recherche une donnee de type: long (signed).
   --------------------------------------------------------------------------
   E: Section de la cle.
   E: Nom de la cle.
   E: Adresse d'une variable d'erreur de type: C_Inifile_error.
      L'argument peut etre NULL mais il est conseille d'en fournir une car
      c'est la seule facon de savoir si une erreur a ete provoquee ou non.
   S: Donnee de type: long (signed).

      En cas d'erreur, si l'adresse d'une variable d'erreur a ete fournie,
      la variable peut prendre l'une des valeurs suivantes:

      - C_INIFILE_MODULE_NO_INIT_ERR
      - C_INIFILE_ARG_ERR
      - C_INIFILE_OPEN_ERR
      - C_INIFILE_READ_ERR
      - C_INIFILE_DYN_ALLOC_ERR
      - C_INIFILE_NO_SECTION_ERR
      - C_INIFILE_NO_KEY_ERR
      - C_INIFILE_CONVERSION_ERR
   -------------------------------------------------------------------------- */
long c_inifile_get_long (char const * const s_section,
                         char const * const s_key,
                         C_Inifile_error * p_err)
{
   char  *  s_str = NULL;
   long     l_ret = 0;


   /*
    * Recupere la donnee trouvee sous forme de chaine.
    */
   s_str = c_inifile_get (s_section, s_key, p_err);

   if (! *p_err && s_str != NULL)
   {
      /*
       * Conversion de la donnee.
       */
      l_ret = strtol (s_str, (char **)NULL, 10);

      if (l_ret == 0 && errno == ERANGE)
      {
         *p_err = C_INIFILE_CONVERSION_ERR;
      }


      /*
       * Liberation de la chaine.
       */
      free (s_str);
      s_str = NULL;
   }


   return l_ret;
}


/* --------------------------------------------------------------------------
   c_inifile_get_ulong
   --------------------------------------------------------------------------
   Recherche une donnee de type: long (unsigned).
   --------------------------------------------------------------------------
   E: Section de la cle.
   E: Nom de la cle.
   E: Adresse d'une variable d'erreur de type: C_Inifile_error.
      L'argument peut etre NULL mais il est conseille d'en fournir une car
      c'est la seule facon de savoir si une erreur a ete provoquee ou non.
   S: Donnee de type: long (unsigned).

      En cas d'erreur, si l'adresse d'une variable d'erreur a ete fournie,
      la variable peut prendre l'une des valeurs suivantes:

      - C_INIFILE_MODULE_NO_INIT_ERR
      - C_INIFILE_ARG_ERR
      - C_INIFILE_OPEN_ERR
      - C_INIFILE_READ_ERR
      - C_INIFILE_DYN_ALLOC_ERR
      - C_INIFILE_NO_SECTION_ERR
      - C_INIFILE_NO_KEY_ERR
      - C_INIFILE_CONVERSION_ERR
   -------------------------------------------------------------------------- */
unsigned long c_inifile_get_ulong (char const * const s_section,
                                   char const * const s_key,
                                   C_Inifile_error * p_err)
{
   char           *  s_str    = NULL;
   unsigned long     ul_ret   = 0;


   /*
    * Recupere la donnee trouvee sous forme de chaine.
    */
   s_str = c_inifile_get (s_section, s_key, p_err);

   if (! *p_err && s_str != NULL)
   {
      /*
       * Conversion de la donnee.
       */
      ul_ret = strtol (s_str, (char **)NULL, 10);

      if (ul_ret == 0 && errno == ERANGE)
      {
         *p_err = C_INIFILE_CONVERSION_ERR;
      }


      /*
       * Liberation de la chaine.
       */
      free (s_str);
      s_str = NULL;
   }


   return ul_ret;
}


/* --------------------------------------------------------------------------
   c_inifile_get_boolean
   --------------------------------------------------------------------------
   Recherche une donnee de type: C_Bool.
   --------------------------------------------------------------------------
   E: Section de la cle.
   E: Nom de la cle.
   E: Adresse d'une variable d'erreur de type: C_Inifile_error.
      L'argument peut etre NULL mais il est conseille d'en fournir une car
      c'est la seule facon de savoir si une erreur a ete provoquee ou non.
   S: Donnee de type: C_Bool.

      En cas d'erreur, si l'adresse d'une variable d'erreur a ete fournie,
      la variable peut prendre l'une des valeurs suivantes:

      - C_INIFILE_MODULE_NO_INIT_ERR
      - C_INIFILE_ARG_ERR
      - C_INIFILE_OPEN_ERR
      - C_INIFILE_READ_ERR
      - C_INIFILE_DYN_ALLOC_ERR
      - C_INIFILE_NO_SECTION_ERR
      - C_INIFILE_NO_KEY_ERR
      - C_INIFILE_CONVERSION_ERR
   -------------------------------------------------------------------------- */
C_Bool c_inifile_get_boolean (char const * const s_section,
                              char const * const s_key,
                              C_Inifile_error * p_err)
{
   char     *  s_str = NULL;
   C_Bool      b_ret = false;


   /*
    * Recupere la donnee trouvee sous forme de chaine.
    */
   s_str = c_inifile_get (s_section, s_key, p_err);

   if (! *p_err && s_str != NULL)
   {
      /*
       * Conversion de la donnee.
       */
      b_ret = (C_Bool) strtol (s_str, (char **)NULL, 10);

      if (b_ret == false && errno == ERANGE)
      {
         *p_err = C_INIFILE_CONVERSION_ERR;
      }


      /*
       * Liberation de la chaine.
       */
      free (s_str);
      s_str = NULL;
   }


   return b_ret;
}


/* --------------------------------------------------------------------------
   c_inifile_get_double
   --------------------------------------------------------------------------
   Recherche une donnee de type: double.
   --------------------------------------------------------------------------
   E: Section de la cle.
   E: Nom de la cle.
   E: Adresse d'une variable d'erreur de type: C_Inifile_error.
      L'argument peut etre NULL mais il est conseille d'en fournir une car
      c'est la seule facon de savoir si une erreur a ete provoquee ou non.
   S: Donnee de type: double.

      En cas d'erreur, si l'adresse d'une variable d'erreur a ete fournie,
      la variable peut prendre l'une des valeurs suivantes:

      - C_INIFILE_MODULE_NO_INIT_ERR
      - C_INIFILE_ARG_ERR
      - C_INIFILE_OPEN_ERR
      - C_INIFILE_READ_ERR
      - C_INIFILE_DYN_ALLOC_ERR
      - C_INIFILE_NO_SECTION_ERR
      - C_INIFILE_NO_KEY_ERR
      - C_INIFILE_CONVERSION_ERR
   -------------------------------------------------------------------------- */
double c_inifile_get_double (char const * const s_section,
                             char const * const s_key,
                             C_Inifile_error * p_err)
{
   char     *  s_str = NULL;
   double      d_ret = 0;


   /*
    * Recupere la donnee trouvee sous forme de chaine.
    */
   s_str = c_inifile_get (s_section, s_key, p_err);

   if (! *p_err && s_str != NULL)
   {
      /*
       * Conversion de la donnee.
       */
      d_ret = (double) strtod (s_str, (char **)NULL);

      if (d_ret == 0 && errno == ERANGE)
      {
         *p_err = C_INIFILE_CONVERSION_ERR;
      }


      /*
       * Liberation de la chaine.
       */
      free (s_str);
      s_str = NULL;
   }


   return d_ret;
}


/* --------------------------------------------------------------------------
   c_inifile_get_float
   --------------------------------------------------------------------------
   Recherche une donnee de type: float.
   --------------------------------------------------------------------------
   E: Section de la cle.
   E: Nom de la cle.
   E: Adresse d'une variable d'erreur de type: C_Inifile_error.
      L'argument peut etre NULL mais il est conseille d'en fournir une car
      c'est la seule facon de savoir si une erreur a ete provoquee ou non.
   S: Donnee de type: float.

      En cas d'erreur, si l'adresse d'une variable d'erreur a ete fournie,
      la variable peut prendre l'une des valeurs suivantes:

      - C_INIFILE_MODULE_NO_INIT_ERR
      - C_INIFILE_ARG_ERR
      - C_INIFILE_OPEN_ERR
      - C_INIFILE_READ_ERR
      - C_INIFILE_DYN_ALLOC_ERR
      - C_INIFILE_NO_SECTION_ERR
      - C_INIFILE_NO_KEY_ERR
      - C_INIFILE_CONVERSION_ERR
   -------------------------------------------------------------------------- */
float c_inifile_get_float (char const * const s_section,
                           char const * const s_key,
                           C_Inifile_error * p_err)
{
   char     *  s_str = NULL;
   float       f_ret = 0;


   /*
    * Recupere la donnee trouvee sous forme de chaine.
    */
   s_str = c_inifile_get (s_section, s_key, p_err);

   if (! *p_err && s_str != NULL)
   {
      /*
       * Conversion de la donnee.
       */
      f_ret = (float) strtod (s_str, (char **)NULL);

      if (f_ret == 0 && errno == ERANGE)
      {
         *p_err = C_INIFILE_CONVERSION_ERR;
      }


      /*
       * Liberation de la chaine.
       */
      free (s_str);
      s_str = NULL;
   }


   return f_ret;
}


/* --------------------------------------------------------------------------
   c_inifile_set_string
   --------------------------------------------------------------------------
   Enregistrement d'une donne de type: chaine avec zero de fin.
   --------------------------------------------------------------------------
   E: Nom de la section ou enregistrer.
      Si la section n'existe pas elle est cree.
   E: Nom de la cle dans la section.
      Si la cle n'existe pas elle est cree sinon la veleur sera remplacee.
   E: Valeur a enregistrer.
   E: Adresse d'une variable d'erreur de type: C_Inifile_error.
      L'argument peut etre NULL.
   S: true  = enregistrement effectue avec succes.
      false = erreur.

      En cas d'erreur, si l'adresse d'une variable d'erreur a ete fournie,
      la variable peut prendre l'une des valeurs suivantes:

      - C_INIFILE_MODULE_NO_INIT_ERR
      - C_INIFILE_ARG_ERR
      - C_INIFILE_OPEN_ERR
      - C_INIFILE_READ_ERR
   -------------------------------------------------------------------------- */
C_Bool c_inifile_set_string (char const * const s_section,
                             char const * const s_key,
                             char const * const s_value,
                             C_Inifile_error * p_err)
{
   C_Bool b_ret = false;


   /*
    * Appel de la fonction d'enregistrement.
    */
   b_ret = c_inifile_set (s_section, s_key, s_value, p_err);


   return b_ret;
}


/* --------------------------------------------------------------------------
   c_inifile_set_integer
   --------------------------------------------------------------------------
   Enregistrement d'une donne de type: int (signed).
   --------------------------------------------------------------------------
   E: Nom de la section ou enregistrer.
      Si la section n'existe pas elle est cree.
   E: Nom de la cle dans la section.
      Si la cle n'existe pas elle est cree sinon la veleur sera remplacee.
   E: Valeur a enregistrer.
   E: Adresse d'une variable d'erreur de type: C_Inifile_error.
      L'argument peut etre NULL.
   S: true  = enregistrement effectue avec succes.
      false = erreur.

      En cas d'erreur, si l'adresse d'une variable d'erreur a ete fournie,
      la variable peut prendre l'une des valeurs suivantes:

      - C_INIFILE_MODULE_NO_INIT_ERR
      - C_INIFILE_ARG_ERR
      - C_INIFILE_OPEN_ERR
      - C_INIFILE_READ_ERR
   -------------------------------------------------------------------------- */
C_Bool c_inifile_set_integer (char const * const s_section,
                              char const * const s_key,
                              const int i_value,
                              C_Inifile_error * p_err)
{
   C_Bool   b_ret = false;
   char     s_buf [BUFSIZ];


   /*
    * Enregistrement de la nouvelle donnee.
    */
   sprintf (s_buf, "%d", i_value);
   b_ret = c_inifile_set (s_section, s_key, s_buf, p_err);


   return b_ret;
}


/* --------------------------------------------------------------------------
   c_inifile_set_uinteger
   --------------------------------------------------------------------------
   Enregistrement d'une donne de type: int (unsigned).
   --------------------------------------------------------------------------
   E: Nom de la section ou enregistrer.
      Si la section n'existe pas elle est cree.
   E: Nom de la cle dans la section.
      Si la cle n'existe pas elle est cree sinon la veleur sera remplacee.
   E: Valeur a enregistrer.
   E: Adresse d'une variable d'erreur de type: C_Inifile_error.
      L'argument peut etre NULL.
   S: true  = enregistrement effectue avec succes.
      false = erreur.

      En cas d'erreur, si l'adresse d'une variable d'erreur a ete fournie,
      la variable peut prendre l'une des valeurs suivantes:

      - C_INIFILE_MODULE_NO_INIT_ERR
      - C_INIFILE_ARG_ERR
      - C_INIFILE_OPEN_ERR
      - C_INIFILE_READ_ERR
   -------------------------------------------------------------------------- */
C_Bool c_inifile_set_uinteger (char const * const s_section,
                               char const * const s_key,
                               const int ui_value,
                               C_Inifile_error * p_err)
{
   C_Bool   b_ret = false;
   char     s_buf [BUFSIZ];


   /*
    * Enregistrement de la nouvelle donnee.
    */
   sprintf (s_buf, "%ud", ui_value);
   b_ret = c_inifile_set (s_section, s_key, s_buf, p_err);


   return b_ret;
}


/* --------------------------------------------------------------------------
   c_inifile_set_long
   --------------------------------------------------------------------------
   Enregistrement d'une donne de type: long (signed).
   --------------------------------------------------------------------------
   E: Nom de la section ou enregistrer.
      Si la section n'existe pas elle est cree.
   E: Nom de la cle dans la section.
      Si la cle n'existe pas elle est cree sinon la veleur sera remplacee.
   E: Valeur a enregistrer.
   E: Adresse d'une variable d'erreur de type: C_Inifile_error.
      L'argument peut etre NULL.
   S: true  = enregistrement effectue avec succes.
      false = erreur.

      En cas d'erreur, si l'adresse d'une variable d'erreur a ete fournie,
      la variable peut prendre l'une des valeurs suivantes:

      - C_INIFILE_MODULE_NO_INIT_ERR
      - C_INIFILE_ARG_ERR
      - C_INIFILE_OPEN_ERR
      - C_INIFILE_READ_ERR
   -------------------------------------------------------------------------- */
C_Bool c_inifile_set_long (char const * const s_section,
                           char const * const s_key,
                           const long l_value,
                           C_Inifile_error * p_err)
{
   C_Bool   b_ret = false;
   char     s_buf [BUFSIZ];


   /*
    * Enregistrement de la nouvelle donnee.
    */
   sprintf (s_buf, "%ld", l_value);
   b_ret = c_inifile_set (s_section, s_key, s_buf, p_err);


   return b_ret;
}


/* --------------------------------------------------------------------------
   c_inifile_set_ulong
   --------------------------------------------------------------------------
   Enregistrement d'une donne de type: long (unsigned).
   --------------------------------------------------------------------------
   E: Nom de la section ou enregistrer.
      Si la section n'existe pas elle est cree.
   E: Nom de la cle dans la section.
      Si la cle n'existe pas elle est cree sinon la veleur sera remplacee.
   E: Valeur a enregistrer.
   E: Adresse d'une variable d'erreur de type: C_Inifile_error.
      L'argument peut etre NULL.
   S: true  = enregistrement effectue avec succes.
      false = erreur.

      En cas d'erreur, si l'adresse d'une variable d'erreur a ete fournie,
      la variable peut prendre l'une des valeurs suivantes:

      - C_INIFILE_MODULE_NO_INIT_ERR
      - C_INIFILE_ARG_ERR
      - C_INIFILE_OPEN_ERR
      - C_INIFILE_READ_ERR
   -------------------------------------------------------------------------- */
C_Bool c_inifile_set_ulong (char const * const s_section,
                            char const * const s_key,
                            const long ul_value,
                            C_Inifile_error * p_err)
{
   C_Bool   b_ret = false;
   char     s_buf [BUFSIZ];


   /*
    * Enregistrement de la nouvelle donnee.
    */
   sprintf (s_buf, "%lu", ul_value);
   b_ret = c_inifile_set (s_section, s_key, s_buf, p_err);


   return b_ret;
}


/* --------------------------------------------------------------------------
   c_inifile_set_boolean
   --------------------------------------------------------------------------
   Enregistrement d'une donne de type: C_Bool
   --------------------------------------------------------------------------
   E: Nom de la section ou enregistrer.
      Si la section n'existe pas elle est cree.
   E: Nom de la cle dans la section.
      Si la cle n'existe pas elle est cree sinon la veleur sera remplacee.
   E: Valeur a enregistrer.
   E: Adresse d'une variable d'erreur de type: C_Inifile_error.
      L'argument peut etre NULL.
   S: true  = enregistrement effectue avec succes.
      false = erreur.

      En cas d'erreur, si l'adresse d'une variable d'erreur a ete fournie,
      la variable peut prendre l'une des valeurs suivantes:

      - C_INIFILE_MODULE_NO_INIT_ERR
      - C_INIFILE_ARG_ERR
      - C_INIFILE_OPEN_ERR
      - C_INIFILE_READ_ERR
   -------------------------------------------------------------------------- */
C_Bool c_inifile_set_boolean (char const * const s_section,
                              char const * const s_key,
                              const C_Bool b_value,
                              C_Inifile_error * p_err)
{
   C_Bool   b_ret = false;
   char     s_buf [BUFSIZ];


   /*
    * Enregistrement de la nouvelle donnee.
    */
   sprintf (s_buf, "%d", b_value);
   b_ret = c_inifile_set (s_section, s_key, s_buf, p_err);


   return b_ret;
}


/* --------------------------------------------------------------------------
   c_inifile_set_double
   --------------------------------------------------------------------------
   Enregistrement d'une donne de type: double
   --------------------------------------------------------------------------
   E: Nom de la section ou enregistrer.
      Si la section n'existe pas elle est cree.
   E: Nom de la cle dans la section.
      Si la cle n'existe pas elle est cree sinon la veleur sera remplacee.
   E: Valeur a enregistrer.
   E: Adresse d'une variable d'erreur de type: C_Inifile_error.
      L'argument peut etre NULL.
   S: true  = enregistrement effectue avec succes.
      false = erreur.

      En cas d'erreur, si l'adresse d'une variable d'erreur a ete fournie,
      la variable peut prendre l'une des valeurs suivantes:

      - C_INIFILE_MODULE_NO_INIT_ERR
      - C_INIFILE_ARG_ERR
      - C_INIFILE_OPEN_ERR
      - C_INIFILE_READ_ERR
   -------------------------------------------------------------------------- */
C_Bool c_inifile_set_double (char const * const s_section,
                             char const * const s_key,
                             const double d_value,
                             C_Inifile_error * p_err)
{
   C_Bool   b_ret = false;
   char     s_buf [BUFSIZ];


   /*
    * Enregistrement de la nouvelle donnee.
    */
   sprintf (s_buf, "%f", d_value);
   b_ret = c_inifile_set (s_section, s_key, s_buf, p_err);


   return b_ret;
}


/* --------------------------------------------------------------------------
   c_inifile_set_float
   --------------------------------------------------------------------------
   Enregistrement d'une donne de type: float
   --------------------------------------------------------------------------
   E: Nom de la section ou enregistrer.
      Si la section n'existe pas elle est cree.
   E: Nom de la cle dans la section.
      Si la cle n'existe pas elle est cree sinon la veleur sera remplacee.
   E: Valeur a enregistrer.
   E: Adresse d'une variable d'erreur de type: C_Inifile_error.
      L'argument peut etre NULL.
   S: true  = enregistrement effectue avec succes.
      false = erreur.

      En cas d'erreur, si l'adresse d'une variable d'erreur a ete fournie,
      la variable peut prendre l'une des valeurs suivantes:

      - C_INIFILE_MODULE_NO_INIT_ERR
      - C_INIFILE_ARG_ERR
      - C_INIFILE_OPEN_ERR
      - C_INIFILE_READ_ERR
   -------------------------------------------------------------------------- */
C_Bool c_inifile_set_float (char const * const s_section,
                            char const * const s_key,
                            const float f_value,
                            C_Inifile_error * p_err)
{
   C_Bool   b_ret = false;
   char     s_buf [BUFSIZ];


   /*
    * Enregistrement de la nouvelle donnee.
    */
   sprintf (s_buf, "%f", f_value);
   b_ret = c_inifile_set (s_section, s_key, s_buf, p_err);


   return b_ret;
}


/* --------------------------------------------------------------------------
   c_inifile_delete
   --------------------------------------------------------------------------
   Suppression d'une section ou d'une cle/valeur.
   --------------------------------------------------------------------------
   E: Nom de la section.
   E: Nom de la cle dans la section a supprimer.
      Si la cle n'est pas fournie, c'est la section entiere qui est supprimee.
   E: Adresse d'une variable d'erreur de type: C_Inifile_error.
      L'argument peut etre NULL.
   S: true  = suppression effectue avec succes.
      false = erreur.

      En cas d'erreur, si l'adresse d'une variable d'erreur a ete fournie,
      la variable peut prendre l'une des valeurs suivantes:

      - C_INIFILE_MODULE_NO_INIT_ERR
      - C_INIFILE_ARG_ERR
      - C_INIFILE_OPEN_ERR
      - C_INIFILE_READ_ERR
      - C_INIFILE_NO_SECTION_ERR
      - C_INIFILE_NO_KEY_ERR
   -------------------------------------------------------------------------- */
C_Bool c_inifile_delete (char const * const s_section,
                         char const * const s_key,
                         C_Inifile_error * p_err)
{
   FILE     *  p_src       = NULL;
   FILE     *  p_dest      = NULL;
   char     *  p_tmp       = NULL;
   char        s_line        [BUFSIZ];
   char     *  s_tmp_line  = NULL;
   C_Bool      b_section   = false;
   C_Bool      b_key       = false;
   C_Bool      b_ret       = false;
   C_Bool      b_copy      = false;


   /*
    * Initialisation de la variable d'erreurs.
    */
   if (p_err)
   {
      *p_err = C_INIFILE_NO_ERROR;
   }


   /*
    * Verification de l'initialisation du module.
    */
   if (! s_conf_filename)
   {
      /*
       * Erreur: module non initialise.
       */
      if (p_err)
      {
         *p_err = C_INIFILE_MODULE_NO_INIT_ERR;
      }

      return false;
   }


   /*
    * Verification de la validite des arguments.
    */
   if (! s_section)
   {
      /*
       * Erreur: arguments invalides ou non fournits.
       */
      if (p_err)
      {
         *p_err = C_INIFILE_ARG_ERR;
      }

      return false;
   }


   /*
    * Ouverture du fichier de configuration source.
    */
   p_src = fopen (s_conf_filename, "r");

   if (! p_src)
   {
      /*
       * Erreur: impossible d'ouvrir le fichier.
       */
      if (p_err)
      {
         *p_err = C_INIFILE_OPEN_ERR;
      }

      return false;
   }


   /*
    * Creation et ouverture d'un fichier de configuration temporaire.
    */
   p_dest = fopen (TMP_CONF_FILENAME, "w");

   if (! p_dest)
   {
      /*
       * Erreur: impossible d'ouvrir le fichier temporaire.
       */
      if (p_err)
      {
         *p_err = C_INIFILE_OPEN_ERR;
      }

	  fclose(p_src);
      return false;
   }


   /*
    * Parcours du fichier.
    */
   while (fgets (s_line, BUFSIZ, p_src))
   {
      /*
       * Si la variable 'b_copy' est sur 'true', cela signifie que
       * l'enregistrement a ete fait et qu'il faut simplement recopier
       * le restant du fichier source dans le fichier temporaire sans
       * faire aucune autre modification.
       */
      if (b_copy)
      {
         fprintf (p_dest, "%s", s_line);
         continue;
      }


      /*
       * Si la ligne est un commentaire on copie la ligne dans le fichier
       * temporaire sans modifications.
       */
      if (s_line [0] == '#')
      {
         fprintf (p_dest, "%s", s_line);
         continue;
      }


      /*
       * Recherche de la section.
       */
      if (! b_section)
      {
         s_tmp_line = c_inifile_str_dup (s_line);
         p_tmp = strtok (s_tmp_line, "[]");

         if (p_tmp)
         {
            if (strcmp (p_tmp, s_section) == 0)
            {
               if (s_key)
               {
                  fprintf (p_dest, "%s", s_line);
               }

               b_section = true;
            }
            else
            {
               fprintf (p_dest, "%s", s_line);
            }
         }

         free (s_tmp_line);
      }
      else
      {
         /*
          * Si la cle n'est pas fournie alors on ne copie pas les donnees
          * enregistrees dans la section trouvee.
          * Sinon on recherche la cle a supprimer.
          */
         if (! s_key)
         {
            b_key = true;

            if (s_line [0] == '\n')
            {
               b_key = true;
               b_copy = true;
               b_ret = true;

               continue;
            }
         }
         else
         {
            if (! b_key)
            {
               s_tmp_line = c_inifile_str_dup (s_line);
               p_tmp = strtok (s_tmp_line, "=");

               if (p_tmp)
               {
                  if (strcmp (p_tmp, s_key) == 0)
                  {
                     b_key = true;
                     b_copy = true;
                     b_ret = true;
                  }
                  else
                  {
                     fprintf (p_dest, "%s", s_line);
                  }
               }

               free (s_tmp_line);
            }
         }
      }
   }


   /*
    * Test si une erreur s'est produite avant la fin du fichier.
    */
   if (ferror (p_src))
   {
      /*
       * Erreur: erreur de lecture du fichier.
       */
      if (p_err)
      {
         *p_err = C_INIFILE_READ_ERR;
      }
   }


   /*
    * On verifie que tout s'est bien passe.
    */
   if (b_section == false)
   {
      if (p_err)
      {
         *p_err = C_INIFILE_NO_SECTION_ERR;
      }
   }

   if (b_key == false)
   {
      if (p_err)
      {
         *p_err = C_INIFILE_NO_KEY_ERR;
      }
   }


   /*
    * Fermeture des fichiers.
    */
   if (p_src)
   {
      fclose (p_src);
   }

   if (p_dest)
   {
      fclose (p_dest);
   }


   /*
    * Finalisation des fichiers.
    */
   remove (s_conf_filename);
   rename (TMP_CONF_FILENAME, s_conf_filename);


   return b_ret;
}


/* public variables ========================================================= */


/* --------------------------------------------------------------------------
   File generated by: HF_Gen 1.0 - HECHT Franck (c) 2006
   -------------------------------------------------------------------------- */
