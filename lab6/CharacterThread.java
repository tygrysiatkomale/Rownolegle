
public class CharacterThread extends Thread {
    private char znak;
    private Obraz obrazRef;

    public CharacterThread(char znak, Obraz obrazRef) {
        this.znak = znak;
        this.obrazRef = obrazRef;
    }

    @Override
    public void run() {
        obrazRef.countOneChar(znak);
        obrazRef.printOneCharHistogram(znak);
    }
}