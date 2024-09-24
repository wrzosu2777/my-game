#ifndef _CAR_PARAMS_INCLUDED_
#define _CAR_PARAMS_INCLUDED_

#define MAX_CARS_TOTAL 400

#define CPARAMS_FILE "data/cars.dat"

#ifndef MAX_UPGRADES
#define MAX_UPGRADES 7
#endif

// constantni parametry aut
typedef struct {
	DWORD max;        // jak moc lze upgradovat (0 - 5)
	DWORD cena;       // cena 1 upgradu
	double speed;   // kolik prida k rychlosti   
 	double acc;     // kolik k akceleraci         - cislo 0.05 == 5%
	double turning; // kolik k zataceni
	double breakes; // kolik k brzdeni
} st_car_upgrade;



// parametry jednotlyvych modelu auta
// u sebe jsou parametry se stejnymi "jednotkami"
typedef struct {
  // technicke udaje
	char   name[56];
	double max_speed;  // maximalni rychlost smerem vpred
	double max_speedB; // maximalni rychlost smerem vzad (je zaporne cislo)

	double acc;     // akcelerace

	double breakes; // sila brzd (0..1) 1 == nebrzbi, 0 == hned stoji (fuj)
	double hbreak;  // sila rucni brzdy viz. ^
	double hbreaks; // sila rucni brzd pri zataceni ( smyku )
	double autoslow; // automaticke zopomalovani = (0..1> 1 == nezpomaluje

	
	double weight;  // hmotnost auta ( dulzite pri narazech ) (zatin nenarazi)
	
	double tearing; // prilavos k povrchu v zatackach ( = (0..1> - 1 == totalni prilnavost, 0 == totalni klouzavost )
	
	double turning; // rychlost zataceni (meneni uhle otoceni) 

	DWORD    light;

	char   file[50]; // jmeno souboru

	DWORD    razeni;    // pocet radicich stupnu

	DWORD    pohar;  // zda se s autem da jezdit tovarni pohar
	DWORD    poharb; // index sede bitmapy pri poharu

	DWORD    kategorie; // kategorie auta A = 0, B = 1,...
	DWORD    cena;
	DWORD    rt;        // typ trati po ktere jede pocitac 0 - 4
	DWORD    zvuk;      // zvuk motoru

	// pozice kol
	POINT  kola[4]; // 0 - leve horni, 1 - prave horni, 2 - leve dolni, 3 - prave dolni

	// MAX_UPGRADES == 7
	char    vycpavka1[4];
	st_car_upgrade upgrade[MAX_UPGRADES]; // vsechny upgrady

	char   line[6][90];

	BOOL    nahon_predni;
	BOOL    nahon_zadni;

	char    vycpavka[4];

} st_GCar_params;

#define CAT_A 0
#define CAT_B 1
#define CAT_C 2
#define CAT_D 3

const char Cats[10] = "ABCDEFGHI";
/*
 upgrady razene 
   -
 motor
 zataceni
 pneu1
 pneu2
 brzdy
 podvozek
 vybaveni
*/
/*
const st_GCar_params  Cparams[20] = {
	{"Audi TT quattro",     22.3,  -7.6,  0.5,  0.99, 
		0.87, 0.87, 0.997, 1575, 0.2, 4,  2, "AudiTT", 5, 0, -1, CAT_C, 1500000, 2, 0,
	{{7, 20}, {33, 20}, {7, 62}, {33, 62},},
	{{5, 80000, 0.05, 0.02, 0, 0}, {5, 50000, 0, 0, 0.05, 0}, {4, 10000, 0.01, 0.01, 0.01, 0},
	{5, 10000, 0.01, 0.01, 0.01} ,{2, 20000, 0, 0, 0, 0.05}, {5, 60000, 0.03, 0.03, 0.03, 0}, 
	{3, 30000, 0.2, 0, 0, 0},},
		{"Hlavn� ideou p�i v�voji typu TT bylo vytvo�it ryz� sportovn� automobil.",  
		 "Neobvykl� design, kter�m se vyzna�uje typ TT a jej� v�robce t�m�� beze",
		 "zm�n p�evzal ze studie z roku 1995, naleznete i v interi�ru automobilu.",
		 "�pi�kov� verze TT quattro s pohonem v�ech kol m� motor o v�konu 165 ",
		 "kW a �estistup�ovou manu�ln� p�evodovku. Syst�m pohonu v�ech kol ",
		 "vyu��v� hydraulicky ovl�danou a elektronicky ��zenou spojku."},},


	{"BMW Z3",  		19.4,  -6.8,  0.5,  0.99, 
		0.87, 0.87, 0.997,  1575, 0.2, 4, 2,  "BMW_Z3", 5, 0, -1, CAT_C, 5000000, 2, 0,
	{{7, 20}, {33, 20}, {7, 62}, {33, 62},},
	{{5, 80000, 0.05, 0.02, 0, 0}, {5, 50000, 0, 0, 0.05, 0}, {4, 10000, 0.01, 0.01, 0.01, 0},
	{5, 10000, 0.01, 0.01, 0.01} ,{2, 20000, 0, 0, 0, 0.05}, {5, 60000, 0.03, 0.03, 0.03, 0}, 
	{3, 30000, 0.2, 0, 0, 0},},
		{"skvele auto",  
		 "2. radka",
		 "nic moc",
		 "uz nevim co psat",
		 "to je ale nuda",
		 "posledni radka"},},


	{"BMW Z3 M-HP",  28.5,  -9,    1.2,  0.99, 
		0.87, 0.87, 0.997,  1575, 0.2, 7, 2, "BMW_Z3M_HP", 5, 0, -1, CAT_A, 5000000, 2, 0,
	{{7, 20}, {33, 20}, {7, 62}, {33, 62},},
	{{5, 80000, 0.05, 0.02, 0, 0}, {5, 50000, 0, 0, 0.05, 0}, {4, 10000, 0.01, 0.01, 0.01, 0},
	{5, 10000, 0.01, 0.01, 0.01} ,{2, 20000, 0, 0, 0, 0.05}, {5, 60000, 0.03, 0.03, 0.03, 0}, 
	{3, 30000, 0.2, 0, 0, 0},},
		{"skvele auto",  
		 "2. radka",
		 "nic moc",
		 "uz nevim co psat",
		 "to je ale nuda",
		 "posledni radka"},},


	{"Dodge Viper GTS",  	28.5,  -9,    0.9,  0.99, 
		0.87, 0.9,  0.997, 1490, 0.2, 4,  2, "Viper_GTS", 5, 0, -1, CAT_A, 5000000, 1, 0,
	{{7, 20}, {33, 20}, {7, 62}, {33, 62},},
	{{5, 80000, 0.05, 0.02, 0, 0}, {5, 50000, 0, 0, 0.05, 0}, {4, 10000, 0.01, 0.01, 0.01, 0},
	{5, 10000, 0.01, 0.01, 0.01} ,{2, 20000, 0, 0, 0, 0.05}, {5, 60000, 0.03, 0.03, 0.03, 0}, 
	{3, 30000, 0.2, 0, 0, 0},},
		{"Super sportovn� automobil zna�ky Dodge, kter� se v Evrop� prod�v� jako",  
		 "Chrysler, plat� za klasick� Muscle Car novodob�ho ra�en�. Dvojsedadlov�",
		 "typ s plastovou karoseri�, kter� se vyr�b� jako roadster s ochrann�m",
		 "obloukem (R/T) nebo jako kup� (GTS) poh�n� motor V10 s objemem 8,0",
		 "litru, jen� dosahuje v�konu 331 kW. Ten je nejv�t��m motorem na sv�t�",
		 "pou�it�m v osobn�m automobilu."},},	  

	  
	{"Chevrolet Corvette coup�",    26.6,  -8.8,  0.7,  0.99, 
	   0.87, 0.87, 0.997, 1472, 0.2, 4,  2, "Corvette_Coupe", 5, 1, 0, CAT_B, 5000000, 1, 0,
	{{7, 20}, {33, 20}, {7, 62}, {33, 62},},
	{{5, 80000, 0.05, 0.02, 0, 0}, {5, 50000, 0, 0, 0.05, 0}, {4, 10000, 0.01, 0.01, 0.01, 0},
	{5, 10000, 0.01, 0.01, 0.01} ,{2, 20000, 0, 0, 0, 0.05}, {5, 60000, 0.03, 0.03, 0.03, 0}, 
	{3, 30000, 0.2, 0, 0, 0},},
		{"P�t� generace dvojsedadlov�ho typu Corvette je velice kvalitn�",  
		 "sportovn� automobil sv� t��dy. M� stejn� jako jej� p�edch�dci",
		 "karoserii vyrobenou z plastu. Jako pohon slou�� motor V8 Small",
		 "Block s objemem 5.7 litru. P�evodovka je �ty�stup�ov� automatick�",
		 "a je um�st�na u zadn� n�pravy, ��m� zabezpe�uje vyv�en�",
		 "rozlo�en� hmotnosti na n�pravy."},},
	  

	{"Chevrolet Corvette cabrio",    26.6,  -8.8,  0.7,  0.99, 
	   0.87, 0.87, 0.997, 1472, 0.2, 4,  2, "Corvette_Cabrio", 5, 0, 2, CAT_B, 5000000, 1, 0,
	{{7, 20}, {33, 20}, {7, 62}, {33, 62},},
	{{5, 80000, 0.05, 0.02, 0, 0}, {5, 50000, 0, 0, 0.05, 0}, {4, 10000, 0.01, 0.01, 0.01, 0},
	{5, 10000, 0.01, 0.01, 0.01} ,{2, 20000, 0, 0, 0, 0.05}, {5, 60000, 0.03, 0.03, 0.03, 0}, 
	{3, 30000, 0.2, 0, 0, 0},},
		{"P�t� generace dvojsedadlov�ho typu Corvette je velice kvalitn�",  
		 "sportovn� automobil sv� t��dy. M� stejn� jako jej� p�edch�dci",
		 "karoserii vyrobenou z plastu. Jako pohon slou�� motor V8 Small",
		 "Block s objemem 5.7 litru. P�evodovka je �ty�stup�ov� automatick�",
		 "a je um�st�na u zadn� n�pravy, ��m� zabezpe�uje vyv�en�",
		 "rozlo�en� hmotnosti na n�pravy."},},


	{"Ferrari F355 Berlinetta",	        27.6,  -9.2,  0.9,  0.99, 
		0.87, 0.87, 0.997, 1350, 0.2, 4,  2, "Ferrari_355B", 5, 1, 4, CAT_B, 5000000, 1, 0,
	{{7, 20}, {33, 20}, {7, 62}, {33, 62},},
	{{5, 80000, 0.05, 0.02, 0, 0}, {5, 50000, 0, 0, 0.05, 0}, {4, 10000, 0.01, 0.01, 0.01, 0},
	{5, 10000, 0.01, 0.01, 0.01} ,{2, 20000, 0, 0, 0, 0.05}, {5, 60000, 0.03, 0.03, 0.03, 0}, 
	{3, 30000, 0.2, 0, 0, 0},},
		{"Okolo dvou t�etin produkce zna�ky Ferrari p�ipad� na �adu F355,",  
		 "tvo�enou kup�m Berlinetta a kabrioletem Spider. Vysokoot��kov�",
		 "motor V8 o objemu 3,5 litru s p�tiventilovou technikou a nej-",
		 "v�t��m v�konem 280 kW (380 k) p�i 8200 ot��k�ch za minutu je",
		 "ulo�en za zadn� n�pravou. To umo��uje �idi�i za��t pocit j�zdy",
		 "v z�vodn�m automobilu."},},


	{"Ferrari F355 Spider",	  27.6,  -9.2,  0.9,  0.99, 
		0.87, 0.87, 0.997, 1350, 0.2, 4,  2, "Ferrari_355S", 5, 0, 6, CAT_B, 5000000, 1, 0,
	{{7, 20}, {33, 20}, {7, 62}, {33, 62},},
	{{5, 80000, 0.05, 0.02, 0, 0}, {5, 50000, 0, 0, 0.05, 0}, {4, 10000, 0.01, 0.01, 0.01, 0},
	{5, 10000, 0.01, 0.01, 0.01} ,{2, 20000, 0, 0, 0, 0.05}, {5, 60000, 0.03, 0.03, 0.03, 0}, 
	{3, 30000, 0.2, 0, 0, 0},},
		{"Okolo dvou t�etin produkce zna�ky Ferrari p�ipad� na �adu F355,",  
		 "tvo�enou kup�m Berlinetta a kabrioletem Spider. Vysokoot��kov�",
		 "motor V8 o objemu 3,5 litru s p�tiventilovou technikou a nej-",
		 "v�t��m v�konem 280 kW (380 k) p�i 8200 ot��k�ch za minutu je",
		 "ulo�en za zadn� n�pravou. To umo��uje �idi�i za��t pocit j�zdy",
		 "v z�vodn�m automobilu."},},


	{"Ferrari 456 GT", 30.2,  -10,   1.0,  0.99, 
		0.87, 0.87, 0.997,  1690, 0.2, 5, 2, "Ferrari_456GT", 5, 0, -1, CAT_A, 5000000, 1, 0,
	{{7, 20}, {33, 20}, {7, 62}, {33, 62},},
	{{5, 80000, 0.05, 0.02, 0, 0}, {5, 50000, 0, 0, 0.05, 0}, {4, 10000, 0.01, 0.01, 0.01, 0},
	{5, 10000, 0.01, 0.01, 0.01} ,{2, 20000, 0, 0, 0, 0.05}, {5, 60000, 0.03, 0.03, 0.03, 0}, 
	{3, 30000, 0.2, 0, 0, 0},},
		{"??????",  
		 "?????",
		 "????",
		 "???",
		 "??",
		 "?"},},
	  

	{"Ferrari 550 Maranello", 32.0,  -10,   1.0,  0.99, 
	  0.87, 0.87, 0.997, 1690, 0.2, 4,  2, "Ferrari_550", 5, 0, -1, CAT_A, 5000000, 1, 0,
	{{7, 20}, {33, 20}, {7, 62}, {33, 62},},
	{{5, 80000, 0.05, 0.02, 0, 0}, {5, 50000, 0, 0, 0.05, 0}, {4, 10000, 0.01, 0.01, 0.01, 0},
	{5, 10000, 0.01, 0.01, 0.01} ,{2, 20000, 0, 0, 0, 0.05}, {5, 60000, 0.03, 0.03, 0.03, 0}, 
	{3, 30000, 0.2, 0, 0, 0},},
		{"T�mto modelem se zna�ka Ferrari u kup� s davn�ctivalcov�m motorem",  
		 "op�t vr�tila ke koncepci s vp�edu ulo�en�m motorem. Maranello je ",
		 "postaveno na technick�m a konstruk�n�m z�kladu typu 456. M� stejn�",
		 "motor V12 a �estistup�ovou p�evodovku. Motor o objemu 5,5 litru",
		 "dosahuje z�sluhou vy���ho kompresn�ho pom�ru a modifikovan�ho sac�ho",
		 "syst�mu v�konu 356 kW (485 k)."},},	  


	{"Ferrari F50", 	33.0,  -10,   1.0,  0.99, 
		0.87, 0.87, 0.997,  1350, 0.2, 4, 2, "Ferrari_F50", 5, 0, -1, CAT_A, 5000000, 1, 0,
	{{7, 20}, {33, 20}, {7, 62}, {33, 62},},
	{{5, 80000, 0.05, 0.02, 0, 0}, {5, 50000, 0, 0, 0.05, 0}, {4, 10000, 0.01, 0.01, 0.01, 0},
	{5, 10000, 0.01, 0.01, 0.01} ,{2, 20000, 0, 0, 0, 0.05}, {5, 60000, 0.03, 0.03, 0.03, 0}, 
	{3, 30000, 0.2, 0, 0, 0},},
		{"??????",  
		 "?????",
		 "????",
		 "???",
		 "??",
		 "?"},},


	{"Ford Focus",     	  17.1,  -6.0,  0.35, 0.99, 
		0.87, 0.87, 0.997,  1575, 0.2, 4, 2, "Ford_Focus", 5, 0, -1, CAT_C, 1500000, 2, 0,
	{{7, 20}, {33, 20}, {7, 62}, {33, 62},},
	{{5, 80000, 0.05, 0.02, 0, 0}, {5, 50000, 0, 0, 0.05, 0}, {4, 10000, 0.01, 0.01, 0.01, 0},
	{5, 10000, 0.01, 0.01, 0.01} ,{2, 20000, 0, 0, 0, 0.05}, {5, 60000, 0.03, 0.03, 0.03, 0}, 
	{3, 30000, 0.2, 0, 0, 0},},
		{"skvele auto",  
		 "2. radka",
		 "nic moc",
		 "uz nevim co psat",
		 "to je ale nuda",
		 "posledni radka"},},
				

	{"Jaguar XKR coup�",	           23.4,  -7.8,  0.6,  0.99, 
		0.87, 0.9,  0.997, 1825, 0.2, 4,  2, "Jaguar_XKR_coupe", 5, 1, 8, CAT_C, 5000000, 2, 0, 
	{{7, 20}, {33, 20}, {7, 62}, {33, 62},},
	{{5, 80000, 0.05, 0.02, 0, 0}, {5, 50000, 0, 0, 0.05, 0}, {4, 10000, 0.01, 0.01, 0.01, 0},
	{5, 10000, 0.01, 0.01, 0.01} ,{2, 20000, 0, 0, 0, 0.05}, {5, 60000, 0.03, 0.03, 0.03, 0}, 
	{3, 30000, 0.2, 0, 0, 0},},
		{"Oproti sv�mu p�edch�dci byla karoserie nov�ho XKR jen m�rn� upravena.",  
		 "Pod n� se ale skr�v� motor z typu XJR. Jeho nejv�t�� v�kon 267 kW",
		 "(363 k) za�azuje XKR mezi v�kon�j�� automobily, p�i�em� kombinace",
		 "vynikaj�c�ch j�zdn�ch v�kon�, klasick�ho designu a relativn� n�zk�",
		 "ceny zabezpe�uje tomuto automobilu ojedin�l� postaven�. Tento �pi�kov�",
		 "model m� automatickou regulaci tvrdosti odpru�en�."},},
				

	{"Jaguar XKR cabrio",	  23.4,  -7.8,  0.6,  0.99, 
		0.87, 0.9,  0.997, 1825, 0.2, 4,  2, "Jaguar_XKR_cabrio", 5, 0, 10, CAT_C, 5000000, 2, 0,
	{{7, 20}, {33, 20}, {7, 62}, {33, 62},},
	{{5, 80000, 0.05, 0.02, 0, 0}, {5, 50000, 0, 0, 0.05, 0}, {4, 10000, 0.01, 0.01, 0.01, 0},
	{5, 10000, 0.01, 0.01, 0.01} ,{2, 20000, 0, 0, 0, 0.05}, {5, 60000, 0.03, 0.03, 0.03, 0}, 
	{3, 30000, 0.2, 0, 0, 0},},
		{"Oproti sv�mu p�edch�dci byla karoserie nov�ho XKR jen m�rn� upravena.",  
		 "Pod n� se ale skr�v� motor z typu XJR. Jeho nejv�t�� v�kon 267 kW",
		 "(363 k) za�azuje XKR mezi v�kon�j�� automobily, p�i�em� kombinace",
		 "vynikaj�c�ch j�zdn�ch v�kon�, klasick�ho designu a relativn� n�zk�",
		 "ceny zabezpe�uje tomuto automobilu ojedin�l� postaven�. Tento �pi�kov�",
		 "model m� automatickou regulaci tvrdosti odpru�en�."},},
	  

	{"Lamborghini Countach",  30.5,  -9.2,  0.8,  0.99, 
	   0.87, 0.9,  0.997, 1490, 0.2, 4,  2, "Countach", 5, 0, -1, CAT_A, 5000000, 0, 0,
	{{7, 20}, {33, 20}, {7, 62}, {33, 62},},
	{{5, 80000, 0.05, 0.02, 0, 0}, {5, 50000, 0, 0, 0.05, 0}, {4, 10000, 0.01, 0.01, 0.01, 0},
	{5, 10000, 0.01, 0.01, 0.01} ,{2, 20000, 0, 0, 0, 0.05}, {5, 60000, 0.03, 0.03, 0.03, 0}, 
	{3, 30000, 0.2, 0, 0, 0},},
		{"skvele auto",  
		 "2. radka",
		 "nic moc",
		 "uz nevim co psat",
		 "to je ale nuda",
		 "posledni radka"},},


	{"Lamborghini Diablo SV",  32.0,  -10,   0.9,  0.99, 
	  	0.87, 0.9,  0.997,  1490, 0.2, 4,  2, "Diablo_SV", 5, 0, -1, CAT_A, 5000000, 0, 0,
	{{7, 20}, {33, 20}, {7, 62}, {33, 62},},
	{{5, 80000, 0.05, 0.02, 0, 0}, {5, 50000, 0, 0, 0.05, 0}, {4, 10000, 0.01, 0.01, 0.01, 0},
	{5, 10000, 0.01, 0.01, 0.01} ,{2, 20000, 0, 0, 0, 0.05}, {5, 60000, 0.03, 0.03, 0.03, 0}, 
	{3, 30000, 0.2, 0, 0, 0},},
		{"V sou�asnosti se nab�dka italsk� automobilky Lamborghini omezuje pouze na",  
		 "model Diablo. Ten s motorem V12 o objemu 5,7 litru ve zvl�tn� sportovn�",
		 "verzi SV (Sport Veloce) dosahuje v�konu a� 390 kW (530 k). P�epracovan�",
		 "brzdov� soustava dostala v�t�� kotou�e a �elisti. S�riov� se nyn� dod�v�",
		 "i tolik pot�ebn� protiblokovac� syst�m ABS. Maxim�ln� rychlost 320 km/h",
		 "zaji��uje Diablu p�edn� m�sto mezi nejrychlej��mi s�riov�mi automobily."},}, 


	{"Mercedes SLK",     	  20.8,  -7.1,  0.5,  0.99, 
		0.87, 0.87, 0.997,  1575, 0.2, 4,  2,"Mercedes_SLK", 5, 0, -1, CAT_C, 1500000, 2, 0,
	{{7, 20}, {33, 20}, {7, 62}, {33, 62},},
	{{5, 80000, 0.05, 0.02, 0, 0}, {5, 50000, 0, 0, 0.05, 0}, {4, 10000, 0.01, 0.01, 0.01, 0},
	{5, 10000, 0.01, 0.01, 0.01} ,{2, 20000, 0, 0, 0, 0.05}, {5, 60000, 0.03, 0.03, 0.03, 0}, 
	{3, 30000, 0.2, 0, 0, 0},},
		{"skvele auto",  
		 "2. radka",
		 "nic moc",
		 "uz nevim co psat",
		 "to je ale nuda",
		 "posledni radka"},},	


	{"McLaren F1",    	   33.0,  -10,   1.1,  0.99, 
		0.87, 0.87, 0.997,  1350, 0.2, 4,  2,"Mclaren" , 5, 0, -1, CAT_A, 5000000, 0, 0,
	{{7, 20}, {33, 20}, {7, 62}, {33, 62},},
	{{5, 80000, 0.05, 0.02, 0, 0}, {5, 50000, 0, 0, 0.05, 0}, {4, 10000, 0.01, 0.01, 0.01, 0},
	{5, 10000, 0.01, 0.01, 0.01} ,{2, 20000, 0, 0, 0, 0.05}, {5, 60000, 0.03, 0.03, 0.03, 0}, 
	{3, 30000, 0.2, 0, 0, 0},},
		{"skvele auto",  
		 "2. radka",
		 "nic moc",
		 "uz nevim co psat",
		 "to je ale nuda",
		 "posledni radka"},},	


	{"Opel Vectra",     	  17.5,  -6.2,  0.3,  0.99, 
		0.87, 0.87, 0.997,  1575, 0.2, 4,  2,"Opel_vectra", 5, 0, -1, CAT_C, 1500000, 2, 0,
	{{7, 20}, {33, 20}, {7, 62}, {33, 62},},
	{{5, 80000, 0.05, 0.02, 0, 0}, {5, 50000, 0, 0, 0.05, 0}, {4, 10000, 0.01, 0.01, 0.01, 0},
	{5, 10000, 0.01, 0.01, 0.01} ,{2, 20000, 0, 0, 0, 0.05}, {5, 60000, 0.03, 0.03, 0.03, 0}, 
	{3, 30000, 0.2, 0, 0, 0},},
		{"skvele auto",  
		 "2. radka",
		 "nic moc",
		 "uz nevim co psat",
		 "to je ale nuda",
		 "posledni radka"},},	


	{"Porshe 911 Turbo",      25.3,  -8.5,  0.7,  0.99, 
		0.87, 0.9,  0.997, 1000, 0.1, 4,  2, "porsche911_Targa", 5, 0, -1, CAT_B, 5000000, 1, 0, 
	{{7, 20}, {33, 20}, {7, 62}, {33, 62},},
	{{5, 80000, 0.05, 0.02, 0, 0}, {5, 50000, 0, 0, 0.05, 0}, {4, 10000, 0.01, 0.01, 0.01, 0},
	{5, 10000, 0.01, 0.01, 0.01} ,{2, 20000, 0, 0, 0, 0.05}, {5, 60000, 0.03, 0.03, 0.03, 0}, 
	{3, 30000, 0.2, 0, 0, 0},},
		{"Porsche 911 Turbo se sv�m motorem (le�at� �estiv�lec s protilehl�mi v�lci)",  
		 "o v�konu 300 kW (408 k), opat�en�m dv�ma turbodmychadly, zauj�m� vrchol ve ",
		 "v�robn�m programu stuttgartsk� zna�ky. Mezin�pravov� diferenci�l s vizk�zn�",
		 "spujkou rozd�luje podle moment�ln�ch r�b�rov�ch podm�nek to�iv� moment",
		 "motoru mezi ob� poh�n�n� n�pravy. Toto Turbo nab�z� p�ekvapiv� dobr� j�zdn�",
		 "vlastnosti a d�ky citliv�mu p�enosu hnac� s�ly i dobr� v�kony."},},
};
*/
#endif
