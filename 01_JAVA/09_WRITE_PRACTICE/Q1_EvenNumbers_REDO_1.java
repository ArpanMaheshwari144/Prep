// JAVA WRITE-PRACTICE — Q1 REDO_1  [BLANK redo -- bina original dekhe likh, phir milaa]
// Task: Stream API se int[] me se sirf EVEN numbers ki List<Integer> banao.
//   {1,2,3,4,5,6} -> [2, 4, 6]
// (hint nahi -- idiom khud yaad karo: Arrays.stream -> ? -> ? -> collect)
//
// compile+run:  javac Q1_EvenNumbers_REDO_1.java && java Q1_EvenNumbers_REDO_1

import java.util.*;
import java.util.stream.*;

public class Q1_EvenNumbers_REDO_1 {

    static List<Integer> evenNumbers(int[] nums) {
        return Arrays.stream(nums).boxed().filter(i -> i % 2 == 0).collect(Collectors.toList());
    }

    static void check(int[] in, List<Integer> exp, int t) {
        List<Integer> got = evenNumbers(in);
        System.out.println("T" + t + ": " + (Objects.equals(got, exp) ? "PASS" : "FAIL") + "  got=" + got + " exp=" + exp);
    }

    public static void main(String[] args) {
        check(new int[]{1,2,3,4,5,6}, Arrays.asList(2,4,6), 1);
        check(new int[]{7,9,11},      Arrays.asList(),      2);
        check(new int[]{2,4},         Arrays.asList(2,4),   3);
    }
}
