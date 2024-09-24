// GFile.h: interface for the GFile class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _G_FILE_INCLUDE_
#define _G_FILE_INCLUDE_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GClasses.h"
#include "file_type.h"

class GFile  
{
public:
	bool LoadData(GMap *map);
	void CloseFile();
	bool OpenFile(const char *filename);
	GFile();
	virtual ~GFile();

	FILE   *m_fh;  // ukazatel na soubor ( je typu integer ?? )
	st_GFile_Header2 m_header; // hlavicka souboru
	char m_filename[50];
};

#endif // #ifndef _G_FILE_INCLUDE_
