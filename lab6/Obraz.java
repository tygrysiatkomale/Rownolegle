import  java.util.Random;


class Obraz {

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
		tab_symb = new char[94];

		final Random random = new Random();

		// for general case where symbols could be not just integers
		for(int k=0;k<94;k++) {
			tab_symb[k] = (char)(k+33); // substitute symbols
		}

		for(int i=0;i<n;i++) {
			for(int j=0;j<m;j++) {
				tab[i][j] = tab_symb[random.nextInt(94)];  // ascii 33-127
				//tab[i][j] = (char)(random.nextInt(94)+33);  // ascii 33-127
				System.out.print(tab[i][j]+" ");
			}
			System.out.print("\n");
		}
		System.out.print("\n\n");

		histogram = new int[94];
		clear_histogram();

		hist_parallel = new int[94];
		clear_histogram_parallel();
	}

	public boolean compareHist() {
		for(int i=0;i<94;i++) {
			if (histogram[i] != hist_parallel[i]){
				return false;
			}
		}
		return true;
	}

	public void checkHistCorrectness() {
		if(compareHist()) {
			System.out.println("The parallel histogram follows the sequential one!");
		} else {
			System.out.println("Error: Parallel histogram is different from sequential!");
		}
	}

	public void clear_histogram(){
		for(int i=0;i<94;i++) histogram[i]=0;
	}

	public void clear_histogram_parallel(){
		for(int i=0;i<94;i++) hist_parallel[i]=0;
	}

	public synchronized void countOneChar(char c) {
		int index = c - 33;
		int count = 0;
		for(int i=0; i<size_n; i++){
			for(int j=0; j<size_m; j++){
				if(tab[i][j] == c){
					count++;
				}
			}
		}
		hist_parallel[index] = count;
	}

	public synchronized void printOneCharHistogram(char c){
		int index = c - 33;
		int count = histogram[index];
		System.out.print("Wątek " + Thread.currentThread().threadId() + ": " + c + " "); // tu bylo getId
		for(int i=0; i<count; i++){
			System.out.print("=");
		}
		System.out.println();
	}

	public void countCharsInRange(int startChar, int endChar) {
		for(int k = startChar; k < endChar; k++){
			char c = (char) (k);
			int count = 0;
			for(int i=0; i<size_n; i++){
				for(int j=0; j<size_m; j++){
					if(tab[i][j] == c){
						count++;
					}
				}
			}
			hist_parallel[k - 33] = count;
		}
	}

	public synchronized void printRangeHistogram(int startChar, int endChar) {
		for(int k = startChar; k < endChar; k++){
			char c = (char) (k);
			int index = k - 33;
			int count = hist_parallel[index];
			System.out.print("Wątek " + Thread.currentThread().threadId() + ": " + c + " ");
			for (int i=0; i<count; i++){
				System.out.print("=");
			}
			System.out.println();
		}
	}

	public void calculate_histogram(){

		for(int i=0;i<size_n;i++) {
			for(int j=0;j<size_m;j++) {

				// optymalna wersja obliczania histogramu, wykorzystujÄca fakt, Ĺźe symbole w tablicy
				// moĹźna przeksztaĹciÄ w indeksy tablicy histogramu
				// histogram[(int)tab[i][j]-33]++;

				// wersja bardziej ogĂłlna dla tablicy symboli nie utoĹźsamianych z indeksami
				// tylko dla tej wersji sensowne jest zrĂłwnoleglenie w dziedzinie zbioru znakĂłw ASCII
				for(int k=0;k<94;k++) {
					if(tab[i][j] == tab_symb[k]) histogram[k]++;
					//if(tab[i][j] == (char)(k+33)) histogram[k]++;
				}

			}
		}

	}

// uniwersalny wzorzec dla rĂłĹźnych wariantĂłw zrĂłwnoleglenia - moĹźna go modyfikowaÄ dla
// rĂłĹźnych wersji dekompozycji albo stosowaÄ tak jak jest zapisane poniĹźej zmieniajÄc tylko
// parametry wywoĹania w wÄtkach
//
//calculate_histogram_wzorzec(start_wiersz, end_wiersz, skok_wiersz,
//                            start_kol, end_kol, skok_kol,
//                            start_znak, end_znak, skok_znak){
//
//  for(int i=start_wiersz;i<end_wiersz;i+=skok_wiersz) {
//     for(int j=start_kol;j<end_kol;j+=skok_kol) {
//        for(int k=start_znak;k<end_znak;k+=skok_znak) {
//           if(tab[i][j] == tab_symb[k]) histogram[k]++;
//


	public void print_histogram(){

		for(int i=0;i<94;i++) {
			System.out.print(tab_symb[i]+" "+histogram[i]+"\n");
			//System.out.print((char)(i+33)+" "+histogram[i]+"\n");
		}

	}

}