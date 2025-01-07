
public class RangeThread implements Runnable {
    private int startChar;
    private int endChar;
    private Obraz obrazRef;

    public RangeThread(int startChar, int endChar, Obraz obrazRef) {
        this.startChar = startChar;
        this.endChar = endChar;
        this.obrazRef = obrazRef;
    }

    @Override
    public void run() {
        obrazRef.countCharsInRange(startChar, endChar);
        obrazRef.printRangeHistogram(startChar, endChar);
    }
}
