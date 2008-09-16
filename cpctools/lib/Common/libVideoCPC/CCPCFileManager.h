/**
 * Classe permettant la gestion d'un fichier CPC
*/                                                                                                           

#ifndef _CCPCFILEMANAGER_H_
#define _CCPCFILEMANAGER_H_

#include <string>

using namespace std;

//
// Gestion d'un fichier CPC
// 
class CCPCFileManager
{
public:
	/// Charge un fichier
	static void LoadRawData(const string &i_filename, unsigned char* &o_data, unsigned int &o_size);
	/// Enleve un eventuel header de fichier
	static bool RemoveFileHeader(unsigned char *i_data, unsigned int i_size, unsigned char* &o_data, unsigned int &o_size);
protected:
	static unsigned int CheckSum(const unsigned char *i_data);
private:
	inline CCPCFileManager() {};
};

//
// Gestion d'un fichier OCP
// 
class COCPFileManager
{
public:
	/// Charge une palette OCP
	static bool LoadColorTable(const string &i_filename, int &o_mode, unsigned int o_palette[16]);
	/// Charge un screen OCP
	static bool LoadScreen(const string &i_filename, unsigned char* &o_data, unsigned int &o_size);
	/// Charge une windows OCP
	static bool LoadWindow(const string &i_filename, unsigned char* &o_data, unsigned int &o_width, unsigned int &o_widtho, unsigned int &o_height);

	/// Lit une palette OCP
	static bool ReadColorTable(unsigned char *i_data, unsigned int i_size, int &o_mode, unsigned int o_paletteHard[16]);
	/// Lit un screen OCP
	static bool ReadScreen(unsigned char *i_data, unsigned int i_size, unsigned char* &o_data, unsigned int &o_size);
	/// Lit une windows OCP
	static bool ReadWindow(unsigned char *i_data, unsigned int i_size, unsigned char* &o_data, unsigned int &o_width, unsigned int &o_widtho, unsigned int &o_height);

protected:
	/// Lit des datas OCP compressé depuis un fichier
	static bool ReadCompressedBlock(unsigned char *i_data, unsigned int i_size, unsigned int &o_readData, unsigned char *o_data,unsigned int &o_size);

private:
	inline COCPFileManager() {};
};

#endif // _CCPCFILEMANAGER_H_