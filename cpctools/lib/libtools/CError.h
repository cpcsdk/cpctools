/**
 * Definition des macros de gestion d'erreur
 * @author Thierry JOUIN
 * @version 1.1
 * @date 19/10/2003
 */

#ifndef _CERROR_HH_
#define _CERROR_HH_

#include <string>
#include <iostream>
#include <sstream>

#include <exception>
#include <stdexcept>

using namespace std;

namespace tools
{
	class CException : public exception
	{
	private :
		string	_message;
		string 	_file;
		int 	_line;

	public:
		CException(const string &file, int line, const string &error="Unknown") throw ();
		virtual ~CException() throw ();

		friend ostream& operator<<(ostream &io_os, const CException &exp)
{
	io_os << exp.what();
#ifdef _DEBUG
	io_os << " [" << exp._file << ":" << exp._line << "]";
#endif

#ifndef _WIN32
	io_os << exp._message ;
#endif
	return io_os;
}
	};
}

#ifndef _WIN32

#define ERROR_STR cerr
#define VERBOSE_STR cout

#define COUT( p ) VERBOSE_STR << p << endl

#define CERR( p ) VERBOSE_STR << p << endl

#define INFO( info ) VERBOSE_STR << "Info: " << info << endl

#define PRINTVAR( var ) VERBOSE_STR << "Var: [" << #var << "] = " << var << endl

#ifdef _DEBUG
#define WARNING( warn ) VERBOSE_STR << "Wrn: " << __FILE__ << "," << __LINE__ << ": " << warn << endl
#else
#define WARNING( warn ) VERBOSE_STR << "Wrn: : " << warn << endl
#endif

#define MSG( msg ) VERBOSE_STR << "Msg: " << msg << endl

#else

#define COUT( p )

#define CERR( p )

#define INFO( info )

#define PRINTVAR( var )

#ifdef _DEBUG
#define WARNING( warn )
#else
#define WARNING( warn )
#endif

#define MSG( msg )

#endif

#define TOOLS_ERROR() { throw tools::CException(__FILE__,__LINE__); }

#define TOOLS_ERRORMSG(message) { \
ostringstream __s;\
__s << message ;\
        throw tools::CException(__FILE__,__LINE__,__s.str()); \
}


#define TOOLS_ASSERT(expr) { \
if (!(expr)) \
 TOOLS_ERROR(); \
}

#define TOOLS_ASSERTMSG(expr,message){ \
if (!(expr)) \
 TOOLS_ERRORMSG(message); \
}

#endif
