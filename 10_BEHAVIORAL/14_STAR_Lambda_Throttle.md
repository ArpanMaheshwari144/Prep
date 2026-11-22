# STAR — app bahut dheema: Lambda throttle (concurrency setting badli thi)  [Konovo]

> Q ye cover karta: "production me slowness kaise debug ki" · "monitoring / graph se root cause" ·
> "kisi aur ke badlaav se bug aaya to kya kiya" · "aisa issue jo tumne khud pakda".

## STAR (Hinglish)

```
   S: app bahut dheema chal raha tha: survey ke page, bid list, match, kai page dheeme.
      dikkat kya hai pata nahi tha, isliye maine ticket raise kiya aur khud investigate kiya.

   T: pata karna app dheema kyun hai, aur theek karna.

   A: pehle browser ke console logs dekhe -> kai jagah API bahut time le rahi thi.
      phir AWS me Lambda ke graph, New Relic, Grafana, Prometheus dekhe.
      Lambda ka throttle graph: pehle sab normal, phir achanak upar,
         lagbhag 1,900-2,000 tak, laal line ko chhoo raha tha.
         page load bhi graph pe lagbhag 8 guna upar gaya.
      throttle tab hota jab Lambda ki ek saath chalne wali copies ki had (concurrency) bhar jaaye.
      maine pakda ki doosri team ke ek dev ne us Lambda ki concurrency setting ghata di thi.
      Lambda doosri team ka tha, isliye maine unhe bataya kya mila;
      jisne setting badli thi usi ne wapas pehle jaisi ki (revert).

   R: throttle ruka, app phir normal chalne laga.
      VP aur CTO ne kaam saraha.
      LEARNING: app dheema ho to sirf code nahi, infra ki setting bhi dekho:
                kya badla, kab badla.
```

## SPOKEN (English)

```
   "At Konovo, the app became very slow: the survey pages, bid lists and matching were all slow,
    and nobody knew why. I raised a ticket and started investigating.

    First I checked the browser console logs and saw several APIs taking a long time. Then I went
    to AWS and looked at the Lambda graphs, along with New Relic, Grafana and Prometheus. The
    Lambda throttle graph was normal and then suddenly shot up to around 1,900 to 2,000 throttles,
    touching the red limit line, and page load time went up about eight times.

    A Lambda throttles when its concurrency limit is full. I found that someone from another team
    had reduced the concurrency setting on that Lambda. Since it was their Lambda, I shared what I
    found with that team, and the developer who changed it reverted the setting. The throttles
    stopped, and the app was back to normal. The VP and CTO appreciated the fix.

    My learning was that when an app is slow, it is not always the code. Check infra settings too:
    what changed and when."
```

## POOCHE TO

```
   "Throttle kya hota hai?"            -> Lambda ki ek saath chalne wali copies ki had (concurrency)
                                          bhar jaaye, to nayi request ruk jaati ya fail hoti.
   "Kaise pata chala Lambda hai?"      -> console me API dheemi, phir Lambda ka throttle graph
                                          achanak upar, laal line tak.
   "Setting kitni se kitni hui?"       -> exact number yaad nahi, concurrency kam kar di gayi thi.
   "8x kaise pata?"                    -> page load ke graph se.
   "Theek kaise kiya?"                 -> Lambda doosri team ka tha; maine unhe bataya, jisne badla tha
                                          usi ne revert kiya. throttle ruka, app normal.
```
