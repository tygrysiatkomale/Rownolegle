import java.util.Scanner;


class Histogram_test {

	public static void main(String[] args) {

		Scanner scanner = new Scanner(System.in);

		System.out.println("Set image size: n (#rows), m(#kolumns)");
		int n = scanner.nextInt();
		int m = scanner.nextInt();
		int variant;
		Obraz obraz_1 = new Obraz(n, m);

		obraz_1.calculate_histogram();
		obraz_1.print_histogram();

		 System.out.println("Set number of threads");
		 int num_threads = scanner.nextInt();

		System.out.println("Select a variant");
		variant = scanner.nextInt();

		if (variant == 1) {
				// Wariant 1, kazdy watek liczy jeden znak
			 CharacterThread[] watki = new CharacterThread[num_threads];
			 for (int i = 0; i < num_threads; i++) {
				 char znak = (char)(33 + i);
				 watki[i] = new CharacterThread(znak, obraz_1);
			 }

			 for (int i = 0; i < num_threads; i++) {
				 watki[i].start();
			 }

			 for (int i = 0; i < num_threads; i++) {
				 try {
					 watki[i].join();
				 } catch (InterruptedException e) {
					 e.printStackTrace();
				 }
			 }
			obraz_1.checkHistCorrectness();
		}

		if (variant == 2) {
			// Wariant 2, kazdy watek ma przypisany zakres znakow
			int totalChars = 94;
			int chunk = totalChars / num_threads;
			int reminder = totalChars % num_threads; // jesli sie nie rowno dzieli

			int currentStart = 33;

			Thread[] threads = new Thread[num_threads];

			for (int i = 0; i < num_threads; i++) {
				int currendEnd = currentStart + chunk;
				if (i < reminder) {
					currendEnd++;
				}

				RangeThread wRunner = new RangeThread(currentStart, currendEnd, obraz_1);
				Thread t = new Thread(wRunner);
				threads[i] = t;
				t.start();

				currentStart = currendEnd;
			}

			for (int i = 0; i < num_threads; i++) {
				try {
					threads[i].join();
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
			obraz_1.checkHistCorrectness();
		}
		}
	}

