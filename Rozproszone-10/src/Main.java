public class Main {
    static int PROCES_NUM = 5;

    public static void main(String[] args) throws InterruptedException {
        Monitor monitor = new Monitor();

        Thread[] workers = new Thread[PROCES_NUM];
        for (int i = 0; i < PROCES_NUM; i++) {
            workers[i] = new Thread(new WorkerProcess(monitor, "Process-" + i));
            workers[i].start();
        }

            Thread.sleep(2000);
            monitor.notifyAllWaiting();

        for (Thread t : workers) {
            t.join();
        }
    }
}
