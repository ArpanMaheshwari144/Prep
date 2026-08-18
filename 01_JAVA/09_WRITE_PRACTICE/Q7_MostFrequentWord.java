// JAVA WRITE-PRACTICE — Q7   (DSA-pattern + Stream MIX)
// Task: List<String> me jo word SABSE ZYADA baar aaya usko lautao.
//   in : ["a", "b", "a", "c", "a"]  -> "a"   (a 3 baar)
//   in : ["x", "y", "y"]            -> "y"
//   in : ["jp"]                     -> "jp"
//
// DSA-pattern: "frequency map banao -> usme se max count wala dhoondo" (classic HashMap pattern).
//   STEP 1: freq map -> groupingBy + counting  (Q6 wala) -> Map<String, Long>
//   STEP 2: us map me se sabse badi VALUE wali entry ka KEY nikaalo.
//
// Naya idiom yahan (STEP 2):
//   - map.entrySet().stream()                    -> Map ki entries (key+value pairs) ki stream
//   - .max(Map.Entry.comparingByValue())         -> value ke hisaab se sabse BADI entry
//         (.max terminal hai, Optional lautata -- kyunki list khaali bhi ho sakti thi)
//   - .get().getKey()                            -> us entry ka key (word) nikaalo
//   - Comparator = "kaise compare karna" -- comparingByValue matlab value pe compare.
//
// compile+run:  javac Q7_MostFrequentWord.java && java Q7_MostFrequentWord

import java.util.*;
import java.util.stream.*;

public class Q7_MostFrequentWord {

    static Map<String, Long> wordFrequency(List<String> words) {
        return words.stream().collect(Collectors.groupingBy(w -> w, Collectors.counting()));
    }
    static String mostFrequentWord(List<String> words) {
        /*
        {a=3, b=2, c=1}
        entrySet()  ->  a=3, b=2, c=1
        stream()    ->  belt pe teeno
        max(byValue)->  a=3   (sabse badi value)
        get()       ->  a=3   (Optional dibbe se bahar)
        getKey()    ->  "a"   (entry ka key)
         */
        Map<String, Long> ans = wordFrequency(words);
        return ans.entrySet().stream().max(Map.Entry.comparingByValue()).get().getKey();
    }

    static void check(List<String> in, String exp, int t) {
        String got = mostFrequentWord(in);
        System.out.println("T" + t + ": " + (Objects.equals(got, exp) ? "PASS" : "FAIL")
                + "  got=" + got + " exp=" + exp);
    }

    public static void main(String[] args) {
        check(Arrays.asList("a", "b", "a", "c", "a"), "a", 1);
        check(Arrays.asList("x", "y", "y"), "y", 2);
        check(Arrays.asList("jp"), "jp", 3);
    }
}
