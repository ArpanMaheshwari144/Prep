# BookMyShow: hand notes (do counter, ek seat)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan BookMyShow pe Avengers ke pehle show ki seat D5 book kar raha hai. Arpan ki misaal: cinema ke do counter ek hi seat bech rahe, aur ek-doosre ko pata nahi.

&nbsp;

**1. Arpan ne D5 chuni aur "book" dabaya. Usi second ek aur ladke ne bhi D5 pe book dabaya. Dono ke app ne seat khaali dekhi, aur dono ko "seat mil gayi" aa gaya.**
Arpan: Seat dekhna aur seat book karna, ise ek saath rakh, alag-alag mat rakh. Maan le do counter hain, main is counter pe, doosra us counter pe, aur andar ek hi seat bachi. Dono counter ko nahi pata ki doosra bhi wahi seat bech raha. Maine kaha "ek seat chahiye, lo mere paise". Mere counter wale ne jaake seat pe ek parchi rakh di: "ye Arpan ki seat hai, Arpan iska payment karega, ye Arpan ko milegi, kisi aur ko nahi." Doosra counter wala dekhega: "achha, yahan Arpan ki parchi lagi hai, ise main bech nahi sakta. Tu ja bhai, seat book ho gayi."
Technical me: "ek saath rakh, alag mat" = dekhna aur rokna ek hi query (UPDATE ... WHERE seat FREE), "seat pe parchi" = hold, "doosra parchi dekh ke mana" = doosre ko 0 row, "seat chali gayi".

**Maan lo Arpan ko family ke saath D5, D6, D7 teeno chahiye; counter wale ne D5, D6 pe parchi lagayi, D7 tak pahuncha to usi second doosre counter wale ne D7 pe Priya ki parchi laga di.**
Arpan: Ye almost same hai. D5 D6 D7 ka kaam ek saath hi hoga, alag-alag nahi: dekhna aur rokna teeno ka ek hi baar, beech me koi nahi aa sakta. Agar teeno available hain tabhi book karo. Maan lo mujhe D1 se D10 poora block chahiye, to main beech me khada hoke bolunga "sab sun lo, D1 se D10 meri", ek parchi pe likh dunga aur sab counter wale usi parchi se padhenge. Jaise ek chhotu board leke khada ho gaya: "D1 se D10 book hai, ise koi book nahi karega".
Technical me: "teeno ek saath, beech me koi nahi" = ek transaction (all-or-nothing, UPDATE ... WHERE seat IN (...) AND FREE; ek bhi na mili to rollback, kisi pe parchi nahi), "ek parchi poore block ki" = ek booking id ke neeche saari seat.

**2. Arpan ne D5 pe parchi laga di, phir pay kiye bina phone rakh ke chai peene chala gaya, kabhi wapas nahi aaya. D5 pe hamesha ke liye Arpan ki parchi, koi aur le hi nahi sakta.**
Arpan: Parchi ko thodi der ke liye wahin rehne de. Counter wala dekh raha: "ye payment kar raha, chal main 5 minute tak dekhta hoon. Aa gaya to theek, nahi to hata dunga yahan se." Usne 5 minute intezaar kiya, Arpan nahi aaya, to parchi phaad ke phenk di. Seat agle ke liye.
Technical me: "5 minute tak dekhta hoon" = hold expiry (hold_until), "parchi phaad ke phenk di, seat agle ke liye" = hold hata ke seat wapas FREE.

**3. Parchi phaadne wala counter wala har minute ghoom ke purani parchiyan dekhta hai. Aaj wo chhutti pe. Arpan ki parchi ko 20 minute ho gaye, abhi bhi D5 pe lagi, aur naya aadmi D5 lena chahta hai.**
Arpan: Sabse pehli baat, parchi uthane wala chhutti pe hai to uska ek chhota bhai rakh lo, wo kaam kar lega. Ya us din ke liye uska replacement laana padega. Ya phir hum us parchi pe hi umar laga dein ki wo apne aap wahan se gayab ho jaaye: parchi 5 minute tak yahan padi hai, 5 minute ke baad tu hat ja, parchi khatam.
Technical me: "chhota bhai / replacement" = backup safai job, "parchi pe hi umar" = shart query me hi (seat FREE ho YA hold_until nikal chuka), phaadne wale ka intezaar nahi.

**4. Avengers ki advance booking subah 10 baje khuli. 10 lakh log ek saath app pe toot pade, seat sirf 50 hazaar. Servers dub gaye, jo asli me pay kar rahe the unke page bhi atke.**
Arpan: Counter badha de, server badha de. Jo sirf dekhne aa raha uske liye ek alag diary, aur jo kuch hilane aa raha uske liye alag diary, primary-replica wali cheez. Aur thode-thode log andar jaao, zyada nahi. Sabse pehli baat, cinema ki capacity kitni hai, utne log andar. Maan lo mere hall me 1000 ki jagah aur saamne 5000 khade, to 1000 andar jaao, baaki 4000 ghar jaao. Aur wo line me lago: "le bhai, ye teri parchi, tu jaayega, phir agla number tera."
Technical me: "counter / server badha de" = machine badhana, "dekhne wale ki alag diary" = read replica, "capacity jitne andar, baaki line me" = virtual queue (waiting room), "ye teri parchi, agla number tera" = token.

**5. Line me Arpan ka number aaya, andar jaane ka token mila jo 10 minute chalega. Usne 8ve minute pe D5 chuni, to D5 pe 5 minute ki parchi lagi. 10ve minute pe token khatam, par parchi ke 3 minute baaki. Pay kar paayega? Kaunsi ghadi chalegi?**
Arpan: Humne use token diya, wo andar aa gaya. Iska matlab wo authenticate ho chuka, ab koi dikkat nahi. Andar aa gaya, to haan bhai, seat ruk gayi iske liye. Payment nahi karega to use hata do. Use utne time me payment karna hai jo uske liye allot hua. Kar diya to theek, nahi to agla.
Technical me: "token diya, andar aa gaya, authenticate ho chuka" = token sirf gate ka pass, "seat ruk gayi, allot hue time me payment" = pay ka faisla hold ki ghadi (hold_until) karti. Do alag ghadiyan: token = gate, hold = seat.

**6. Arpan ne 4:30 pe Pay dabaya, PSP dheema. 5:00 pe parchi ki umar khatam, 5:10 pe Priya D5 maang rahi, PSP ka jawab 6:00 pe "paisa kat gaya".**

**6a. Seat kiski?**
Arpan: Time ke andar Pay dabaya to seat Arpan ki, Priya ko nahi; humne vaada kar diya, warna consumer court ka case. PSP dheema hai to galti Arpan ki nahi, hamari taraf ki hai. To parchi pe status likh lo: pay chal raha hai, PSP ka jawab baaki. Jab tak ye likha hai, umar badha do, seat kisi aur ko dikhe hi nahi. Ya query me hi daal do.
Technical me: "status likh lo" = HELD -> PAYMENT_PENDING (Pay dabate hi), "umar badha do" = is status pe hold expire nahi hota (query aur safai job isko nahi chhoote).

**6b. Maan lo chhotu (PSP) jawab hi nahi de raha, 1-2 baar jagaya phir bhi chup. User ko kab tak rokun?**
Arpan: PSP do hi haal me hota: haan bol diya to BOOKED, theek. Dheema hai to 1-2 baar ping karo "bhai kya chal raha". Phir bhi chup to booking FAILED dikha do aur seat chhod do, kyunki peeche line lagi hai, aur bhi log pay kar rahe. Jaise mere paas do ladke: ek ko baar-baar bol raha "ja pay kar", wo mana kar raha, to "chhod, doosra ladka tu ja". Ab jo bhi naya pay karne aaye, wo doosre PSP pe jaayega, ye wala dheema hai.

Aur agar dheeme wale ne peeche se paisa kaat liya tha? User seedha PSP se baat nahi karta, beech me hum baithe hain, jaise bank me seedha manager ke paas nahi ja sakte. Har payment ki id hamare paas hai. User ko bol do "paisa kata ho to wapas aa jaayega, dobara pay kar le", aur baad me kata nikla to rollback, paisa wapas. IRCTC, GPay, Paytm sab roz yahi karte; isiliye wo likhte hain "agar paise kate to", kyunki unhe bhi pakka nahi pata.

Hum perfect system ke peeche nahi bhaag rahe. PSP hamare bas me nahi, wo dheema hai to na uski galti na hamari. Perfect consistency maango to ticket bechoge kaise; dikkat aayegi, bas user se saaf baat karo aur paisa wapas karo.
Technical me: "1-2 baar ping" = status check retry (timeout), "FAILED dikha do, seat chhod do" = booking FAILED + hold release (paisa kata nikla to refund), "doosra ladka / naye log doosre PSP pe" = circuit breaker -> PSP-2, "id hamare paas" = payment id (idempotency key), "baad me kata nikla to wapas" = reconciliation job + refund.

**6c. Gola ghoomta raha, Arpan ne thak ke back daba diya, booking FAILED, seat chhod di. Thodi der baad Razorpay ne paisa kaat liya: paisa gaya, seat bhi nahi. Ab?** (Myntra wala hi jawab; BookMyShow ka bhi apna PSP nahi, Razorpay hi hai)
Arpan: Razorpay khud 10 minute tak rukta hai, kyunki uske peeche bank aur NPCI hain, sarkari raasta, wo uske haath me nahi. 10 minute user ko nahi rok sakte, itne me to Doomsday ka poora hall book ho jaayega. To 30 se 55 second gola ghumao, uske baad saaf bol do: "payment abhi ho nahi pa raha, paisa kata hoga to apne aap wapas aa jaayega, doosre tareeke se pay kar lo". Doosra pay doosre PSP pe (Amazon Pay), Razorpay pe nahi.

Haan, onboard karte waqt Razorpay se SLA zaroor lo: unka system kitna chalega (uptime, jaise 99.9%) aur down hua to kitni der me khabar + theek karenge. Ye unke haath me hai, isliye likh ke dete hain. Bank kitni der lega, wo nahi.

Pehla wala DB me FAILED likh do. Peeche hum khud Razorpay se poochte rahenge, aur uska event bhi aayega: paisa katte hi chhota sa event, jaise S3 file aane pe bhejta, aur hum haath utha ke "mil gaya" bol dete. Baad me pata chala paisa kata tha, to Razorpay ko bolo "is user ka paisa lautao". Wo bank se poochh ke kata tha to wapas karega, nahi kata to kuch nahi. Bank se poochhne ka haq Razorpay ka hai, BookMyShow ka nahi; bank hamare server ko jawab hi nahi dega. Paisa Razorpay ne kaata, to lautayega bhi wahi; hum apni taraf DB me likh ke rakhte hain. Badi film pe 3-4 company (Stripe, Amazon Pay) line me rakho.
Technical me: "30-55 sec gola" = hamara client timeout (PSP ka 10 min wala timeout alag), "onboard pe SLA" = PSP ka uptime SLA + incident response (latency ka nahi, wo bank / NPCI pe), "doosra pay doosre PSP pe" = PSP failover (backup PSP), "hum khud poochte" = status poll (fetch payment API), "chhota event" = webhook, "mil gaya" = ack (2xx), "baad me kata nikla to lautao" = late authorisation + refund (capture nahi kiya to PSP khud auto-refund).


&nbsp;

Poora design = [09_bookmyshow.md](09_bookmyshow.md)
