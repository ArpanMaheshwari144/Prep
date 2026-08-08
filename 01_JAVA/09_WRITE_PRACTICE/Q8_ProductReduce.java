// JAVA WRITE-PRACTICE — Q8
// Task: List<Integer> ke SAARE numbers ka GUNA (product) lautao.
//   in : [1, 2, 3, 4]   -> 24
//   in : [5]            -> 5
//   in : [2, 2, 2]      -> 8
//
// Naya idiom: reduce
//   - sum() ready-made tha; product ke liye apna "jodne wala" logic chahiye -> reduce.
//   - reduce(identity, (acc, x) -> acc OP x)
//         identity = shuruaati value (product ke liye 1, sum ke liye 0)
//         acc      = ab tak ka jama result (accumulator)
//         har element pe: acc = acc * x
//   - DSA-connect: int p = 1; for(int x: nums) p *= x;  <- reduce wahi hai.
//
// compile+run:  javac Q8_ProductReduce.java && java Q8_ProductReduce

import java.util.*;
import java.util.stream.*;

public class Q8_ProductReduce {

    static int product(List<Integer> nums) {
        return nums.stream().reduce(1, (i, j) -> i * j);
    }

    static void check(List<Integer> in, int exp, int t) {
        int got = product(in);
        System.out.println("T" + t + ": " + (got == exp ? "PASS" : "FAIL") + "  got=" + got + " exp=" + exp);
    }

    public static void main(String[] args) {
        check(Arrays.asList(1, 2, 3, 4), 24, 1);
        check(Arrays.asList(5), 5, 2);
        check(Arrays.asList(2, 2, 2), 8, 3);
    }
}
