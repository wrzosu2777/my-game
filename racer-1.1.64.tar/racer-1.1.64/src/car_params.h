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
		{"Hlavní ideou pøi vıvoji typu TT bylo vytvoøit ryzí sportovní automobil.",  
		 "Neobvyklı design, kterım se vyznaèuje typ TT a jej vırobce témìø beze",
		 "zmìn pøevzal ze studie z roku 1995, naleznete i v interiéru automobilu.",
		 "Špièková verze TT quattro s pohonem všech kol má motor o vıkonu 165 ",
		 "kW a šestistupòovou manuální pøevodovku. Systém pohonu všech kol ",
		 "vyuívá hydraulicky ovládanou a elektronicky øízenou spojku."},},


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
		{"Super sportovní automobil znaèky Dodge, kterı se v Evropì prodává jako",  
		 "Chrysler, platí za klasickı Muscle Car novodobého raení. Dvojsedadlovı",
		 "typ s plastovou karoserií, kterı se vyrábí jako roadster s ochrannım",
		 "obloukem (R/T) nebo jako kupé (GTS) pohání motor V10 s objemem 8,0",
		 "litru, jen dosahuje vıkonu 331 kW. Ten je nejvìtším motorem na svìtì",
		 "pouitım v osobním automobilu."},},	  

	  
	{"Chevrolet Corvette coupé",    26.6,  -8.8,  0.7,  0.99, 
	   0.87, 0.87, 0.997, 1472, 0.2, 4,  2, "Corvette_Coupe", 5, 1, 0, CAT_B, 5000000, 1, 0,
	{{7, 20}, {33, 20}, {7, 62}, {33, 62},},
	{{5, 80000, 0.05, 0.02, 0, 0}, {5, 50000, 0, 0, 0.05, 0}, {4, 10000, 0.01, 0.01, 0.01, 0},
	{5, 10000, 0.01, 0.01, 0.01} ,{2, 20000, 0, 0, 0, 0.05}, {5, 60000, 0.03, 0.03, 0.03, 0}, 
	{3, 30000, 0.2, 0, 0, 0},},
		{"Pátá generace dvojsedadlového typu Corvette je velice kvalitní",  
		 "sportovní automobil své tøídy. Má stejnì jako její pøedchùdci",
		 "karoserii vyrobenou z plastu. Jako pohon slouí motor V8 Small",
		 "Block s objemem 5.7 litru. Pøevodovka je ètyøstupòová automatická",
		 "a je umístìna u zadní nápravy, èím zabezpeèuje vyváené",
		 "rozloení hmotnosti na nápravy."},},
	  

	{"Chevrolet Corvette cabrio",    26.6,  -8.8,  0.7,  0.99, 
	   0.87, 0.87, 0.997, 1472, 0.2, 4,  2, "Corvette_Cabrio", 5, 0, 2, CAT_B, 5000000, 1, 0,
	{{7, 20}, {33, 20}, {7, 62}, {33, 62},},
	{{5, 80000, 0.05, 0.02, 0, 0}, {5, 50000, 0, 0, 0.05, 0}, {4, 10000, 0.01, 0.01, 0.01, 0},
	{5, 10000, 0.01, 0.01, 0.01} ,{2, 20000, 0, 0, 0, 0.05}, {5, 60000, 0.03, 0.03, 0.03, 0}, 
	{3, 30000, 0.2, 0, 0, 0},},
		{"Pátá generace dvojsedadlového typu Corvette je velice kvalitní",  
		 "sportovní automobil své tøídy. Má stejnì jako její pøedchùdci",
		 "karoserii vyrobenou z plastu. Jako pohon slouí motor V8 Small",
		 "Block s objemem 5.7 litru. Pøevodovka je ètyøstupòová automatická",
		 "a je umístìna u zadní nápravy, èím zabezpeèuje vyváené",
		 "rozloení hmotnosti na nápravy."},},


	{"Ferrari F355 Berlinetta",	        27.6,  -9.2,  0.9,  0.99, 
		0.87, 0.87, 0.997, 1350, 0.2, 4,  2, "Ferrari_355B", 5, 1, 4, CAT_B, 5000000, 1, 0,
	{{7, 20}, {33, 20}, {7, 62}, {33, 62},},
	{{5, 80000, 0.05, 0.02, 0, 0}, {5, 50000, 0, 0, 0.05, 0}, {4, 10000, 0.01, 0.01, 0.01, 0},
	{5, 10000, 0.01, 0.01, 0.01} ,{2, 20000, 0, 0, 0, 0.05}, {5, 60000, 0.03, 0.03, 0.03, 0}, 
	{3, 30000, 0.2, 0, 0, 0},},
		{"Okolo dvou tøetin produkce znaèky Ferrari pøipadá na øadu F355,",  
		 "tvoøenou kupém Berlinetta a kabrioletem Spider. Vysokootáèkovı",
		 "motor V8 o objemu 3,5 litru s pìtiventilovou technikou a nej-",
		 "vìtším vıkonem 280 kW (380 k) pøi 8200 otáèkách za minutu je",
		 "uloen za zadní nápravou. To umoòuje øidièi zaít pocit jízdy",
		 "v závodním automobilu."},},


	{"Ferrari F355 Spider",	  27.6,  -9.2,  0.9,  0.99, 
		0.87, 0.87, 0.997, 1350, 0.2, 4,  2, "Ferrari_355S", 5, 0, 6, CAT_B, 5000000, 1, 0,
	{{7, 20}, {33, 20}, {7, 62}, {33, 62},},
	{{5, 80000, 0.05, 0.02, 0, 0}, {5, 50000, 0, 0, 0.05, 0}, {4, 10000, 0.01, 0.01, 0.01, 0},
	{5, 10000, 0.01, 0.01, 0.01} ,{2, 20000, 0, 0, 0, 0.05}, {5, 60000, 0.03, 0.03, 0.03, 0}, 
	{3, 30000, 0.2, 0, 0, 0},},
		{"Okolo dvou tøetin produkce znaèky Ferrari pøipadá na øadu F355,",  
		 "tvoøenou kupém Berlinetta a kabrioletem Spider. Vysokootáèkovı",
		 "motor V8 o objemu 3,5 litru s pìtiventilovou technikou a nej-",
		 "vìtším vıkonem 280 kW (380 k) pøi 8200 otáèkách za minutu je",
		 "uloen za zadní nápravou. To umoòuje øidièi zaít pocit jízdy",
		 "v závodním automobilu."},},


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
		{"Tímto modelem se znaèka Ferrari u kupé s davnáctivalcovım motorem",  
		 "opìt vrátila ke koncepci s vpøedu uloenım motorem. Maranello je ",
		 "postaveno na technickém a konstrukèním základu typu 456. Má stejnı",
		 "motor V12 a šestistupòovou pøevodovku. Motor o objemu 5,5 litru",
		 "dosahuje zásluhou vyššího kompresního pomìru a modifikovaného sacího",
		 "systému vıkonu 356 kW (485 k)."},},	  


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
				

	{"Jaguar XKR coupé",	           23.4,  -7.8,  0.6,  0.99, 
		0.87, 0.9,  0.997, 1825, 0.2, 4,  2, "Jaguar_XKR_coupe", 5, 1, 8, CAT_C, 5000000, 2, 0, 
	{{7, 20}, {33, 20}, {7, 62}, {33, 62},},
	{{5, 80000, 0.05, 0.02, 0, 0}, {5, 50000, 0, 0, 0.05, 0}, {4, 10000, 0.01, 0.01, 0.01, 0},
	{5, 10000, 0.01, 0.01, 0.01} ,{2, 20000, 0, 0, 0, 0.05}, {5, 60000, 0.03, 0.03, 0.03, 0}, 
	{3, 30000, 0.2, 0, 0, 0},},
		{"Oproti svému pøedchùdci byla karoserie nového XKR jen mírnì upravena.",  
		 "Pod ní se ale skrıvá motor z typu XJR. Jeho nejvìtší vıkon 267 kW",
		 "(363 k) zaøazuje XKR mezi vıkonìjší automobily, pøièem kombinace",
		 "vynikajících jízdních vıkonù, klasického designu a relativnì nízké",
		 "ceny zabezpeèuje tomuto automobilu ojedinìlé postavení. Tento špièkovı",
		 "model má automatickou regulaci tvrdosti odpruení."},},
				

	{"Jaguar XKR cabrio",	  23.4,  -7.8,  0.6,  0.99, 
		0.87, 0.9,  0.997, 1825, 0.2, 4,  2, "Jaguar_XKR_cabrio", 5, 0, 10, CAT_C, 5000000, 2, 0,
	{{7, 20}, {33, 20}, {7, 62}, {33, 62},},
	{{5, 80000, 0.05, 0.02, 0, 0}, {5, 50000, 0, 0, 0.05, 0}, {4, 10000, 0.01, 0.01, 0.01, 0},
	{5, 10000, 0.01, 0.01, 0.01} ,{2, 20000, 0, 0, 0, 0.05}, {5, 60000, 0.03, 0.03, 0.03, 0}, 
	{3, 30000, 0.2, 0, 0, 0},},
		{"Oproti svému pøedchùdci byla karoserie nového XKR jen mírnì upravena.",  
		 "Pod ní se ale skrıvá motor z typu XJR. Jeho nejvìtší vıkon 267 kW",
		 "(363 k) zaøazuje XKR mezi vıkonìjší automobily, pøièem kombinace",
		 "vynikajících jízdních vıkonù, klasického designu a relativnì nízké",
		 "ceny zabezpeèuje tomuto automobilu ojedinìlé postavení. Tento špièkovı",
		 "model má automatickou regulaci tvrdosti odpruení."},},
	  

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
		{"V souèasnosti se nabídka italské automobilky Lamborghini omezuje pouze na",  
		 "model Diablo. Ten s motorem V12 o objemu 5,7 litru ve zvláštní sportovní",
		 "verzi SV (Sport Veloce) dosahuje vıkonu a 390 kW (530 k). Pøepracovaná",
		 "brzdová soustava dostala vìtší kotouèe a èelisti. Sériovì se nyní dodává",
		 "i tolik potøebnı protiblokovací systém ABS. Maximální rychlost 320 km/h",
		 "zajišuje Diablu pøední místo mezi nejrychlejšími sériovımi automobily."},}, 


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
		{"Porsche 911 Turbo se svım motorem (leatı šestiválec s protilehlımi válci)",  
		 "o vıkonu 300 kW (408 k), opatøenım dvìma turbodmychadly, zaujímá vrchol ve ",
		 "vırobním programu stuttgartské znaèky. Mezinápravovı diferenciál s vizkózní",
		 "spujkou rozdìluje podle momentálních rábìrovıch podmínek toèivı moment",
		 "motoru mezi obì pohánìné nápravy. Toto Turbo nabízí pøekvapivì dobré jízdní",
		 "vlastnosti a díky citlivému pøenosu hnací síly i dobré vıkony."},},
};
*/
#endif
