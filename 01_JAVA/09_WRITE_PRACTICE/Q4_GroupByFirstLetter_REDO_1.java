// JAVA WRITE-PRACTICE — Q4 REDO_1  [BLANK redo -- bina original dekhe likh, phir milaa]
// Task: List<String> words ko PEHLE AKSHAR ke hisaab se group karo.
//   Return: Map<Character, List<String>>
//   ["apple","banana","avocado","cherry"] -> {a=[apple,avocado], b=[banana], c=[cherry]}
// (hint nahi -- idiom khud yaad: stream -> collect(groupingBy(...)))
//
// compile+run:  javac Q4_GroupByFirstLetter_REDO_1.java && java Q4_GroupByFirstLetter_REDO_1

import java.util.*;
import java.util.stream.*;

public class Q4_GroupByFirstLetter_REDO_1 {

    static Map<Character, List<String>> groupByFirstLetter(List<String> words) {
        return words.stream().collect(Collectors.groupingBy(word -> word.charAt(0), Collectors.toList()));
    }

    static void check(List<String> in, Map<Character, List<String>> exp, int t) {
        Map<Character, List<String>> got = groupByFirstLetter(in);
        System.out.println("T" + t + ": " + (Objects.equals(got, exp) ? "PASS" : "FAIL") + "  got=" + got + " exp=" + exp);
    }

    public static void main(String[] args) {
        Map<Character, List<String>> e1 = new HashMap<>();
        e1.put('a', Arrays.asList("apple","avocado"));
        e1.put('b', Arrays.asList("banana"));
        e1.put('c', Arrays.asList("cherry"));
        check(Arrays.asList("apple","banana","avocado","cherry"), e1, 1);

        Map<Character, List<String>> e2 = new HashMap<>();
        e2.put('j', Arrays.asList("jp","java"));
        check(Arrays.asList("jp","java"), e2, 2);
    }
}
