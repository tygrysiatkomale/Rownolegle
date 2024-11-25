import java.util.Arrays;
import java.util.Random;
import java.util.concurrent.ForkJoinPool;

public class MergeSortForkJoin {

    public static void main(String[] args) {
        int size = 20;
        int[] array = generateRandomArray(size);

        System.out.println("Tablica przed sortowaniem:");
        System.out.println(Arrays.toString(array));

        ForkJoinPool pool = new ForkJoinPool();

        DivideTask mainTask = new DivideTask(array);

        int[] sortedArray = pool.invoke(mainTask);

        if (isSorted(sortedArray)) {
            System.out.println("Tablica jest poprawnie posortowana.");
        } else {
            System.out.println("Błąd w sortowaniu.");
        }

        System.out.println("Tablica po sortowaniu:");
        System.out.println(Arrays.toString(sortedArray));
    }

    private static int[] generateRandomArray(int size) {
        Random rand = new Random();
        int[] result = new int[size];

        for (int i = 0; i < size; i++) {
            result[i] = rand.nextInt(100);
        }

        return result;
    }

    private static boolean isSorted(int[] array) {
        for (int i = 1; i < array.length; i++) {
            if (array[i] < array[i - 1]) {
                return false;
            }
        }
        return true;
    }
}
