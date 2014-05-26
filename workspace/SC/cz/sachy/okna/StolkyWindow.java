package cz.sachy.okna;

import java.awt.Button;
import java.awt.Frame;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.List;
import java.awt.TextArea;
import java.awt.TextField;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.io.IOException;
import java.util.Iterator;
import java.util.Map;

import cz.sachy.struktury.Global;
import cz.sachy.struktury.Stolek;
import cz.sachy.util.Zprava;

public class StolkyWindow extends Frame {
  private static final long serialVersionUID = 4570881727035060231L;
  
  public List listStolky = null;
  private Button buttonNovy = null;
  private Button buttonPrisednout = null;
  private Button buttonPozorovatel = null;
  private Button buttonChat = null;
  private TextArea textareaChat = null;
  private TextField textFieldChat = null;
  
  public void chat(String s) {
    textareaChat.setText(s + "\n" + textareaChat.getText());
  }
  
  protected Stolek getOznacenyStolek() {
    int index = listStolky.getSelectedIndex();
    if (index < 0) {
      return null;
    }
    Iterator i = Global.stolky.entrySet().iterator();
    while (i.hasNext()) {
      Map.Entry entry = (Map.Entry) i.next();
      Stolek s = (Stolek) entry.getValue();
      if (s.poradiVListBoxu == index) {
        return s; 
      }
    }
    return null;
  }
  
  public void nastavButtonPrisednout() {
    Stolek s = getOznacenyStolek();
    if (s == null || s.bily == Global.id || s.cerny == Global.id ||
        s.isPrihlizi(Global.id)) {
//    Prisednout lze jen ke stolku, kde nesedim
      buttonPrisednout.setEnabled(false);
      buttonPozorovatel.setEnabled(false);
      return;
    }
        
    buttonPozorovatel.setEnabled(true);
    if (s.bily != 0 && s.cerny != 0) {
      // Prisednout lze jen ke stolku s mistem volnym
      buttonPrisednout.setEnabled(false);
    } else {
      buttonPrisednout.setEnabled(true);
    }
  }
  
  protected void zmacklPrisednout(boolean pozorovatel) {
    try {
      int id = 0;
      Stolek s = getOznacenyStolek();
      if (s != null) {
        id = s.id;
      }
      
      if (id == 0) {
        // TODO
        return;
      }
      // TODO thread
      Zprava.posliPrisedl(id, pozorovatel);
      buttonPrisednout.setEnabled(false);
    } catch(IOException ex) {
      // TODO
    }
  }
  
  public StolkyWindow() {
    super("StolkyWindow");
    Global.stolkyWindow = this;
    setLayout(new GridBagLayout());
    
    Insets stdInset = new Insets(2, 2, 2, 2); 
    
    // listStolky
    GridBagConstraints gListStolky = new GridBagConstraints();
    gListStolky.gridheight = 3;
    gListStolky.insets = stdInset;
    gListStolky.weightx = 1.0;
    gListStolky.fill = GridBagConstraints.HORIZONTAL;
    listStolky = new List();
    //Co
    Iterator i = Global.stolky.values().iterator();
    while (i.hasNext()) {
      Stolek s = (Stolek) i.next();
      listStolky.add(s.getListBoxText());
    }
    listStolky.addItemListener(new ItemListener(){

      public void itemStateChanged(ItemEvent e) {
        nastavButtonPrisednout();
      }
      
    });
    add(listStolky, gListStolky);
    
    // butonNovy
    
    buttonNovy = new Button("Nový stolek");
    GridBagConstraints gButtonNovy = new GridBagConstraints();
    gButtonNovy.gridx = 1;
    gButtonNovy.insets = stdInset;
    gButtonNovy.fill = GridBagConstraints.HORIZONTAL;
    buttonNovy.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        // TODO thread
        new NovyStolekWindow().setVisible(true);
      }    
    });
    add(buttonNovy, gButtonNovy);
    
    // butonPøisednout
    
    buttonPrisednout = new Button("Pøisednout");
    GridBagConstraints gButtonPrisednout = new GridBagConstraints();
    gButtonPrisednout.gridx = 1;
    gButtonPrisednout.gridy = 1;
    gButtonPrisednout.insets = stdInset;
    gButtonPrisednout.fill = GridBagConstraints.HORIZONTAL;
    buttonPrisednout.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        zmacklPrisednout(false);        
      }
    });
    add(buttonPrisednout, gButtonPrisednout);
    buttonPrisednout.setEnabled(false);

    // butonPozorovatel
    
    buttonPozorovatel = new Button("Pozorovatel");
    GridBagConstraints gButtonPozorovatel = new GridBagConstraints();
    gButtonPozorovatel.gridx = 1;
    gButtonPozorovatel.gridy = 2;
    gButtonPozorovatel.insets = stdInset;
    gButtonPozorovatel.fill = GridBagConstraints.HORIZONTAL;
    buttonPozorovatel.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        zmacklPrisednout(true);        
      }
    });
    add(buttonPozorovatel, gButtonPozorovatel);
    buttonPozorovatel.setEnabled(false);
    
//  textfield chat
    textFieldChat = new TextField();
    GridBagConstraints gTextFieldChat = new GridBagConstraints();
    gTextFieldChat.gridy = 3;
    gTextFieldChat.gridwidth = 1;
    gTextFieldChat.insets = stdInset;
    gTextFieldChat.fill = GridBagConstraints.HORIZONTAL;
    add(textFieldChat, gTextFieldChat);
    
    // butonChat
    
    buttonChat = new Button("Po¹li");
    buttonChat.addActionListener(new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        try {
          // TODO thread
         Zprava.posliChat(textFieldChat.getText());
         textFieldChat.setText("");
        } catch(IOException ex) {
          // TODO
        }
      }
    });
    GridBagConstraints gButtonChat = new GridBagConstraints();
    gButtonChat.gridx = 1;
    gButtonChat.gridy = 3;
    gButtonChat.insets = stdInset;
    gButtonChat.fill = GridBagConstraints.HORIZONTAL;
    add(buttonChat, gButtonChat);
    
    // textarea chat
    textareaChat = new TextArea();
    GridBagConstraints gtextareaChat = new GridBagConstraints();
    gtextareaChat.gridy = 4;
    gtextareaChat.gridwidth = 2;
    gtextareaChat.insets = stdInset;
    add(textareaChat, gtextareaChat);
    
    pack();
    
    
    addWindowListener(new WindowListener() {

      public void windowOpened(WindowEvent arg0) {
      }

      public void windowClosing(WindowEvent arg0) {
        StolkyWindow.this.dispose();
      }

      public void windowClosed(WindowEvent arg0) {
        try {
          Global.socket.close();
        } catch (Exception e) {}
        System.exit(0);
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
  }

}
