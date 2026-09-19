// ============================================================
// HashMap #4 — ek hi bucket me do key gir gayin. Andar kya banta hai?
// ============================================================
// chalana:   java H4_Bucket.java
// ============================================================
//
// H3 me dekha:  JP aur audit -- dono ka index 6.
//
// Ab hum KHUD wo table banayenge jo HashMap andar banata hai:
//
//      table = ek ARRAY, size 16
//      har khaana = ya KHAALI, ya usme key-on ki LIST (chain)
//
//      [0]  ledger
//      [1]  .
//      [2]  bank
//      ...
//      [6]  JP  ->  audit          <- CHAIN
//
// ============================================================

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
