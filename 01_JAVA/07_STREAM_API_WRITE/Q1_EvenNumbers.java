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

// CHALA KE (11-Nov): [2, 4, 6] · REDO_1 (boxed pehle, phir filter) T1 T2 T3 PASS -- dono kram chalte.
//
// .boxed() bhool gaye to? COMPILE hi nahi hota:
//   "method collect in interface IntStream cannot be applied to given types"
//   IntStream ka collect() Collectors.toList() nahi leta, kyunki List me int nahi, Integer jaata hai.
//
// PROD TRAP: Collectors.toList() vs naya .toList() (Java 16+)
//   Collectors.toList() pe add(8) -> [2, 4, 6, 8]
//   .toList() pe add(8)          -> UnsupportedOperationException
//   .toList() wali list BADAL NAHI SAKTE. Method ne list lauta di, aage kisi caller ne add() kiya -> prod me exception.
//   Aage list me jodna hai to Collectors.toList() (ya new ArrayList<>(...)); sirf padhna hai to .toList() theek.
