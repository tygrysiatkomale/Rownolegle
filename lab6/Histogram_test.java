//import java.util.Scanner;
//
//class Histogram_test {
//
//	public static void main(String[] args) {
//
//		Scanner scanner = new Scanner(System.in);
//
//		System.out.println("Set image size: n (#rows), m(#columns)");
//		int n = scanner.nextInt();
//		int m = scanner.nextInt();
//		Obraz obraz_1 = new Obraz(n, m);
//
//		// Obliczanie i drukowanie histogramu sekwencyjnie
//		obraz_1.calculate_histogram();
//		obraz_1.print_histogram();
//
//		// Tworzenie wątków dla każdego znaku
//		CharacterThread[] watki = new CharacterThread[94];
//
//		for (int i = 0; i < 94; i++) {
//			watki[i] = new CharacterThread(obraz_1.tab_symb[i], obraz_1);
//			watki[i].setName("Wątek " + (i + 1));
//			watki[i].start();
//		}
//
//		// Oczekiwanie na zakończenie wszystkich wątków
//		for (int i = 0; i < 94; i++) {
//			try {
//				watki[i].join();
//			} catch (InterruptedException e) {
//				e.printStackTrace();
//			}
//		}
//
//		// Porównanie histogramów
//		obraz_1.compare_histograms();
//
//	}
//
//}


import java.util.Scanner;

class Histogram_test {

	public static void main(String[] args) {

		Scanner scanner = new Scanner(System.in);

		System.out.println("Set image size: n (#rows), m(#columns)");
		int n = scanner.nextInt();
		int m = scanner.nextInt();
		Obraz obraz_1 = new Obraz(n, m);

		// Obliczanie i drukowanie histogramu sekwencyjnie
		obraz_1.calculate_histogram();
		obraz_1.print_histogram();

		// Ustalanie liczby wątków i zakresów
		System.out.println("Set number of threads:");
		int num_threads = scanner.nextInt();

		int zakres = 94 / num_threads;
		Thread[] watki = new Thread[num_threads];

		for (int i = 0; i < num_threads; i++) {
			int startIndex = i * zakres;
			int endIndex = (i == num_threads - 1) ? 94 : startIndex + zakres;

			CharacterThread watekZakres = new CharacterThread(startIndex, endIndex, obraz_1);
			watki[i] = new Thread(watekZakres);
			watki[i].setName("Wątek " + (i + 1));
			watki[i].start();
		}


		// Oczekiwanie na zakończenie wszystkich wątków
		for (int i = 0; i < num_threads; i++) {
			try {
				watki[i].join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

		// Porównanie histogramów
		obraz_1.compare_histograms();

	}

}
