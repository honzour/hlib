package cz.sachy.struktury;

import java.net.Socket;
import java.util.HashMap;
import java.util.Map;

import cz.sachy.okna.StolkyWindow;

/**
 * Tøída, která obsahuje globální promìnné.
 * @author honza
 *
 */
public class Global {
  /**
   * Socket pøipojení k serveru.
   */
  public static Socket socket;
  /**
   * Id hráèe.
   */
  public static int id;
  /**
   * Mapa hráèù
   * Integer id -> String login
   */
  public static Map hraci = new HashMap();
  /**
   * Mapa stolkù.
   * Integer id -> Stolek
   */
  public static Map stolky = new HashMap();
  
  public static StolkyWindow stolkyWindow = null;
}
