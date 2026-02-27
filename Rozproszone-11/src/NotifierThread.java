import java.util.concurrent.RecursiveTask;

public class NotifierThread extends Thread {
    private final Monitor monitor;

    public static final String RED    = "\u001B[31m";
    public static final String RESET  = "\u001B[0m";
    public NotifierThread(Monitor monitor) {
        this.monitor = monitor;
    }

    @Override
    public void run() {
        try {
            while (true) {
                Thread.sleep(1000); // czekamy 1s

                if (monitor.hasWaiting()) {
                    int count = monitor.notifyCurrentWaiting();
                    System.out.println(RED + "Send (" + count + ") process notify messages." + RESET);
                } else {
                    break; // nikt nie czeka – kończymy pętlę
                }
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}
