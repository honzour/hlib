package cz.sachy;

import java.applet.Applet;
import java.awt.Graphics;

import cz.sachy.okna.LoginWindow;


public class SCApplet extends Applet {

  private static final long serialVersionUID = 1469928684160730058L;

  public void init() {
    new LoginWindow().setVisible(true);
  }

  public void stop() {
  }

  public void paint(Graphics g) {
   g.drawString("Java funguje.", 20, 20);
  }
}
