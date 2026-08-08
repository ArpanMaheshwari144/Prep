// JAVA WRITE-PRACTICE — Q4
// Task: List<String> words ko unke PEHLE AKSHAR ke hisaab se group karo.
//       Return: Map<Character, List<String>>  (key = first letter, value = us letter se shuru hone wale words)
//   in : ["apple", "banana", "avocado", "cherry", "blueberry"]
//   out: { a=[apple, avocado], b=[banana, blueberry], c=[cherry] }
//
// Naya idiom yahan:
//   - ab tak collect(toList()) / sum() kiya -- yahan MAP me collect karna hai (buckets).
//   - Collectors.groupingBy(w -> KEY)  -> stream ko key ke hisaab se DIBBON (buckets) me baant deta.
//         key nikalne wala function do -> Java har element ko uske key wale dibbe (List) me daal deta.
//   - word ka pehla letter: w.charAt(0)   (char lautata, Character me autobox ho jayega Map key ke liye)
//
// compile+run:  javac Q4_GroupByFirstLetter.java && java Q4_GroupByFirstLetter

import java.util.*;
import java.util.stream.*;

public class Q4_GroupByFirstLetter {

    static Map<Character, List<String>> groupByFirstLetter(List<String> words) {
        // TYPE INFERENCE -- `w` ki type kahin likhi nahi, phir Java ko kaise pata `w` = String?
        //   type LEFT se aata: method me declare kiya `List<String> words`
        //     -> words.stream() banata Stream<String> (String ki stream)
        //     -> lambda `w -> ...` me Java samajh jaata "stream String rakhti -> w = String".
        //   isliye `String w` likhna nahi padta; Java khud bhar deta (chaho to `(String w) -> ...` likh sakte, SAME).
        //   List<Integer> -> Stream<Integer> -> n = Integer;  int[] -> IntStream -> n = int.
        //   = C++ ka `auto` (for(auto x : nums)) -- compiler type khud nikaal leta. Type upar se neeche behta.
        return words.stream().collect(Collectors.groupingBy(w -> w.charAt(0)));
    }

    static void check(List<String> in, Map<Character, List<String>> exp, int t) {
        Map<Character, List<String>> got = groupByFirstLetter(in);
        System.out.println("T" + t + ": " + (Objects.equals(got, exp) ? "PASS" : "FAIL")
                + "  got=" + got + " exp=" + exp);
    }

    public static void main(String[] args) {
        Map<Character, List<String>> e1 = new HashMap<>();
        e1.put('a', Arrays.asList("apple", "avocado"));
        e1.put('b', Arrays.asList("banana", "blueberry"));
        e1.put('c', Arrays.asList("cherry"));
        check(Arrays.asList("apple", "banana", "avocado", "cherry", "blueberry"), e1, 1);

        Map<Character, List<String>> e2 = new HashMap<>();
        e2.put('j', Arrays.asList("jp", "java"));
        check(Arrays.asList("jp", "java"), e2, 2);
    }
}
