// GFile.cpp: implementation of the GFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdh.h"
#include "GFile.h"
#include "GObject.h"
#include "GMap.h"

#include <iostream>


void TESTZEROF(int X)
{
    if (X==0) { cerr << " loading in GFile failed" << endl; }
}

st_GFile_Driving driving;
/*////////////////////////////////////////////////////////////////////////////////
  Konstruktor
*/
GFile::GFile()
{
	m_fh = NULL;
}

/*////////////////////////////////////////////////////////////////////////////////
  Destruktok
*/
GFile::~GFile()
{
	if (m_fh != NULL) CloseFile();
}

/*////////////////////////////////////////////////////////////////////////////////
  Otevre a cte soubor s trati
  filename : jmeno souboru
*/
bool GFile::OpenFile(const char *filename)
{

	strcpy(m_filename, filename);

	char s[150];
	snprintf(s, 150, "maps/%s.EDB", filename);
	
	string path = locateFile(s);
	
//LINUX 
//	return false;
	
	
	CloseFile(); // pro jistotu
	if ((m_fh = fopen( path.c_str(), "rb" )) == NULL ) {
        cerr << "no file" << endl;
		return false;    // nepodarilo se otevrit soubor
    }
	
    TESTZEROF(fread(&m_header, sizeof(m_header), 1, m_fh)); // nacteni hlavicky
	if (m_header.sign != FILE_SIGN) {
		CloseFile();
        cerr << "bad file" << endl;
		return false;
	}

	return true;
}

/*////////////////////////////////////////////////////////////////////////////////
  Zavre soubor
*/
void GFile::CloseFile()
{
	if (m_fh != NULL) fclose( m_fh );
	m_fh = NULL;
}

/*////////////////////////////////////////////////////////////////////////////////
  Nacte data ze souboru 
  map  :  objekt may, kam se ma soubor nacist
*/

bool GFile::LoadData(GMap *map)
{
	int i;
	// init
	if (m_fh == NULL) return false;

	//TODO: testovat navratove hodnoty pri nacitani
	// hopnem na zacetek souboru

	//LINUX
	
	fseek(m_fh, 0, SEEK_SET);

	// cteni hlavicky
    TESTZEROF(fread(&m_header, sizeof(m_header), 1, m_fh)); // nacteni hlavicky
	if (m_header.sign != FILE_SIGN) return false;

	map->Reset(m_header.x, m_header.y, m_header.bonusu, m_header.cam_start, m_header.cam_end);

	// cteni silnice 
	st_GFile_Road road;
	for (int j = 0; j < MAX_ROADS_TYPES; j++)
		for (i = 0; i < m_header.road[j]; i++) {
            TESTZEROF(fread(&road, sizeof(road), 1, m_fh)); // nacteni hlavicky
			map->AddRoadPoint(j, road);
		}
		
	// cteni objektu
//	printf("\n");
	st_GFile_Object ob;
	for (i = 0; i < m_header.objects; i++) {
        TESTZEROF(fread(&ob, sizeof(ob), 1, m_fh)); // nacteni hlavicky
		for (int j = 0; j < MAX_OBJECTS; j++)
			if (Oparams[j] != NULL && Oparams[j]->INDEX == ob.INDEX) {
				ob.ID = j;
				map->AddObject(ob);
			}
	}
//	printf("\n");

	// cteni inteligence jezdeni
    TESTZEROF(fread(&driving, sizeof(driving), 1, m_fh)); // nacteni hlavicky

	map->InitDone(m_filename);
	return true;
}
