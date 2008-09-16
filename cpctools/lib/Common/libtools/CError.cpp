/**
 * Definition des macros de gestion d'erreur
 * @author Thierry JOUIN
 * @version 1.1
 * @date 19/10/2003
 */

#include "CError.h"

using namespace tools;

CException::CException(const string &file, int line, const string &error)  throw () :
#if _WINDOWS
exception(error.c_str()),
#else
_message(error.c_str()),
#endif
_file(file),
_line(line)
{

}

CException::~CException() throw ()
{
}
/*
ostream& tools::operator<<(ostream &io_os, const CException &exp)
{
	io_os << exp.what();
#ifdef _DEBUG
	io_os << " [" << exp._file << ":" << exp._line << "]";
#endif

#ifndef _WINDOWS
	io_os << exp._message ;
#endif
	return io_os;
}
*/
