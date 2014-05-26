package cz.sachy.okna;

import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.MediaTracker;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.io.IOException;

import javax.swing.ImageIcon;

import cz.sachy.pravidla.Pozice;
import cz.sachy.pravidla.ZobrazPole;
import cz.sachy.struktury.Global;
import cz.sachy.struktury.Stolek;
import cz.sachy.util.Zprava;

public class Sachovnice extends Component {

  private static final long serialVersionUID = -1546971568204126417L;
  
  SachWindow okno;
  int hrana;
  int pole;
  int okraj;
  
  boolean otocena;
  public int predtahnuto = 0;
  
  int oznaceno;
  
  public Pozice pozice;
  Image bk, ck, bp, cp, bs, cs, bj, cj, bd, cd, bv, cv;
  
  public void tahniVPartii(int tah) {
    
    if (predtahnuto == tah) {
      System.out.println("Netahnu " + tah + ", protoze je predtazeno.");
      return;
    }
    System.out.println("Tahnu " + tah);
    pozice.tahni(tah, true, true, new ZobrazPole() {
      public void zobrazPole(int pole) {
        repaintPole(pole);
      }
    });
    pozice.nalezTahy();
    predtahnuto = tah;
  }
  
  public Sachovnice(boolean otocena, SachWindow okno) {
    this(420, otocena, okno);
  }
  
  public void novaPartie() {
    oznaceno = 0;
    pozice = new Pozice();
    pozice.nalezTahy();
  }
  
  public Sachovnice(int hrana, boolean otocena, SachWindow okno) {
    this.hrana = hrana;
    this.okno = okno;
    
    okraj = 10;
    pole = (hrana - 2 * okraj) / 8;
    
    this.otocena = otocena;
    novaPartie();
    
  //  sToolkit toolkit = Toolkit.getDefaultToolkit();
    
    bk = new ImageIcon(ClassLoader.getSystemResource("cz/sachy/obrazky/bk.gif")).getImage();
    ck = new ImageIcon(ClassLoader.getSystemResource("cz/sachy/obrazky/ck.gif")).getImage();
    bd = new ImageIcon(ClassLoader.getSystemResource("cz/sachy/obrazky/bd.gif")).getImage();
    cd = new ImageIcon(ClassLoader.getSystemResource("cz/sachy/obrazky/cd.gif")).getImage();
    bv = new ImageIcon(ClassLoader.getSystemResource("cz/sachy/obrazky/bv.gif")).getImage();
    cv = new ImageIcon(ClassLoader.getSystemResource("cz/sachy/obrazky/cv.gif")).getImage();
    bp = new ImageIcon(ClassLoader.getSystemResource("cz/sachy/obrazky/bp.png")).getImage();
    cp = new ImageIcon(ClassLoader.getSystemResource("cz/sachy/obrazky/cp.png")).getImage();
    bs = new ImageIcon(ClassLoader.getSystemResource("cz/sachy/obrazky/bs.png")).getImage();
    cs = new ImageIcon(ClassLoader.getSystemResource("cz/sachy/obrazky/cs.png")).getImage();
    bj = new ImageIcon(ClassLoader.getSystemResource("cz/sachy/obrazky/bj.png")).getImage();
    cj = new ImageIcon(ClassLoader.getSystemResource("cz/sachy/obrazky/cj.png")).getImage();
    MediaTracker mediaTracker = new MediaTracker(this);
    mediaTracker.addImage(bk, 0);
    mediaTracker.addImage(ck, 1);
    mediaTracker.addImage(bp, 2);
    mediaTracker.addImage(cp, 3);
    mediaTracker.addImage(bs, 4);
    mediaTracker.addImage(cs, 5);
    mediaTracker.addImage(bj, 6);
    mediaTracker.addImage(cj, 7);
    mediaTracker.addImage(bd, 8);
    mediaTracker.addImage(cd, 9);
    mediaTracker.addImage(bv, 10);
    mediaTracker.addImage(cv, 11);
    try
    {
        mediaTracker.waitForAll();
    }
    catch (InterruptedException ie)
    {
        System.err.println(ie);
        System.exit(1);
    }
    addMouseListener(new MouseListener() {

      public void mouseClicked(MouseEvent e) {
        if (pozice.bily && Sachovnice.this.otocena ||
            !pozice.bily && !Sachovnice.this.otocena ||
            !Sachovnice.this.okno.stolek.bStart || !Sachovnice.this.okno.stolek.cStart) {
          // Nejsem na tahu
          return;
        }
        if (!naPoli(e.getX(), e.getY())) {
          return;
        }

        int pole = getPole(e.getX(), e.getY());
        byte kamen = pozice.sch[pole];
        
        if (oznaceno > 0) {
          if (kamen < 0 && !pozice.bily || kamen > 0 && pozice.bily) {
            int tmp = oznaceno;
            oznaceno = pole;
            repaintPole(tmp);
            repaintPole(pole);
          } else {
            if (pozice.JeTam2(oznaceno, pole)) {
              int tah = pozice.DoplnTah(oznaceno, pole);
              tahniVPartii(tah);
              try {
                Stolek s = (Stolek)Global.stolky.get(new Integer(Sachovnice.this.okno.id));
                Zprava.posliTah(Sachovnice.this.okno.id, tah,
                  Global.id == s.bily ?
                  (int)Sachovnice.this.okno.bileHodiny.stop() :
                  (int)Sachovnice.this.okno.cerneHodiny.stop()
                );
              } catch (IOException ex) {
                // TODO
              }
            }
          }
        } else {
          
          
          if (kamen == 0) return;
          if (kamen < 0 && pozice.bily) return;
          if (kamen > 0 && !pozice.bily) return;
          oznaceno = pole;
          repaintPole(pole);
        }
         
      }

      public void mousePressed(MouseEvent arg0) {
      }

      public void mouseReleased(MouseEvent arg0) {
      }

      public void mouseEntered(MouseEvent arg0) {
      }

      public void mouseExited(MouseEvent arg0) {
      }
      
    });    
    
  }
  
  public void otoc() {
    otocena = !otocena;
    repaint();
  }
  
  public void repaintPole(int p) {
    p -= Pozice.a1;
    int x = p % 10;
    int y = p / 10;
    if (otocena) {
      x = 7 - x;
    } else {
      y = 7 - y;
    }
    x = okraj + x * pole;
    y = okraj + y * pole;
    repaint(x, y, pole, pole);
  }
  
  private boolean naPoli(int x, int y) {
    return x >= okraj && y >= okraj && x < hrana - okraj && y < hrana - okraj;
  }
  
  private int getPole(int x, int y) {
    x -= okraj;
    y -= okraj;
    x /= pole;
    y /= pole;
    if (otocena) {
      x = 7 - x;
    } else {
      y = 7 - y;
    }
    return Pozice.a1 + x + 10 * y;
  }
  
  public Dimension getMaximumSize() {
    return getPreferredSize();
  }
  
  public Dimension getMinimumSize() {
    return getPreferredSize();
  }
  
  public Dimension getPreferredSize() {
    return new Dimension(hrana + 2 * okraj, hrana + 2 * okraj);
  }
  
  private int getX(int i) {
    if (otocena) {
      i = 7 - i;
    }
    return okraj + i * pole;
  }
  
  private int getY(int j) {
    if (!otocena) {
      j = 7 - j;
    }
    return okraj + j * pole;
  }
  
  public void paint(Graphics g) {
    g.clearRect(0, 0, hrana, hrana);
    
    
    Color cerna = new Color(150, 110, 110); 
    Color bila = new Color(220, 220, 100);
    Color figurovaCerna = new Color(0, 0, 0);
    Color figurovaBila = new Color(255, 255, 255);
    g.setFont(new Font("Dialog", Font.BOLD, pole / 2));
    g.setColor(cerna);
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        if (((i + j) & 1) == 0) {
          g.fillRect(getX(i), getY(j), pole, pole);
        }
      }
    }
    
    g.setColor(bila);
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        if (((i + j) & 1) == 1) {
          g.fillRect(getX(i), getY(j), pole, pole);
        }
      }
    }
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        int p = Pozice.a1 + i + j * 10;
        byte f = pozice.sch[p];
        if (f == 0) continue;
        if (f < 0) {
          g.setColor(figurovaCerna);
        } else {
          g.setColor(figurovaBila);
        }
       /* if (f < 0) {
          f = (byte)-f;
        }*/
        int x = getX(i);
        int y = getY(j);
        
        switch (f) {
        case 1:
          g.drawImage(bp, x, y, null);
          break;
        case 2:
          g.drawImage(bj, x, y, null);
          break;
        case 3:
          g.drawImage(bs, x, y, null);
          break;
        case 4:
          g.drawImage(bv, x, y, null);
          break;
        case 5:
          g.drawImage(bd, x, y, null);
          break;
        case 6:
          g.drawImage(bk, x, y, null);
          break;
        case -1:
          g.drawImage(cp, x, y, null);
          break;
        case -2:
          g.drawImage(cj, x, y, null);
          break;
        case -3:
          g.drawImage(cs, x, y, null);
          break;
        case -4:
          g.drawImage(cv, x, y, null);
          break;
        case -5:
          g.drawImage(cd, x, y, null);
          break;
        case -6:
          g.drawImage(ck, x, y, null);
          break;
        }
        if (p == oznaceno) {
          g.setColor(new Color(0, 0, 0));
          g.fillRect(x + 3, y + 3, pole - 6, 3);
          g.fillRect(x + 3, y - 6 + pole, pole - 6, 3);
          g.fillRect(x + 3, y + 3, 3, pole - 6);
          g.fillRect(x + pole - 6, y + 3, 3, pole - 6);
          //g.drawLine(x, y, x, y + this.pole);
        }
      }
    }
  }
  
}
