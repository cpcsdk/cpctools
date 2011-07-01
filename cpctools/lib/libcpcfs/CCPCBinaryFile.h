/**
 * @class CCPCBinaryFile
 * Classe permettant la gestion d'un fichier binaire CPC
 * @author Thierry JOUIN
 * @version 1.1
 * @date 31/10/2001
 */

#include <iostream>
#include <fstream>

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "libdsk.h"
#include <errno.h>
#include <string.h>

#include "CError.h"

#include "CCPCFile.h"

#ifndef _CCPCBINARYFILE_HH_
#define _CCPCBINARYFILE_HH_

class CCPCBinaryFile:public CCPCFile
{
private:
  /// Fichier complet (header + contenu)
  unsigned char *_file;
  /// Header du fichier
  unsigned char *_header;
  /// Contenu du fichier
  unsigned char *_content;

protected:
  /// Affiche les infos sur le ficher
  virtual std::ostream& printInfo(std::ostream &io_os) const;

public:
  /// Constructeur
  CCPCBinaryFile();
  /// Destructeur
  ~CCPCBinaryFile();

  /// Renvoie le type de fichier
  virtual TFileType getType() const;

  /// Renvoie la taille du fichier (en octet)
  virtual unsigned int getSize() const;

  /// Renvoie l'adresse d'implementation du fichier
  unsigned int getAdress() const;
  /// Renvoie l'adresse d'execution du fichier
  unsigned int getExecute() const;
  /// Met a jour l'adresse d'implementation du fichier
  void setAdress(unsigned int i_adr);
  /// Met a jour l'adresse d'execution du fichier
  void setExecute(unsigned int i_exe);

  /// Creation d'un fichier a partir d'un buffer
  virtual void createFile(const void* i_buffer, const unsigned int i_size, bool i_createHeader = false);
  /// Lecture et creation d'un fichier a partir d'un fichier
  virtual void openFile(const std::string &i_filename,bool i_createHeader = false);
  /// Ecriture d'un fichier dans une stream
  virtual void saveFile(const std::string &i_filename,bool i_saveHeader = true) const;

  /// Renvoie la taille des donnees contenues dans le fichier (header eventuellement compris)
  virtual unsigned int getDatasSize() const;
  /// Renvoie un pointeur sur les donnees contenues dans le fichier
  virtual unsigned char* getDatas() const;

  /// Verifie si le fichier est un fichier binaire
  static bool isFileBinary(const std::string &filename);
  /// Verifie que le buffer est bien un fichier binaire
  static bool isBinary(const unsigned char *i_buffer, int bufferSize = -1);
  /// Crée un header binaire
  static void createBinaryHeader(unsigned char *o_header, const unsigned int i_start, const unsigned int i_size, const unsigned int i_exec, const unsigned char i_type);

  /// Affichage du header du fichier
  virtual std::ostream& printHeader(std::ostream& io_os) const;
  /// Affichage de contenu du fichier
  virtual std::ostream& printContent(std::ostream& io_os) const;

};

#endif
