import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

public class Monitor {
    private final BlockingQueue<Message> queue = new LinkedBlockingQueue<>();
    private final BlockingQueue<Message> waitingQueue = new LinkedBlockingQueue<>();
    private final Object lock = new Object(); // do synchronizacji licznika

    public static final String RED   = "\u001B[31m";;
    public static final String YELLOW = "\u001B[33m";;
    public static final String GREEN = "\u001B[32m";;
    public static final String BLUE = "\u001B[34m";
    public static final String RESET = "\u001B[0m";
    public Monitor() {
        queue.add(new Message(Message.LOCK, "initial lock"));
    }

    public void enterCriticalSection(String processName) throws InterruptedException {
        Message lockMsg = queue.take();
        System.out.println( YELLOW + processName + " is lock." + RESET);
    }

    public void exitCriticalSection(String processName) {
        queue.add(new Message(Message.LOCK, "releasing lock"));
        System.out.println(BLUE + processName + " released lock." + RESET);
    }

    public void waitCondition(String processName) throws InterruptedException {
        exitCriticalSection(processName);
        waitingQueue.put(new Message(Message.NOTIFY, processName));
        Message notifyMsg = queue.take();
        System.out.println(GREEN + processName + " notified." + RESET);
    }

    public void notifyAllWaiting() {
        int waiting = waitingQueue.size();
        System.out.println( RED + "Send (" + waiting + ") process notify messages." + RESET);
        for (int i = 0; i < waiting; i++) {
            waitingQueue.poll(); // usunięcie z kolejki
            queue.add(new Message(Message.NOTIFY, "notify"));
        }
    }

    public void waitOnMonitor(String processName) throws InterruptedException {
        queue.put(new Message(Message.LOCK, "lock"));

        while (true) {
            Message msg = queue.take();
            if (msg.type == Message.NOTIFY) {
                System.out.println(processName + " notified.");
                break;
            } else {
                // Nie nasza wiadomość — oddaj z powrotem
                queue.put(msg);
            }
        }
    }
}
