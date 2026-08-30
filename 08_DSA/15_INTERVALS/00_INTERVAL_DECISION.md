# INTERVAL-FAMILY — kaunsa tool? (30-Aug)

> Input HAMESHA same: list of [start,end]. FARAK = kya POOCHA gaya (verb), input nahi.

## Decision
```
"MERGE overlapping / combine"      -> sort by START + merge
"MAX non-overlapping pick / min remove" -> GREEDY: sort by END, free ho to pick
"kitne ek-SAATH / MIN rooms"       -> line-sweep (start +1, end -1, running-count ka MAX)
```

## Greedy (max non-overlapping) — samajh
- Greedy = ALAG technique NAHI. Wahi tera sort + loop + check-overlap.
- "Greedy" sirf ek NAAM: har step pe locally-best (jaldi-khatam wali) pakdo -> globally-max mil jaata.
- ★ KEY = sort by **END** (start se nahi).

### KYUN end se? (contrast)
```
A:1-10  B:2-3  C:4-5

START se sort (A,B,C): A pick(khatam 10) -> B,C dono clash -> sirf 1.   (lambi meeting sab kha gayi)
END se sort  (B,C,A): B pick(3) -> C(start4>=3) pick(5) -> A clash -> 2.  SAHI.
```
Jaldi-khatam wali max room chhodti -> zyada fit.

## Ek line
```
"max select non-overlap" dikhe -> END se sort + greedy pick.
"min rooms / kitne ek-saath" -> line-sweep.
"merge overlapping" -> START se sort + merge.
```
