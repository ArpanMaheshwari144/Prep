# Payment: hand notes (dukaan pe pay)

Sirf block ki heading padho, scene aur hal khud bolo. Atke to neeche dekho.

**Scene:** Arpan dukaan pe khade ho ke app se pay karta hai. Uska paisa HDFC me, dukaan ka ICICI me. Hamara app kisi bank ka nahi.
**Shabd:** parchi / register = hamara DB · beech ki company = PSP · ladka = poochne wala job · beech ka khaata = hold

Wallet se wallet = banking jaisa hi (ek transaction, parchi ka number, ek query, ledger). Naya sirf: paisa bahar ke bank se aana aur jaana.

&nbsp;

**1. App HDFC ka nahi, HDFC hamari sunta nahi**
Har bank se khud judna saalon ka kaam. Beech me ek centralized company jo sab bankon se judi hai, bas uski API use karo.
**PSP (Razorpay, Juspay)**
Interview: "Bank se seedha nahi judte, PSP ki API se. Wo thodi fees leta hai, bank / NPCI se judna aur card ka data (PCI) wo sambhalta. Wallet me paisa rakhne ka RBI PPI licence phir bhi hamara."

**2. PSP 30 second chup**
Apne paas ek parchi rakho: pay dabate hi INITIATED, jawab nahi to PENDING. App pe FAILED ya SUCCESS nahi, "pakka hote hi batayenge".
**INITIATED, PENDING, timeout**
Interview: "Unknown = PENDING. FAILED dikhaya to user dobara pay karega, SUCCESS dikhaya to nakli paisa. PSP call pe 5 sec timeout."

**3. PSP batana bhool gaya, payment hamesha PENDING**
Beech me ek banda bitha do jo PSP se poochhta rahe "hua ya nahi". Aur PSP khud bhi batata hai.
**poochne wala job + webhook**
Interview: "Webhook se PSP batata hai, aur har minute job PENDING utha ke PSP se poochhta hai. Raat ko PSP ki file se milaan."

**4. PSP-1 poora band**
Naye payment PSP-2 pe bhej do. Jo PSP-1 pe atke hain unhe roko, "thodi der me batayenge", unhe PSP-2 pe dobara mat bhejo.
**circuit breaker, fallback PSP**
Interview: "Fail badhe to breaker PSP-1 ko call rokta hai, naye PSP-2 pe. PENDING wala PSP-2 pe nahi, warna do baar kat sakta."

**5. Server PSP ko call ke beech gira**
Pehle register me likho, phir bhejo. Restart pe likha mila to PSP se poochho. Dobara bhejna pade to wahi key, PSP do baar nahi kaatega.
**INITIATED pehle + idempotency key**
Interview: "PSP call se pehle INITIATED commit. Job INITIATED uthata hai, payment id hi PSP ki idempotency key."

**6. Dukaan ke 500 ICICI me jodte waqt FAILED**
Sab ulta kar do: wallet me wapas, register me ulti line, jaise bank wala "aaj nahi, kal aana" bole to paise lauta deta hai.
**saga (compensate)**
Interview: "Bahar ka bank ek transaction me nahi aata, isliye saga: kadam-dar-kadam, fail pe ulta kadam."

**7. Beech ke 2 minute me 500 gayab dikh rahe**
Wallet se kaatte hi likh lo ki 500 abhi kahan hain. Koi pooche to bata do.
**hold (beech ka khaata)**
Interview: "Wallet -500, hold +500, payout INITIATED. SUCCESS pe hold se nikalo, FAILED pe wapas wallet. Paisa har waqt kahin dikhe."

**8. "Sabse pehle poochh lo, sab taiyaar ho?"**
Ek dheema hua to sab uske liye ruke, jaise cheating me dost dheema likhe to dono ka paper chhoot jaaye.
**2PC nahi**
Interview: "2PC me ek ruka to sab ruke, aur bank / PSP us round me aate hi nahi. Isliye saga + hold."

**9. Diwali pe 3,000 payment / sec, register nahi jhel raha**
Register ke tukde, har tukde ka apna likhne wala. Kaam baant do.
**shard by account**
Interview: "Account id se shard: ek user ka wallet, ledger, payment ek shard pe. Date se nahi, warna aaj ki saari likhai ek pe."

**10. Arpan tukda 1, dukaan tukda 3**
Ek ladka 500 aur diary le ke jaata hai. Pahuncha to hisaab poora, gadbad hui to diary dekh ke paise wapas.
**saga do shard ke beech**
Interview: "Cross-shard transfer bhi saga + hold se. Ek transaction do shard pe nahi chalta."

&nbsp;

Poora design = [06_payment_system.md](06_payment_system.md)
