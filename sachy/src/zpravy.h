#ifndef zpravyH
#define zpravyH
/*
typy
int: 4 bytové celé èíslo
bool: 1 byte. 0 ne, jinak ano. "jinak" bude v¾dy 1.
utf8: øetìzec v kódování utf8 zakonèený nulou
*/

// od hráèe k serveru

// Chyba ètení nebo kontroly zprávy - odstøihnout hráèe, 0
#define ZPRAVA_ERROR 0
// Verze klienta, int
#define ZPRAVA_VERZE 1
// Nic, 0
#define ZPRAVA_ZIJU 2
// Pøihlá¹ení, rùzné >= 2, | utf8 login | utf8 heslo |
#define ZPRAVA_LOGIN 3
// bool bily, int s celkem, int s na tah
#define ZPRAVA_STOLEK 4
// int id_stolku, utf8 chat
#define ZPRAVA_CHAT 5
// Pøisednutí ke stolku
// int id_stolku, bool pozorovatel
#define ZPRAVA_PRISEDL 6
// Odsednutí od stolku
// int id_stolku
#define ZPRAVA_ODSEDL 7
// Tah
// int id_stolku, int tah ve formátu pha, vyu¾ity spodní 2 byty
#define ZPRAVA_TAH 8


// od serveru k hráèi

// Odpovìï na zprávu, 0
#define ZPRAVA_OK 0
// Odpovìï na zprávu VERZE, 0
#define ZPRAVA_SPATNA_VERZE 1
// Odpovìï na zprávu LOGIN,
// int id
// int poèet v¹ech nevyko¹tìných hráèù po úspì¹ném LOGIN vèetnì volajícího, n
// int poèet stolkù, m
// n * hráè [int id, utf8 login]
// m * stolek [int id, int bily, int cerny, int sCelkem, int sTah,
//     int nPrihlizi, [int idPrihlizi1 [, int idPrihlizi2 [, ...]]]]
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
// int id_stolku, int id_hrace, bool pozorovatel
#define ZPRAVA_STOLEK_PRISEDL 14
// Po odsednutí od stolku v¹em
// int id_stolku, int id_hrace
#define ZPRAVA_STOLEK_ODSEDL 15
// Tah
// int id_stolku, int tah ve formátu pha, vyu¾ity spodní 2 byty
#define ZPRAVA_STOLEK_TAH 16


#endif

