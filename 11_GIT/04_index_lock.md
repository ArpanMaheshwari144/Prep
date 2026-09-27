# GIT #4 — `index.lock`: "Unable to create index.lock: File exists" (atka hua board)

> Live hua 27-Sep: Spring notes commit karte waqt git ne mana kar diya — raat 1:45 ki ek
> purani lock file padi thi, koi git chal nahi raha tha. Hata ke commit ho gaya.

---

## KAHANI (jo hua)
`git add` chalaya -> error:
```
fatal: Unable to create 'C:/DSA_PRACTICE/.git/index.lock': File exists.
Another git process seems to be running in this repository ...
```
Dekha: `.git/index.lock` raat 1:45 ki (0 byte), aur us waqt koi `git.exe` process chal hi nahi raha tha
-> purana atka board -> hataya -> commit + push ho gaya.
(1:45 pe kaunsa command beech me ruka tha — check nahi kiya; VS Code ka git ya koi aur command ho sakta.)

## ASLI BAAT — `.git/index` = agle commit ki LIST
```
.git/index   -> isme likha hota ki agle commit me kya-kya jaayega (git add wali list)

Koi bhi git command index badalne se PEHLE ek khaali file banata:
   .git/index.lock    <- "board laga diya: main likh raha hoon, beech me mat aana"

Kaam khatam -> lock file KHUD mit jaati
```

## KYUN chahiye
```
Do git command ek saath index me likhein -> file kharab (jaise do log ek register me ek saath likhein)
Lock hai -> doosra command RUK jaata: "index.lock: File exists"
```

## Atakta KAB hai
```
git command beech me mara (crash / terminal band / kill / laptop band)
   -> lock mitane ka mauka hi nahi mila -> board laga reh gaya
   -> ab har git command yahi error dega, jab tak file na hatao
```

## FIX — pehle check, phir hatao
```bash
# 1. koi git chal to nahi raha?  (Windows)
tasklist | findstr git
# editor (VS Code / IntelliJ) ka git bhi dekh lo — koi commit / pull chal raha ho

# 2. kuch nahi chal raha -> purana board -> hatao
rm .git/index.lock          # (PowerShell: Remove-Item .git\index.lock)
```
```
SAFE    koi git process NAHI chal raha  -> purana board, hata do
KHATRA  sach me git command chal raha   -> hataya to index kharab ho sakta -> pehle use khatam hone do
```

## Pehle se jaanta hai — bas naam alag
```
synchronized / mutex   ek waqt pe ek thread
DB row lock            ek waqt pe ek transaction (HikariCP incident: query lock pakad ke baithi rahi)
index.lock             ek waqt pe ek git command

Farak: yahan lock ek FILE hai. Process lock pakad ke mar jaaye -> lock atka reh jaata.
HikariCP wale din bhi lock atka tha -> DevOps ne query kill ki. Yahan file delete.
```

## 1-line recall
> **`index.lock` = git ka "main likh raha hoon" board. Command beech me mara to atak jaata — koi git
> nahi chal raha to file hata do, chal raha ho to ruko.**
