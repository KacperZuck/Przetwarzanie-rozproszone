import java.io.IOException;

public class Konsument implements Runnable{

    private int id;
    private final String name;

    private int produkt;
    private int czas;

    public static volatile boolean run = true;
    public Konsument(int id, String k, int p) {
        this.id = id;
        this.name = k;
        this.produkt = p;
        this.czas = 1000;
    }

    public void Kup(Magazyn m){
        while (Main.run) {
            try {
                Thread.sleep(czas);
                m.Usun(produkt, this);
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

    public void run() {
        try{
            System.in.read();
            run = false;
            System.out.println(" wykryto -----");

        }
        catch (IOException e){

        }

    }
}
