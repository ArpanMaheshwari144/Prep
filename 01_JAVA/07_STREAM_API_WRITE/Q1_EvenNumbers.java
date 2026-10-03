// JAVA WRITE-PRACTICE — Q1
// Task: Stream API use karke int[] me se sirf EVEN numbers ki List<Integer> banao.
//   nums = {1,2,3,4,5,6}  ->  [2, 4, 6]
//
// Hint (jitna chahiye): Arrays.stream(nums) -> filter -> boxed -> collect(Collectors.toList())
// compile+run:  javac Q1_EvenNumbers.java && java Q1_EvenNumbers

import java.util.*;
import java.util.stream.*;

public class Q1_EvenNumbers {
    static List<Integer> evenNumbers(int[] nums) {
        // .boxed() KYUN? -- int[] -> Arrays.stream() ek IntStream deta (primitive int ki stream).
        //   List<Integer> sirf OBJECTS rakhti (Integer), primitive int nahi.
        //   .boxed() = har primitive `int` ko `Integer` OBJECT me pack (box) kar do (= autoboxing).
        //   flow: int[] -> Arrays.stream -> IntStream -> .boxed() -> Stream<Integer> -> collect -> List<Integer>
        //   (agar input Integer[] hota to .boxed() ki zaroorat na padti -- wo pehle se object.)
        // NOTE: filter/condition me hamesha BOOLEAN chahiye (n % 2 == 0), int nahi -- C++ jaisa auto-convert nahi.
        return Arrays.stream(nums).filter(n -> n % 2 == 0).boxed().collect(Collectors.toList());
        }

    public static void main(String[] args) {
        int[] nums = {1, 2, 3, 4, 5, 6};
        System.out.println(evenNumbers(nums));   // expect: [2, 4, 6]
    }
}
