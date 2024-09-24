#ifndef _OBJECTS_PARAMS_INCLUDE_
#define _OBJECTS_PARAMS_INCLUDE_

#include "file_type.h"

// typy objektu 
#define OBJECT_NONE    0  // neinicializovany objekt
#define OBJECT_LOW     1  // objekt na zemi - treba sipka na silnici ( nepohybujici se objekt, ktery lze prejet )
#define OBJECT_STATIC  2  // staticky objekt na zemi - treba barak
#define OBJECT_SOFT    3  // meky objekt, ktery se pri stretu s autem zmeni ( DUCHODCE :) cha )
#define OBJECT_HI      4  // vysoky objekt ( nedotyka se zeme (cast mostu) )
#define OBJECT_FLYING  5  // letajici objekt

// typy pro vysku objektu
#define OBJ_HI_GROUNG  0  // objekt je soucasti zeme
#define OBJ_HI_LAY     1  // objekt lezi na zemi
// tady je jako vysk auta
#define OBJ_HI_NORMAL  2  // objekt se muze nachazet nad autem
#define OBJ_HI_TOP     3  // nejvyssi normalni objekty
#define OBJ_HI_FLY     4  // letajici objekty


#define MAX_OBJECTS             500 // maximalni pocet objektu
#define MAX_OBJECTS_BITMAPS    1000 // pocet bitmap u objektu 

extern BITMAP* b_objects[MAX_OBJECTS_BITMAPS];  // bitmapy k objektum


typedef struct {
	int    INDEX;    // unikatni cislo
	char   name[30]; // jemno objektu

	char   sname[4];  // zkracene jemno (3pismena a 0)

  // technicke udaje
	int    type; // typ objektu

  // graficke udaje
	int    bindex;  // index bitmapy;
	int    stin;    // odsazeni stinu
	int    large;   // zda se ma pri modu large kreslit primo do podkladu


	int    wb2;     // sirka bitmapy /2
	int    hb2;      // vyska bitmapy /2

// rozmery na narazeni
	int    w2;      // sirka/2
	int    h2;      // vyska/2

	int dangle;  // lze -li objektu dat pocatecni natoceni (1/0)
	int angles;  // zda objekt muze rotovat (1/0)

	int turning;  // schopnost zatacet (1/0)
	int speed;    // moznost pohybu (1/0)

	int light;    // ma - li pod sebou svetlo (1/0)

	int anims;    // kolik ma animacnich okenek
	int canims;    // kolik ma animacnich okenek pri narazu

	char   file[30]; // 1.cast nazvu souboru

	int startpos; // zda je pouzito jako startovni pozice, checkpoint (1 = start, 2 = start/cil, 3 = checkpoint);
 
	int sound;   // sda ma objekt zvuk (1/0)
	int reptime; // po jake dobe se ma opakovat
	int soundindex;  // index zvuku v nejakem poli.... //TODO:

	int hi;      // vyska - odpovida OBJ_HI_..
} st_GObjects_params;

extern st_GObjects_params *Oparams[MAX_OBJECTS];

#define OPARAMS_FILE "data/objects.dat"

void LoadOparams();
void SaveOparams();
void DestroyOparams();
void ObjIndex(st_GFile_Object *obj);
int NewObj();


#endif // #ifndef _OBJECTS_PARAMS_INCLUDE_
