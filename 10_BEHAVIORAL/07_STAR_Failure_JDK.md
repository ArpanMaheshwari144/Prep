# STAR — FAILURE: Underestimated a JDK 8 → 11 Migration  [Konovo]

> Q ye cover karta: "tell me about a FAILURE / mistake" · "a time you were WRONG / assumed wrong" ·
> "something didn't go as planned" · "what did you LEARN from a mistake".
> (Resume line: JDK 8→11 migration, zero downtime, resolved dependency conflicts.)

> ★ KEY FRAMING: failure = MERI ASSUMPTION thi (version-bump = mechanical), NOT the exact dependency.
>   isliye exact dependency-naam yaad hona ZAROORI NAHI. altitude = assumption + general-shape + LESSON.
>   (interviewer failure-story me self-awareness/lesson dekhta, purani dependency ka naam nahi.)

---

## STAR (Hinglish scaffold)

```
   S: JDK 8 -> 11 migration main owner tha. app COMPILE ho gaya + START bhi ho gaya,
      par RUNTIME pe localhost par toot raha tha. sab theek dikh raha tha -> problem SEE karne me hi time laga.

   T: migration mera tha -> pata karna mujh pe tha ki kyun tootа.

   A (failure OWN karo): meri galti = maine socha version-bump mechanical hai (bas number badlo).
      maine ye nahi socha ki Java 11 ne kuch modules HATA diye jo Java 8 me BUNDLED the.
      AI tha nahi -> StackOverflow + docs me dig kiya -> couple removed-modules trace kiye ->
      pom me EXPLICITLY wapas add kiya -> chal gaya.

   R: end me ZERO DOWNTIME ship hua. par asli cheez = LESSON.

   LESSON: "it compiles" != "it runs". version-migration mechanical nahi hota.
           ab KISI bhi migration se pehle release-notes me removed/breaking changes UPFRONT padhta hoon,
           runtime pe discover karne ke bajaye.
```

---

## INTERVIEW DELIVERY (English, ~60-75 sec — bol-ke practice)

> **S:** "During a JDK 8 to 11 migration that I owned, I hit a confusing issue — the app compiled and even
> started, but broke at runtime on localhost. Everything looked fine, so it took me a while to even realize
> where the problem was."
>
> **T:** "Since I owned the migration, it was on me to figure out why."
>
> **A:** "Honestly, my mistake was assuming a version bump would be mostly mechanical. I hadn't accounted for
> the fact that Java 11 removed some modules that were bundled in Java 8. With no AI back then, I dug through
> Stack Overflow and the docs, traced it to a couple of those removed modules, and added them back explicitly
> in the pom — and it worked."
>
> **R:** "It ended up shipping with zero downtime. But the real takeaway was the lesson."
>
> **Lesson:** "It taught me that 'it compiles' doesn't mean 'it runs' — a version migration isn't mechanical.
> Now, before any migration, I read the release notes for removed or breaking changes upfront, instead of
> discovering them at runtime."

---

## PROBE HANDLE (agar poochein "which dependency exactly?")
```
   safe line: "It was a couple of the Java EE modules Java 11 unbundled — around the XML/binding side, if I recall."
   -> "if I recall" laga -> honest, aur exact-naam pe bet nahi. exact detail pe mat fasna.
   (failure-story me interviewer wahan grill nahi karta -> lesson/ownership dekhta.)
```

## BOL-KE TIPS
```
   - failure OWN karo seedha: "my mistake was assuming..." -> blame kisi aur pe MAT daalo.
   - LESSON = sabse important 30% -> usko clearly, confidently bol.
   - low-drama theek hai (prod nahi gira) -> par genuine seekh -> safe + honest failure.
   - altitude: assumption + general-shape + lesson. exact dependency nahi (fuzzy/purana/irrelevant ab Java 21 me).
```
