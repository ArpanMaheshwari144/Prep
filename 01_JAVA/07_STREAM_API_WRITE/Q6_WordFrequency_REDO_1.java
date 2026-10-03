// JAVA WRITE-PRACTICE — Q6 REDO_1  [BLANK redo -- bina original dekhe likh, phir milaa]
// Task: List<String> me har word KITNI BAAR aaya -> Map<String, Long> lautao.
//   ["a","b","a","c","a","b"] -> {a=3, b=2, c=1}
// (hint nahi -- idiom khud yaad: groupingBy + ? ; value Long kyun?)
//
// compile+run:  javac Q6_WordFrequency_REDO_1.java && java Q6_WordFrequency_REDO_1

import java.util.*;
import java.util.stream.*;

public class Q6_WordFrequency_REDO_1 {

    static Map<String, Long> wordFrequency(List<String> words) {
        return words.stream().collect(Collectors.groupingBy(word -> word, Collectors.counting()));
    }

    static void check(List<String> in, Map<String, Long> exp, int t) {
        Map<String, Long> got = wordFrequency(in);
        System.out.println("T" + t + ": " + (Objects.equals(got, exp) ? "PASS" : "FAIL") + "  got=" + got + " exp=" + exp);
    }

    public static void main(String[] args) {
        Map<String, Long> e1 = new HashMap<>();
        e1.put("a",3L); e1.put("b",2L); e1.put("c",1L);
        check(Arrays.asList("a","b","a","c","a","b"), e1, 1);

        Map<String, Long> e2 = new HashMap<>();
        e2.put("jp",2L); e2.put("br",1L);
        check(Arrays.asList("jp","br","jp"), e2, 2);
    }
}
