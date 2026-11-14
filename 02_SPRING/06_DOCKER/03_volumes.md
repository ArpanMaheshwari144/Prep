# Docker 03 — Volume (data dabbe ke bahar)

[← Back to README](00_README.md) | [← Spring+MySQL](02_spring_mysql_host.md) | [Transition →](04_transition.md)

---

## Koi poochhe to aise bolo

**Volume kya hai:** dabbe (container) ke bahar, machine ki disk pe ek alag almari, jo dabbe se judi rehti hai. MySQL jo bhi likhta hai wo dabbe ke andar nahi, is almari me jaata hai. Dabba hata do, almari bachi rehti hai; naya dabba usi almari se jodo to saara data wapas.

**Kyun:** dabba to aata-jaata rehta hai (naya version, crash, hata ke dobara banana). Agar DB ka data dabbe ke andar hi rakha to dabba gaya to data gaya. Volume se dabba aur data ki zindagi alag ho jaati hai.

---

## Kahani: mez aur almari

```
bina volume: kaagaz mez pe rakha     -> mez hatai, kaagaz bhi gaya
volume ke saath: kaagaz almari me    -> mez hatai, almari wahi
                                        nayi mez lagao, almari se jodo, kaagaz safe

Container = mez (aati-jaati)
Volume    = almari (tikti hai)
```

---

## Chala ke dekha

**Bina volume:** MySQL me user (Arpan) aur token the. Dabba hata ke naya chalaya, andar dekha to table hi khaali. Saboot: dabba gaya to data gaya.

**Volume ke saath:** MySQL ka data wala folder ek volume se joda. Dabba hataya, naya dabba usi volume se joda, andar dekha to Arpan wali row wahi baithi thi. Volume ne data bacha liya.

---

## Do tarah ki almari

Ek, jise Docker khud sambhalta hai (naam do, kahan rakhi Docker jaane). DB ke liye yahi theek.

Doosri, apne laptop ka koi folder seedha dabbe se jod do. Kaam ki tab, jab code badlo aur dabbe me turant dikhe (dev me). Par Windows folder Linux dabbe se jodo to kabhi permission ki dikkat aati hai.

---

## Do trap jo khud dekhe

**Bina naam ki almariyan jama:** MySQL ki image khud kehti hai "mera data folder kahin bahar rakho". To bina volume diye chalaya tab bhi Docker ne chupchaap ek bina naam ki almari bana di. Teen baar aise chalaya to teen anjaan almariyan pad gayi, jinhe dobara jodna mushkil. Isliye hamesha naam wali almari do, aur bekaar wali saaf karo.

**Galat jagah jodi:** almari dabbe ke andar ke us folder se jodni hai jahan MySQL sach me likhta hai. Galat folder se jodi to almari khaali rehti hai aur data phir dabbe ke andar, yaani phir jaane ke liye taiyaar. Sahi folder image ke docs se lo.

---

## Aage ki kahani

Ab tak Spring laptop pe tha, MySQL dabbe me. Agla kadam: Spring bhi dabbe me. Tab `localhost` ka matlab hi badal jaata hai ([04](04_transition.md)).

[← Back to README](00_README.md) | [← Spring+MySQL](02_spring_mysql_host.md) | [Transition →](04_transition.md)
