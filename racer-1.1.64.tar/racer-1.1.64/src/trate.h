#ifndef _TARTE_HEADER_
#define _TARTE_HEADER_


typedef struct {
	char  name[20];
	char  file[20];
	char  popis[100];
} st_trat;


#define MAX_TRATI 8
/*
const st_trat Tparams[MAX_TRATI] = {
	{"Vesni�ka",               "TEST1", "Mal� z�vodn� trat vesnic�."},
	{"Rychl�k",                "TEST2", "Superrychl� z�vodn� trat."},
	{"Ostrov",                 "TEST3", "Trat na ostrov� kolem kopce."},
	{"Z�zeninka",              "TEST4", "Z�ludn� okruh s nep��jemn�m koncem."},
	{"P�se�n� bou�e",          "TEST5", "Dlouh� rovinky na okruhu mezi palmami."},
	{"D�lnice",                "TEST6", "Trat na nedostav�n� d�lnici."},
	{"Hakkinenova chlouba",    "TEST7", "Zimn� z�vodn� okruh."},
	{"Zim�k",                  "TEST8", "Trat, kde nen� ani jedna rovinka."},
};
*/

const st_trat Tparams[MAX_TRATI] = {
	{"Village",                "TEST1", "Small village track."},
	{"Speedy",                 "TEST2", "Superfast race circuit."},
	{"Island",                 "TEST3", "Track around a beachy island."},
	{"Stentonic",              "TEST4", "Tricky track with critical finish."},
	{"Sand storm",             "TEST5", "Long straigths on a race circuit."},
	{"Highway",                "TEST6", "Highway under construction."},
	{"Hakkinen's pride",       "TEST7", "Winter race circuit."},
	{"Da Winter",              "TEST8", "Not a single straight piece."},
};


#endif
