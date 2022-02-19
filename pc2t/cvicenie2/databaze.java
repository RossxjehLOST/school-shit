package cvicenie2;

public class databaze {

    // premenne - bod 1
    private String meno;
    private int rocnik;
    private double uvazek = 0;
    private static double maximalnyUvazek = 1; // staticky uvazok  - bod 2


    // bod 3 - konstruktor
    public databaze(String meno, int rocnik) {
        this.meno = meno;
        this.rocnik = rocnik;
    }

    // gettery - bod 4
    public String getMeno() {
        return meno;
    }

    public int getRocnik() {
        return rocnik;
    }

    public double getUvazek() {
        return uvazek;
    }

    public static void setMaximalnyUvazek(double maxUvazek) {
        maximalnyUvazek = maxUvazek; // bod 5
    }

    // bod 6
    public boolean pridajUvazek(double uvazek) {
        if (this.uvazek + uvazek > maximalnyUvazek)
            return false;
        this.uvazek += uvazek;
        return true;
    }


}