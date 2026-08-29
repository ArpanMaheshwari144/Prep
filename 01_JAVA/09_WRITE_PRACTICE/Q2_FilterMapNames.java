// JAVA WRITE-PRACTICE — Q2
// Task: List<String> names me se sirf jinki LENGTH > 3 ho, unko UPPERCASE karke
//       ek nayi List<String> me lautao.
//   in : ["al", "bob", "arpan", "jp", "claude"]
//   out: ["ARPAN", "CLAUDE"]
//
// Naya idiom yahan:
//   - list.stream()        -> Collection se stream (Q1 me Arrays.stream(int[]) tha; ye already objects hai to .boxed() nahi chahiye)
//   - .map(x -> ...)       -> har element ko TRANSFORM (badal do) -- filter "chhaanta", map "badalta"
//   - method reference     -> `s -> s.toUpperCase()` ko chhota likh sakte: `String::toUpperCase`
//
// compile+run:  javac Q2_FilterMapNames.java && java Q2_FilterMapNames

import java.util.*;
import java.util.stream.*;

public class Q2_FilterMapNames {

    static List<String> bigNamesUpper(List<String> names) {
        return names.stream().filter(x -> x.length() > 3).map(String::toUpperCase).collect(Collectors.toList());
    }

    static void check(List<String> in, List<String> exp, int t) {
        List<String> got = bigNamesUpper(in);
        System.out.println("T" + t + ": " + (Objects.equals(got, exp) ? "PASS" : "FAIL")
                + "  got=" + got + " exp=" + exp);
    }

    public static void main(String[] args) {
        check(Arrays.asList("al", "bob", "arpan", "jp", "claude"),
              Arrays.asList("ARPAN", "CLAUDE"), 1);
        check(Arrays.asList("a", "xy", "abc"),
              Arrays.asList(), 2);                                    // koi length>3 nahi -> empty
        check(Arrays.asList("hello", "world"),
              Arrays.asList("HELLO", "WORLD"), 3);
    }
}
