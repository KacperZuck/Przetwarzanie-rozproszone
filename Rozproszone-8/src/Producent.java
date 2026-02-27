import java.io.IOException;
import java.util.concurrent.ExecutorService;

public class Producent {

private int id;
private final String name;

private int czas;
private int produkt;

    public static volatile boolean run = true;
    public Producent(int id, String p, int produkt) {
        this.id = id;
        this.name = p;
        this.produkt = produkt;
        this.czas = 1000;
    }
    public void Produkuj(Magazyn m){
        while (Main.run) {
            try {
                Thread.sleep(czas);
                m.Dodaj(produkt, this);
            } catch (InterruptedException e) {
                break;
            }
        }
    }

    public void SetCzas(int t){
        this.czas = t;
    }

    public String GetName() {
        return name;
    }

}
