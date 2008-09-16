/**
 * @class CDataLink
 * Classe permettant la gestion d'un data pour le linker
 * @author Thierry JOUIN
 * @version 1.1
 * @date 04/08/2003
 */

#include <string>

#ifndef _CDATALINK_H_
#define _CDATALINK_H_

class CDataLink
{
public:
	CDataLink();
	/// Constructor
	CDataLink(std::string &i_filename,std::string &i_label, int i_mask=0xffff);
	/// Copy constructor
	CDataLink(const CDataLink &i_data);
	/// Destructor
	virtual ~CDataLink();

	/// Affectation operator
	CDataLink& operator=(const CDataLink &i_data);
			
	/// Load the file
	void loadFile();

	/// Get data size
	int getSize() const;
	/// Get data mask
	int getMask() const;
	/// Get data label
	std::string getLabel();
	/// Get data
	unsigned char* getDatas();

	static CDataLink NullData(unsigned int i_size);

protected:
	/// File name
	std::string mFilename;
	/// File label
	std::string mLabel;
	/// File size
	int mSize;
	/// File mask
	int mMask;
	/// File datas
	unsigned char *mData;
private:
};

#endif