package cz.sachy.obrazky;

public class Elo {
  
  public static final double K = 25;
  
  public static double stredniHodnota(double eloNase, double eloSoupere) {
    return 1.0 / (
        1.0 + Math.pow(10, ((eloSoupere - eloNase)/400.0))
        );  
  }
  
  public static double noveElo(double eloNase, double eloSoupere, double vysledek, double K) {
    return eloNase + K * (vysledek - stredniHodnota(eloNase, eloSoupere));
  }

  /**
   * @param args
   */
  public static void main(String[] args) {
    System.out.println(noveElo(1909, 1909, 0.5, K));
  }

}
