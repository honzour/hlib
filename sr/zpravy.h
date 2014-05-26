#ifndef zpravyH
#define zpravyH
/*
typy
int: 4 bytové celé èíslo
bool: 1 byte. 0 ne, jinak ano. "jinak" bude v¾dy 1.
utf8: øetìzec v kódování utf8 zakonèený nulou
*/

// od hráèe k serveru

/**
 Chyba ètení nebo kontroly zprávy - odstøihnout hráèe, 0
 @see ::ZpracujError()
*/
#define ZPRAVA_ERROR 0
/**
 Verze klienta, int
*/
#define ZPRAVA_VERZE 1
/** Nic, 0
*/
#define ZPRAVA_ZIJU 2
/** Pøihlá¹ení, rùzné >= 2, | utf8 login | utf8 heslo |
*/
#define ZPRAVA_LOGIN 3
/** bool bily, int s celkem, int s na tah
*/
#define ZPRAVA_STOLEK 4
/** int id_stolku, utf8 chat
*/
#define ZPRAVA_CHAT 5
/** Pøisednutí ke stolku
 int id_stolku, bool pozorovatel
*/
#define ZPRAVA_PRISEDL 6
// Odsednutí od stolku
// int id_stolku
#define ZPRAVA_ODSEDL 7
// Tah
// int id_stolku, int tah ve formátu pha, vyu¾ity spodní 2 byty, int cas (toho kdo tahl, po tahu, v ms)
#define ZPRAVA_TAH 8
// Pøijetí nebo navrhnutí remízy
// int id_stolku
#define ZPRAVA_NAVRHUJI_REMIS 9
// Vzdání
// int id_stolku
#define ZPRAVA_VZDAVAM 10
// Pøesednutí u stolku s jedním hráèem
// int id_stolku
#define ZPRAVA_PRESEDAVAM 11
// Potvrzení zaèátku partie hráèem
// int id_stolku
#define ZPRAVA_START 12
// Hráèi spadl praporek
// ind id_stolku
#define ZPRAVA_SPADL_JSEM 13

// od serveru k hráèi

// Odpovìï na zprávu, 0
#define ZPRAVA_OK 0
// Odpovìï na zprávu VERZE, 0
#define ZPRAVA_SPATNA_VERZE 1
/** Odpovìï na zprávu LOGIN,
 int id
 int poèet v¹ech nevyko¹tìných hráèù po úspì¹ném LOGIN vèetnì volajícího, n
 int poèet stolkù, m
 n * hráè [int id, utf8 login]
 m * stolek [int id, int bily, int cerny, int sCelkem, int sTah,
     int nPrihlizi, [int idPrihlizi1 [, int idPrihlizi2 [, ...]]]]
*/
#define ZPRAVA_LOGIN_OK 2
// Odpovìï na zprávu LOGIN
#define ZPRAVA_LOGIN_ERROR 3
// Chyba klienta, 0
#define ZPRAVA_KLIENT_ERROR 4
// Interní chyba, 0
#define ZPRAVA_INTERNAL_ERROR 5
// Loguje se podruhé
#define ZPRAVA_LOGIN_DUPLICITA 6
// int odesilatel, int stolek, utf8 chat
#define ZPRAVA_CHAT_OK 7
// Odpovìï na zprávu ZIJU, 0
#define ZPRAVA_ZIJU_OK 8
// Odpovìï na zprávu, 0
#define ZPRAVA_VERZE_OK 9
// Po úspì¹ném zpracování ZPRAVA_LOGIN v¹em kromì právì pøihlá¹eného
// int id, utf8 login
#define ZPRAVA_NEKDO_LOGIN 10
// Po odhlá¹ení hráèe (jakoukoli formou) v¹em kromì právì odhlá¹eného.
// Klient si automaticky domyslí odsednutí odhla¹ovaného hráèe od v¹ech stolkù,
// i kdy¾ nebyla zpráva ZPRAVA_STOLEK_ODSEDL explicitnì poslána.
// Pokud klient hraje, bude pøedem poslána zpráva s koncem partie.
// int id
#define ZPRAVA_NEKDO_LOGOUT 11
// Po vytvoøení stolku v¹em
// int id, int bily, int cerny, int sCelkem, int sTah,
//     int nPrihlizi, [int idPrihlizi1 [, int idPrihlizi2 [, ...]]]
#define ZPRAVA_STOLEK_NOVY 12
// Pøi ru¹ení stolku v¹em
// int id
//#define ZPRAVA_STOLEK_ZRUSEN 13
// Po pøisednutí ke stolku v¹em
// int id_stolku, int id_hrace, int bCasMs, int cCasMs, bool pozorovatel
#define ZPRAVA_STOLEK_PRISEDL 14
// Po odsednutí od stolku v¹em
// int id_stolku, int id_hrace
#define ZPRAVA_STOLEK_ODSEDL 15
// Tah
// int id_stolku, int tah ve formátu pha, vyu¾ity spodní 2 byty, int cas (toho kdo tahl, po tahu, v ms)
#define ZPRAVA_STOLEK_TAH 16
// Pøijetí nebo navrhnutí remízy
// int id_stolku, int id_hrace
#define ZPRAVA_STOLEK_NAVRHUJE_REMIS 17
// Konec partie
// int id_stolku, int -1 (èerný vyhrál), 0 (remis), 1 (bílý vyhrál), int výsledek podrobnì
#define ZPRAVA_STOLEK_KONEC_PARTIE 18
// Pøesednutí
// int id_stolku
#define ZPRAVA_STOLEK_PRESEDNUTI 19
// Potvrzení zaèátku partie hráèem
// int id_stolku, int id_hrace
#define ZPRAVA_STOLEK_START 20
// Poté, co pøisedl pøihlí¾enící se po¹lou tahy (pokud se je¹tì netáhlo, mù¾e, ale nemusí se mu ta 0 poslat)
// int id_stolku, int poèet tahù, int t0, int t1, int t2, ... int tn
#define ZPRAVA_STOLEK_TAHY_PRIHLIZEJICIMU 21


// Podrobnì výsledek partie
// A¾ do kódu 7 kompatibilní se sachylib (v kódu se na to spoléhám)
#define KONEC_NENI 0
#define KONEC_BILY_V_MATU 1
#define KONEC_CERNY_V_MATU 2
// 3 x opakování pozice
#define KONEC_REMIS_3 3
// 50 tahù bez nevratných zmìn
#define KONEC_REMIS_50 4
#define KONEC_REMIS_BILY_V_PATU 5
#define KONEC_REMIS_CERNY_V_PATU 6
// Napø. 2 králové
#define KONEC_REMIS_MALO_MATERIALU 7
// Bílý spadnul na èas, ale èerný nemá matící materiál
#define KONEC_PRAPOREK_BILEHO_REMIS 8
#define KONEC_PRAPOREK_CERNEHO_REMIS 9
#define KONEC_PRAPOREK_BILEHO_PROHRA 10
#define KONEC_PRAPOREK_CERNEHO_PROHRA 11
#define KONEC_BILY_VZDAL 12
#define KONEC_CERNY_VZDAL 13
// èerný navrhl remízu a bílý pøijal
#define KONEC_REMIS_BILY_PRIJAL 14
#define KONEC_REMIS_CERNY_PRIJAL 15
// Jednomu spadlo spojení. Stejné, jako by spadnul praporek
#define KONEC_SPADNUTI_BILEHO_REMIS 16
#define KONEC_SPADNUTI_CERNEHO_REMIS 17
#define KONEC_SPADNUTI_BILEHO_PROHRA 18
#define KONEC_SPADNUTI_CERNEHO_PROHRA 19
// Výhry a remízy a zru¹ení partie z moci úøední.
#define KONEC_BILY_VYHRAL_JINAK 100
#define KONEC_CERNY_VYHRAL_JINAK 101
#define KONEC_REMIS_JINAK 102
#define KONEC_ZRUSENO 103
#endif

