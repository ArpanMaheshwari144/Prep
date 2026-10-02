# STAR — FAILURE: Underestimated a JDK 8 → 11 Migration  [Konovo]

> Q ye cover karta: "tell me about a FAILURE / mistake" · "a time you were WRONG" ·
> "what did you LEARN from a mistake" · "a DECISION you made caused a problem — what would you do differently?"

## STAR (Hinglish)

```
   S: JDK 8 -> 11 migration main owner tha. app COMPILE ho gaya + START bhi ho gaya,
      par RUNTIME pe localhost par toot raha tha. sab theek dikh raha tha -> problem SEE karne me hi time laga.

   T: migration mera tha -> pata karna mujh pe tha ki kyun toota.

   A: meri galti (MERA FAISLA) = maine socha version-bump mechanical hai (bas number badlo).
      maine ye nahi socha ki Java 11 ne kuch modules HATA diye jo Java 8 me BUNDLED the.
      AI tha nahi -> StackOverflow + docs me dig kiya -> couple removed-modules trace kiye ->
      pom me EXPLICITLY wapas add kiya -> chal gaya.

   R: end me ZERO DOWNTIME ship hua.

   LESSON: "compile hone ka matlab nahi ki RUN ho gaya"
           "number badalne ka matlab nahi ki MIGRATE ho gaya"
   ALAG KYA KARTA: shuru karne se PEHLE release-notes me removed/breaking changes padhta,
           runtime pe dhoondhne ke bajaye.
```

## SPOKEN (English)

```
   "During a JDK 8 to 11 migration that I owned, the app compiled and even started, but broke at
    runtime on localhost. Everything looked fine, so it took me a while to even see where the problem was.

    Since I owned the migration, it was on me to figure out why.

    My mistake was assuming a version bump would be mostly mechanical. I hadn't accounted for the fact
    that Java 11 removed some modules that were bundled in Java 8. I dug through Stack Overflow and the
    docs, traced it to a couple of those removed modules, and added them back explicitly in the pom.

    It shipped with zero downtime. It taught me two things: 'it compiles' doesn't mean 'it runs', and
    changing the version number doesn't mean you've migrated. Now, before any migration, I read the
    release notes for removed or breaking changes upfront, instead of discovering them at runtime."
```
