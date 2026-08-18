# IntelliJ — NAVIGATION + SETUP TIPS (Arpan ke liye, jo Java/Spring source padhne me kaam aaye)

> Ye 27-Jul seekhe -- todoapp me Spring Security ka andar (interface -> impl -> DAO) khol ke dekhne me use kiye.

## ★ CODE-NAVIGATION SHORTCUTS

| Shortcut | Kya karta | Kab use |
|---|---|---|
| **Ctrl + Click** (ya **Ctrl + B**) | Go to DECLARATION | method/class/variable ki DEFINITION pe jao (interface/method signature) |
| **Ctrl + Alt + B** | Go to IMPLEMENTATION(s) | interface ka CONCRETE class dekho (e.g. `AuthenticationManager` -> `ProviderManager`). agar multiple -> "Choose Implementation" list |
| **Ctrl + N** | Search CLASS (by name) | koi bhi class kholo naam se -- LIBRARY classes bhi (e.g. `DaoAuthenticationProvider`) |
| **Ctrl + Shift + N** | Search FILE | file naam se |
| **Ctrl + Alt + Left** | Back (jaha se aaye) | navigation ke baad wapas |
| **Ctrl + F12** | File structure | current file ke saare methods/fields ek list me |
| **Alt + F7** | Find Usages | ye method/class KAHAN-KAHAN use hua |

## ★ INTERFACE vs IMPLEMENTATION (aaj ka core seekh)
- **Ctrl+Click on interface-method** -> sirf CONTRACT dikhta (signature, logic nahi). e.g. `AuthenticationManager.authenticate()` = bas method.
- **Actual LOGIC dekhne ko -> Ctrl+Alt+B (Go to Implementation)** -> concrete class (`ProviderManager`).
- Concrete class RUNTIME-wired dependencies (jaise `DaoAuthenticationProvider`) code me HARDCODE nahi -- interface (`List<AuthenticationProvider>`) pe kaam karta, Spring auto-config list bharti. Un tak jaane ko **Ctrl+N -> class-naam** search karo.

## ★ SETUP / DEPENDENCY ISSUES
- **"Cannot find declaration to go to"** = dependency resolved nahi / project import nahi hua.
  - FIX 1: **indexing** complete hone do (bottom bar dekho).
  - FIX 2: **Maven** tool-window -> **Reload All Maven Projects** (refresh icon) -> dependencies resolve.
  - FIX 3: **File -> Invalidate Caches / Restart**.
- **★★ Java/Maven project HAMESHA apne OWN folder se kholo** (jisme `pom.xml` ho), mixed-repo root se NAHI.
  - galti: poora `DSA_PRACTICE` khola -> todoapp (nested Maven project) ka pom import nahi hua -> Spring JARs classpath pe nahi -> navigation fail.
  - sahi: `File -> Open -> 07_PROJECTS/todoapp` (seedha project-folder) -> pom detect -> deps download -> navigation chalti.
  - (ya current window me todoapp ka `pom.xml` right-click -> "Add as Maven Project".)

## ★ READER MODE
- Library source (decompiled/attached) READ-ONLY "Reader Mode" me khulta -- javadoc formatted dikhta (jaise `AuthenticationManager` ka contract, `DaoAuthenticationProvider` ka SEC-2056 note). editing nahi, sirf padhna.
