// JAVA WRITE-PRACTICE — Q5
// Task: List<String> words se ek Map banao -> key = word, value = us word ki LENGTH.
//   in : ["jp", "java", "arpan"]
//   out: { jp=2, java=4, arpan=5 }
//
// Naya idiom yahan:
//   - Q4 groupingBy = ek key pe LIST (bucket).  Q5 toMap = ek key pe SIRF EK value.
//   - Collectors.toMap(keyFn, valueFn)
//         keyFn   -> element se KEY nikaalo   (yahan: word khud -> w)
//         valueFn -> element se VALUE nikaalo (yahan: w.length())
//   - dhyaan: agar do same key aa gayi to toMap CRASH karta (duplicate key exception) --
//             tab teesra arg (merge function) dena padta. Yahan keys unique hain to 2-arg kaafi.
//
// compile+run:  javac Q5_WordLengthMap.java && java Q5_WordLengthMap

import java.util.*;
import java.util.stream.*;

public class Q5_WordLengthMap {

    static Map<String, Integer> wordLengthMap(List<String> words) {
        return words.stream().collect(Collectors.toMap(w ->w, w->w.length()));
    }

    static void check(List<String> in, Map<String, Integer> exp, int t) {
        Map<String, Integer> got = wordLengthMap(in);
        System.out.println("T" + t + ": " + (Objects.equals(got, exp) ? "PASS" : "FAIL")
                + "  got=" + got + " exp=" + exp);
    }

    public static void main(String[] args) {
        Map<String, Integer> e1 = new HashMap<>();
        e1.put("jp", 2); e1.put("java", 4); e1.put("arpan", 5);
        check(Arrays.asList("jp", "java", "arpan"), e1, 1);

        Map<String, Integer> e2 = new HashMap<>();
        e2.put("a", 1); e2.put("bb", 2);
        check(Arrays.asList("a", "bb"), e2, 2);
    }
}
