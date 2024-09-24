#ifndef _LANGUAGE_HEADER_
#define _LANGUAGE_HEADER_

#define LANGUAGE_ENGLISH

#ifndef LANGUAGE_ENGLISH
///////////////////////////////////////////////////////////////////////////////////////
// LANG_CZECH
///////////////////////////////////////////////////////////////////////////////////////

// Obecne
#define LNG_ZPET      "ZpÏt"
#define LNG_OK        "Ok"

#define LNG_LOADING "NaËÌt·m..."


// Hlavni menu
#define LNG_1HRAC     "JEDEN HR¡»"
#define LNG_SINGLE_L1 "Zahraj si s·m"
#define LNG_SINGLE_L2 "sÛlo"
#define LNG_2HRACI    "DVA HR¡»I"
#define LNG_DOUBLE_L1 "ZmÏ¯ si svÈ sÌly"
#define LNG_DOUBLE_L2 "s k·moöem"
#define LNG_NASTAVENI "NASTAVENÕ"
#define LNG_NASTAVENI_L1 "Nastav si ˙roveÚ grafick˝ch detail˘,"
#define LNG_NASTAVENI_L2 "zvuk˘ Ëi obtÌûnost hry"
#define LNG_AUTORI    "AUTOÿI"
#define LNG_AUTORI_L1    "PodÌvej se, kdo vlastnÏ stvo¯il tuto"
#define LNG_AUTORI_L2    "hru :-)"
#define LNG_KONEC     "KONEC"

// Menu treninku
#define LNG_VYBERAUTA "V˝bÏr auta"
#define LNG_MULTI     "MULTIPLAYER"
#define LNG_ZPETV     "ZPÃT"
#define LNG_STAR      "Start"
#define LNG_NASTAVENIZAVODU "NastavenÌ z·vodu"
#define LNG_NASTAVENIVYRAZOVACKY "NastavenÌ vy¯azovaËky"
#define LNG_TRIDA  "T¯Ìda %c"
#define LNG_DEN    "Den"
#define LNG_NOC    "Noc"
#define LNG_OPP_0  "Bez oponent˘"
#define LNG_OPP_1  "1 oponent"
#define LNG_OPP_24 "%d oponenti"
#define LNG_OPP    "%d oponent˘"
#define LNG_KOL_1  "1 kolo"
#define LNG_KOL_24 "%d kola"
#define LNG_KOL    "%d kol"

// Menu Single/Double
#define LNG_SINGLEPLAY "Jeden hr·Ë"
#define LNG_DOUBLEPLAY "Dva hr·Ëi"
#define LNG_KARIERA   "KARI…RA"
#define LNG_KARIERA_L1   "VydÏl·vej penÌze, kupuj nov· auta"
#define LNG_KARIERA_L2   "a staÚ se nejlepöÌm"
#define LNG_TRENINK   "TR…NINK"
#define LNG_TRENINK_L1   "Vyber si traù, zvol si poËet soupe¯˘"
#define LNG_TRENINK_L2   "a trÈnuj na kariÈru"
#define LNG_POHAR     "POH¡R"
#define LNG_POHAR_L1   "Vyber si auto, zvol si poËet soupe¯˘"
#define LNG_POHAR_L2   "a poh·ruj"
#define LNG_DUEL     "DUEL"
#define LNG_DUEL_L1   "Z·voÔte s kamar·dem vaöimi auty"
#define LNG_DUEL_L2   "z kariÈry"
#define LNG_VYRAZOVACKA     "VYÿAZOVA»KA"
#define LNG_VYRAZOVACKA_L1  "V û·dnÈm kole nesmÌte dojet"
#define LNG_VYRAZOVACKA_L2  "jako poslednÌ"

// Menu nastaveni
#define LNG_NASTAVENI_H  "NastavenÌ"
#define LNG_NAS_1HRAC    "1.hr·Ë"
#define LNG_NAS_2HRAC    "2.hr·Ë"
#define LNG_HUDBA        "Hudba"
#define LNG_ZVUKY        "Zvuky" 
#define LNG_STINY        "StÌny"
#define LNG_STINIS       "statickÈ"
#define LNG_STINID       "plovoucÌ"
#define LNG_SVETLA       "SvÏtla"
#define LNG_SVETLAS      "statick·"
#define LNG_SVETLAD      "plovoucÌ"
#define LNG_SCREEN       "RozliöenÌ"
#define LNG_SCREEN640    "640X480"
#define LNG_SCREEN800    "800X600"
#define LNG_NORMALNI     "norm·lnÌ"
#define LNG_GAME_SPEED   "Rychlost hry"
#define LNG_OBTIZNOST    "ObtÌûnost"
const char LNG_DOBTIZNOST[5][30] = 
{
	"primitivnÌ",
	"snadn·",
	"norm·lnÌ",
	"tÏûöÌ",
	"n·roËn·",
};



// menu kariera
#define LNG_KARIERAV        "KariÈra"
#define LNG_VYBERZAVODU     "V›BÃR Z¡VODU"
#define LNG_VYBERZAVODU_L1  "Vyber si z·vod Ëi poh·r"
#define LNG_VYBERZAVODU_L2  "a vydÏl·vej penÌze"
#define LNG_KOUPEVOZU       "KOUPÃ NOV…HO VOZU"
#define LNG_KOUPEVOZU_L1    "Prodej sv˘j star˝ v˘z a kup si"
#define LNG_KOUPEVOZU_L2    "nov˝, lepöÌ, rychlejöÌ"
#define LNG_SERVIS          "AUTOSERVIS"
#define LNG_SERVIS_L1       "Autoservis ti umoûÚuje upgradovat"
#define LNG_SERVIS_L2       "jednotlivÈ Ë·sti vozu"
#define LNG_HISTORIE        "HISTORIE"
#define LNG_HISTORIE_L1     "Statistika, jak byl kter˝"
#define LNG_HISTORIE_L2     "tv˘j z·vod ˙spÏön˝"
#define LNG_KONEC           "KONEC"
#define LNG_KONEC_L1        "UloûÌ kariÈru a potÈ ji ukonËÌ"

// menu pohar
#define LNG_TRAT_1          "1 traù"
#define LNG_TRAT_24         "%d tratÏ"
#define LNG_TRATE           "%d tratÌ"
#define LNG_VYBERAUTA_POHAR "V˝bÏr auta pro poh·r"
#define LNG_ZAVOD_CISLO     "Z·vod Ë.%d"
#define LNG_VYSLEDKY_POHARU "V˝sledky poh·rovÈho utk·nÌ"

// Autoservis
#define LNG_AUTOSERVIS "Autoservis"
#define LNG_KOUPIT     "KOUPIT"
#define LNG_UPGRADE    "UPGRADE"
#define LNG_CZK        "KË"
#define LNG_CENA       "Cena: %d KË"
#define LNG_PENIZE     "PenÌze: %d KË"
#define LNG_UPGR_BRZDY "Brzdy t¯Ìdy %.1f"
#define LNG_UPGR_ACC  "ZrychlenÌ %.1f s"
#define LNG_UPGR_SPEED "Maxim·lnÌ rychlost %.0f km/h"
#define LNG_UPGR_TURN "Zat·ËenÌ %.1f"


#ifndef MAX_UPGRADES
#define MAX_UPGRADES 7
#endif

const char utext[MAX_UPGRADES*4][30] = 
{
	"MOTOR", "Motor zlepöuje", "zrychlenÌ a", "maxim·lnÌ rychlost",
	"ZAT¡»ENÕ", "Zlepöuje zat·ËenÌ", "a tÌm i ovladatelnost", "vozidla",
	"PÿEDNÕ PNEU", "P¯ednÌ pneu zlepöuje", "zrychlenÌ, zat·ËenÌ a", "maxim·lnÌ rychlost.",
	"ZADNÕ PNEU", "ZadnÌ pneu zlepöuje", "zrychlenÌ, zat·ËenÌ a", "maxim·lnÌ rychlost.",
	"BRZDY", "Zv˝öenÌ ˙Ëinku", "a tÌm zkr·cenÌ", "brzdnÈ dr·hy.",
	"PODVOZEK", "VylepöenÌ podvozku", "m· vliv na vÏtöinu", "vlastnostÌ vozidla.",
	"PÿEVODOVKA", "LepöÌ p¯evodovka", "zajiöùuje zejmÈna", "lepöÌ akceleraci.",
};
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


// Autori
#define LNG_AUTORI_H "Auto¯i"
#define LNG_BERNY    "BERNARD LIDICK› - BERNY"
#define LNG_BERNY_L1    "program·tor - berny@mujmejl.cz"
#define LNG_BERNY_L2    "popoh·nÏË grafika do pr·ce"
#define LNG_BEZA     "ZDENÃK B÷SWART - BEZA"
#define LNG_BEZA_L1   "grafik - 2zdeny@seznam.cz"
#define LNG_BEZA_L2   "popoh·nÏË program·tora do pr·ce"
#define LNG_HIPPO     "HIPPO GAMES - 2001"
#define LNG_HIPPO_L1  "www.nipax.cz/hippo"

// koupe vozu
#define LNG_KOUPE_VOZU "KoupÏ novÈho vozu"
#define LNG_KOUPIT_VUZ "Koupit"
#define LNG_PRODAT_VUZ "Prodat"
#define LNG_MAXSPEED   "Maxim·lnÌ rychlost"
#define LNG_ZRYCHLENI  "ZrychlenÌ"
#define LNG_ZATACENI   "Zat·ËenÌ"

// Zavody v kariere
#define LNG_VYBERZAVODUKARIERY "V˝bÏr z·vodu"
#define LNG_STARTOVNE "StartovnÈ %d KË"
#define LNG_DOTACE    "Dotace %d KË"
const char uobtiznost[3][30] = 
{
	"amatÈ¯i",
	"poloprofi",
	"profi",
};

// Historie
#define LNG_HIST_DOKONCEN   "DokonËen: %d kr·t"
#define LNG_HIST_VITEZSTVI  "VÌtÏzstvÌ: %d"
#define LNG_HIST_NEJLIP     "NejlepsÌ umÌstÏnÌ: %d"
#define LNG_HIST_PRUMPORADI "Pr˘mÏrnÈ po¯adÌ: %d"
#define LNG_HIST_NADPIS     "Historie hr·Ëe"
#define LNG_HIST_BODY       "Bod˘ %d"

// GRun
#define LNG_UKONCENI      "Pro ukonËenÌ stisknÏte Esc."
#define LNG_OPRAVDUKONEC  "Opravdu konec?"
#define LNG_AN            "A/N"
#define LNG_RUN_PORADI    "POÿADÕ"
#define LNG_RUN_JMENOAUTO "JM…NO/AUTO"
#define LNG_RUN_MAX       "MAX"
#define LNG_RUN_PRUMERNA  "PRŸMÃRN¡"
#define LNG_RUN_RYCHLOST  "RYCHLOST"
#define LNG_RUN_CAS       "»AS"

#else // LANG_ENGLISH 

///////////////////////////////////////////////////////////////////////////////////////
// LANG_ENGLISH
///////////////////////////////////////////////////////////////////////////////////////

// Obecne
#define LNG_ZPET      "Back"
#define LNG_OK        "Ok"


// Hlavni menu
#define LNG_1HRAC     "ONE PLAYER"
#define LNG_SINGLE_L1 "Play your own"
#define LNG_SINGLE_L2 "game"
#define LNG_2HRACI    "TWO PLAYERS"
#define LNG_DOUBLE_L1 "Compare your skills"
#define LNG_DOUBLE_L2 "with your friend"
#define LNG_NASTAVENI "SETTINGS"
#define LNG_NASTAVENI_L1 "Change graphics,"
#define LNG_NASTAVENI_L2 "sound or difficulty"
#define LNG_AUTORI    "CREDITS"
#define LNG_AUTORI_L1    "Look, who made this"
#define LNG_AUTORI_L2    "excellent game :-)"
#define LNG_KONEC     "END"

// Menu treninku
#define LNG_VYBERAUTA "Car selection"
#define LNG_MULTI     "MULTIPLAYER"
#define LNG_ZPETV     "BACK"
#define LNG_STAR      "Start"
#define LNG_NASTAVENIZAVODU "Race settings"
#define LNG_NASTAVENIVYRAZOVACKY "Last-out settings"
#define LNG_TRIDA  "Class %c"
#define LNG_DEN    "Day"
#define LNG_NOC    "Night"
#define LNG_OPP_0  "No opponents"
#define LNG_OPP_1  "1 oponent"
#define LNG_OPP_24 "%d opponents"
#define LNG_OPP    "%d opponents"
#define LNG_KOL_1  "1 lap"
#define LNG_KOL_24 "%d laps"
#define LNG_KOL    "%d laps"

// Menu Single/Double
#define LNG_SINGLEPLAY "One player"
#define LNG_DOUBLEPLAY "Two players"
#define LNG_KARIERA   "CAREER"
#define LNG_KARIERA_L1   "Earn money, buy cars"
#define LNG_KARIERA_L2   "and become the best"
#define LNG_TRENINK   "PRACTICE"
#define LNG_TRENINK_L1   "Choose tack, opponents"
#define LNG_TRENINK_L2   "and train for career"
#define LNG_POHAR     "CUP"
#define LNG_POHAR_L1   "Choose car, opponents"
#define LNG_POHAR_L2   "and play a small cup"
#define LNG_DUEL     "DUEL"
#define LNG_DUEL_L1   "Play with your"
#define LNG_DUEL_L2   "cars from career"
#define LNG_VYRAZOVACKA     "LAST-OUT"
#define LNG_VYRAZOVACKA_L1  "You must not be the"
#define LNG_VYRAZOVACKA_L2  "last one in any round"

// Menu nastaveni
#define LNG_NASTAVENI_H  "Settings"
#define LNG_NAS_1HRAC    "1st player"
#define LNG_NAS_2HRAC    "2nd player"
#define LNG_HUDBA        "Music"
#define LNG_ZVUKY        "Sound" 
#define LNG_STINY        "Shadows"
#define LNG_STINIS       "static"
#define LNG_STINID       "dynamic"
#define LNG_SVETLA       "Lights"
#define LNG_SVETLAS      "static"
#define LNG_SVETLAD      "dynamic"
#define LNG_SCREEN       "Resolution"
#define LNG_SCREEN640    "640X480"
#define LNG_SCREEN800    "800X600"
#define LNG_NORMALNI     "Normal"
#define LNG_GAME_SPEED   "Game speed"
#define LNG_OBTIZNOST    "Difficulty"
const char LNG_DOBTIZNOST[5][30] = 
{
	"very easy",
	"easy",
	"normal",
	"hard",
	"very hard",
};



// menu kariera
#define LNG_KARIERAV        "Career"
#define LNG_VYBERZAVODU     "RACE"
#define LNG_VYBERZAVODU_L1  "Choose race and"
#define LNG_VYBERZAVODU_L2  "and earn money"
#define LNG_KOUPEVOZU       "BUY NEW CAR"
#define LNG_KOUPEVOZU_L1    "You must win all"
#define LNG_KOUPEVOZU_L2    "for next category"
#define LNG_SERVIS          "CAR SERVICE"
#define LNG_SERVIS_L1       "upgrade"
#define LNG_SERVIS_L2       "parts of your car"
#define LNG_HISTORIE        "HISTORY"
#define LNG_HISTORIE_L1     "About your successes"
#define LNG_HISTORIE_L2     "and losses"
#define LNG_KONEC           "END"
#define LNG_KONEC_L1        "Saves career and quits"

// menu pohar
#define LNG_TRAT_1          "1 track"
#define LNG_TRAT_24         "%d tracks"
#define LNG_TRATE           "%d tracks"
#define LNG_VYBERAUTA_POHAR "Choose cup car"
#define LNG_ZAVOD_CISLO     "Race no.%d"
#define LNG_VYSLEDKY_POHARU "Results of competition"

// Autoservis
#define LNG_AUTOSERVIS "Car service"
#define LNG_KOUPIT     "BUY"
#define LNG_UPGRADE    "UPGRADE"
#define LNG_CENA       "Prize: %d CZK"
#define LNG_PENIZE     "Money: %d CZK"
#define LNG_UPGR_BRZDY "Breaks class %.1f"
#define LNG_UPGR_ACC  "Acceleration %.1f s"
#define LNG_UPGR_SPEED "Maximum speed %.0f km/h"
#define LNG_UPGR_TURN "Turning %.1f"


#ifndef MAX_UPGRADES
#define MAX_UPGRADES 7
#endif

const char utext[MAX_UPGRADES*4][30] = 
{
	"ENGINE", "Engine increases", "max speed and", "acceleration",
	"TURNING", "Enhances turning", "and control", "of vehicle",
	"FRONT TIRES", "Front tires enhance", "acceleration, max", "speed,...",
	"BACK TIRES", "Back tires enhance", "acceleration, max", "speed,...",
	"BREAKS", "Decreases vehicle", "stopping", "distance.",
	"CHASIS", "Better char chasis", "influences allover", "the vehicle.",
	"GEARBOX", "Better gearbox", "has impact mainly", "on acceleration.",
};
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


// Autori
#define LNG_AUTORI_H "Credits"
#define LNG_BERNY    "BERNARD LIDICKY - BERNY"
#define LNG_BERNY_L1    "programmer - bernard@alderan.cz"
#define LNG_BERNY_L2    "motivator for the graphic designer"
#define LNG_BEZA     "ZDENEK BOSWART - BEZA"
#define LNG_BEZA_L1   "graphic designer - 2zdeny@seznam.cz"
#define LNG_BEZA_L2   "motivator for the programmer"
#define LNG_HIPPO     "HIPPO GAMES - 2001-2021"
#define LNG_HIPPO_L1  "hippo.nipax.cz"

// koupe vozu
#define LNG_KOUPE_VOZU "Buy new car"
#define LNG_KOUPIT_VUZ "Buy"
#define LNG_PRODAT_VUZ "Sell"
#define LNG_MAXSPEED   "Maximum speed"
#define LNG_ZRYCHLENI  "Acceleration"
#define LNG_ZATACENI   "Turning"

// Zavody v kariere
#define LNG_VYBERZAVODUKARIERY "Race selection"
#define LNG_CZK       "CZK"
#define LNG_STARTOVNE "Start %d CZK"
#define LNG_DOTACE    "Grant %d CZK"
const char uobtiznost[3][30] = 
{
	"amateurs",
	"semiprofessional",
	"professional",
};

// Historie
#define LNG_HIST_DOKONCEN   "Finished: %d times"
#define LNG_HIST_VITEZSTVI  "Victories: %d"
#define LNG_HIST_NEJLIP     "Best place: %d"
#define LNG_HIST_PRUMPORADI "Average place: %d"
#define LNG_HIST_NADPIS     "Player history"
#define LNG_HIST_BODY       "Points %d"

// GRun
#define LNG_UKONCENI      "Press Esc"
#define LNG_OPRAVDUKONEC  "Really end race?"
#define LNG_AN            "Y/N"
#define LNG_RUN_PORADI    "POSITION"
#define LNG_RUN_JMENOAUTO "NAME/CAR"
#define LNG_RUN_MAX       "MAX"
#define LNG_RUN_PRUMERNA  "AVERAGE"
#define LNG_RUN_RYCHLOST  "SPEED"
#define LNG_RUN_CAS       "TIME"

#define LNG_LOADING "Loading..."


#endif // LANG_ENGLISH
#endif
