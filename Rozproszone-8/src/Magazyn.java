import java.util.concurrent.ThreadLocalRandom;

public class Magazyn {

    private int max = 30;
    private int max_pojedynczego=10;
    private int zajeta_pojemonsc;
    private int PRODUKTY;
    private int produkt[];

    public Magazyn(int n, int n1, int n2){
        this.PRODUKTY = n;
        this.produkt = new int[n];
        this.max_pojedynczego = n1;
        this.max = n2;
    }

    public synchronized void Store(int x, int v, String name){
        produkt[x] += v;
        zajeta_pojemonsc +=v;

        System.out.println("Zawartosc magazynu po modyfikacji ("+v+") produktu: "+(x+1)+", przez producenta: "+name);
        for (int i=0;i<PRODUKTY;i++){
            System.out.print(produkt[i]+" ");
        }
        System.out.println();
    }

    public  void Dodaj(int x, Producent p){
        if(zajeta_pojemonsc != max || max_pojedynczego != produkt[x]){
            Store(x,1, p.GetName());
        }
        else{
            System.out.println("Brak miejsca w magazynie");
        }

        int opoznienie = ThreadLocalRandom.current().nextInt(PRODUKTY+3)*500;

        p.SetCzas(opoznienie);
        p.Produkuj(this);
    }

    public void Usun(int x, Konsument k){

        int opoznienie = ThreadLocalRandom.current().nextInt(PRODUKTY+3)*1000;
        if(zajeta_pojemonsc != 0 && produkt[x]!=0){
            Store(x,-1,k.GetName());

        }
        else if (zajeta_pojemonsc == 0) {
            System.out.println("Brak produktow w magazynie");

        } else if (produkt[x]==0) {
            System.out.println("brak produktu "+x+" w magazynie");

        }


        k.SetCzas(opoznienie);
        k.Kup(this);

    }
}
