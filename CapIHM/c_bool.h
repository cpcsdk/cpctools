/******************************************************************************
 *   Copyright (C) 2005-2006 by HECHT Franck                                  *
 *   franhec@gmail.com                                                        *
 *                                                                            *
 *   This program is free software; you can redistribute it and/or modify     *
 *   it under the terms of the GNU General Public License as published by     *
 *   the Free Software Foundation; either version 2 of the License, or        *
 *   (at your option) any later version.                                      *
 *                                                                            *
 *   This program is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *   GNU General Public License for more details.                             *
 *                                                                            *
 *   You should have received a copy of the GNU General Public License        *
 *   along with this program; if not, write to the                            *
 *   Free Software Foundation, Inc.,                                          *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                *
 ******************************************************************************/

/* Informations -------------------------------------------------------------
   
   Auteur            :  HECHT Franck (c) 2006
   Module            :  C_Bool
   Fichier           :  c_bool.h
   Date de creation  :  14/01/2006 - 14:21
   
   Commentaires      :  Type booleen personnel.
   
   -------------------------------------------------------------------------- */


#ifndef H_HF_C_BOOL_140120061421
#define H_HF_C_BOOL_140120061421


/* Valeurs booleennes pour le type: C_Bool. */
#ifndef true
#  define true    1
#endif
#ifndef false
#  define false   0
#endif
#ifndef True
#  define True    1
#endif
#ifndef False
#  define False   0
#endif
#ifndef C_True
#  define C_True  1
#endif
#ifndef C_False
#  define C_False 0
#endif
#ifndef TRUE
#  define TRUE    1
#endif
#ifndef FALSE
#  define FALSE   0
#endif


/* --------------------------------------------------------------------------
   C_Bool
   --------------------------------------------------------------------------
   Type boolean personnel.
   -------------------------------------------------------------------------- */
typedef unsigned char C_Bool;


#endif /* H_HF_C_BOOL_140120061421 */
