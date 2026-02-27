import java.util.LinkedList;
import java.util.Queue;
import java.util.concurrent.BlockingQueue;

public class Monitor {
    private final BlockingQueue<Message> queue;
    private final Queue<String> waitingQueue = new LinkedList<>();
    private final Object lock = new Object();
    public static final String GREEN  = "\u001B[32m";
    public static final String RESET  = "\u001B[0m";
    public Monitor(BlockingQueue<Message> queue) {
        this.queue = queue;
    }

    public void waitOnMonitor(String processName) throws InterruptedException {
        queue.put(new Message(Message.LOCK, "lock")); // zwalnia blokadę

        synchronized (lock) {
            waitingQueue.add(processName);
        }

        while (true) {
            Message msg = queue.take();
            if (msg.type == Message.NOTIFY) {
                System.out.println(GREEN + processName + " notified." + RESET);
                break;
            } else {
                queue.put(msg); // nie moja wiadomość
            }
        }
    }

    public int notifyCurrentWaiting() throws InterruptedException {
        int notifiedCount = 0;
        synchronized (lock) {
            while (!waitingQueue.isEmpty()) {
                waitingQueue.poll(); // usuwamy z listy
                queue.put(new Message(Message.NOTIFY, "notify"));
                notifiedCount++;
            }
        }
        return notifiedCount;
    }

    public boolean hasWaiting() {
        synchronized (lock) {
            return !waitingQueue.isEmpty();
        }
    }
}
