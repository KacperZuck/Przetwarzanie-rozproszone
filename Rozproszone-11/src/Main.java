import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

public class Main {

    static int numberOfProcesses = 5;

    public static void main(String[] args) throws InterruptedException {
        BlockingQueue<Message> queue = new LinkedBlockingQueue<>();
        queue.put(new Message(Message.LOCK, "lock")); // początkowa blokada

        Monitor monitor = new Monitor(queue);

        List<Thread> threads = new ArrayList<>();

        for (int i = 0; i < numberOfProcesses; i++) {
            int finalI = i;
            Thread t = new Thread(() -> {
                try {
                    queue.take(); // zdobywa LOCK
                    System.out.println("Process-" + finalI + " acquired lock.");
                    System.out.println("Process-" + finalI + " entering critical section.");

                    Thread.sleep(500); // symulacja pracy

                    System.out.println("Process-" + finalI + " waiting...");
                    monitor.waitOnMonitor("Process-" + finalI);

                    System.out.println("Process-" + finalI + " resumed work after wait.");
                    queue.put(new Message(Message.LOCK, "lock")); // oddajemy LOCK

                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            });
            threads.add(t);
            t.start();
        }

        // Startujemy wątek powiadamiający
        NotifierThread notifier = new NotifierThread(monitor);
        notifier.start();

        // Czekamy na wszystkie wątki
        for (Thread t : threads) {
            t.join();
        }
        notifier.join();
        System.out.println("All processes ended.");
    }
}
