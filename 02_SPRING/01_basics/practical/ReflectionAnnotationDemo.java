// ============================================================
// REFLECTION + ANNOTATION DEMO — "Spring sticker (annotation) kaise PADHTA hai?" = REFLECTION
// Plain Java (Spring dependency ZERO). Yahan 02_SPRING me isliye ki maqsad Spring-internals samajhna.
//
// ★★ HONEST CAVEAT (revise me galatfehmi na ho):
//    Ye demo sirf "PADHNA" (sticker read + decide) step ISOLATE karke dikhata — taaki reflection
//    wali aankh aankhon se dikhe. Yahan koi proxy ACTUALLY nahi banta (BankService raw object;
//    getClass() pe koi $$CGLIB$$ nahi).
//    REAL Spring aisa ALAG nahi karta: padhna + proxy-banana DONO ek hi flow me SAATH chalte
//    (BeanPostProcessor sticker dekhta aur usi waqt raw ki jagah proxy return karta).
//      -> demo = teaching-simplification (ek step isolate) · Spring = one integrated flow.
//    (asli proxy BANTE hue SpringInternalsDemo /internals me dekha -> UserService$$SpringCGLIB$$)
// ============================================================

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