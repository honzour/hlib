package cz.sachy.struktury;

import java.util.Iterator;
import java.util.Vector;

import cz.sachy.okna.SachWindow;

/**
 * Reprezentace jednoho stolku.
 * @author honza
 *
 */
public class Stolek {
  public static final String VOLNO = "volno"; 
  
  /**
   * id stolku.
   */
  public int id;
  /**
   * id hráèe za bílé nebo 0
   */
  public int bily;
  public int cerny;
  public int sCelkem;
  public int sTah;
  public Vector prihlizi;
  public boolean hrajeSe;
  public boolean bStart;
  public boolean cStart;
  public boolean bRemis;
  public boolean cRemis;
  
  /**
   * Okno, je-li stolek zobrazený nebo null.
   */
  public SachWindow okno;
  
  public int poradiVListBoxu;
  
  public Stolek(int id) {
    this.id = id;
    bStart = cStart = hrajeSe = bRemis = cRemis = false;
  }
  
  public String getListBoxText() {

    String bStr = (String) Global.hraci.get(new Integer(bily));
    String cStr = (String) Global.hraci.get(new Integer(cerny));
    if (bStr == null) {
      bStr = VOLNO;
    }
    if (cStr == null) {
      cStr = VOLNO;
    }
    String s = new String(bStr + " : " + cStr);
    return s;
  }
  
  public boolean isPrihlizi(int id) {
    if (prihlizi.isEmpty()) {
      return false;
    }
    Iterator i = prihlizi.iterator();
    while (i.hasNext()) {
      if (((Integer)i.next()).intValue() == id) return true;
    }
    return false;
  }
}
