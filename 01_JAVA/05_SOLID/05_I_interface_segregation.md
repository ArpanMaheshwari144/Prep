# I: Interface Segregation (chhote interface)

**Niyam:** kisi class ko wo method likhne pe majboor mat karo jo uske kaam ke nahi.

## Kahani

Ek mota interface sabke liye bana diya. Read-only report wali class ko sirf padhna tha, par interface ki wajah se
`delete()`, `update()` bhi likhne pade: khaali ya khatarnak.

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

Jisko jo chahiye, wahi le. Remote jaisa: TV ke remote me AC ke button nahi.

## Interview line

```
I keep interfaces small and focused, so a read-only report repository implements only Readable and is
never forced to carry a delete method it should not have.
```
