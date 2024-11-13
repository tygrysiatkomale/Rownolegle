public class CharacterThread extends Thread {

    private char character;
    private int index;
    private Obraz obraz;

    public CharacterThread(char character, int index, Obraz obraz) {
        this.character = character;
        this.index = index;
        this.obraz = obraz;
    }

    @Override
    public void run() {
        int count = 0;
        char[][] tab = obraz.getTab();
        int size_n = obraz.getSizeN();
        int size_m = obraz.getSizeM();

        for (int i = 0; i < size_n; i++) {
            for (int j = 0; j < size_m; j++) {
                if (tab[i][j] == character) {
                    count++;
                }
            }
        }
        obraz.setHistParallel(index, count);
    }
}
