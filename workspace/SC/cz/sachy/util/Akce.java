package cz.sachy.util;

import java.util.Iterator;
import java.util.Map;
import java.util.Vector;

import cz.sachy.okna.MessageBox;
import cz.sachy.okna.SachWindow;
import cz.sachy.struktury.Global;
import cz.sachy.struktury.Stolek;

public class Akce {
  public static void zpracujZijuOk() {
    
  }

  /**
   * 
   * @param index
   * @param z
   * @return nový index
   */
  public static int[] parsujStolek(int index, Zprava z, int poradi) {
    int[] r = new int[2];
    int id = Zprava.array2int(z.param, index);
    boolean bude = false;
    Stolek stolek = new Stolek(id);
    index += 4;
    stolek.bily = Zprava.array2int(z.param, index);
    index += 4;
    stolek.cerny = Zprava.array2int(z.param, index);
    index += 4;
    if (stolek.bily == Global.id || stolek.cerny == Global.id) {
      bude = true;
    }
    stolek.sCelkem = Zprava.array2int(z.param, index);
    index += 4;
    stolek.sTah = Zprava.array2int(z.param, index);
    index += 4;
    int prihlizi = Zprava.array2int(z.param, index);
    index += 4;
    stolek.prihlizi = new Vector();
    for (int j = 0; j < prihlizi; j++) {
      int idh = Zprava.array2int(z.param, index);
      stolek.prihlizi.add(new Integer(idh));
      index += 4;
      if (idh == Global.id) {
        bude = true;
      }
    }
    stolek.poradiVListBoxu = poradi;
    Global.stolky.put(new Integer(id), stolek);
    if (bude) {
      System.out.println("Okno zacatek");
      stolek.okno = new SachWindow(id);
      stolek.okno.setVisible(true);
      System.out.println("Okno konec");
    }
    r[0] = id;
    r[1] = index;
    return r;
  }
  
  public static void zpracujStolekNovy(Zprava z) {
     int[] r = parsujStolek(0, z, Global.stolkyWindow.listStolky.getItemCount());
     Stolek stolek = (Stolek)Global.stolky.get(new Integer(r[0]));
     Global.stolkyWindow.listStolky.add(stolek.getListBoxText());
  }
  
  public static void zpracujStolekPrisedl(Zprava z) {
    
    int idStolku = Zprava.array2int(z.param, 0);
    int idHrace = Zprava.array2int(z.param, 4);
    boolean pozorovatel = Zprava.array2boolean(z.param, 8);
    
    Stolek stolek = (Stolek) Global.stolky.get(new Integer(idStolku));
    if (stolek == null) {
      // TODO
      return;
    }
    
    if (pozorovatel) {
      stolek.prihlizi.add(new Integer(idHrace));
    } else {
      if (stolek.bily <= 0) {
        stolek.bily = idHrace;
      } else {
        stolek.cerny = idHrace;
      }
    }
    
    if (idHrace == Global.id) {
      stolek.okno = new SachWindow(stolek.id);
      stolek.okno.setVisible(true);
    } else {
      if ((idHrace == stolek.bily || idHrace == stolek.cerny)
          && stolek.okno != null) {
        stolek.okno.nastavButtony();
      }
    }
    Global.stolkyWindow.listStolky.remove(stolek.poradiVListBoxu);
    Global.stolkyWindow.listStolky.add(stolek.getListBoxText(), stolek.poradiVListBoxu);
    Global.stolkyWindow.nastavButtonPrisednout();
  }
  
  private static void zpracujStolekOdsedlParam(int idStolku, int idHrace) {
    
    Stolek stolek = (Stolek) Global.stolky.get(new Integer(idStolku));
    if (stolek == null) {
      // TODO
      return;
    }
    
    if (stolek.bily == idHrace) {
      stolek.bily = 0;
      stolek.bStart = stolek.cStart = stolek.hrajeSe = false;
    }
    if (stolek.cerny == idHrace) {
      stolek.cerny = 0;
      stolek.bStart = stolek.cStart = stolek.hrajeSe = false;
    }
    stolek.prihlizi.remove(new Integer(idHrace));
    
    if (stolek.bily == 0 && stolek.cerny == 0) {
      if (stolek.okno != null) {
        stolek.okno.dispose();
      }
      Global.stolkyWindow.listStolky.remove(stolek.poradiVListBoxu);
      Global.stolky.remove(new Integer(stolek.id));
    } else {
      Global.stolkyWindow.listStolky.remove(stolek.poradiVListBoxu);
      Global.stolkyWindow.listStolky.add(stolek.getListBoxText(), stolek.poradiVListBoxu);
      if (stolek.okno != null) stolek.okno.nastavButtony();
    }
    Global.stolkyWindow.nastavButtonPrisednout();
  }
  
  public static void zpracujStolekOdsedl(Zprava z) {
    int idStolku = Zprava.array2int(z.param, 0);
    int idHrace = Zprava.array2int(z.param, 4);
    
    zpracujStolekOdsedlParam(idStolku, idHrace);
  //  Global.stolkyWindow.nastavButtonPrisednout();
  }
  
  public static void zpracujStolekPresednuti(Zprava z) {
    int idStolku = Zprava.array2int(z.param, 0);
    
    Stolek stolek = (Stolek) Global.stolky.get(new Integer(idStolku));
    if (stolek == null) {
      // TODO
      return;
    }
    
    int tmp = stolek.bily;
    stolek.bily = stolek.cerny;
    stolek.cerny = tmp;
    if (stolek.bily == Global.id || stolek.cerny == Global.id) {
      if (stolek.okno != null) stolek.okno.sachovnice.otoc();
    }
    Global.stolkyWindow.listStolky.remove(stolek.poradiVListBoxu);
    Global.stolkyWindow.listStolky.add(stolek.getListBoxText(), stolek.poradiVListBoxu);
    if (stolek.okno != null) stolek.okno.nastavButtony();
  }
  
  public static void zpracujStolekStart(Zprava z) {
    int idStolku = Zprava.array2int(z.param, 0);
    int hrac = Zprava.array2int(z.param, 4);
    
    Stolek s = (Stolek)Global.stolky.get(new Integer(idStolku));
    if (s == null) {
      System.out.println("Nestartuju, protoze stolek je null.");
      return;
    }
    if (hrac == s.bily) {
      s.bStart = true;
    }
    if (hrac == s.cerny) {
      s.cStart = true;
    }
    if (s.bStart && s.cStart) {
      s.hrajeSe = true;
      s.bRemis = s.cRemis = false;
      s.okno.sachovnice.novaPartie();
      s.okno.bileHodiny.novaPartie();
      s.okno.cerneHodiny.novaPartie();
      s.okno.bileHodiny.start();
      s.okno.repaint();
    }
    if (s.okno == null) {
      return;
    }
    s.okno.nastavButtony();
  }
  
  
  public static void zpracujStolekTahyPrihlizejicimu(Zprava z) {
    int idStolku = Zprava.array2int(z.param, 0);
    int pocet = Zprava.array2int(z.param, 4);
    if (pocet == 0) return;
    
    Stolek s = (Stolek)Global.stolky.get(new Integer(idStolku));
    if (s == null) {
      System.out.println("Netahnu, protoze stolek je null.");
      return;
    }
    if (s.okno == null) {
      System.out.println("Netahnu, protoze okno je null.");
      return;
    }
    
    for (int i = 0; i < pocet; i++) {
      int tah = Zprava.array2int(z.param, 8 + 4 * i);
      s.okno.sachovnice.tahniVPartii(tah);
    }
    s.okno.sachovnice.pozice.nalezTahy();
    s.okno.nastavButtony();
  }
  
  public static void zpracujStolekTah(Zprava z) {
    int idStolku = Zprava.array2int(z.param, 0);
    int tah = Zprava.array2int(z.param, 4);
    
    Stolek s = (Stolek)Global.stolky.get(new Integer(idStolku));
    if (s == null) {
      System.out.println("Netahnu, protoze stolek je null.");
      return;
    }
    if (s.okno == null) {
      System.out.println("Netahnu, protoze okno je null.");
      return;
    }

    //if (tah bileho) {
    if (s.okno.sachovnice.pozice.bily && s.okno.sachovnice.predtahnuto != tah ||
        !s.okno.sachovnice.pozice.bily &&
        s.okno.sachovnice.predtahnuto == tah) {
      s.cRemis = false;
      s.okno.bileHodiny.stop();
      s.okno.cerneHodiny.start();
    } else {
      s.bRemis = false;
      s.okno.cerneHodiny.stop();
      s.okno.bileHodiny.start();
    }
    s.okno.nastavButtony();
    s.okno.sachovnice.tahniVPartii(tah);
    s.okno.sachovnice.pozice.nalezTahy();
  }
  
  public static void zpracujNavrhujiRemis(Zprava z) {
    int idStolku = Zprava.array2int(z.param, 0);
    int idHrace = Zprava.array2int(z.param, 4);
    Stolek s = (Stolek)Global.stolky.get(new Integer(idStolku));
    if (s == null) {
      return;
    }
    if (Global.id != s.bily && Global.id != s.cerny) return;
    if (idHrace == s.bily) {
      s.bRemis = true;
    } else {
      s.cRemis = true;
    }
    if (s.okno != null) {
      s.okno.nastavButtony();
    }
  }
  
  public static void zpracujKonecPartie(Zprava z) {
    int idStolku = Zprava.array2int(z.param, 0);
    int vysledek = Zprava.array2int(z.param, 4);
    int podrobne = Zprava.array2int(z.param, 8);
    
    MessageBox.messageBoxLater(
        Zprava.getStringKratkyKonec(vysledek),
        Zprava.getStringKonec(podrobne), false);
    Stolek s = (Stolek)Global.stolky.get(new Integer(idStolku));
    if (s == null) {
      return;
    }
    s.bStart = s.cStart = s.hrajeSe = false;
    if (s.okno == null) {
      return;
    }
    s.okno.nastavButtony();
    s.okno.sachovnice.predtahnuto = 0;
    s.okno.bileHodiny.konecPartie();
    s.okno.cerneHodiny.konecPartie();
  }
  
  public static void zpracujChatOk(Zprava z) {
    int odesilatel = Zprava.array2int(z.param, 0);
    int stolek = Zprava.array2int(z.param, 4);
    if (stolek > 0) {
      // TODO
      return;
    }
    String hrac = (String) Global.hraci.get(new Integer(odesilatel));
    if (hrac == null) {
      // TODO
      hrac = "?";
    }
    String chat = Zprava.array2string(z.param, 8);
    Global.stolkyWindow.chat(hrac + ": "+ chat); 
  }
  
  public static void zpracujNekdoLogin(Zprava z) {
    int hrac = Zprava.array2int(z.param, 0);
    String login = Zprava.array2string(z.param, 4);
    Global.hraci.put(new Integer(hrac), login);
  }
  
  public static void zpracujNekdoLogout(Zprava z) {
    int hrac = Zprava.array2int(z.param, 0);
    Iterator i = Global.stolky.entrySet().iterator();
    while (i.hasNext()) {
      Map.Entry e = (Map.Entry) i.next();
      Stolek s = (Stolek) e.getValue();
      zpracujStolekOdsedlParam(s.id, hrac);
      //System.out.println(o);
    }
    Global.hraci.remove(new Integer(hrac));
  }
  
  public static void zpracujZpravu(Zprava z) {
    System.out.println("zpracujZpravu " + z.typ);
    switch (z.typ) {
    case Zprava.ZPRAVA_ZIJU_OK:
      break;
    case Zprava.ZPRAVA_CHAT_OK:
      zpracujChatOk(z);
      break;
    case Zprava.ZPRAVA_NEKDO_LOGIN:
      zpracujNekdoLogin(z);
      break;
    case Zprava.ZPRAVA_NEKDO_LOGOUT:
      zpracujNekdoLogout(z);
      break;
    case Zprava.ZPRAVA_STOLEK_NOVY:
      zpracujStolekNovy(z);
      break;
    case Zprava.ZPRAVA_STOLEK_PRISEDL:
      zpracujStolekPrisedl(z);
      break;
    case Zprava.ZPRAVA_STOLEK_ODSEDL:
      zpracujStolekOdsedl(z);
      break;
    case Zprava.ZPRAVA_STOLEK_PRESEDNUTI:
      zpracujStolekPresednuti(z);
      break;
    case Zprava.ZPRAVA_STOLEK_START:
      zpracujStolekStart(z);
      break;
    case Zprava.ZPRAVA_STOLEK_TAH:
      zpracujStolekTah(z);
      break;
    case Zprava.ZPRAVA_STOLEK_TAHY_PRIHLIZEJICIMU:
      zpracujStolekTahyPrihlizejicimu(z);
      break;
    case Zprava.ZPRAVA_STOLEK_NAVRHUJE_REMIS:
      zpracujNavrhujiRemis(z);
      break;
    case Zprava.ZPRAVA_STOLEK_KONEC_PARTIE:
      zpracujKonecPartie(z);
      break;
    case Zprava.ZPRAVA_KLIENT_ERROR:
      MessageBox.messageBoxLater("Chyba klienta", "Do¹lo k chybì klienta a budete odpojeni.", true);
      break;
    case Zprava.ZPRAVA_INTERNAL_ERROR:
      MessageBox.messageBoxLater("Chyba klienta", "Do¹lo k chybì serveru a budete odpojeni.", true);
      break;
    default: System.out.println("Neznama zprava " + z.typ);
    }

  }
}
