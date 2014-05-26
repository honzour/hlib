package cz.sachy.okna;

import java.awt.Button;
import java.awt.Color;
import java.awt.Frame;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.Label;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.IOException;

import cz.sachy.struktury.Global;
import cz.sachy.struktury.Stolek;
import cz.sachy.util.Zprava;

public class SachWindow extends Frame {
  private static final long serialVersionUID = 923446201025716000L;
  public static final String NIKDO = "[nikdo]";
  
  public final int id;
  public Sachovnice sachovnice; 
  public Hodiny bileHodiny;
  public Hodiny cerneHodiny;
  public Stolek stolek;
  public Label bily = new Label(NIKDO);
  public Label cerny = new Label(NIKDO);
  public Button start = new Button("Start");
  public Button vzdej = new Button("Vzdej");
  public Button remis = new Button("Remis");
  public Button pgn = new Button("PGN");
  public Button presednout = new Button("Pøesednout");
  public Button cas = new Button("Èas");
  public Color blackColor;
  public Color redColor;
  
  
  private static String getJmenoHrace(int id) {
    String login = (String) Global.hraci.get(new Integer(id));
    if (login == null) {
      return NIKDO;
    }
    return login;
  }
  
  public void nastavHrace() {
    bily.setText(getJmenoHrace(stolek.bily));
    cerny.setText(getJmenoHrace(stolek.cerny));
  }
  
  public SachWindow(int id) {
    super("SŠachovnice");
    System.out.println("SachWindow::SachWindow");
    this.id = id;
    stolek = (Stolek) Global.stolky.get(new Integer(id));
    nastavHrace();
    bileHodiny = new Hodiny(stolek.sCelkem * 1000, stolek.sTah * 1000, true);
    cerneHodiny = new Hodiny(stolek.sCelkem * 1000, stolek.sTah * 1000, false);
    sachovnice = new Sachovnice(stolek.bily != Global.id, this);
    setLayout(new GridBagLayout());
    GridBagConstraints g = new GridBagConstraints();
    
    g.insets = new Insets(0, 0, 0, 0);
    g.gridheight = 5;
    add(sachovnice, g);
    
    g.anchor = GridBagConstraints.NORTHWEST;
    g.gridheight = 1;
    g.gridx = 1;
    g.insets = new Insets(10, 10, 0, 0);
    //bileHodiny.start();
    add(bileHodiny, g);
    
    g.gridx = 2;
    g.insets = new Insets(10, 0, 0, 10);
    //cerneHodiny.start();
    add(cerneHodiny, g);

    g.fill = GridBagConstraints.HORIZONTAL;
    g.gridx = 1;
    g.gridy = 1;
    g.insets = new Insets(10, 2, 2, 2);
    add(bily, g);
    
    g.gridx = 2;
    add(cerny, g);
    
    g.gridx = 1;
    g.gridy = 2;
    g.insets = new Insets(10, 2, 2, 2);
    add(start, g);
    
    g.gridx = 2;
    add(vzdej, g);
    
    g.gridy = 3;
    g.gridx = 1;
    g.insets = new Insets(2, 2, 2, 2);
    add(remis, g);
    
    g.gridx = 2;
    add(presednout, g);
    
    g.gridy = 4;
    g.gridx = 1;
    add(cas, g);
    
    g.gridx = 2;
    add(pgn, g);
    
    blackColor = start.getForeground();
    redColor = Color.RED;
    
    pack();
    addWindowListener(new WindowListener() {

      public void windowOpened(WindowEvent arg0) {
      }

      public void windowClosing(WindowEvent arg0) {
        dispose();        
      }

      public void windowClosed(WindowEvent arg0) {
        try {
          Stolek s = (Stolek) Global.stolky.get(new Integer(SachWindow.this.id));
          if (s == null) {
        	  return;
          }
          s.okno = null;
          // TODO thread
          Zprava.posliOdsedl(SachWindow.this.id);
        } catch (IOException e) {
          // TODO
        }
      }

      public void windowIconified(WindowEvent arg0) {
      }

      public void windowDeiconified(WindowEvent arg0) {
      }

      public void windowActivated(WindowEvent arg0) {
      }

      public void windowDeactivated(WindowEvent arg0) {
      }
      
    });
    nastavButtony();
    vzdej.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        try {
          Zprava.posliVzdavam(SachWindow.this.id);
        } catch (IOException ex) {
          // TODO
        }
      }
    });
    remis.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        try {
          Zprava.posliRemis(SachWindow.this.id);
        } catch (IOException ex) {
          // TODO
        }
      }
    });
    presednout.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        try {
          Zprava.posliPresedavam(SachWindow.this.id);
        } catch (IOException ex) {
          // TODO
        }
      }
    });
    start.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        try {
          Zprava.posliStart(SachWindow.this.id);
        } catch (IOException ex) {
          // TODO
        }
      }
    });
    System.out.println("SachWindow::SachWindow konec");
  }
  
  public void nastavButtony() {
    Stolek s = stolek;
    
    start.setEnabled(
          !s.hrajeSe && (
           s.bily ==  Global.id && s.cerny != 0 ||
           s.cerny ==  Global.id && s.bily != 0
           ));
    if (start.isEnabled()) {
      if (s.bily ==  Global.id && !s.bStart && s.cStart ||
          s.cerny ==  Global.id && !s.cStart && s.bStart) {
        /*Font f = start.getFont();
        f.
        start.setFont(new Font());*/
        start.setForeground(redColor);
      } else {
        start.setForeground(blackColor);
      }
    } else {
      start.setForeground(blackColor);
    }
    boolean vzdejRemis = s.hrajeSe && (s.bily == Global.id || s.cerny == Global.id); 
    vzdej.setEnabled(vzdejRemis);
    remis.setEnabled(vzdejRemis);
    System.out.println("bRemis " + s.bRemis + " cRemis " + s.cRemis);
    if (vzdejRemis && (s.cRemis && Global.id == s.bily ||
        s.bRemis && Global.id == s.cerny)) {
      System.out.println("red");
      remis.setForeground(redColor);
    } else {
      System.out.println("black");
      remis.setForeground(blackColor);
    }
    
    pgn.setEnabled(false);
    presednout.setEnabled(
      (s.bily == 0 || s.cerny == 0) &&
      (Global.id == s.bily || Global.id == s.cerny)
    );
    cas.setEnabled(false);
    nastavHrace();
  }

}
