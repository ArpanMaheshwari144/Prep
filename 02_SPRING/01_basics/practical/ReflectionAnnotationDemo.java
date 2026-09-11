import java.lang.annotation.*;
import java.lang.reflect.Method;

@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.METHOD)
@interface
MyTransactional { }

class BankService {
    @MyTransactional
    public void transfer() { }

    public void ordinaryMethod() { }
}

public class ReflectionAnnotationDemo {
    public static void main(String[] args) {
        Method[] methods = BankService.class.getDeclaredMethods();
        for (Method method : methods) {
            if(method.isAnnotationPresent(MyTransactional.class)) {
                System.out.println(method.getName() + " PROXY chahiye");
            }
            else{
                System.out.println(method.getName() + " plain");
            }
        }
    }
}