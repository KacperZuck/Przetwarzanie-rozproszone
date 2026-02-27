
import java.io.IOException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadLocalRandom;
import java.util.concurrent.*;

public class Main {
    public static final int PRODUKTY =6;    // czyli tez ilość producentów i konsumentow
    public static final int MAX_POEJEDYNCZEGO=12;
    public static final int MAX_PRODUKTOW=30;
    public static Magazyn magazyn;
    final ExecutorService executor = Executors.newFixedThreadPool (2*PRODUKTY);
    public static volatile boolean run = true;

    public static void main(String[] args) throws InterruptedException {


        final ExecutorService executor = Executors.newFixedThreadPool (2*PRODUKTY) ;

        magazyn = new Magazyn(PRODUKTY, MAX_POEJEDYNCZEGO,MAX_PRODUKTOW);
        final Producent pro[] = new Producent[PRODUKTY];
        final Konsument kon[] = new Konsument[PRODUKTY];

        for(int i=0; i<PRODUKTY; i++){
            int losowa = ThreadLocalRandom.current().nextInt(PRODUKTY);
            pro[i] = new Producent(1, "Producent "+(i+1), losowa);
            kon[i] = new Konsument(1, "Konsument "+(1+i), losowa);
        }



        Thread stopListener = new Thread(() -> {
            try {
                System.out.println("Naciśnij klawisz, aby zakończyć program.");
                System.in.read();
                run = false;
                executor.shutdownNow();  // natychmiastowe przerwanie zadań
                System.out.println("Zatrzymano działanie programu.");
            } catch (IOException e) {
                e.printStackTrace();
            }
        });
        stopListener.start();

        for (int i = 0; i < PRODUKTY; i++) {
            int finalI = i;

            executor.submit(() -> pro[finalI].Produkuj(magazyn));
            executor.submit(() -> kon[finalI].Kup(magazyn));
        }

        executor.awaitTermination(Long.MAX_VALUE, TimeUnit.MILLISECONDS);
    }
}