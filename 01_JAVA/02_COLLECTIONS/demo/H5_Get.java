// ============================================================
// HashMap #5 — get() : bucket tak pahunch gaye. Ab SAHI key kaise pakde?
// ============================================================
// chalana:   java H5_Get.java
// ============================================================
//
// H4 me dekha:  bucket [6] me DO key hain -> JP aur audit
//
// To get("audit") me HashMap ko do kaam karne padte:
//      1. index nikaalo            -> pata chala KAHAN dekhna hai
//      2. us chain me GHUMO        -> aur har key se MILAO
//
// Milane ke do tareeke hain Java me, aur dono ka matlab ALAG hai:
//
//      ==        ->  "kya ye WAHI cheez hai?"  (memory me ek hi jagah?)
//      .equals() ->  "kya ye BARABAR hai?"     (andar ka maal same?)
//
// Is stub ka maqsad: dono ka farak APNI AANKH se dekhna.
//
// ============================================================

import java.util.ArrayList;
import java.util.List;

public class H5_Get {

    public static void main(String[] args) {

        String val1 = "audit";
        String val2 = new String("audit");

        System.out.println(val1);
        System.out.println(val2);
        System.out.println(val1 == val2);
        System.out.println(val1.equals(val2));


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


        String str = new String("audit");

        for (String k : keys) {
            int hashCode = k.hashCode();
            int spread = hashCode ^ (hashCode >>> 16);
            int index = (n - 1) & spread;

            for(String x : table[index]){
                System.out.println(x == str);
                System.out.println(x.equals(str));
            }
        }
    }
}
