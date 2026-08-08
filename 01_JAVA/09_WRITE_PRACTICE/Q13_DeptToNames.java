// JAVA WRITE-PRACTICE — Q13   (thoda HARD — multi-step)
// Task: "name,dept" strings ki list -> Map< dept , list-of-names-in-that-dept >.
//   in : ["arpan,eng", "bob,sales", "cara,eng", "dev,sales"]
//   out: { eng=[arpan, cara], sales=[bob, dev] }
//
// Do cheez ek saath:
//   1) har string ko TODO -> "arpan,eng".split(",") -> ["arpan","eng"]  (parts[0]=name, parts[1]=dept)
//   2) dept pe GROUP karo, PAR bucket me poora string nahi -- sirf NAME chahiye.
//
// Naya idiom: groupingBy ka 2nd arg = "downstream collector" jo har bucket ka kya banega decide karta.
//   Q6 me counting() tha (count).  Yahan mapping():
//     Collectors.mapping( elem -> value , Collectors.toList() )
//        -> group karte-karte har elem ko 'value' me badal ke, list bana do.
//   yani: dept pe group + har entry me se sirf name nikaal ke list.
//
// SOCH (khud jodo):
//   list.stream()
//       .collect(Collectors.groupingBy(
//                    s -> s.split(",")[1],                                  // KEY = dept
//                    Collectors.mapping(s -> s.split(",")[0], Collectors.toList())  // VALUE = names list
//       ));
//
// compile+run:  javac Q13_DeptToNames.java && java Q13_DeptToNames

import java.util.*;
import java.util.stream.*;

public class Q13_DeptToNames {

    static Map<String, List<String>> deptToNames(List<String> rows) {
        return rows.stream().collect(Collectors.groupingBy(s -> s.split(",")[1],
                Collectors.mapping(s -> s.split(",")[0], Collectors.toList())));
    }

    static void check(List<String> in, Map<String, List<String>> exp, int t) {
        Map<String, List<String>> got = deptToNames(in);
        System.out.println("T" + t + ": " + (Objects.equals(got, exp) ? "PASS" : "FAIL") + "  got=" + got + " exp=" + exp);
    }

    public static void main(String[] args) {
        Map<String, List<String>> e1 = new HashMap<>();
        e1.put("eng", Arrays.asList("arpan", "cara"));
        e1.put("sales", Arrays.asList("bob", "dev"));
        check(Arrays.asList("arpan,eng", "bob,sales", "cara,eng", "dev,sales"), e1, 1);

        Map<String, List<String>> e2 = new HashMap<>();
        e2.put("hr", Arrays.asList("zoya"));
        check(Arrays.asList("zoya,hr"), e2, 2);
    }
}
