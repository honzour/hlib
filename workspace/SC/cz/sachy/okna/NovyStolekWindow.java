package cz.sachy.okna;

import java.awt.Button;
import java.awt.Checkbox;
import java.awt.CheckboxGroup;
import java.awt.Frame;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.Label;
import java.awt.TextField;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.IOException;

import cz.sachy.util.Zprava;

public class NovyStolekWindow extends Frame {

  private static final long serialVersionUID = -3629241584049456076L;
  
  Checkbox bily;
  Checkbox cerny;
  TextField casNaPartii;
  TextField casNaTah;
  Button budiz;

  public NovyStolekWindow() {
    super("Nový stolek");
    setLayout(new GridBagLayout());
    GridBagConstraints g = new GridBagConstraints();
    g.insets = new Insets(2, 2, 2, 2);
    g.anchor = GridBagConstraints.WEST;
    add(new Label("Budete: "), g);
    CheckboxGroup bcGroup = new CheckboxGroup();
    g.gridx = 1;
    add(bily = new Checkbox("bílý", bcGroup, true), g);
    g.gridx = 2;
    add(cerny = new Checkbox("èerný", bcGroup, false), g);
    g.gridx = 0;
    g.gridy = 1;
    g.anchor = GridBagConstraints.WEST;
    add(new Label("Èas na partii (min): "), g);
    g.gridx = 1;
    g.anchor = GridBagConstraints.WEST;
    g.gridwidth = 2;
    g.fill = GridBagConstraints.BOTH;
    add(casNaPartii = new TextField("5"), g);
    g.gridwidth = 1;
    g.gridx = 0;
    g.gridy = 2;
    g.anchor = GridBagConstraints.WEST;
    add(new Label("Bonus na tah (s): "), g);
    g.gridx = 1;
    g.gridwidth = 2;
    g.fill = GridBagConstraints.BOTH;
    add(casNaTah = new TextField("5"), g);
    g.fill = GridBagConstraints.NONE;
    g.gridx = 0;
    g.gridy = 3;
    g.gridwidth = 3;
    g.anchor = GridBagConstraints.CENTER;
    add(budiz = new Button("  Budiž  "), g);
    budiz.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent arg0) {
        int minut;
        int vterin;
        
        minut = parsujTextBox(casNaPartii, false);
        vterin = parsujTextBox(casNaTah, true);
        if (minut < 1 || vterin < 1) {
          return;
        }
        boolean b = bily.getState(); 
        akce(minut, vterin, b);
        NovyStolekWindow.this.dispose();        
      }
    });
    pack();
    addWindowListener(new WindowListener(){

      public void windowOpened(WindowEvent arg0) {
      }

      public void windowClosing(WindowEvent arg0) {
        System.out.println("windowClosing");
        dispose();
        //setVisible(false);
        System.out.println("dispose");
      }

      public void windowClosed(WindowEvent arg0) {
      }

      public void windowIconified(WindowEvent arg0) {
      }

      public void windowDeiconified(WindowEvent arg0) {
      }

      public void windowActivated(WindowEvent arg0) {
      }

      public void windowDeactivated(WindowEvent arg0) {
      }});
  }
  
  private int parsujTextBox(TextField t, boolean iNula) {
    Integer r;
    try {
      r = new Integer(t.getText());
      if (r.intValue() < (iNula ? 0 : 1)) {
        return 0;
      }
    } catch (NumberFormatException e) {
      t.requestFocus();
      setTitle("Chybný formát èísla");
      return 0;
    }
    return r.intValue();
  }
  
  private void akce(int min, int s, boolean bily) {
    try {
      // TODO thread
     Zprava.posliStolek(bily, min * 60, s);
    } catch(IOException ex) {
      // TODO
    }
  }
}
