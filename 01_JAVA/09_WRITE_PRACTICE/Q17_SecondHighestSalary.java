// JAVA WRITE-PRACTICE — Q17   (interview classic)
// Task: List<Emp> se DOOSRI sabse badi salary (distinct) — SIRF streams se.
//   [ (a,100), (b,300), (c,200) ]          -> 200
//   [ (a,100), (b,300), (c,300) ]          -> 100   (300 do baar = ek hi gina)
//   sirf ek hi distinct salary ho           -> -1
//
// compile+run:  javac Q17_SecondHighestSalary.java && java Q17_SecondHighestSalary

import java.util.*;
import java.util.stream.*;

public class Q17_SecondHighestSalary {

    static class Emp {
        String name; int salary;
        Emp(String name, int salary) { this.name = name; this.salary = salary; }
    }

    static int secondHighest(List<Emp> emps) {
        Optional<Integer> ans = emps.stream().map(e -> e.salary).distinct().sorted(Comparator.reverseOrder()).skip(1).findFirst();
        return ans.orElse(-1);
    }

    static void check(List<Emp> in, int exp, int t) {
        int got = secondHighest(in);
        System.out.println("T" + t + ": " + (got == exp ? "PASS" : "FAIL") + "  got=" + got + " exp=" + exp);
    }

    public static void main(String[] args) {
        check(Arrays.asList(new Emp("a",100), new Emp("b",300), new Emp("c",200)), 200, 1);
        check(Arrays.asList(new Emp("a",100), new Emp("b",300), new Emp("c",300)), 100, 2);
        check(Arrays.asList(new Emp("a",500), new Emp("b",500)), -1, 3);
        check(Arrays.asList(new Emp("a",700)), -1, 4);
        check(Arrays.asList(new Emp("a",50), new Emp("b",40), new Emp("c",60), new Emp("d",60), new Emp("e",10)), 50, 5);
    }
}
