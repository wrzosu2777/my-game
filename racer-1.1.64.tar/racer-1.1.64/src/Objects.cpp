#include "stdafx.h"
#include "objects.h"
#include <iostream>

using namespace std;

st_GObjects_params *Oparams[MAX_OBJECTS];

int Opar_maxINDEX = 0;

void SaveOparams()
{
	FILE *f;
	
	string path = locateFile(OPARAMS_FILE);
	f = fopen(path.c_str(), "wb");
	if (f) {
		fwrite(&Opar_maxINDEX, sizeof(Opar_maxINDEX), 1, f);
		for(int i = 0; i < MAX_OBJECTS; i++)
			if (Oparams[i] != NULL)
				fwrite(Oparams[i], sizeof(st_GObjects_params), 1, f);
		fclose(f);
	}
}

void DestroyOparams()
{
	for(int i = 0; i < MAX_OBJECTS; i++)
		if (Oparams[i] != NULL) {
			delete Oparams[i];
			Oparams[i] = NULL;
		}
}


int NewObj()
{
	int i;
	for(i = 0; i < MAX_OBJECTS; i++)
		if (Oparams[i] == NULL) {
			Oparams[i] = new st_GObjects_params;
			ZeroMemory(Oparams[i], sizeof(st_GObjects_params));
			Oparams[i]->INDEX = ++Opar_maxINDEX;
			break;
		}
	return i;
}

void LoadOparams()
{
	DestroyOparams();

	FILE *f;
	st_GObjects_params obj;


	f = fopen(locateFile(OPARAMS_FILE).c_str(), "rb");
	int i = 0;
	int bmp = 0;

	if (f) {
		if (fread(&Opar_maxINDEX, sizeof(Opar_maxINDEX), 1, f) == 0)
        {
            cerr << "Some loading error I guess." << endl;
        }
		while((fread(&obj, sizeof(st_GObjects_params), 1, f) > 0) && (i < MAX_OBJECTS)) {
			obj.bindex = bmp;
			Oparams[i] = new st_GObjects_params;
			*Oparams[i] = obj;
			bmp += 2 + obj.anims + obj.canims;
			i++;
			
		}
		fclose(f);
	}

}

void ObjIndex(st_GFile_Object *obj)
{
	for(int i = 0; i < MAX_OBJECTS; i++)
		if (Oparams[i] != NULL)
			if (Oparams[i]->INDEX == obj->INDEX) {
				obj->ID = i;
				return;
			}
}
