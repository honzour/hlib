#ifndef zpravyH
#define zpravyH
/*
typy
int: 4 bytov� cel� ��slo
bool: 1 byte. 0 ne, jinak ano. "jinak" bude v�dy 1.
utf8: �et�zec v k�dov�n� utf8 zakon�en� nulou
*/

// od hr��e k serveru

// Chyba �ten� nebo kontroly zpr�vy - odst�ihnout hr��e, 0
#define ZPRAVA_ERROR 0
// Verze klienta, int
#define ZPRAVA_VERZE 1
// Nic, 0
#define ZPRAVA_ZIJU 2
// P�ihl�en�, r�zn� >= 2, | utf8 login | utf8 heslo |
#define ZPRAVA_LOGIN 3
// bool bily, int s celkem, int s na tah
#define ZPRAVA_STOLEK 4
// int id_stolku, utf8 chat
#define ZPRAVA_CHAT 5
// P�isednut� ke stolku
// int id_stolku, bool pozorovatel
#define ZPRAVA_PRISEDL 6
// Odsednut� od stolku
// int id_stolku
#define ZPRAVA_ODSEDL 7
// Tah
// int id_stolku, int tah ve form�tu pha, vyu�ity spodn� 2 byty
#define ZPRAVA_TAH 8


// od serveru k hr��i

// Odpov�� na zpr�vu, 0
#define ZPRAVA_OK 0
// Odpov�� na zpr�vu VERZE, 0
#define ZPRAVA_SPATNA_VERZE 1
// Odpov�� na zpr�vu LOGIN,
// int id
// int po�et v�ech nevyko�t�n�ch hr��� po �sp�n�m LOGIN v�etn� volaj�c�ho, n
// int po�et stolk�, m
// n * hr�� [int id, utf8 login]
// m * stolek [int id, int bily, int cerny, int sCelkem, int sTah,
//     int nPrihlizi, [int idPrihlizi1 [, int idPrihlizi2 [, ...]]]]
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
// int id_stolku, int id_hrace, bool pozorovatel
#define ZPRAVA_STOLEK_PRISEDL 14
// Po odsednut� od stolku v�em
// int id_stolku, int id_hrace
#define ZPRAVA_STOLEK_ODSEDL 15
// Tah
// int id_stolku, int tah ve form�tu pha, vyu�ity spodn� 2 byty
#define ZPRAVA_STOLEK_TAH 16


#endif

