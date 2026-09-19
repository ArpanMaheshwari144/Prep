// ============================================================
// HashMap #2 — index sirf aakhri 4 bit dekhta hai. To TAKKAR hogi?
// ============================================================
// chalana:   java H2_Takkar.java
// ============================================================
//
// H1 me dekha:  index = (n-1) & hash   ->  sirf aakhri 4 bit bachte
//
// Ab sawaal: do BILKUL alag hashCode wali key,
//            kya ek hi index pe gir sakti hain?
//
// ============================================================

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
