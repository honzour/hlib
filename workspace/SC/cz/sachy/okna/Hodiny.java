package cz.sachy.okna;

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Rectangle;
import java.awt.image.BufferedImage;
import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;

/**
 * Hodiny jednoho hr��e. Tj. p�lka �achov�ch dvouhodin.
 * @author honza
 *
 */
public class Hodiny extends Canvas {

  private static final long serialVersionUID = 6284263968390723319L;
  Timer timer = new Timer(true);
  
  long casNaPartiiMs;
  long casNaTahMs;
  
  long casNaHodinachPriSpusteni;
  long casSpusteni;
  volatile String casHodin;
  boolean bily;
  
  /**
   * Konstruktor.
   * @param casNaPartiiMs celkov� �as na partii v ms
   * @param bily jsou to hodiny b�l�ho? (kv�li barv� hodin)
   */
  public Hodiny(long casNaPartiiMs, long casNaTahMs, boolean bily) {
    this.casNaPartiiMs = casNaPartiiMs;
    this.casNaTahMs = casNaTahMs;
    casNaHodinachPriSpusteni = this.casNaPartiiMs;
    casHodin = getString(casNaHodinachPriSpusteni);
    this.bily = bily;
  }
  
  public void novaPartie() {
    timer = new Timer(true);
    casNaHodinachPriSpusteni = casNaPartiiMs;
    casHodin = getString(casNaHodinachPriSpusteni);
    repaint();
  }
  
  /**
   * P�evede �as v ms na zobraziteln� �et�zec.
   * @param c �as
   * @return �et�zec
   */
  private static String getString(long c) {
    if (c < 0) {
      return "-" + getString(-c);
    }
    c += 500;
    int s = (int) (c / 1000);
    int m = s / 60;
    s -= m * 60;
    int h = m / 60;
    m -= h * 60;
    return ((h < 10 ? "0" : "") + h + ":" + (m < 10 ? "0" : "") + m + ":" + (s < 10 ? "0" : "") +s);
  }
  

  /**
   * V�zva hodin�m, aby p�ekreslily �as.
   *
   */
  public void tik() {
    long c = new Date().getTime();
    c = casNaHodinachPriSpusteni + casSpusteni - c;
    casHodin = getString(c);
    Hodiny.this.repaint();
  }
  
  /**
   * Zm��knut� hodin. (Spu�t�n� po ka�d�m tahu soupe�e nebo na za��tku u b�l�ho.)
   *
   */
  public void start() {
    casSpusteni = new Date().getTime();
    timer.schedule(new TimerTask() {

      public void run() {
        Hodiny.this.tik();
      }
      
    }, 0, 1000);
  }
  
  public void start(long ms) {
    casNaHodinachPriSpusteni = ms;
    casSpusteni = new Date().getTime();
    timer.schedule(new TimerTask() {

      public void run() {
        Hodiny.this.tik();
      }
      
    }, 0, 1000);
  }
  
  /**
   * Odm��knut� hodin. (Zastaven� po tahu, ktery dojde po siti.)
   * @return cas v ms do konce
   */
  public long stop(long ms) {
    timer.cancel();
    timer = new Timer(true);
    casNaHodinachPriSpusteni = ms;
    casHodin = getString(casNaHodinachPriSpusteni);
    repaint();
    return casNaHodinachPriSpusteni;
  }
  
  /**
   * Odm��knut� hodin. (Zastaven� po tahu.)
   * @return cas v ms do konce
   */
  public long stop() {
    timer.cancel();
    timer = new Timer(true);
    long c = new Date().getTime();
    casNaHodinachPriSpusteni += (casSpusteni - c + casNaTahMs);
    casHodin = getString(casNaHodinachPriSpusteni);
    repaint();
    return casNaHodinachPriSpusteni;
  }
  
  public void konecPartie() {
    timer.cancel();
  }
  
  public Dimension getMaximumSize() {
    return getPreferredSize();
  }
  
  public Dimension getMinimumSize() {
    return getPreferredSize();
  }
  
  public Dimension getPreferredSize() {
    FontMetrics f = getGraphics().getFontMetrics();
    int x = f.stringWidth("0-00:00:000");
    int y = (f.getHeight() * 3) / 2;
    return new Dimension(x, y);
  }
  public void paint(Graphics g) {
    Rectangle b = this.getBounds();
    BufferedImage img = new BufferedImage(b.width, b.height, BufferedImage.TYPE_INT_RGB);
    Graphics imG = img.getGraphics();
    if (bily) {
      imG.setColor(new Color(255, 255, 255));
      imG.fillRect(0, 0, b.width, b.height);
      imG.setColor(new Color(0, 0, 0));
    }
    //imG.setColor(g.getColor());
    imG.drawString(casHodin, imG.getFontMetrics().stringWidth("0"), imG.getFontMetrics().getHeight());
    g.drawImage(img, 0, 0, b.width, b.height, null);
  }
  
  public void update(Graphics g) {
    paint(g);
  }
}
