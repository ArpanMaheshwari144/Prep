package hashmap;

public class H2_Takkar {

    public static void main(String[] args) {

        int n = 16;

        String[] keys = { "Arpan", "bank", "JP", "audit", "ledger" };

        for(String k : keys){
            System.out.println(k);
            System.out.println(k.hashCode());
            System.out.println((n-1) & k.hashCode());
        }

    }
}
