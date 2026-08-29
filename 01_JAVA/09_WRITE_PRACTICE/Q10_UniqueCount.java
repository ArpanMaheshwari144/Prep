// JAVA WRITE-PRACTICE — Q10
// Task: List<Integer> me kitne ALAG-ALAG (unique/distinct) numbers hain, count lautao.
//   in : [1, 2, 2, 3, 3, 3]  -> 3   (unique: 1,2,3)
//   in : [5, 5, 5]           -> 1
//   in : [1, 2, 3, 4]        -> 4
//
// Naya idiom: distinct + count
//   - .distinct()  -> duplicate hata do (Set jaisa, par order rakhta)
//   - .count()     -> terminal, kitne bache ginn ke long lautata
//   - DSA-connect: new HashSet<>(nums).size()  -- yahi, stream me distinct+count.
//
// compile+run:  javac Q10_UniqueCount.java && java Q10_UniqueCount

import java.util.*;
import java.util.stream.*;

public class Q10_UniqueCount {

    static long uniqueCount(List<Integer> nums) {
        return nums.stream().distinct().count();
    }

    static void check(List<Integer> in, long exp, int t) {
        long got = uniqueCount(in);
        System.out.println("T" + t + ": " + (got == exp ? "PASS" : "FAIL") + "  got=" + got + " exp=" + exp);
    }

    public static void main(String[] args) {
        check(Arrays.asList(1, 2, 2, 3, 3, 3), 3, 1);
        check(Arrays.asList(5, 5, 5), 1, 2);
        check(Arrays.asList(1, 2, 3, 4), 4, 3);
    }
}
