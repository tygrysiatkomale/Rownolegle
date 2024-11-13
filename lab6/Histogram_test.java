import java.util.Scanner;

public class Histogram_test {

	public static void main(String[] args) {

		Scanner scanner = new Scanner(System.in);

		System.out.println("Podaj rozmiar obrazu: n (liczba wierszy), m (liczba kolumn)");
		int n = scanner.nextInt();
		int m = scanner.nextInt();
		Obraz obraz_1 = new Obraz(n, m);

		// Obliczanie histogramu sekwencyjnie
		obraz_1.calculate_histogram();
		System.out.println("Histogram sekwencyjny:");
		obraz_1.print_histogram(obraz_1.getHistogram());

		// Obliczanie histogramu równolegle
		CharacterThread[] threads = new CharacterThread[94];
		char[] symbols = new char[94];
		for (int i = 0; i < 94; i++) {
			symbols[i] = (char) (i + 33);
		}

		for (int i = 0; i < 94; i++) {
			threads[i] = new CharacterThread(symbols[i], i, obraz_1);
			threads[i].start();
		}

		// Oczekiwanie na zakończenie wątków
		for (int i = 0; i < 94; i++) {
			try {
				threads[i].join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}

		// Wyświetlanie histogramu równoległego
		System.out.println("\nHistogram równoległy:");
		obraz_1.print_histogram(obraz_1.getHistParallel());

		// Weryfikacja poprawności
		if (obraz_1.compareHistograms()) {
			System.out.println("\nHistogramy są zgodne. Obliczenia równoległe są poprawne.");
		} else {
			System.out.println("\nHistogramy nie są zgodne. Wystąpił błąd w obliczeniach równoległych.");
		}

		scanner.close();
	}
}
