package hashmap;

public class H3_Spread {

    public static void main(String[] args) {

        int n = 16;

        String[] keys = { "Arpan", "bank", "JP", "audit", "ledger" };

        for (String k : keys) {
            int hashCode = k.hashCode();
            int spread = hashCode ^ (hashCode >>> 16);
            int index = (n-1) & spread;
            System.out.println(k + " " + spread + " " + index + " " + hashCode);
        }
    }
}
