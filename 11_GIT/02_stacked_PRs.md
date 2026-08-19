# Stacked PRs — bade change ko chhote dependent PRs ki chain me todo

> Market me trending dev-workflow. Senior-engineering practice: chhoti PRs = tez review + wait nahi.

---

## Problem (kyun banaya)
- Ek bada feature -> ek MASSIVE PR (~2000 line). Reviewer ghabra jaata -> review slow, bugs chhoot jaate.
- Aur agar part-2 ka kaam part-1 ke UPAR karna hai -> part-1 merge hone ka WAIT karna padta.

## Idea — ghar banane wali analogy
neev -> deewar -> chhat (ek doosre pe DEPEND karte).
Teeno ko alag-alag CHHOTE PR me todo, par CHAIN me:

```
main
 └── PR1 = neev     (base: main)
      └── PR2 = deewar   (base: PR1  <- neev ke upar)
           └── PR3 = chhat    (base: PR2  <- deewar ke upar)
```

- ★ Random line-cut NAHI -> har PR ek LOGICAL/complete tukda (dependency ke hisaab se).

## "base" ka matlab (2 cheez)
1. **Kahaan se shuru** (checkout kis branch se) -> PR2 = PR1 se, NA main se.
2. **Diff kiske against** dikhega -> PR1 ke against -> sirf NAYA tukda (deewar) dikhta.

## Kaise banate (git flow)
```bash
# PR1 (neev) — base main
git checkout main
git checkout -b PR1-neev
# ...kaam, push, GitHub pe PR (base = main)

# PR2 (deewar) — PR1 ke UPAR chahiye
git checkout PR1-neev          # <- BASE se checkout (main se NAHI)
git checkout -b PR2-deewar
# ...kaam, push, GitHub pe PR -> "base branch" dropdown me main ki jagah PR1-neev chuno
```
-> PR2 me PR1 ka code MAUJOOD hai (neev+deewar), par PR sirf DEEWAR ka diff dikhati (base PR1 hai).

## Faayda
1. Chhote PR -> tez review, kam bugs.
2. Review-velocity -> reviewer ek-ek logical piece dekhta.
3. Dependent kaam BLOCK nahi -> PR1 review me hai, tum PR2 parallel banate raho.

## Dikkat
- PR1 merge/change hoti -> PR2, PR3 ka base REBASE karna padta (chain update). Manual me painful.
- ISLIYE tools: **Graphite** (popular, GUI+CLI) · **ghstack** (Meta) · **spr** · **git-branchless**.

## 1-line recall
Bade kaam ko chhote DEPENDENT PR ki chain me tod (har PR pichhle ke UPAR, base = pichhli PR).
Chhote PR = tez review + wait nahi. Rebase-pain ke liye Graphite jaise tools.
