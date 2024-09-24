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
	{"Vesnièka",               "TEST1", "Malá závodní trat vesnicí."},
	{"Rychlík",                "TEST2", "Superrychlá závodní trat."},
	{"Ostrov",                 "TEST3", "Trat na ostrovì kolem kopce."},
	{"Zúzeninka",              "TEST4", "Záludný okruh s nepøíjemným koncem."},
	{"Píseèná bouøe",          "TEST5", "Dlouhé rovinky na okruhu mezi palmami."},
	{"Dálnice",                "TEST6", "Trat na nedostavìné dálnici."},
	{"Hakkinenova chlouba",    "TEST7", "Zimní závodní okruh."},
	{"Zimák",                  "TEST8", "Trat, kde není ani jedna rovinka."},
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
