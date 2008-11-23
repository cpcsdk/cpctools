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
   Fichier  :  c_inifile.h

   Auteur   :  HECHT Franck - (c) 2006
   Mail     :  franhec@gmail.com
   Creation :  22/05/2006 - 10:31

   But      :  Gestion de fichiers de configuration.
   --------------------------------------------------------------------------
   Dernieres modifications:
   -------------------------------------------------------------------------- */


#ifndef H_HF_C_INIFILE_220520061031
#define H_HF_C_INIFILE_220520061031


#include "c_bool.h"


/* macros =================================================================== */
/* constants ================================================================ */

/* --------------------------------------------------------------------------
   C_Inifile_error
   --------------------------------------------------------------------------
   Constantes d'erreurs du module.
   -------------------------------------------------------------------------- */
typedef enum
{
   C_INIFILE_NO_ERROR,           /* Pas d'erreurs.                            */
   C_INIFILE_MODULE_INIT_ERR,    /* Module deja initialise.                   */
   C_INIFILE_MODULE_NO_INIT_ERR, /* Module non initialise.                    */
   C_INIFILE_OPEN_ERR,           /* Erreur d'ouverture du fichier.            */
   C_INIFILE_READ_ERR,           /* Erreur de lecture du fichier.             */
   C_INIFILE_ARG_ERR,            /* Arguments invalides ou non fournis.       */
   C_INIFILE_DYN_ALLOC_ERR,      /* Erreur d'allocation dynamique.            */
   C_INIFILE_NO_SECTION_ERR,     /* La section n'a pas ete trouvee.           */
   C_INIFILE_NO_KEY_ERR,         /* La cle n'a pas ete trouvee.               */
   C_INIFILE_CONVERSION_ERR,     /* Erreur durant la conversion de la donnee. */

   NB_INIFILE_ERROR
}
C_Inifile_error;


/* types ==================================================================== */
/* structures =============================================================== */
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
C_Bool c_inifile_init (char const * const s_filename, C_Inifile_error * p_err);


/* --------------------------------------------------------------------------
   c_inifile_close
   --------------------------------------------------------------------------
   Fermer le module.
   --------------------------------------------------------------------------
   E:
   S:
   -------------------------------------------------------------------------- */
void c_inifile_close (void);


/* --------------------------------------------------------------------------
   c_inifile_get_init
   --------------------------------------------------------------------------
   Determine si le module est charge ou non.
   --------------------------------------------------------------------------
   E:
   S: true  = Le module est charge.
      false = Le module n'est pas charge.
   -------------------------------------------------------------------------- */
C_Bool c_inifile_get_init (void);


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
                             C_Inifile_error * p_err);


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
                           C_Inifile_error * p_err);


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
                                     C_Inifile_error * p_err);


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
                         C_Inifile_error * p_err);


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
                                   C_Inifile_error * p_err);


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
                              C_Inifile_error * p_err);


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
                             C_Inifile_error * p_err);


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
                           C_Inifile_error * p_err);


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
                             C_Inifile_error * p_err);


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
                              C_Inifile_error * p_err);


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
                               C_Inifile_error * p_err);


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
                           C_Inifile_error * p_err);


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
                            C_Inifile_error * p_err);


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
                              C_Inifile_error * p_err);


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
                             C_Inifile_error * p_err);


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
                            C_Inifile_error * p_err);


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
                         C_Inifile_error * p_err);


/* public variables ========================================================= */


#endif /* H_HF_C_INIFILE_220520061031 */


/* --------------------------------------------------------------------------
   File generated by: HF_Gen 1.0 - HECHT Franck (c) 2006
   -------------------------------------------------------------------------- */
