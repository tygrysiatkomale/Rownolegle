import java.util.Random;





class Obraz {

	private int size_n;
	private int size_m;
	private char[][] tab;
	public char[] tab_symb;
	private int[] histogram;
	private int[] hist_parallel;

	public Obraz(int n, int m) {

		this.size_n = n;
		this.size_m = m;
		tab = new char[n][m];
		tab_symb = new char[94];

		final Random random = new Random();

		// Wypełnianie tablicy symbolami ASCII od 33 do 126
		for(int k=0; k<94; k++) {
			tab_symb[k] = (char)(k+33); // substitute symbols
		}

		// Wypełnianie obrazu losowymi znakami
		for(int i=0; i<n; i++) {
			for(int j=0; j<m; j++) {
				tab[i][j] = tab_symb[random.nextInt(94)];  // ascii 33-126
				System.out.print(tab[i][j]+" ");
			}
			System.out.print("\n");
		}
		System.out.print("\n\n");

		histogram = new int[94];
		hist_parallel = new int[94]; // Tablica dla histogramu równoległego
		clear_histogram();
		clear_hist_parallel();
	}

	public void clear_histogram() {
		for(int i=0; i<94; i++) histogram[i]=0;
	}

	public void clear_hist_parallel() {
		for(int i=0; i<94; i++) hist_parallel[i]=0;
	}

	// Obliczanie histogramu sekwencyjnie
	public void calculate_histogram() {

		for(int i=0; i<size_n; i++) {
			for(int j=0; j<size_m; j++) {
				histogram[(int)tab[i][j]-33]++;
			}
		}

	}

	// Obliczanie histogramu równolegle dla danego znaku
	public void calculate_histogram_parallel(char znak) {
		int count = 0;
		for(int i=0; i<size_n; i++) {
			for(int j=0; j<size_m; j++) {
				if(tab[i][j] == znak) {
					count++;
				}
			}
		}
		int index = (int)znak - 33;
		hist_parallel[index] = count;
	}

	// Drukowanie histogramu sekwencyjnego
	public void print_histogram() {

		for(int i=0; i<94; i++) {
			System.out.print(tab_symb[i]+" "+histogram[i]+"\n");
		}

	}

	// Drukowanie fragmentu histogramu dla danego wątku
	public synchronized void print_histogram_parallel(char znak) {
		int index = (int)znak - 33;
		int count = hist_parallel[index];
		System.out.print(Thread.currentThread().getName() + ": " + znak + " ");
		for(int i=0; i<count; i++) {
			System.out.print("=");
		}
		System.out.println();
	}

	// Porównanie histogramów sekwencyjnego i równoległego
	public void compare_histograms() {
		boolean equal = true;
		for(int i=0; i<94; i++) {
			if(histogram[i] != hist_parallel[i]) {
				equal = false;
				System.out.println("Różnica w histogramie dla znaku " + tab_symb[i]);
			}
		}
		if(equal) {
			System.out.println("Histogramy są identyczne.");
		} else {
			System.out.println("Histogramy nie są identyczne.");
		}
	}

	public void calculate_histogram_parallel_range(int startIndex, int endIndex) {
		for(int i = 0; i < size_n; i++) {
			for(int j = 0; j < size_m; j++) {
				char znak = tab[i][j];
				int index = (int)znak - 33;
				if(index >= startIndex && index < endIndex) {
					synchronized(this) {
						hist_parallel[index]++;
					}
				}
			}
		}
	}

	public synchronized void print_histogram_parallel_range(int startIndex, int endIndex) {
		for(int k = startIndex; k < endIndex; k++) {
			char znak = tab_symb[k];
			int count = hist_parallel[k];
			System.out.print(Thread.currentThread().getName() + ": " + znak + " ");
			for(int i = 0; i < count; i++) {
				System.out.print("=");
			}
			System.out.println();
		}
	}

}
