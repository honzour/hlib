package cz.sachy.okna;
import java.awt.Button;
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

import cz.sachy.vlakna.LoginInfo;
import cz.sachy.vlakna.LoginThread;



public class LoginWindow extends Frame implements LoginInfo {

  private static final long serialVersionUID = 341741362003414464L;
  TextField login;
  TextField heslo;
  TextField host;
  TextField port;
  
  public LoginWindow() {
    super("Honzovo ©achové centrum");
    
    setLayout(new GridBagLayout());
    GridBagConstraints g = new GridBagConstraints();
    g.fill = GridBagConstraints.HORIZONTAL;
    g.weightx = 1;
    g.insets = new Insets(2, 2, 2, 2);
    
    add(new Label("Login:"), g);
    g.gridy = 1;
    add(login = new TextField(), g);
    g.gridy = 2;
    add(new Label("Heslo:"), g);
    g.gridy = 3;
    add(heslo = new TextField(), g);
    g.gridy = 4;
    add(new Label("Host:"), g);
    g.gridy = 5;
    add(host = new TextField("localhost"), g);
    g.gridy = 6;
    add(new Label("Port:"), g);
    g.gridy = 7;
    add(port = new TextField("5001"), g);
    g.gridy = 8;
    Button login = new Button("OK");
    login.addActionListener(new ActionListener() {

      public void actionPerformed(ActionEvent arg0) {
        Thread t = new LoginThread(LoginWindow.this);
        t.start();
        LoginWindow.this.dispose();
      }
      
    });
    add(login, g);
    pack();
    addWindowListener(new WindowListener() {

      public void windowOpened(WindowEvent arg0) {
      }

      public void windowClosing(WindowEvent arg0) {
        dispose();
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
      }
      
    });

  }
  

  public String getLogin() {
    return login.getText();
  }

  public String getPassword() {
    return heslo.getText();
  }
  
  public String getHost() {
    return host.getText();
  }

  public int getPort() {
    try {
      return Integer.parseInt(port.getText());
    } catch (Exception e) {return 5001;}
  }

}
