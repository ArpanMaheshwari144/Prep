# E-commerce: hand notes (mausi ka mart, ab online bhi)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Mausi ka mart (Reliance Smart jaisa), ab online bhi. Dukaan bhi chal rahi, counter pe billing bhi, aur app se ghar delivery bhi. Stock ek hi: wahi shelf, wahi godown.

**Faisla FR se (interview me pehle):** Sale me pehle hi bol do: ghee 500 hai to 500 ko hi milega, baaki ko "out of stock" dikhega. Yahan consistency chahiye: 500 hai to 500 hi bikenge, 400 hai to 400 hi, ek bhi zyada nahi. Sabko milna zaroori nahi, baaki ko "out of stock". Ye CAP wali baat nahi hai, kyunki CAP tab aata hai jab 3-4-5 system hon aur beech ka taar toote; yahan stock ki ginti ek hi jagah hai (alag dukaan, jaise Koramangala aur Whitefield, to unka stock bhi alag). FR me hi ye cut off kar do, phir design usi hisaab se.

**Nichod (Arpan):** E-commerce me sabse bada kaam inventory management hai. Wo kar liya to 90% design wahin sort, baaki to frontend pe dikhana hai.

&nbsp;

**1. Shelf pe aakhri ek ghee. Ravi ne app pe order kiya, aur usi minute aunty wahi dabba counter pe le gayi.**
Arpan: Shelf ka maal app pe mat dikhao, warna dikkat hogi. Aunty ke haath se cheen thodi lunga. Ravi ko godown ka dikhao, aunty ko shelf ka. ATM jaisa: ATM me paisa khatam hua to duniya ka paisa khatam nahi hua, gaadi se phir aa jaata hai. Godown me ek dabba hai to Ravi ko dikhega, nahi to out of stock.
Technical me: har raaste ka alag stock. Online = godown ki ginti, counter = shelf. Dono kabhi ek dabbe pe nahi takraate.

**2. Ladka godown se 10 dabbe shelf pe le gaya, kahin likha nahi. App pe abhi bhi 40, sach me 30.**
Arpan: Parchi pe likha hai kitna maal hai. Godown se uthaya to parchi pe kaat ke likho. Aur ye ek hi kaam bana do: godown se uthana aur parchi update karna alag kaam nahi, ek hi kaam hain. To bhoolne ka mauka hi nahi.
Technical me: parchi = DB. Maal hilaana aur ginti badalna ek hi transaction me (atomic), alag-alag kadam nahi.

**3. Diwali sale, 8 baje 50 hazaar log ghee pe. App atak gaya, aata-daal wale bhi atke.**
Arpan: Bheed sale wale maal ki wajah se hai. Use bagal wali khaali dukaan me le jaao. Jo sale ke liye aaya wo wahin aaye, wahin se nikle, badi dukaan me ghuse hi nahi. Aata-daal wale aaram se khareedein.
Technical me: sale ka alag raasta / alag service. Uski bheed baaki dukaan (normal orders) tak nahi pahunchti.

**4. Sale dukaan me ek counter, ek parchi "ghee: 500". Ek waqt me ek hi aadmi parchi chhoota, line 50 hazaar ki.**
Arpan: 10 counter bana do, har counter pe 50 ghee, line baant do. Har counter pe do aadmi: ek bechne wala, ek kaatne wala. Kaatna to haath chalane jitna tez, millisecond bhi nahi.
Technical me: ek row pe sab toot pade = hot row. Ginti 10 tukdon me todo. Kaatna tez jagah pe (Redis me ginti, DECR).

**Maan lo Redis me ghee ki ginti kat gayi (Ravi gate ke andar aaya), par DB me order likhne se pehle server gir gaya.**
Arpan: Ab Redis kehta ek ghee kam hai, aur DB me koi order hi nahi. Sach DB hai, jaise Cache aur Banking me kaha tha. Ek banda peeche se Redis aur DB ko milata rahe: Redis me jitni ginti kati, utne order DB me hain ya nahi. Koi kati ginti bina order ke gate ki ghadi (5 minute) se zyada purani hai, matlab wo banda na pay kar paaya na order bana, to wo ghee wapas ginti me jod do, taaki doosra le sake. Jiska paisa aa gaya, uska dabba uska hi rahega.
Technical me: banda milata rahe = reconciliation job, DB sach = Redis sirf tez ginti, ghadi se purani bina order wali kati ginti wapas = compensation (gate ki 5 min ki ghadi ke baad hi, chalte hue pay ko nahi chhedna).

**5. Counter 3 ke 50 khatam, uski line me 2000 log. Counter 7 pe abhi 30 pade.**
Arpan: Jiska khatam hua wo counter band, aur line ko bolo jahan ghee bacha hai wahan jaao. "Khatam" tab bolo jab sabka zero ho jaaye.
Technical me: ek tukda khaali to doosre tukde se lo. Sold out tabhi jab saare tukde 0.

**6. 500 kat gaye, "sold out" dikh gaya, par 60 logon ne pay hi nahi kiya.**
Arpan: Jab tak pay nahi hua, ginti nahi kategi. Cart me daalna sirf list hai. Cart kabhi khaali mat karo: Myntra pe joota daal ke mahine baad aao to bhi wahin dikhta. Blinkit pe mummy se list banwa raha hoon, 5 minute me sab gaayab ho gaya to kya baat hui? Galle pe panni le ke khada aadmi: jab tak paisa nahi diya aur bill nahi bana, maal uska nahi.
Technical me: cart = sirf list, koi reserve nahi. Stock pay ke waqt katta. Fail hua to wapas (atomic DB query).

**7. 3000 log ek hi second me Pay dabaate hain, ghee 500.**
Arpan: Pay ki request hamare server pe aati hai, wahan line lagao: jo pehle aaya wo pehle paaya, duniya ka dastoor. Jiska paisa aaya, ginti kaato, dabba uska. 500 khatam hone ke baad jiska paisa aaya uska turant rollback, do minute me paisa wapas, aur "out of stock" bol do. Out of stock to dikhna hi hai, chahe pehle dikhe chahe baad me. Asli hal: sale ke liye stock pehle se zyada mangwao. Samsung pe discount lagaunga to 500 nahi, 10-20 hazaar laaunga.
Technical me: paisa aane pe server pe ek-ek karke ginti kaato, 500 ke baad aaye paise ka refund (rollback). Doosra raasta: Pay dabaate hi dabba ghadi (10 min) ke saath rok do, refund kam, par dabba ghadi tak ruka. Dono ki keemat.

**7b. Rollback me bhi pech, 10 minute wale me bhi pech. Koi screwdriver? (Apple store: 500 iPhone, bahar 3000 log)**
Arpan: Jitne phone utne log andar, baaki bahar. Kisi ka card decline hua ya koi chala gaya to agla batch bulao, jitne bache utne. 3000 ko andar bula ke kya fayda. App pe poora app to rok nahi sakte, aata-daal wale bhi hain. To ghee ke liye ek alag counter lagao: sale wala maal us gate se guzrega, normal checkout chalta rahega. Us counter se sirf 500 ghee guzarne hain, to dikkat hi khatam.
Technical me: sale gate (Buy now, cart me nahi jaata). Gate sirf utne logon ko Pay tak jaane deta jitna stock (Redis DECR). Fail / chala gaya = agla batch. Gate ke bahar walon ka paisa kabhi katta hi nahi, aur koi dabba bina bike nahi padta. Gate ek baar banta, har sale pe sirf list badalti (item + ginti + waqt).

**Maan lo Ravi ne 4 tab khole, Buy now 4 baar.**
Arpan: Gate pe user id hi key: ek user ka ek hi ghee rukega, Ravi ka pehle se ruka hai to wahi lauta do, naya mat roko.
Technical me: idempotency key (wahi ID dobara aaye to chhodo).

**Maan lo gate se 500 andar, Ravi Pay page pe gaya aur chai peene chala gaya, na pay kiya na cancel. Uska phone Redis me "gaya" gina hai, bahar 2500 khade.**
Arpan: Jab tak pay nahi karoge, phone bika nahi, stock me utna hi hai. Dukaan pe ghee ka dabba haath me pakad ke khada hoon, pay nahi kar raha, peeche wala dukaandar se bolta hai, to dukaandar kehta: ya to pay karke le ja, ya wapas rakh de. To andar aate hi ghadi, 5 minute. Pay kiya to phone uska, nahi kiya to wapas line me aur agla andar.
Technical me: "ya pay kar ya rakh de" = reservation expiry (TTL, 5 min), "wapas line me, agla andar" = release (Redis INCR) + agla batch. TTL khatam hone se ginti apne aap wapas nahi badhti; ek job (ya expiry ki khabar) INCR karti hai.

**Maan lo Ravi ki ghadi 10:05 pe khatam, phone Priya ko gaya, aur 10:05 + 1 second pe Ravi lauta aur Pay daba diya.**
Arpan: Saaf baat, 5 minute mera time. Andar pay kiya to phone Ravi ka. Code chalne me time lagta, to 1-2 second upar de sakte, zyada nahi. Uske baad pay dabaya to phone uska nahi, Priya ka; time window hai, cheez pakad ke baithe nahi reh sakte. BookMyShow me bhi aisa: seat rakh ke baitha raha to timer khatam, seat hat gayi, kisi aur ne le li, mujhe housefull. Aur agar time ke andar pay daba diya aur PSP dheema hai, to wo hamari zimmedari, user ki nahi.
Technical me: "5 min ke baad phone uska nahi" = Pay pe server check kare ki reservation abhi zinda hai (expiry check), "1-2 second upar" = chhota grace period, "andar pay, PSP dheema = hamari zimmedari" = Pay dabate hi PAYMENT_PENDING, hold ruk jaata (BMS jaisa).

**8. Net gaya / PSP chup: Ravi ko laga pay ho gaya, par PSP tak request pahunchi nahi, ya paisa kata aur PSP anjaan.**
Arpan: Design har jagah se thoda toot-ta hai. Chahe 10 minute rok lo, chahe paisa aane pe kaato, kinaare pe dikkat rehti hai. Aakhri second pe pay kiya to wahan BMS wali cheez hi leni padegi. Paisa kata to zimmedari hamari, PSP se hum baat karenge.
Technical me: PAYMENT_PENDING (BMS jaisa) + status job PSP se poochta rahe. Paisa kata mila to dabba uska; dabba na bacha ho to refund. Perfect design koi nahi, bas kitne refund.

**Maan lo PSP 30-50 second chup, Myntra ko pata nahi Ravi ka paisa kata ya nahi.**
Arpan: Order ki khabar Myntra ki apni hai: Pay dabate hi apne paas order PENDING aur payment INITIATED likh diya, stock rok liya. Paisa kata ya nahi, ye PSP hi batayega, Myntra Ravi ka bank nahi dekh sakta. Razorpay khud 10 minute tak rukta hai (peeche bank aur NPCI), itni der Ravi ko nahi rok sakte. Isliye ghadi hamari: 30-50 second, uske baad Ravi ko "time out, paisa kata to wapas aa jaayega, dobara pay karo" dikhao; dobara pay kare to Razorpay ki jagah doosre PSP pe bhejo, kyunki pata hai Razorpay abhi dheema hai. Baad me pata chala Razorpay pe bhi kata tha, to Razorpay ko bolo "lautao".
Technical me: order PENDING + payment INITIATED (Myntra ka apna DB), paise ki khabar sirf PSP se (webhook / status API, din ka settlement report), hamara client timeout (50 sec, PSP ka 10 min alag), der se "success" aaya to capture nahi, PSP khud wapas karega (late authorisation + auto-refund) -> isliye dobara pay doosre PSP pe chal jaata (PSP failover), do baar kata to ek wapas.

**Maan lo Ravi ka paisa kat gaya, par Myntra ko ye khabar der se mile ya mile hi na. Har baar Razorpay se poochhte rahoge?** (BookMyShow 6c wala hi jawab)
Arpan: Razorpay ko bolo "paisa katte hi mujhe chhota sa event bhej do", jaise S3 file aane pe event bhejta. Event aaya = kata, hum "mil gaya" bol dete. Event raste me kho gaya to wo Razorpay ka HLD hai, wo dobara bhejega. Aur Big Billion Day jaisi sale pe 3-4 company (Stripe, Amazon Pay) line me rakho, ek dheemi hui to doosri khadi.
Technical me: "paisa katte hi event" = webhook (PSP callback), "mil gaya" = ack (2xx), "dobara bhejega" = webhook redelivery, "3-4 company line me" = backup PSP (failover).

**Maan lo PSP ka "success" do baar aaya (ek retry se, ek status job se).**
Arpan: Payment id hi key: pehli baar aaya to order pakka, doosri baar wahi id dekh ke chhod do. Do order nahi, ginti do baar nahi kategi.
Technical me: idempotency key (wahi ID dobara aaye to chhodo).

**Agar pooche: ek order me ghee, aata, daal. Aata khatam.**
Arpan: Poora order mat roko. Jo hai wo bhejo, aate pe out of stock dikha do. Wo chahe to baaki do le le.
Technical me: har cheez alag check. Adhoora order chalta.

**Agar pooche: Lucknow ka store khaali, Kanpur me 20 dabbe.**
Arpan: Lucknow wale ko Lucknow ka store dikhega. Dark store ka matlab hi aas-paas ka store. Whitefield me baithe ko Koramangala wala dikhta.
Technical me: har store ka apna stock, app location dekh ke paas wala store chunta.

&nbsp;

Poora design = [17_ecommerce.md](17_ecommerce.md)
