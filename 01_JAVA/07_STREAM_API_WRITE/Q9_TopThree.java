// JAVA WRITE-PRACTICE — Q9
// Task: List<Integer> me se sabse BADE 3 numbers (descending order) ki List lautao.
//   in : [5, 1, 9, 3, 7, 2]  -> [9, 7, 5]
//   in : [4, 4, 1]           -> [4, 4, 1]
//   in : [10, 20]            -> [20, 10]   (3 se kam hain to jitne hain)
//
// Naya idiom: sorted + limit
//   - .sorted(Comparator.reverseOrder())  -> stream ko ULTA (bada->chhota) sort
//   - .limit(3)                           -> sirf pehle 3 lo (baaki chhod do)
//   - phir collect(toList())
//   - DSA-connect: sort desc karke top-k lena (ya heap) -- yahi.
//
// compile+run:  javac Q9_TopThree.java && java Q9_TopThree

import java.util.*;
import java.util.stream.*;

public class Q9_TopThree {

    static List<Integer> topThree(List<Integer> nums) {
        return nums.stream().sorted(Comparator.reverseOrder()).limit(3).collect(Collectors.toList());
    }

    static void check(List<Integer> in, List<Integer> exp, int t) {
        List<Integer> got = topThree(in);
        System.out.println("T" + t + ": " + (Objects.equals(got, exp) ? "PASS" : "FAIL") + "  got=" + got + " exp=" + exp);
    }

    public static void main(String[] args) {
        check(Arrays.asList(5, 1, 9, 3, 7, 2), Arrays.asList(9, 7, 5), 1);
        check(Arrays.asList(4, 4, 1), Arrays.asList(4, 4, 1), 2);
        check(Arrays.asList(10, 20), Arrays.asList(20, 10), 3);
    }
}
