// JAVA WRITE-PRACTICE — Q16   (HARD — multi-key sort, Comparator chaining)
// Task: List<Emp> ko sort karo -> pehle DEPT ascending, us ke andar SALARY descending.
//       Return: sorted order me names ki List.
//   [ (arpan,eng,100), (bob,sales,200), (cara,eng,150), (dev,sales,50) ]
//   -> eng(150 cara, 100 arpan), sales(200 bob, 50 dev)  ->  [cara, arpan, bob, dev]
//
// Naya idiom: Comparator chaining (comparing + thenComparing + reversed)
//   - Comparator.comparing(keyFn)               -> ek key pe sort (asc)
//   - .thenComparing(anotherComparator)         -> tie hone pe doosri key
//   - Comparator.comparingInt(fn).reversed()    -> int key pe DESCENDING
//   - .sorted(comparator) -> phir map(name) -> collect(toList())
//   - DSA-connect: custom comparator wala sort -- 2 key ke saath.
//
// compile+run:  javac Q16_MultiKeySort.java && java Q16_MultiKeySort

import java.util.*;
import java.util.stream.*;
import static java.util.stream.Collectors.*;

public class Q16_MultiKeySort {

    static class Emp {
        String name, dept; int salary;
        Emp(String name, String dept, int salary) { this.name = name; this.dept = dept; this.salary = salary; }
    }

    static List<String> sortNames(List<Emp> emps) {
        // TYPE kyun ek jagah likha, doosri me nahi? -> context hai ya nahi.
        //   .map(e -> e.name)  -> e ka type NAHI likha: yahan Stream<Emp> pe hain -> Java stream se
        //        janta e=Emp (context maujood) -> khud infer.
        //   Comparator.comparing((Emp e) -> e.dept) -> (Emp e) LIKHA: ye comparator ALAG standalone
        //        ban raha, stream se juda nahi -> Java ke paas e ka type janne ka context NAHI ->
        //        khud batana pada. + thenComparing chain se inference aur toot-ti -> explicit type zaroori.
        //   RULE: stream-ke-andar (map/filter) -> type stream se aata (na likho); comparator/collector
        //        jo alag+chained ban raha -> context nahi -> type khud likho. (comparing se (Emp e) hatao -> error.)
        return emps.stream().sorted(Comparator.comparing((Emp e) -> e.dept)
                .thenComparing(Comparator.comparingInt((Emp e) -> e.salary).reversed()))
                .map(e -> e.name).collect(toList());
    }

    static void check(List<Emp> in, List<String> exp, int t) {
        List<String> got = sortNames(in);
        System.out.println("T" + t + ": " + (Objects.equals(got, exp) ? "PASS" : "FAIL") + "  got=" + got + " exp=" + exp);
    }

    public static void main(String[] args) {
        List<Emp> a = Arrays.asList(new Emp("arpan","eng",100), new Emp("bob","sales",200),
                                    new Emp("cara","eng",150),  new Emp("dev","sales",50));
        check(a, Arrays.asList("cara","arpan","bob","dev"), 1);   // eng(desc sal), sales(desc sal)

        List<Emp> b = Arrays.asList(new Emp("p","a",10), new Emp("q","a",20));
        check(b, Arrays.asList("q","p"), 2);   // same dept, salary desc
    }
}
