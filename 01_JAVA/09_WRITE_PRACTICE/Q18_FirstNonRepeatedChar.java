// JAVA WRITE-PRACTICE — Q18   (interview classic)
// Task: String me PEHLA character jo sirf EK baar aaya ho — SIRF streams se.
//   "swiss"      -> 'w'
//   "aabbcc"     -> null   (koi nahi)
//   "leetcode"   -> 'l'
//   "loveleetcode" -> 'v'
//
// compile+run:  javac Q18_FirstNonRepeatedChar.java && java Q18_FirstNonRepeatedChar

import java.util.*;
import java.util.stream.*;

public class Q18_FirstNonRepeatedChar {

    static Character firstNonRepeated(String s) {
        return null;
    }

    static void check(String in, Character exp, int t) {
        Character got = firstNonRepeated(in);
        System.out.println("T" + t + ": " + (Objects.equals(got, exp) ? "PASS" : "FAIL") + "  got=" + got + " exp=" + exp);
    }

    public static void main(String[] args) {
        check("swiss", 'w', 1);
        check("aabbcc", null, 2);
        check("leetcode", 'l', 3);
        check("loveleetcode", 'v', 4);
        check("z", 'z', 5);
        check("", null, 6);
    }
}
