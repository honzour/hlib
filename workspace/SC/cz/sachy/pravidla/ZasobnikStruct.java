package cz.sachy.pravidla;

public class ZasobnikStruct {
  byte roch;
  byte mimoch;
  int tah;
  byte brani;
  
  public ZasobnikStruct(byte roch, byte mimoch, int tah) {
    set(roch, mimoch, tah);
  }
  
  public void set(byte roch, byte mimoch, int tah) {
    this.roch = roch;
    this.mimoch = mimoch;
    this.tah = tah;
  }
}
