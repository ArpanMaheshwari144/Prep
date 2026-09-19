// ============================================================
// HashMap #1 — key ka BUCKET tay kaise hota hai?
// ============================================================
// chalana:   cd C:\DSA_PRACTICE\01_JAVA\02_COLLECTIONS\demo
//            java H1_IndexKaise.java
// ============================================================
//
// HashMap ke andar index nikalne ka formula BAS itna hai:
//
//         index = (n - 1) & hash
//
//    n    = table (array) ka size, default 16
//    hash = key ka hashCode()
//
// ============================================================

public class H1_IndexKaise {

    public static void main(String[] args) {

        int n = 16;
        String key = "Arpan";

        int hashCode = key.hashCode();

        int index = (n-1) & hashCode;

        System.out.println(index);
        System.out.println(key);
        System.out.println(hashCode);

    }
}
