package hashmap;

import java.util.*;

public class H4_Bucket {

    public static void main(String[] args) {

        int n = 16;

        String[] keys = { "Arpan", "bank", "JP", "audit", "ledger" };

        List<String>[] table = new List[n];
        for (String k : keys) {
            int hashCode = k.hashCode();
            int spread = hashCode ^ (hashCode >>> 16);
            int index = (n - 1) & spread;
            if (table[index] == null) {
                table[index] = new ArrayList<>();
            }
            table[index].add(k);
        }

        for(int i = 0; i < n; i++){
            System.out.println(i + " : " + (table[i] == null ? "." : table[i]));
        }
    }
}
