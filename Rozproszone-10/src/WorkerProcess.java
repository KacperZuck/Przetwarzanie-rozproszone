public class WorkerProcess implements Runnable {
    private final Monitor monitor;
    private final String name;

    public WorkerProcess(Monitor monitor, String name) {
        this.monitor = monitor;
        this.name = name;
    }

    @Override
    public void run() {
        try {
            monitor.enterCriticalSection(name);
            System.out.println(name + " entering critical section.");
            Thread.sleep(500); // symulacja pracy

           // if (Math.random() < 0.5) {
                System.out.println(name + " waiting...");
                monitor.waitCondition(name);
                System.out.println(name + " resumed work after wait.");
           // }

            monitor.exitCriticalSection(name);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}
