# STAR — MongoDB se MySQL wala sync Lambda load me toota, survey launch ruk gaye  [Konovo]

> Q ye cover karta: "production issue jo tumne debug kiya" · "root cause kaise nikala" ·
> "jab doosri cheez rok nahi sakte the, tab kya kiya" · "fix ke baad kaise pakka kiya".

## STAR (Hinglish)

```
   S: do DB: survey MongoDB me banta, ek sync Lambda use MySQL me daalta,
      kyunki baaki services MySQL se padhti. Ek din sync toota -> kuch survey launch nahi hue.

   T: wajah nikaalo, atke survey wapas laao, aur ye dobara na ho.

   A: logs -> MySQL connection drop ke error.
      RDS dekha -> usi waqt bid list match aur bade file upload chal rahe the. Ek saath bahut
         connection aaye, pool me jagah nahi bachi -> sync Lambda ko connection mila hi nahi.
      code me retry kamzor tha: 1-2 baar try karke haar maan leta, error phek deta.
      bid list match rok nahi sakte: client ka kaam hai, fixed time pe chalta, bada wala ~30 min tak.
         -> to load ke saath jeena tha, use rokna nahi.
      fix (code / config, AWS console me nahi):
         pool size badhaya - DB settings config file se aati (credentials Secrets Manager se),
            file me badla, Lambda ne naya value utha liya
         timeout + retry badhaye - 1-2 se ~5-7 baar
      atke survey gine (~7, exact 7-10 ke beech) -> MongoDB se data utha ke MySQL me seedha likha.
      test server: bid list upload + usi waqt survey launch -> retry 3-4 baar me nikal gaya,
         survey sync hua. Phir production.

   R: atke survey wapas, production pe uske baad ye dikkat dobara nahi aayi.
      LEARNING: shared DB pe heavy kaam ke peak ko rok nahi sakte -> jo usi DB pe hai use
                peak jhelne laayak banao (pool + retry), aur fix ko usi load pe test karo.
```

## SPOKEN (English)

```
   "At Konovo, surveys were created in MongoDB, and a sync Lambda copied them to MySQL, because
    other services read from MySQL. One day the sync broke and some surveys couldn't launch.

    In the logs I saw MySQL connection-drop errors. In RDS I saw that at the same time the bid list
    matching and some large file uploads were running. Too many connections came in at once, the pool
    had no room left, and the sync Lambda couldn't get a connection. The code also gave up too early -
    it retried once or twice and then threw an error.

    We couldn't stop bid list matching - it's client work that runs at a fixed time, up to about thirty
    minutes for the big ones. So the sync had to survive that peak. I increased the pool size in the
    config the Lambda reads, and raised the timeout and retries to around five to seven.

    Then I found the stuck surveys - about seven - and re-synced them by taking the data from MongoDB
    and writing it to MySQL. On the test server I ran a bid list upload and launched a survey at the
    same time - it retried a few times and went through. We shipped it to production, and this issue
    hasn't come back since."
```

## POOCHE TO

```
   "Pool kitna tha, kitna kiya?"        -> exact number yaad nahi. Config file me badhaya, Lambda ne
                                           wahi se padha (credentials Secrets Manager se).
   "AWS / RDS me kuch badla?"           -> nahi, code / config me badla. RDS me sirf dekha (metrics, queries).
   "Bid list match rok kyun nahi diya?" -> client ka kaam, fixed time pe chalta - unhe nahi bol sakte
                                           "abhi mat chalao". Isliye sync ko load jhelne laayak banaya.
   "Retry se aur load nahi badhega?"    -> peak thoda hi der ka hai (~30 min max); retry us dauran
                                           connection milne tak rukta hai, phir nikal jaata.
   "Kaise pakka kiya ki theek hua?"     -> test server pe wahi load banaya (bid list upload + survey launch
                                           saath), sync chala; phir production pe dobara nahi hua.
   "Kitne survey atke?"                 -> lagbhag 7 (7-10 ke beech, exact yaad nahi). MongoDB se utha ke
                                           MySQL me seedha likhe.
```
