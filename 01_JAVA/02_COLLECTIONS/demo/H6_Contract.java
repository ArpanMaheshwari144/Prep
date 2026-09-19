// ============================================================
// HashMap #6 — hashCode / equals ka CONTRACT
// ============================================================
// chalana:   java H6_Contract.java
// ============================================================
//
// H5 me dekha:  HashMap get() do cheez karta hai
//                  1. hash se BUCKET dhoondta
//                  2. us chain me equals() se SAHI key pehchanta
//
// To agar apni class me ye do method THEEK se na likhe, to kya tootega?
//
// Is stub me hum TEEN halat dekhenge, ek hi Employee class ke saath:
//
//     (a) dono me se KOI nahi likha    ->  Object wale default chalte
//     (b) sirf equals() likha          ->  hash alag, equals same
//     (c) dono likhe                   ->  sahi
//
// ============================================================

import java.util.*;

public class H6_Contract {
    static class Employee{
        int id;
        Employee(int id){
            this.id = id;
        }

        @Override
        public boolean equals(Object obj) {
            Employee e = (Employee) obj;
            return id == e.id;
        }

        @Override
        public int hashCode() {
            return Objects.hash(id);
        }
    }

    public static void main(String[] args) {

        Employee obj1 = new Employee(101);
        Employee obj2 = new Employee(101);

        System.out.println(obj1.hashCode());
        System.out.println(obj2.hashCode());
        System.out.println(obj1.equals(obj2));

        HashMap<Employee, String> map = new HashMap<>();
        map.put(obj1, "A");
        System.out.println(map.get(obj2));
    }
}


