# I: Interface Segregation (chhote interface)

**Koi poochhe "Interface Segregation kya hai" to aise bolo:**
```
Mota interface mat banao jisse class ko bekaar method likhne padein. Report wali class sirf
padhti hai, to use sirf Readable do; delete() uske paas hoga hi nahi. TV ke remote me AC ke
button nahi hote.
```

## Kahani

Ek mota `Repository` interface (`findAll`, `delete`, `save`). Report wali class read-only thi, par interface ki majboori me
`delete()` likhna pada, to khaali chhod diya ("mujhe kya"). Phir kisi ne job likhi: "purani report hatao".

```java
interface Repository { List<String> findAll(); void delete(String id); void save(String r); }
class ReportRepository implements Repository {
    public void delete(String id) { }   // khaali
}
```

Chala ke dekha:

```
--- purani report hatao wali job ---
r1 delete: OK
r2 delete: OK
r3 delete: OK
job khatam. bachi report: [r1, r2, r3]
```

**Dikkat:** job ne teeno pe "OK" bola, ek bhi nahi hati. Koi error nahi, sab samjhe safai ho gayi, DB bharta raha.

## Fix

```java
interface Readable { List<Payment> findAll(); }
interface Writable { void save(Payment p); void delete(Long id); }
class ReportRepository  implements Readable { ... }              // sirf padhna
class PaymentRepository implements Readable, Writable { ... }    // dono
```

Ab report pe `delete()` bulao to compile hi nahi hota:

```
error: cannot find symbol
    repo.delete("r1");
```

## Trap

Majboori me method khaali chhodna (L me bhi yahi tha, jad ek): jo kaam class kar hi nahi sakti, wo method uske paas ho hi nahi.
Khaali method chupchaap "OK" bolta hai, exception se bhi bura.
