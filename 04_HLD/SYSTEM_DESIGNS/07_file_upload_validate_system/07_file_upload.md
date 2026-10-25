# File upload: shuru se, ek kahani

Kram wahi jo interview me hota hai: **FR, phir NFR, phir numbers, phir design**. Design sabse seedhe dabbe se
shuru hota hai, aur har dikkat aane pe ek naya dabba judta hai. Diagram me peela dabba = abhi juda. Poori file me
ek hi kahani hai: Arpan income tax ki site pe apni `report.pdf` daalta hai.

---

## File upload hai kya, aur kyun chahiye

Arpan income tax ki site pe apni `report.pdf` daalta hai. Site ko us file ke saath teen kaam karne hain: file
lena aur sambhal ke rakhna, file ki jaanch karna, aur Arpan ko batate rehna ki file kahan tak pahunchi.

Jaanch hamari site khud nahi karti. Ye kaam bahar ki ek company karti hai (Validator): file me virus to nahi,
format sahi hai ya nahi. Ek file jaanchne me use lagbhag 2-3 second lagte hain.

Arpan ko haal batane ke liye har file ko ek tracking number milta hai, jaise `T-101`. Is number se wo kabhi bhi
dekh sakta hai ki uski file kahan tak pahunchi.

Ye JP ne sach me poocha tha (SDE-3 writeup). Aam product design hai, finance nahi.

## 1. FR: kya karna hai

```
file upload ho, bahar wali company jaanche, file store ho, user ko tracking number aur haal mile
scope ke bahar: editing, sharing, versions
```

## 2. NFR: kaisa chalna chahiye

```
- user ko rukna na pade (yahi is design ka dil hai)
- file kabhi khoye nahi
- haal hamesha pata ho ki file kahan tak pahunchi
- bheed me bhi chale, aur safe rahe
```

## 3. Numbers

```
- roz 1 lakh file -> ~1-2 upload har second (kam hai)
- deadline wale din (31 March) aakhri ghante me ~100 guna -> ~150-200 upload / sec
- log apna haal baar-baar dekhte hain -> padhna bahut zyada, likhna kam
```

Pehle poochho: jaanch kitna time leti hai (2-3 sec), aur nateeja turant chahiye ya user baad me haal dekh lega.
Turant nahi chahiye, to aage async + tracking aayega.

---

## 4. Design
