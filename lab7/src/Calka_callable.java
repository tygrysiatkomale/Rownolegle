import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.util.concurrent.*;

public class Calka_callable implements Callable<Double>{

    private double xp;
    private double xk;
    private double dx;

    public Calka_callable(double xp, double xk, double dx) {
        this.xp = xp;
        this.xk = xk;
        this.dx = dx;
    }

    @Override
    public Double call() {
        double calka = 0.0;
        int N = (int) Math.ceil((xk-xp) / dx);
        double actualDx = (xk - xp) / N;

        for (int i = 0; i < N; i++) {
            double x1 = xp + actualDx * i;
            double x2 = x1 + actualDx;
            calka += ((getFunction(x1) + getFunction(x2)) / 2) * actualDx;
        }
        System.out.println("Watek " + Thread.currentThread().getName() +
                ": " + calka);
        return calka;
    }

    private double getFunction(double x) {
        return Math.sin(x);
    }


    public static void main(String[] args) {
        double start = 0.0;
        double end = Math.PI;
        double dx = 0.002;
        int nthreads = 10;
        int ntasks = 40;

        double actualDx = (end - start) / ntasks;

        // Create thread pool
        ExecutorService executor = Executors.newFixedThreadPool(nthreads);

        List<Future<Double>> results = new ArrayList<>();

        // Create and pass the tasks
        for (int i = 0; i < ntasks; i++) {
            double x1 = start + i * actualDx;
            double x2 = x1 + actualDx;

            // Create new task
            Calka_callable task = new Calka_callable(x1, x2, dx);

            // Pass the task to the thread pool
            Future<Double> result = executor.submit(task);

            // Add the Future object to the list
            results.add(result);
        }

        // Receive results
        double calkaResult = 0;

        for (Future<Double> result : results) {
            try {
                calkaResult += result.get();
            } catch (InterruptedException | ExecutionException e) {
                e.printStackTrace();
            }
        }
        executor.shutdown();

        System.out.println("Calka result: "+ calkaResult);
    }
}