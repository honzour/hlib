package cz.sachy.util;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.Reader;
import java.io.Writer;

import cz.sachy.struktury.Global;


public class Zprava {
  
  /// Od hr��e k serveru
  
//Chyba �ten� nebo kontroly zpr�vy - odst�ihnout hr��e, 0
  public static final int ZPRAVA_ERROR = 0;
//   Verze klienta, int
  public static final int ZPRAVA_VERZE = 1;
//   Nic, 0
  public static final int ZPRAVA_ZIJU = 2;
//   P�ihl�en�, r�zn� >= 2, | utf8 login | utf8 heslo |
  public static final int ZPRAVA_LOGIN = 3;
//   bool bily, int s celkem, int s na tah
  public static final int ZPRAVA_STOLEK = 4;
//   int id_stolku, utf8 chat
  public static final int ZPRAVA_CHAT = 5;
//   P�isednut� ke stolku
//   int id_stolku, bool pozorovatel
  public static final int ZPRAVA_PRISEDL = 6;
//   Odsednut� od stolku
//   int id_stolku
  public static final int ZPRAVA_ODSEDL = 7;
//   Tah
//   int id_stolku, int tah ve form�tu pha, vyu�ity spodn� 2 byty, , int cas (toho kdo tahl, po tahu, v ms)
  public static final int ZPRAVA_TAH = 8;
//P�ijet� nebo navrhnut� rem�zy
//int id_stolku
  public static final int ZPRAVA_NAVRHUJI_REMIS = 9;
//Vzd�n�
//int id_stolku
  public static final int ZPRAVA_VZDAVAM = 10;
//P�esednut� u stolku s jedn�m hr��em
//int id_stolku
  public static final int ZPRAVA_PRESEDAVAM = 11;
//Potvrzen� za��tku partie hr��em
//int id_stolku
  public static final int ZPRAVA_START = 12;
//Hr��i spadl praporek
//ind id_stolku
  public static final int ZPRAVA_SPADL_JSEM = 13;


  
  /// Od serveru k hr��i
  
  public static final int ZPRAVA_OK = 0;
//Odpov�� na zpr�vu VERZE, 0
  public static final int ZPRAVA_SPATNA_VERZE = 1;
//Odpov�� na zpr�vu LOGIN,
//int id
//int po�et v�ech nevyko�t�n�ch hr��� po �sp�n�m LOGIN v�etn� volaj�c�ho, n
//int po�et stolk�, m
//n * hr�� [int id, utf8 login]
//m * stolek [int id, int bily, int cerny, int sCelkem, int sTah,
//    int nPrihlizi, [int idPrihlizi1 [, int idPrihlizi2 [, ...]]]]
  public static final int ZPRAVA_LOGIN_OK = 2;
//Odpov�� na zpr�vu LOGIN
  public static final int ZPRAVA_LOGIN_ERROR = 3;
//Chyba klienta, 0
  public static final int ZPRAVA_KLIENT_ERROR = 4;
//Intern� chyba, 0
  public static final int ZPRAVA_INTERNAL_ERROR = 5;
//Loguje se podruh�
  public static final int ZPRAVA_LOGIN_DUPLICITA = 6;
//int odesilatel, int stolek, utf8 chat
  public static final int ZPRAVA_CHAT_OK = 7;
//Odpov�� na zpr�vu ZIJU, 0
  public static final int ZPRAVA_ZIJU_OK = 8;
                               
  public static final int  ZPRAVA_VERZE_OK = 9;
//Po �sp�n�m zpracov�n� ZPRAVA_LOGIN v�em krom� pr�v� p�ihl�en�ho
//int id, utf8 login
  public static final int ZPRAVA_NEKDO_LOGIN = 10;
//Po odhl�en� hr��e (jakoukoli formou) v�em krom� pr�v� odhl�en�ho.
//Klient si automaticky domysl� odsednut� odhla�ovan�ho hr��e od v�ech stolk�,
//i kdy� nebyla zpr�va ZPRAVA_STOLEK_ODSEDL explicitn� posl�na.
//int id
  public static final int ZPRAVA_NEKDO_LOGOUT = 11;
//Po vytvo�en� stolku v�em
//int id, int bily, int cerny, int sCelkem, int sTah,
//  int nPrihlizi, [int idPrihlizi1 [, int idPrihlizi2 [, ...]]]
  public static final int ZPRAVA_STOLEK_NOVY = 12;
//P�i ru�en� stolku v�em
//int id
//#define ZPRAVA_STOLEK_ZRUSEN 13
//Po p�isednut� ke stolku v�em
//int id_stolku, int id_hrace, int bCasMs, int cCasMs, bool pozorovatel
  public static final int ZPRAVA_STOLEK_PRISEDL = 14;
//Po odsednut� od stolku v�em
//int id_stolku, int id_hrace
  public static final int ZPRAVA_STOLEK_ODSEDL = 15;
//Tah
//int id_stolku, int tah ve form�tu pha, vyu�ity spodn� 2 byty, int cas (toho kdo tahl, po tahu, v ms)
  public static final int ZPRAVA_STOLEK_TAH = 16;
//P�ijet� nebo navrhnut� rem�zy
//int id_stolku, int id_hrace
  public static final int ZPRAVA_STOLEK_NAVRHUJE_REMIS = 17;
//Konec partie
//int id_stolku, int -1 (�ern� vyhr�l), 0 (remis), 1 (b�l� vyhr�l), int v�sledek podrobn�
  public static final int ZPRAVA_STOLEK_KONEC_PARTIE = 18;
//P�esednut�
//int id_stolku
  public static final int ZPRAVA_STOLEK_PRESEDNUTI = 19;
// Potvrzen� za��tku partie hr��em
//int id_stolku, int id_hrace
  public static final int ZPRAVA_STOLEK_START = 20;
//Pot�, co p�isedl p�ihl�en�c� se po�lou tahy (pokud se je�t� net�hlo, m��e, ale nemus� se mu ta 0 poslat)
//int id_stolku, int po�et tah�, int t0, int t1, int t2, ... int tn
  public static final int  ZPRAVA_STOLEK_TAHY_PRIHLIZEJICIMU = 21;

  
  

//Podrobn� v�sledek partie
//A� do k�du 7 kompatibiln� se sachylib (v k�du se na to spol�h�m)
  public static final int KONEC_NENI = 0;
  public static final int KONEC_BILY_V_MATU = 1;
  public static final int KONEC_CERNY_V_MATU = 2;
//3 x opakov�n� pozice
  public static final int KONEC_REMIS_3 = 3;
//50 tah� bez nevratn�ch zm�n
  public static final int KONEC_REMIS_50 = 4;
  public static final int KONEC_REMIS_BILY_V_PATU = 5;
  public static final int KONEC_REMIS_CERNY_V_PATU = 6;
//Nap�. 2 kr�lov�
  public static final int KONEC_REMIS_MALO_MATERIALU = 7;
//B�l� spadnul na �as, ale �ern� nem� mat�c� materi�l
  public static final int KONEC_PRAPOREK_BILEHO_REMIS = 8;
  public static final int KONEC_PRAPOREK_CERNEHO_REMIS = 9;
  public static final int KONEC_PRAPOREK_BILEHO_PROHRA = 10;
  public static final int KONEC_PRAPOREK_CERNEHO_PROHRA = 11;
  public static final int KONEC_BILY_VZDAL = 12;
  public static final int KONEC_CERNY_VZDAL = 13;
//�ern� navrhl rem�zu a b�l� p�ijal
  public static final int KONEC_REMIS_BILY_PRIJAL = 14;
  public static final int KONEC_REMIS_CERNY_PRIJAL = 15;
//Jednomu spadlo spojen�. Stejn�, jako by spadnul praporek
  public static final int KONEC_SPADNUTI_BILEHO_REMIS = 16;
  public static final int KONEC_SPADNUTI_CERNEHO_REMIS = 17;
  public static final int KONEC_SPADNUTI_BILEHO_PROHRA = 18;
  public static final int KONEC_SPADNUTI_CERNEHO_PROHRA = 19;
//V�hry a rem�zy a zru�en� partie z moci ��edn�.
  public static final int KONEC_BILY_VYHRAL_JINAK = 100;
  public static final int KONEC_CERNY_VYHRAL_JINAK = 101;
  public static final int KONEC_REMIS_JINAK = 102;
  public static final int KONEC_ZRUSENO = 103;
  
  public static String getStringKratkyKonec(int typ) {
    switch (typ) {
    case -1: return "�ern� vyhr�l";
    case 0: return "Rem�za";
    case 1: return "B�l� vyhr�l";
    default: return "Partie neskon�ila";
    }
  }
  
  public static String getStringKonec(int typ) {
    switch (typ) {
    case KONEC_NENI: return "Prtie neskon�ila.";
    case KONEC_BILY_V_MATU: return "B�l� dostal mat.";
    case KONEC_CERNY_V_MATU: return "�ern� dostal mat.";
//  3 x opakov�n� pozice
    case KONEC_REMIS_3: return "3x opakovan� pozice";
//  50 tah� bez nevratn�ch zm�n
    case KONEC_REMIS_50: return "pravidlo 50 tah�";
    case KONEC_REMIS_BILY_V_PATU: return "B�l� dostal pat.";
    case KONEC_REMIS_CERNY_V_PATU: return "�ern� dostal pat.";
//  Nap�. 2 kr�lov�
    case KONEC_REMIS_MALO_MATERIALU: return "Nen� mat�c� materi�l.";
//  B�l� spadnul na �as, ale �ern� nem� mat�c� materi�l
    case KONEC_PRAPOREK_BILEHO_REMIS: return "�ern� nem� mat�c� materi�l.";
    case KONEC_PRAPOREK_CERNEHO_REMIS: return "B�l� nem� mat�c� materi�l.";
    case KONEC_PRAPOREK_BILEHO_PROHRA: return "B�l� spadnul.";
    case KONEC_PRAPOREK_CERNEHO_PROHRA: return "�ern� spadnul.";
    case KONEC_BILY_VZDAL: return "B�l� se vzdal.";
    case KONEC_CERNY_VZDAL: return "�ern� se vzdal.";
//  �ern� navrhl rem�zu a b�l� p�ijal
    case KONEC_REMIS_BILY_PRIJAL: return "B�l� p�ijal nab�dku rem�zy.";
    case KONEC_REMIS_CERNY_PRIJAL: return "�ern� p�ijal nab�dku rem�zy.";
//  Jednomu spadlo spojen�. Stejn�, jako by spadnul praporek
    case KONEC_SPADNUTI_BILEHO_REMIS: return "B�l�mu spadlo spojen�, �ern� nem� mat�c� materi�l.";
    case KONEC_SPADNUTI_CERNEHO_REMIS: return "�ern�mu spadlo spojen�, b�l� nem� mat�c� materi�l.";
    case KONEC_SPADNUTI_BILEHO_PROHRA: return "B�l�mu spadlo spojen.";
    case KONEC_SPADNUTI_CERNEHO_PROHRA: return "�ern�mu spadlo spojen�.";
//  V�hry a rem�zy a zru�en� partie z moci ��edn�.
    case KONEC_BILY_VYHRAL_JINAK: return "B�l� vyhr�l jinak.";
    case KONEC_CERNY_VYHRAL_JINAK: return "�ern� vyhr�l jinak.";
    case KONEC_REMIS_JINAK: return "Rem�za jin�m zp�sobem.";
    case KONEC_ZRUSENO: return "Partie zru�ena.";
        default: return "Partie skon�ila jin�m zp�sobem.";
    }
  }
  
  public boolean posledni;
  public int typ;
  public byte[] param;
  public static Zprava read(InputStream input) {
    Zprava z = new Zprava();
    byte[] array = new byte[4];
    byte[] posl = new byte[1];
    try {
      readAllArray(posl, input);
      z.posledni = (posl[0] != 0);
      readAllArray(array, input);
      z.typ = array2int(array);
      readAllArray(array, input);
      int delka = array2int(array);
      z.param = new byte[delka];
      if (delka > 0) {
        readAllArray(z.param, input);
      }
    } catch (IOException e) {
      return null;
    }
    return z;
  }
  
  public String toString() {
    return "Zprava " + typ + " delky " + param.length; 
  }
  
  public static int array2int(byte[] array) {
    if (array.length != 4) throw new IllegalArgumentException();
    int r = 0;
    for (int i = 0; i < array.length ; i++) {
      r <<= 8;
      r |= (array[i] >= 0 ? array[i] : 256 + array[i]);
    }
    return r;
  }
  
  public static boolean array2boolean(byte[] array, int index) {
    return array[index] != 0;
  }
  
  public static int array2int(byte[] array, int index) {
    if (array.length < 4 + index) throw new IllegalArgumentException();
    int r = 0;
    for (int i = index; i < index + 4 ; i++) {
      r <<= 8;
      r |= (array[i] >= 0 ? array[i] : 256 + array[i]);
    }
    return r;
  }
  
  public static String array2string(byte[] array, int index) {
    int i;
    for (i = index; array[i] != 0 ; i++);
    i -= index;
    byte[] bstr = new byte[i];
    System.arraycopy(array, index, bstr, 0, i);
    ByteArrayInputStream input = new ByteArrayInputStream(bstr);
    try {
      Reader in = new InputStreamReader(input, "UTF8");
      StringBuffer s = new StringBuffer();
      while (true) {
        i = in.read();
        if (i < 0) break;
        s.append((char) i);
      }
      return new String(s);
    } catch (Exception e) {
      throw new RuntimeException(e);
    }
  }

  
  public static void readAllArray(byte[] array, InputStream input) throws IOException {
    int i = array.length;
    while (i > 0) {
      int precteno = input.read(array, array.length - i, i);
      if (precteno <= 0) {
        throw new IOException();
      }
      i -= precteno;
    }
    return;
  }
  
  private static byte array[] = new byte[10240];
  private static  int index = 0;
  
  public static void addBoolean(boolean b) {
    array[index++] = (byte)(b ? 1 : 0);
  }
  
  
  public static void addInt(int i) {
    array[index++] = (byte)(i >> 24);
    i &= 0xFFFFFF;
    array[index++] = (byte)(i >> 16);
    i &= 0xFFFF;
    array[index++] = (byte)(i >> 8);
    i &= 0xFF;
    array[index++] = (byte)i;
  }
  
  public static void addString(String str) {
    try {
    ByteArrayOutputStream output = new ByteArrayOutputStream();
    Writer out = new OutputStreamWriter(output, "UTF8");
    out.write(str);
    out.flush();
    byte[] pole = output.toByteArray();
    addArray(pole);
    array[index++] = 0;
    } catch (Exception e) {
      throw new RuntimeException(e);
    }
  }
  
  public static void addArray(byte b[]) {
    System.arraycopy(b, 0, array, index, b.length);
    index += b.length;
  }
  
  public static void setDelka() {
    int delka = index - 8;
    array[4] = (byte)(delka >> 24);
    delka &= 0xFFFFFF;
    array[5] = (byte)(delka >> 16);
    delka &= 0xFFFF;
    array[6] = (byte)(delka >> 8);
    delka &= 0xFF;
    array[7] = (byte)delka;
  }
  
  public static byte[] getAndClearArray() {
    byte r[] = new byte[index];
    System.arraycopy(array, 0, r, 0, index);
    index = 0; 
    return r;
  }
 
  private static void posliBuffer() throws IOException {
    Zprava.setDelka();
    Global.socket.getOutputStream().write(Zprava.getAndClearArray());
    Global.socket.getOutputStream().flush();
  }
  
  public static synchronized void posliZiju() throws IOException {
    Zprava.addInt(Zprava.ZPRAVA_ZIJU);
    Zprava.addInt(4);
    Zprava.addInt(0);
    posliBuffer();
  }
  
  public static synchronized void posliChat(String s) throws IOException {
    Zprava.addInt(Zprava.ZPRAVA_CHAT);
    Zprava.addInt(0);
    Zprava.addInt(0);
    Zprava.addString(s);
    posliBuffer();
  }

  
  public static synchronized void posliStolek(boolean bily, int sCelkem, int sTah) throws IOException {
    Zprava.addInt(Zprava.ZPRAVA_STOLEK);
    Zprava.addInt(0);
    addBoolean(bily);
    Zprava.addInt(sCelkem);
    Zprava.addInt(sTah);
    posliBuffer();
  }
  
  private static synchronized void posliZpravuStolku(int idStolku, int zprava) throws IOException {
    Zprava.addInt(zprava);
    Zprava.addInt(0);
    Zprava.addInt(idStolku);
    posliBuffer();
  }
  
  public static synchronized void posliOdsedl(int idStolku) throws IOException {
    posliZpravuStolku(idStolku, Zprava.ZPRAVA_ODSEDL);
  }
  
  public static synchronized void posliPresedavam(int idStolku) throws IOException {
    posliZpravuStolku(idStolku, Zprava.ZPRAVA_PRESEDAVAM);
  }
  
  public static synchronized void posliStart(int idStolku) throws IOException {
    posliZpravuStolku(idStolku, Zprava.ZPRAVA_START);
  }

  
  public static synchronized void posliVzdavam(int idStolku) throws IOException {
    posliZpravuStolku(idStolku, Zprava.ZPRAVA_VZDAVAM);
  }
  public static synchronized void posliRemis(int idStolku) throws IOException {
    posliZpravuStolku(idStolku, Zprava.ZPRAVA_NAVRHUJI_REMIS);
  }
  
  public static synchronized void posliPrisedl(int idStolku, boolean pozorovatel) throws IOException {
    Zprava.addInt(Zprava.ZPRAVA_PRISEDL);
    Zprava.addInt(0);
    Zprava.addInt(idStolku);
    Zprava.addBoolean(pozorovatel);
    posliBuffer();
  }
  
  public static synchronized void posliTah(int idStolku, int tah, int cas) throws IOException {
    Zprava.addInt(Zprava.ZPRAVA_TAH);
    Zprava.addInt(0);
    Zprava.addInt(idStolku);
    Zprava.addInt(tah);
    Zprava.addInt(cas);
    posliBuffer();
  }
}
