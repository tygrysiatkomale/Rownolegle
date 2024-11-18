import java.util.Arrays;
import java.util.concurrent.RecursiveTask;

public class DivideTask extends RecursiveTask<int[]> {

    private int[] arrayToDivide;

    public DivideTask(int[] arrayToDivide) {
        this.arrayToDivide = arrayToDivide;
    }

    @Override
    protected int[] compute() {
        // Warunek zakończenia rekurencji
        if (arrayToDivide.length <= 1) {
            return arrayToDivide;
        }

        // Podział tablicy na dwie połowy
        int middle = arrayToDivide.length / 2;

        int[] leftPart = Arrays.copyOfRange(arrayToDivide, 0, middle);
        int[] rightPart = Arrays.copyOfRange(arrayToDivide, middle, arrayToDivide.length);

        // Tworzenie zadań dla lewej i prawej części
        DivideTask taskLeft = new DivideTask(leftPart);
        DivideTask taskRight = new DivideTask(rightPart);

        // Uruchomienie zadań
        taskLeft.fork(); // Rozpoczęcie asynchroniczne lewej części
        int[] rightResult = taskRight.compute(); // Bezpośrednie wykonanie prawej części
        int[] leftResult = taskLeft.join(); // Oczekiwanie na wynik lewej części

        // Scalenie wyników
        int[] result = mergeArrays(leftResult, rightResult);

        return result;
    }

    private int[] mergeArrays(int[] leftArray, int[] rightArray) {
        int[] merged = new int[leftArray.length + rightArray.length];

        int i = 0, j = 0, k = 0;

        // Scalanie dwóch posortowanych tablic
        while (i < leftArray.length && j < rightArray.length) {
            if (leftArray[i] <= rightArray[j]) {
                merged[k++] = leftArray[i++];
            } else {
                merged[k++] = rightArray[j++];
            }
        }

        // Kopiowanie pozostałych elementów z lewej tablicy
        while (i < leftArray.length) {
            merged[k++] = leftArray[i++];
        }

        // Kopiowanie pozostałych elementów z prawej tablicy
        while (j < rightArray.length) {
            merged[k++] = rightArray[j++];
        }

        return merged;
    }
}
