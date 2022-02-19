package cvicenie2;
import java.util.Scanner;

public class DatabaseOperation {

    public static void main(String[] args) {

        databaze[] databazaOsob = new databaze[3]; // bod 1 - pole objektov - velkost 3
        Scanner sc;
        for (int i = 0; i < 3; i++) { // prechadzanie textu
            System.out.println("Index : " + (i + 1));
            System.out.print("Zadejte meno: ");// zadavanie udajov - bod 2
            sc = new Scanner(System.in);
            String meno = sc.next();
            while (true) {
                System.out.print("Zadejte rok narodenia: ");
                sc = new Scanner(System.in);
                if (sc.hasNextInt())
                    break;
                else
                    System.out.println("Neplatná hodnota"); // neplatná hodnota -> opytaj sa znovu
                sc.close();
            }
            int rocnik = sc.nextInt(); //uloženie hodnot
            databazaOsob[i] = new databaze(meno, rocnik);
        }

        while (true) { // nastavenie max úvazku
            System.out.print("Zadejte maximálnu výšku úvazku: "); //bod 3
            sc = new Scanner(System.in);
            if (sc.hasNextDouble()) {
                databaze.setMaximalnyUvazek(sc.nextDouble()); // platná hodnota -> ulož
                break; // ukončenie po úspěšnom navýšení
            } else
                System.out.println("Neplatná hodnota"); // zadá sa neplatná hodnota -> opýta sa znovu
        }

        while (true) { // navýšenie úvazku- bod 4
            System.out.print("Zadajte index osoby a výšku úvazku: ");
            sc = new Scanner(System.in);
            int num = sc.nextInt() - 1;
            double uvazek = sc.nextDouble();
            System.out.print(databazaOsob[num].getMeno() + " " + databazaOsob[num].getRocnik()); // print mana, ročník
            if (databazaOsob[num].pridajUvazek(uvazek))
                System.out.println(" Úvazek bol zvýšený na " + databazaOsob[num].getUvazek()); // úspěšné navýšení
            else
                System.out.println(" Úvazek nejde navýšiť"); // prekročenie max hodnoty-> opýtaj sa znovu
        }
    }

}