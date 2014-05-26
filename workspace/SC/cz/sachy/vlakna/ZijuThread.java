package cz.sachy.vlakna;

import java.awt.EventQueue;

import cz.sachy.struktury.Global;
import cz.sachy.util.Akce;
import cz.sachy.util.Zprava;

public class ZijuThread extends Thread {
  
  
  public void run() {
    try {
      
      while (true) {
        System.out.println("Posilam ziju");
        Zprava.posliZiju();
        System.out.println("Ziju poslano");
        Zprava z;
        do {
          z = Zprava.read(Global.socket.getInputStream());
          System.out.println("Zprava nactena " + z.typ);
          if (z == null) {
            break;
          }
          final Zprava fz = z;
          System.out.println("invoke later");
          EventQueue.invokeLater(new Runnable() {
            public void run() {
              System.out.println("uz to bezi");
              Akce.zpracujZpravu(fz); 
            }
          });
          System.out.println("Neco nacteno " + ((z != null && !z.posledni) ?
              "ale ne posledni" : ""));          
        } while (z != null && !z.posledni);
      }
    } catch (Exception e) {
      return; 
    }
  }
      
      
/*      EventQueue.invokeLater(
          new Runnable() {
            public void run() {
              new StolkyWindow().setVisible(true);
            }
            
          }
          );
*/
}
