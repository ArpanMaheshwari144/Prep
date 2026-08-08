// JAVA WRITE-PRACTICE — Q3
// Task: List<Integer> me se sirf EVEN numbers ke SQUARE ka TOTAL (sum) lautao (int).
//   in : [1, 2, 3, 4]      -> 2*2 + 4*4 = 4 + 16 = 20
//   in : [5, 7, 9]         -> 0   (koi even nahi)
//   in : [2, 4, 6]         -> 4 + 16 + 36 = 56
//
// Naya idiom yahan:
//   - collect/toList nahi -- yahan REDUCE karna hai ek single number me (aggregate).
//   - .mapToInt(n -> ...)  -> Stream<Integer> ko wapas IntStream me le jaata (.boxed() ka ULTA).
//                             IntStream pe hi .sum() / .max() / .average() jaise aggregate milte.
//   - .sum()               -> terminal op, seedha int deta (List nahi).
//
// compile+run:  javac Q3_SumOfSquaresEven.java && java Q3_SumOfSquaresEven

import java.util.*;
import java.util.stream.*;

public class Q3_SumOfSquaresEven {

    static int sumOfSquaresEven(List<Integer> nums) {
        // TODO: tu likh -- stream() -> filter(even) -> mapToInt(n -> n*n) -> sum()
        return nums.stream().filter(x -> x%2==0).mapToInt(x -> x*x).sum();
    }

    static void check(List<Integer> in, int exp, int t) {
        int got = sumOfSquaresEven(in);
        System.out.println("T" + t + ": " + (got == exp ? "PASS" : "FAIL")
                + "  got=" + got + " exp=" + exp);
    }

    public static void main(String[] args) {
        check(Arrays.asList(1, 2, 3, 4), 20, 1);
        check(Arrays.asList(5, 7, 9), 0, 2);
        check(Arrays.asList(2, 4, 6), 56, 3);
    }
}
