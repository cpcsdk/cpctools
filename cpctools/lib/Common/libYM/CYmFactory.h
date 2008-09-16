/**
 * @class CYmFactory
 * Classe permettant de charge les differents types de YM
 * @author Thierry JOUIN
 * @version 1.1
 * @date 31/10/2001
 */                                                                                                           

#ifndef _CYMFACTORY_HH_
#define _CYMFACTORY_HH_

class CYmFactory  
{
public:
	static CYm *createYm(const std::string i_YmFileName);
private:
  CYmFactory();
  virtual ~CYmFactory();
};

#endif
