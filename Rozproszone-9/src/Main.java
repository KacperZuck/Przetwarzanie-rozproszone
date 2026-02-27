import java.util.*;
import java.util.concurrent.*;

public class Main {
    static boolean run = true;

    private static class CachingPrime {
        private final Map<Long, Boolean> cache = new ConcurrentHashMap<>();

        public boolean isPrime(final long x) {
//            return cache.computeIfAbsent(x, this::computeIsIfPrime);
            if (cache.containsKey(x)) {
                return cache.get(x);
            } else {
                cache.put(x, computeIsIfPrime(x));
                return cache.get(x);
            }
        }

        private boolean computeIsIfPrime(final long x) {
            final String currentThreadName = Thread.currentThread().getName();
            System.out.printf("\t[%s] Running computation for : %d%n", currentThreadName, x);

            try {
                Thread.sleep(1000); // 10 sekund
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }

            if (x < 2) return false;
            for (long i = 2; i * i <= x; i++) {
                if (x % i == 0) return false;
            }
            return true;
        }
    }

    public static void main(String[] args) throws Exception {
        Scanner input = new Scanner(System.in);
        final int numberThreads = 4;
        final ExecutorService executor = Executors.newFixedThreadPool(numberThreads);
        final CachingPrime sharedCache = new CachingPrime();

        while (run) {
            long[] numbers = new long[numberThreads];
            for (int i = 0; i < numberThreads; i++) {
                System.out.print("Podaj liczbe: ");
                numbers[i] = input.nextLong();
            }

            Map<Long, Future<Boolean>> futureMap = new ConcurrentHashMap<>();
            List<Future<Boolean>> results = new ArrayList<>();

            for (int i = 0; i < numberThreads; i++) {
                final long number = numbers[i];

                if (!futureMap.containsKey(number)) {
                    Future<Boolean> future = executor.submit(() -> sharedCache.isPrime(number));
                    futureMap.put(number, future);
                }
                results.add(futureMap.get(number));
            }

            for (int i = 0; i < numberThreads; i++) {
                boolean isPrime = results.get(i).get(); // czekamy na wynik
                System.out.println(numbers[i] + (isPrime ? " jest liczba pierwsza" : " nie jest liczba pierwsza"));
            }

            input.nextLine();
            System.out.print("Czy chcesz kontynuowac program? (Y/N): ");
            String response = input.nextLine();
            if (!response.equalsIgnoreCase("y")) {
                run = false;
                break;
            }
        }

        executor.shutdownNow();
        System.out.println("Zatrzymano działanie programu.");
    }
}
