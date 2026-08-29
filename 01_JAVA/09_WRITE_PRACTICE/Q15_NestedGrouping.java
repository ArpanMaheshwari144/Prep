// JAVA WRITE-PRACTICE — Q15   (HARD — 2-level nested grouping)
// Task: List<Person> ko DO level pe group karo -> dept -> gender -> names-list.
//   Return: Map< dept , Map< gender , List<name> > >
//   [ (arpan,eng,M), (bob,eng,M), (cara,eng,F), (dev,sales,M) ]
//   -> { eng={ M=[arpan,bob], F=[cara] }, sales={ M=[dev] } }
//
// Naya idiom: groupingBy ke ANDAR groupingBy (nested downstream)
//   - Q4 groupingBy -> ek level. Yahan downstream me DOBARA groupingBy -> 2 level.
//   - groupingBy(dept, groupingBy(gender, mapping(name, toList())))
//         bahar dept pe bucket -> uske andar gender pe bucket -> uske andar sirf naam ki list.
//   - DSA-connect: nested HashMap<dept, HashMap<gender, List>> -- manually banana painful, yahan 1 expression.
//
// compile+run:  javac Q15_NestedGrouping.java && java Q15_NestedGrouping

import java.util.*;
import java.util.stream.*;
import static java.util.stream.Collectors.*;

public class Q15_NestedGrouping {

    static class Person {
        String name, dept, gender;
        Person(String name, String dept, String gender) { this.name = name; this.dept = dept; this.gender = gender; }
    }

    static Map<String, Map<String, List<String>>> group(List<Person> people) {
        return people.stream().collect(groupingBy(p -> p.dept, groupingBy(
                p -> p.gender, mapping(
                        p -> p.name, toList()))));
    }

    static void check(List<Person> in, Map<String, Map<String, List<String>>> exp, int t) {
        Map<String, Map<String, List<String>>> got = group(in);
        System.out.println("T" + t + ": " + (Objects.equals(got, exp) ? "PASS" : "FAIL") + "\n   got=" + got + "\n   exp=" + exp);
    }

    public static void main(String[] args) {
        List<Person> a = Arrays.asList(new Person("arpan","eng","M"), new Person("bob","eng","M"),
                                       new Person("cara","eng","F"),  new Person("dev","sales","M"));
        Map<String, List<String>> engMap = new HashMap<>();
        engMap.put("M", Arrays.asList("arpan","bob")); engMap.put("F", Arrays.asList("cara"));
        Map<String, List<String>> salesMap = new HashMap<>();
        salesMap.put("M", Arrays.asList("dev"));
        Map<String, Map<String, List<String>>> e1 = new HashMap<>();
        e1.put("eng", engMap); e1.put("sales", salesMap);
        check(a, e1, 1);
    }
}
