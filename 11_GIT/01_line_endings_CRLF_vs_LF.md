# GIT #1 — Line Endings: CRLF vs LF (aur "modified-but-not-really" ka raaz)

> Ye asli dev cheez hai — Windows + git use karne wala HAR banda isse takraata hai.
> 28-Jul ko live hua: DSA folder galti se move hua -> 35 files "modified" dikhi, par ek bhi akshar nahi badla tha.

---

## KAHANI (jo hua)
DSA folder galti se `09_DATABASE` ke andar move ho gaya. Wapas root pe laane pe
`git status` ne **35 files ko "modified" (M)** dikhaya. Ghabraaya ki kaam badal gaya?
Compare kiya -> **content 100% same, sirf line-ending alag.** `git restore` -> saaf.

## ASLI BAAT — har line ke end pe ek INVISIBLE "newline" nishaan hota hai
Tujhe dikhta nahi, par har line ke end me ek special character hota hai jo bolta "yahan line khatam".
Aur do systems ise DO ALAG tareeke se likhte:

| System            | Newline    | Kitne character | Naam |
|-------------------|------------|-----------------|------|
| **Windows**       | `\r\n`     | **2 char**      | CRLF (Carriage Return + Line Feed) |
| **Linux / Mac / git** | `\n`   | **1 char**      | LF (Line Feed) |

- **CR** = `\r` = Carriage Return  (purane typewriter: "gaadi wapas shuru me")
- **LF** = `\n` = Line Feed        ("agli line pe jao")
- Windows dono use karta (2 char), baaki sirf LF (1 char). **Yehi pura jhagda.**

## VISUAL — same code, alag invisible nishaan
```
   int a = 5;<LF>          <- git/Linux version (1 char end)
   int a = 5;<CR><LF>      <- Windows version (2 char end)
   ^^^^^^^^^^  same code    ^^^^^^^^  farak sirf yahan (invisible)
```

## KYUN "modified" dikha (28-Jul ka case)
1. Git ne files repo me **LF** (1-char) me store ki hui thi.
2. Windows me folder move/copy hone pe copy ke line-endings **CRLF** (2-char) ban gaye.
3. Wo Windows-copy wapas laane pe git ne compare kiya:
   > "stored = LF, ye file = CRLF -> ALAG -> **modified**"
4. Par asli code (letters/words/logic) = **bilkul same.** Sirf invisible nishaan badla.

=> **"modified" hamesha content-change nahi hota** — kabhi sirf line-ending hota hai.
   Confirm karne ka tareeka:  `diff --strip-trailing-cr fileA fileB`  (CRLF ignore karke compare).

## ★ WO WARNING JO TU HAR COMMIT/PUSH PE DEKHTA HAI (yehi cheez hai!)
Har `git add`/commit pe git ye line deta:
```
warning: in the working copy of '...file...', LF will be replaced by CRLF the next time Git touches it
```
Iska matlab: **"is file me repo ke andar LF (1-char) hai, par teri Windows machine pe ye CRLF (2-char) ban jaayega."**
- Ye **ERROR NAHI** — sirf INFORMATION. Koi dikkat nahi, kaam kuch nahi karna.
- Mahino tak ye dikhti rahi par naam nahi pata tha -> ignore karta tha (sahi tha).
- Ab pata: ye wahi CRLF/LF wali baat hai. Git bas convert-hone ka heads-up de raha.

## FIX / PREVENTION (ye yaad rakh — real teams me use hota)
- **Turant saaf:** `git restore <path>` -> git ke LF version wapas, status clean (content same rehta).
- **Automatic handle (Windows pe recommended):**
  ```
  git config --global core.autocrlf true
  ```
  -> checkout pe LF->CRLF (editor ke liye), commit pe CRLF->LF (repo ke liye). Tujhe kuch karna hi nahi.
  (Linux/Mac pe `input` set karte: commit pe CRLF->LF, checkout pe kuch nahi.)
- **`.gitattributes`** file repo me -> team ka koi bhi OS ho, line-ending force-consistent:
  ```
  * text=auto
  *.sh text eol=lf
  *.bat text eol=crlf
  ```
  Badi teams me isi se "modified-but-not-really" confusion nahi hota.

## POWER LINE (interview / real-dev)
> "Windows aur git ke line-endings alag hain — Windows CRLF (`\r\n`), git LF (`\n`).
>  Isliye kabhi files bina content-change ke bhi 'modified' dikhti hain. Isse `core.autocrlf`
>  ya `.gitattributes` se handle karte hain taaki cross-platform repo consistent rahe."

---
> Ye DSA nahi — REAL dev knowledge. Pattern wahi: naya dikha -> samjha -> ab tera.
