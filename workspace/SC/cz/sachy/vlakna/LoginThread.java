package cz.sachy.vlakna;

import java.awt.EventQueue;
import java.io.IOException;
import java.net.Socket;

import cz.sachy.okna.MessageBox;
import cz.sachy.okna.StolkyWindow;
import cz.sachy.struktury.Global;
import cz.sachy.util.Akce;
import cz.sachy.util.Zprava;


public class LoginThread extends Thread {
  LoginInfo info;
  String login;
  String heslo;
  String host;
  int port;
  public LoginThread(LoginInfo info) {
    super();
    this.info = info;
    this.login = info.getLogin();
    this.heslo = info.getPassword();
    this.host = info.getHost();
    this.port = info.getPort();
  }
  public void run() {
    
    final Socket s;
    
    try {
      Socket sTmp = new Socket(host, port);
      s = sTmp;
    } catch (IOException e) {
        MessageBox.messageBoxLater("Chyba", "Chyba pøipojení, server asi nebìží nebo firewall.", true);
        return;
    }
    
    
    try {
      Zprava.addInt(Zprava.ZPRAVA_VERZE);
      Zprava.addInt(4);
      Zprava.addInt(0);
      
      s.getOutputStream().write(Zprava.getAndClearArray());
      s.getOutputStream().flush();
      
      Zprava z = Zprava.read(s.getInputStream());
      System.out.println(z);
      
      if (z.typ != Zprava.ZPRAVA_VERZE_OK) {
        MessageBox.messageBoxLater("Chyba", "©patná verze", true);
        return;
      }
      
      Zprava.addInt(Zprava.ZPRAVA_LOGIN);
      Zprava.addInt(0);
      Zprava.addString(login);
      Zprava.addString(heslo);
      Zprava.setDelka();
      byte[] tmp = Zprava.getAndClearArray();
      s.getOutputStream().write(tmp);
      s.getOutputStream().flush();
      
      z = Zprava.read(s.getInputStream());
      
      if (z.typ == Zprava.ZPRAVA_LOGIN_DUPLICITA) {
        MessageBox.messageBoxLater("Chyba", "U¾ jste jednou pøihlá¹ený", true);
        return;
      }
      
      if (z.typ != Zprava.ZPRAVA_LOGIN_OK) {
        MessageBox.messageBoxLater("Chyba", "©patný  login nebo heslo", true);
        return;
      }
      
      if (z.param.length < 12) {
        MessageBox.messageBoxLater("Chyba", "©patná odpovìï na login", true);
        return;
      }
      
      Global.id = Zprava.array2int(z.param, 0);
      int n = Zprava.array2int(z.param, 4);
      int m = Zprava.array2int(z.param, 8);
      int index = 12;
      for (int i = 0; i < n; i++) {
        int id = Zprava.array2int(z.param, index);
        index += 4;
        String login = Zprava.array2string(z.param, index);
        Global.hraci.put(new Integer(id), login);
        while (z.param[index] != 0) index++;
        index++;
      }
      for (int i = 0; i < m; i++) {
        index = Akce.parsujStolek(index, z, i)[1];        
      }
      
      Global.socket = s;
      
      EventQueue.invokeLater(
          new Runnable() {
            public void run() {
              System.out.println("Spoustim ziju");
              Thread t = new ZijuThread();
              t.start();
              new StolkyWindow().setVisible(true);
            }
            
          }
          );
    } catch (Exception e) {
      MessageBox.messageBoxLater("Chyba", "Bìhem pøihla¹ování do¹lo k chybì", true);
      return;
    }
  }
}

