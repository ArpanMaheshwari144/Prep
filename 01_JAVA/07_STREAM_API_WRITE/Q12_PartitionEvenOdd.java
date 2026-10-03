// JAVA WRITE-PRACTICE — Q12
// Task: List<Integer> ko DO group me baanto -> true=even wale, false=odd wale.
//       Return: Map<Boolean, List<Integer>>
//   in : [1, 2, 3, 4, 5, 6]  -> { false=[1, 3, 5], true=[2, 4, 6] }
//   in : [2, 4]              -> { false=[], true=[2, 4] }
//
// Naya idiom: partitioningBy
//   - groupingBy = kisi bhi key pe kai buckets.
//   - partitioningBy(condition) = SIRF 2 bucket -> true wala aur false wala (boolean pe split).
//         hamesha dono keys (true, false) deta -- ek khaali bhi ho to bhi.
//   - DSA-connect: do list banao, loop me if-even->evens.add else odds.add -- yahi ek line.
//
// compile+run:  javac Q12_PartitionEvenOdd.java && java Q12_PartitionEvenOdd

import java.util.*;
import java.util.stream.*;

public class Q12_PartitionEvenOdd {

    static Map<Boolean, List<Integer>> partition(List<Integer> nums) {
        return nums.stream().collect(Collectors.partitioningBy(x -> x % 2 == 0));
    }

    static void check(List<Integer> in, Map<Boolean, List<Integer>> exp, int t) {
        Map<Boolean, List<Integer>> got = partition(in);
        System.out.println("T" + t + ": " + (Objects.equals(got, exp) ? "PASS" : "FAIL") + "  got=" + got + " exp=" + exp);
    }

    public static void main(String[] args) {
        Map<Boolean, List<Integer>> e1 = new HashMap<>();
        e1.put(false, Arrays.asList(1, 3, 5)); e1.put(true, Arrays.asList(2, 4, 6));
        check(Arrays.asList(1, 2, 3, 4, 5, 6), e1, 1);

        Map<Boolean, List<Integer>> e2 = new HashMap<>();
        e2.put(false, Arrays.asList()); e2.put(true, Arrays.asList(2, 4));
        check(Arrays.asList(2, 4), e2, 2);
    }
}
