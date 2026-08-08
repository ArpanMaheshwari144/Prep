// JAVA WRITE-PRACTICE — Q6
// Task: List<String> words me har word KITNI BAAR aaya, uska count Map me lautao.
//   in : ["a", "b", "a", "c", "a", "b"]
//   out: { a=3, b=2, c=1 }
//
// Naya idiom yahan (groupingBy ka "level-2"):
//   - Q4 me groupingBy(keyFn) tha -> har key pe LIST banti thi.
//   - groupingBy ka DUSRA arg = "downstream collector" -- matlab "har bucket ka kya karna hai".
//         groupingBy(keyFn, Collectors.counting())  -> list banane ke bajaye, gin ke COUNT rakho.
//   - counting() Long lautata (isliye value Long hai, Integer nahi).
//
//   DSA-connect: ye wahi HashMap freq-count hai jo tu manually karta:
//        map.put(w, map.getOrDefault(w, 0) + 1);      <- ek line groupingBy+counting me.
//
// compile+run:  javac Q6_WordFrequency.java && java Q6_WordFrequency

import java.util.*;
import java.util.stream.*;

public class Q6_WordFrequency {

    static Map<String, Long> wordFrequency(List<String> words) {
        return words.stream().collect(Collectors.groupingBy(w -> w, Collectors.counting()));
    }

    static void check(List<String> in, Map<String, Long> exp, int t) {
        Map<String, Long> got = wordFrequency(in);
        System.out.println("T" + t + ": " + (Objects.equals(got, exp) ? "PASS" : "FAIL")
                + "  got=" + got + " exp=" + exp);
    }

    public static void main(String[] args) {
        Map<String, Long> e1 = new HashMap<>();
        e1.put("a", 3L); e1.put("b", 2L); e1.put("c", 1L);
        check(Arrays.asList("a", "b", "a", "c", "a", "b"), e1, 1);

        Map<String, Long> e2 = new HashMap<>();
        e2.put("jp", 2L); e2.put("br", 1L);
        check(Arrays.asList("jp", "br", "jp"), e2, 2);
    }
}
