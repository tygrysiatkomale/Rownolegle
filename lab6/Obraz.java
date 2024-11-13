import java.util.Random;

public class Obraz {

	private int size_n;
	private int size_m;
	private char[][] tab;
	private char[] tab_symb;
	private int[] histogram;
	private int[] hist_parallel;

	public Obraz(int n, int m) {
		this.size_n = n;
		this.size_m = m;
		tab = new char[n][m];
		tab_symb = new char[94]; // Zakres znaków ASCII od 33 do 126
		histogram = new int[94];
		hist_parallel = new int[94];

		final Random random = new Random();

		// Inicjalizacja tablicy symboli
		for (int k = 0; k < 94; k++) {
			tab_symb[k] = (char) (k + 33);
		}

		// Wypełnianie tablicy znaków losowymi symbolami
		for (int i = 0; i < size_n; i++) {
			for (int j = 0; j < size_m; j++) {
				tab[i][j] = tab_symb[random.nextInt(94)];
				System.out.print(tab[i][j] + " ");
			}
			System.out.println();
		}
		System.out.println();

		clear_histogram();
	}

	public void clear_histogram() {
		for (int i = 0; i < 94; i++) {
			histogram[i] = 0;
			hist_parallel[i] = 0;
		}
	}

	public void calculate_histogram() {
		for (int i = 0; i < size_n; i++) {
			for (int j = 0; j < size_m; j++) {
				int index = (int) tab[i][j] - 33;
				if (index >= 0 && index < 94) {
					histogram[index]++;
				}
			}
		}
	}

	public void print_histogram(int[] hist) {
		for (int i = 0; i < 94; i++) {
			System.out.println(tab_symb[i] + " " + hist[i]);
		}
	}

	// Gettery i settery
	public char[][] getTab() {
		return tab;
	}

	public int getSizeN() {
		return size_n;
	}

	public int getSizeM() {
		return size_m;
	}

	public void setHistParallel(int index, int count) {
		hist_parallel[index] = count;
	}

	public int[] getHistogram() {
		return histogram;
	}

	public int[] getHistParallel() {
		return hist_parallel;
	}

	// Porównanie histogramów
	public boolean compareHistograms() {
		for (int i = 0; i < 94; i++) {
			if (histogram[i] != hist_parallel[i]) {
				return false;
			}
		}
		return true;
	}
}
