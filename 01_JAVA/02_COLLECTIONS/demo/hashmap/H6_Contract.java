package hashmap;

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


