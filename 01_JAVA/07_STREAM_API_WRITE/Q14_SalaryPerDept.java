// JAVA WRITE-PRACTICE — Q14   (thoda HARD — downstream aggregation)
// Task: List<Emp> se Map< dept , us dept ki TOTAL salary >.
//   [ (arpan,eng,100), (bob,sales,200), (cara,eng,150), (dev,sales,50) ]
//   -> { eng=250, sales=250 }
//
// Naya idiom: groupingBy + summingInt (downstream collector)
//   - Q6 me counting() (ginti), Q13 me mapping() (list) tha -- ab summingInt() (jod).
//   - groupingBy(keyFn, Collectors.summingInt(valueFn))
//         key = dept, har bucket me list ki jagah -> salary ka SUM.
//   - DSA-connect: HashMap me for-loop, map[dept] += salary -- yahi ek line.
//
// compile+run:  javac Q14_SalaryPerDept.java && java Q14_SalaryPerDept

import java.util.*;
import java.util.stream.*;
import static java.util.stream.Collectors.*;

public class Q14_SalaryPerDept {

    static class Emp {
        String name, dept; int salary;
        Emp(String name, String dept, int salary) { this.name = name; this.dept = dept; this.salary = salary; }
    }

    static Map<String, Integer> salaryPerDept(List<Emp> emps) {
        return emps.stream().collect(Collectors.groupingBy(e -> e.dept, summingInt(e -> e.salary)));
    }

    static void check(List<Emp> in, Map<String, Integer> exp, int t) {
        Map<String, Integer> got = salaryPerDept(in);
        System.out.println("T" + t + ": " + (Objects.equals(got, exp) ? "PASS" : "FAIL") + "  got=" + got + " exp=" + exp);
    }

    public static void main(String[] args) {
        List<Emp> a = Arrays.asList(new Emp("arpan","eng",100), new Emp("bob","sales",200),
                                    new Emp("cara","eng",150),  new Emp("dev","sales",50));
        Map<String,Integer> e1 = new HashMap<>(); e1.put("eng",250); e1.put("sales",250);
        check(a, e1, 1);

        List<Emp> b = Arrays.asList(new Emp("x","hr",500));
        Map<String,Integer> e2 = new HashMap<>(); e2.put("hr",500);
        check(b, e2, 2);
    }
}
