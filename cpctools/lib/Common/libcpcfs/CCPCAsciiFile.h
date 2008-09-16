/**
 * @class CCPCAsciiFile
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

#ifndef _CCPCASCIIFILE_HH_
#define _CCPCASCIIFILE_HH_

//! Class used to manage ASCII files
class CCPCAsciiFile:public CCPCFile
{
private:
  //! File content
  unsigned char *_content;
  //! File size
  unsigned int _size;

protected:
  //! Display file info
  virtual std::ostream& printInfo(std::ostream &io_os) const;

public:
  //! Constructor
  CCPCAsciiFile();
  //! Destructor
  ~CCPCAsciiFile();

  //! Return the file type
  virtual TFileType getType() const;

  //! Return file size in bytes
  virtual unsigned int getSize() const;

  //! Create a file from a buffer in memory
  virtual void createFile(const void* i_buffer, const unsigned int i_size, bool i_createHeader = false);
  //! Read & create a file from a file on disc
  virtual void openFile(const std::string &i_filename,bool i_createHeader = false);
  //! Write file in a file
  virtual void saveFile(const std::string &i_filename,bool i_saveHeader = true) const;

  //! Renvoie la taille des donnees contenues dans le fichier (header eventuellement compris)
  virtual unsigned int getDatasSize() const;
  //! Renvoie un pointeur sur les donnees contenues dans le fichier
  virtual unsigned char* getDatas() const;

  //! Affichage du header du fichier
  virtual std::ostream& printHeader(std::ostream& io_os) const;
  //! Affichage de contenu du fichier
  virtual std::ostream& printContent(std::ostream& io_os) const;

};

#endif
