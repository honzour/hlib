package cz.sachy.struktury;

import java.net.Socket;
import java.util.HashMap;
import java.util.Map;

import cz.sachy.okna.StolkyWindow;

/**
 * T��da, kter� obsahuje glob�ln� prom�nn�.
 * @author honza
 *
 */
public class Global {
  /**
   * Socket p�ipojen� k serveru.
   */
  public static Socket socket;
  /**
   * Id hr��e.
   */
  public static int id;
  /**
   * Mapa hr���
   * Integer id -> String login
   */
  public static Map hraci = new HashMap();
  /**
   * Mapa stolk�.
   * Integer id -> Stolek
   */
  public static Map stolky = new HashMap();
  
  public static StolkyWindow stolkyWindow = null;
}
