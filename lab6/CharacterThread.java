//public class CharacterThread extends Thread {
//
//    private char znak;
//    private Obraz obraz;
//
//    public CharacterThread(char znak, Obraz obraz) {
//        this.znak = znak;
//        this.obraz = obraz;
//    }
//
//    @Override
//    public void run() {
//        obraz.calculate_histogram_parallel(znak);
//        obraz.print_histogram_parallel(znak);
//    }
//}

class CharacterThread implements Runnable {
    private int startIndex;
    private int endIndex;
    private Obraz obraz;

    public CharacterThread(int startIndex, int endIndex, Obraz obraz) {
        this.startIndex = startIndex;
        this.endIndex = endIndex;
        this.obraz = obraz;
    }

    @Override
    public void run() {
        obraz.calculate_histogram_parallel_range(startIndex, endIndex);
        obraz.print_histogram_parallel_range(startIndex, endIndex);
    }
}

