// JAVA WRITE-PRACTICE — Q11
// Task: List<List<Integer>> (list ke andar list) ko ek FLAT List<Integer> me kholo.
//   in : [[1, 2], [3, 4], [5]]  -> [1, 2, 3, 4, 5]
//   in : [[], [7]]              -> [7]
//   in : [[1], [1]]             -> [1, 1]
//
// Naya idiom: flatMap
//   - map    -> ek element -> ek element.
//   - flatMap-> ek element (jo khud ek list/stream hai) -> uske ANDAR ke saare elements belt pe khol do.
//         .flatMap(inner -> inner.stream())   -> har andar-wali list ko stream bana ke MERGE
//   - "nested ko flat karna" = flatMap ka kaam.
//   - DSA-connect: for(list l : outer) for(x : l) result.add(x);  <- do nested loop = flatMap.
//
// compile+run:  javac Q11_FlattenLists.java && java Q11_FlattenLists

import java.util.*;
import java.util.stream.*;

public class Q11_FlattenLists {

    static List<Integer> flatten(List<List<Integer>> lists) {
        return lists.stream().flatMap(list -> list.stream()).collect(Collectors.toList());
    }

    static void check(List<List<Integer>> in, List<Integer> exp, int t) {
        List<Integer> got = flatten(in);
        System.out.println("T" + t + ": " + (Objects.equals(got, exp) ? "PASS" : "FAIL") + "  got=" + got + " exp=" + exp);
    }

    public static void main(String[] args) {
        check(Arrays.asList(Arrays.asList(1, 2), Arrays.asList(3, 4), Arrays.asList(5)),
              Arrays.asList(1, 2, 3, 4, 5), 1);
        check(Arrays.asList(Arrays.asList(), Arrays.asList(7)), Arrays.asList(7), 2);
        check(Arrays.asList(Arrays.asList(1), Arrays.asList(1)), Arrays.asList(1, 1), 3);
    }
}
