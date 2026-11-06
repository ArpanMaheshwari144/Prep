# I: Interface Segregation (chhote interface)

**Niyam:** kisi class ko wo method likhne pe majboor mat karo jo uske kaam ke hi nahi hain.

## Kahani

Ek mota interface bana diya jo sabke liye tha. Report wali class read-only thi, use sirf padhna tha. Par interface ki wajah se
use `delete()` aur `update()` bhi likhne pade. Ye method uske kisi kaam ke nahi, isliye khaali pade hain, jo bekaar hai. Aur
khatarnak bhi hai, kyunki ye maujood hain to koi galti se bula bhi sakta hai.

```java
interface Repository { void save(); void delete(); void update(); void generatePdf(); void sendEmail(); }
class ReportRepository implements Repository {
    public void delete() { }   // bekaar + khatarnak, koi galti se bula de to?
    ...
}
```

## Fix

```java
interface Readable { List<Payment> findAll(); }
interface Writable { void save(Payment p); void delete(Long id); }
class ReportRepository  implements Readable { ... }              // sirf padhna
class PaymentRepository implements Readable, Writable { ... }    // dono
```

Ab jisko jo chahiye, wo sirf wahi interface leta hai. Report wali class sirf `Readable` leti hai, to uske paas `delete()` hai hi
nahi. Ye remote jaisa hai: TV ke remote me AC ke button nahi hote.
