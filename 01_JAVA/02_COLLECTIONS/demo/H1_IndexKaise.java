public class H1_IndexKaise {

    public static void main(String[] args) {

        int n = 16;
        String key = "Arpan";

        int hashCode = key.hashCode();

        int index = (n-1) & hashCode;

        System.out.println(index);
        System.out.println(key);
        System.out.println(hashCode);

    }
}
