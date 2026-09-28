// JAVA WRITE-PRACTICE — Q3 REDO_1  [BLANK redo -- bina original dekhe likh, phir milaa]
// Task: List<Integer> me se sirf EVEN numbers ke SQUARE ka TOTAL (int) lautao.
//   [1,2,3,4] -> 20   (4 + 16)
// (hint nahi -- idiom khud yaad: stream -> filter -> ? -> sum)
//
// compile+run:  javac Q3_SumOfSquaresEven_REDO_1.java && java Q3_SumOfSquaresEven_REDO_1

import java.util.*;
import java.util.stream.*;

public class Q3_SumOfSquaresEven_REDO_1 {

    static int sumOfSquaresEven(List<Integer> nums) {
        return nums.stream().filter(i -> i % 2 == 0).mapToInt(i -> i * i).sum();
    }

    static void check(List<Integer> in, int exp, int t) {
        int got = sumOfSquaresEven(in);
        System.out.println("T" + t + ": " + (got == exp ? "PASS" : "FAIL") + "  got=" + got + " exp=" + exp);
    }

    public static void main(String[] args) {
        check(Arrays.asList(1,2,3,4), 20, 1);
        check(Arrays.asList(5,7,9),   0,  2);
        check(Arrays.asList(2,4,6),   56, 3);
    }
}
