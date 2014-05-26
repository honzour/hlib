#ifndef zpravyH
#define zpravyH
/*
typy
int: 4 bytov� cel� ��slo
bool: 1 byte. 0 ne, jinak ano. "jinak" bude v�dy 1.
utf8: �et�zec v k�dov�n� utf8 zakon�en� nulou
*/

// od hr��e k serveru

/**
 Chyba �ten� nebo kontroly zpr�vy - odst�ihnout hr��e, 0
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
/** P�ihl�en�, r�zn� >= 2, | utf8 login | utf8 heslo |
*/
#define ZPRAVA_LOGIN 3
/** bool bily, int s celkem, int s na tah
*/
#define ZPRAVA_STOLEK 4
/** int id_stolku, utf8 chat
*/
#define ZPRAVA_CHAT 5
/** P�isednut� ke stolku
 int id_stolku, bool pozorovatel
*/
#define ZPRAVA_PRISEDL 6
// Odsednut� od stolku
// int id_stolku
#define ZPRAVA_ODSEDL 7
// Tah
// int id_stolku, int tah ve form�tu pha, vyu�ity spodn� 2 byty, int cas (toho kdo tahl, po tahu, v ms)
#define ZPRAVA_TAH 8
// P�ijet� nebo navrhnut� rem�zy
// int id_stolku
#define ZPRAVA_NAVRHUJI_REMIS 9
// Vzd�n�
// int id_stolku
#define ZPRAVA_VZDAVAM 10
// P�esednut� u stolku s jedn�m hr��em
// int id_stolku
#define ZPRAVA_PRESEDAVAM 11
// Potvrzen� za��tku partie hr��em
// int id_stolku
#define ZPRAVA_START 12
// Hr��i spadl praporek
// ind id_stolku
#define ZPRAVA_SPADL_JSEM 13

// od serveru k hr��i

// Odpov�� na zpr�vu, 0
#define ZPRAVA_OK 0
// Odpov�� na zpr�vu VERZE, 0
#define ZPRAVA_SPATNA_VERZE 1
/** Odpov�� na zpr�vu LOGIN,
 int id
 int po�et v�ech nevyko�t�n�ch hr��� po �sp�n�m LOGIN v�etn� volaj�c�ho, n
 int po�et stolk�, m
 n * hr�� [int id, utf8 login]
 m * stolek [int id, int bily, int cerny, int sCelkem, int sTah,
     int nPrihlizi, [int idPrihlizi1 [, int idPrihlizi2 [, ...]]]]
*/
#define ZPRAVA_LOGIN_OK 2
// Odpov�� na zpr�vu LOGIN
#define ZPRAVA_LOGIN_ERROR 3
// Chyba klienta, 0
#define ZPRAVA_KLIENT_ERROR 4
// Intern� chyba, 0
#define ZPRAVA_INTERNAL_ERROR 5
// Loguje se podruh�
#define ZPRAVA_LOGIN_DUPLICITA 6
// int odesilatel, int stolek, utf8 chat
#define ZPRAVA_CHAT_OK 7
// Odpov�� na zpr�vu ZIJU, 0
#define ZPRAVA_ZIJU_OK 8
// Odpov�� na zpr�vu, 0
#define ZPRAVA_VERZE_OK 9
// Po �sp�n�m zpracov�n� ZPRAVA_LOGIN v�em krom� pr�v� p�ihl�en�ho
// int id, utf8 login
#define ZPRAVA_NEKDO_LOGIN 10
// Po odhl�en� hr��e (jakoukoli formou) v�em krom� pr�v� odhl�en�ho.
// Klient si automaticky domysl� odsednut� odhla�ovan�ho hr��e od v�ech stolk�,
// i kdy� nebyla zpr�va ZPRAVA_STOLEK_ODSEDL explicitn� posl�na.
// Pokud klient hraje, bude p�edem posl�na zpr�va s koncem partie.
// int id
#define ZPRAVA_NEKDO_LOGOUT 11
// Po vytvo�en� stolku v�em
// int id, int bily, int cerny, int sCelkem, int sTah,
//     int nPrihlizi, [int idPrihlizi1 [, int idPrihlizi2 [, ...]]]
#define ZPRAVA_STOLEK_NOVY 12
// P�i ru�en� stolku v�em
// int id
//#define ZPRAVA_STOLEK_ZRUSEN 13
// Po p�isednut� ke stolku v�em
// int id_stolku, int id_hrace, int bCasMs, int cCasMs, bool pozorovatel
#define ZPRAVA_STOLEK_PRISEDL 14
// Po odsednut� od stolku v�em
// int id_stolku, int id_hrace
#define ZPRAVA_STOLEK_ODSEDL 15
// Tah
// int id_stolku, int tah ve form�tu pha, vyu�ity spodn� 2 byty, int cas (toho kdo tahl, po tahu, v ms)
#define ZPRAVA_STOLEK_TAH 16
// P�ijet� nebo navrhnut� rem�zy
// int id_stolku, int id_hrace
#define ZPRAVA_STOLEK_NAVRHUJE_REMIS 17
// Konec partie
// int id_stolku, int -1 (�ern� vyhr�l), 0 (remis), 1 (b�l� vyhr�l), int v�sledek podrobn�
#define ZPRAVA_STOLEK_KONEC_PARTIE 18
// P�esednut�
// int id_stolku
#define ZPRAVA_STOLEK_PRESEDNUTI 19
// Potvrzen� za��tku partie hr��em
// int id_stolku, int id_hrace
#define ZPRAVA_STOLEK_START 20
// Pot�, co p�isedl p�ihl�en�c� se po�lou tahy (pokud se je�t� net�hlo, m��e, ale nemus� se mu ta 0 poslat)
// int id_stolku, int po�et tah�, int t0, int t1, int t2, ... int tn
#define ZPRAVA_STOLEK_TAHY_PRIHLIZEJICIMU 21


// Podrobn� v�sledek partie
// A� do k�du 7 kompatibiln� se sachylib (v k�du se na to spol�h�m)
#define KONEC_NENI 0
#define KONEC_BILY_V_MATU 1
#define KONEC_CERNY_V_MATU 2
// 3 x opakov�n� pozice
#define KONEC_REMIS_3 3
// 50 tah� bez nevratn�ch zm�n
#define KONEC_REMIS_50 4
#define KONEC_REMIS_BILY_V_PATU 5
#define KONEC_REMIS_CERNY_V_PATU 6
// Nap�. 2 kr�lov�
#define KONEC_REMIS_MALO_MATERIALU 7
// B�l� spadnul na �as, ale �ern� nem� mat�c� materi�l
#define KONEC_PRAPOREK_BILEHO_REMIS 8
#define KONEC_PRAPOREK_CERNEHO_REMIS 9
#define KONEC_PRAPOREK_BILEHO_PROHRA 10
#define KONEC_PRAPOREK_CERNEHO_PROHRA 11
#define KONEC_BILY_VZDAL 12
#define KONEC_CERNY_VZDAL 13
// �ern� navrhl rem�zu a b�l� p�ijal
#define KONEC_REMIS_BILY_PRIJAL 14
#define KONEC_REMIS_CERNY_PRIJAL 15
// Jednomu spadlo spojen�. Stejn�, jako by spadnul praporek
#define KONEC_SPADNUTI_BILEHO_REMIS 16
#define KONEC_SPADNUTI_CERNEHO_REMIS 17
#define KONEC_SPADNUTI_BILEHO_PROHRA 18
#define KONEC_SPADNUTI_CERNEHO_PROHRA 19
// V�hry a rem�zy a zru�en� partie z moci ��edn�.
#define KONEC_BILY_VYHRAL_JINAK 100
#define KONEC_CERNY_VYHRAL_JINAK 101
#define KONEC_REMIS_JINAK 102
#define KONEC_ZRUSENO 103
#endif

