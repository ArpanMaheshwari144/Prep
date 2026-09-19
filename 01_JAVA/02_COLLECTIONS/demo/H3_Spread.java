// ============================================================
// HashMap #3 — HashMap ka ASLI hash() : upar ke bits NEECHE laana
// ============================================================
// chalana:   java H3_Spread.java
// ============================================================
//
// H2 me dikkat dikhi:
//     "Arpan" aur "bank" dono index 12 pe gire,
//     kyunki index sirf AAKHRI 4 bit dekhta hai aur upar ke 28 phenk deta hai.
//
// HashMap ka ilaaj (JDK ka asli code):
//
//        spread = h ^ (h >>> 16)
//        index  = (n - 1) & spread
//
//    h >>> 16  ->  upar ke 16 bit NEECHE aa gaye
//    ^         ->  wo neeche wale bits ko badal denge
//    -> ab upar ke bits bhi index ke faisle me hissa lete hain
//
// ============================================================

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
