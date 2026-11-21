# DB 05 — Normalization (ek baat ek hi jagah) + Denormalization (padhne ke liye jaan-boojh ke dohrao)

---

## Koi poochhe to aise bolo

Normalization matlab ek badi table ko chhoti, judi hui tables me todna, taaki koi bhi baat sirf ek jagah likhi ho. Agar ek
hi cheez (jaise teacher ka phone) das row me dohrayi gayi, to badalte waqt kahin chhoot jaayegi aur data aapas me ulta ho
jaayega. Nuksaan: padhne ke liye JOIN lagane padte. Isliye jahan padhai bahut zyada ho aur tez chahiye (dashboard, feed),
wahan jaan-boojh ke data dohrate hain; use denormalization kehte hain.

---

## Kahani: college ka ek mota register

```
| student | course | teacher | teacher ka phone |
| Arpan   | DBMS   | Rao     | 99999            |
| Arpan   | OS     | Iyer    | 88888            |
| Ravi    | DBMS   | Rao     | 99999            |   <- Rao ka phone do baar likha
```

Isse teen dikkat:
- **Badalna:** Rao ka phone badla to har row me badalna padega; ek bhi chhooti to do alag phone number dikhenge.
- **Jodna:** naya course "AI" jodna, par abhi koi student nahi, to row hi nahi ban sakti.
- **Mitana:** Arpan ki OS wali row hatayi to Iyer ka phone bhi gayab, kyunki wo kisi aur row me tha hi nahi.

---

## Hal: teen kadam

- **1NF:** ek khaane me ek hi cheez. "DBMS, OS" jaisi list ek khaane me mat bharo, har course ki alag row.
- **2NF:** jo cheez key ke sirf ek hisse pe tiki hai, use alag nikaalo. Course ka naam sirf course pe tika, student se
  lena-dena nahi. To STUDENTS, COURSES aur ek jodne wali table STUDENT_COURSE alag.
- **3NF:** jo cheez key pe nahi, kisi doosre column pe tiki hai, use bhi alag karo. Teacher ka phone teacher pe tika, course
  pe nahi. To INSTRUCTORS(id, naam, phone) alag, aur course me sirf teacher ka id.

Yaad ki ek line: har column key pe tike, poori key pe tike, aur sirf key pe tike.

---

## Kab kya

- **Paisa, order, banking** (likhai zyada, galti nahi chalegi): normalize, 3NF.
- **Dashboard, report, feed** (padhai zyada, tez chahiye): denormalize, data dohra lo taaki JOIN kam.

---

## HLD se jod

Twitter me har user ki "parchi" (feed) me tweet pehle se rakhe = denormalization. MongoDB me poora order ek document me =
denormalization. Payment / banking me hisaab ek hi jagah = normalization.

---

## Trap

Denormalize kiya to update wali dikkat wapas aati: teacher ka naam do jagah likha, ek hi jagah badla. Isliye jahan dohraya,
wahan badalne ka raasta pakka ho (sab jagah ek saath, ya peeche koi job sambhale).
