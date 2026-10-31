# Payment: hand notes (dukaan pe pay)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan dukaan pe khade ho ke app se pay karta hai. Uska paisa HDFC me, dukaan wale ka ICICI me. Hamara app na HDFC ka, na ICICI ka.

&nbsp;

**1. Arpan ne "pay 500" dabaya, app ko HDFC khaate se 500 nikalne hain. Par app HDFC ka hissa nahi, HDFC hamari sunta hi nahi. Har bank se khud judne jaayein to alag manzoori, saalon lag jaayenge.**
Arpan: Ek aisa system bana do, aisi API jo ye kaam khud kare. Har cheez ke liye alag-alag nahi, sirf ek centralized tareeka ho, koi centralized authority jo ye kaam khud kare. Bas hum uski cheez use karein.
Technical me: "centralized authority jo ye kaam khud kare" = PSP (Razorpay, Juspay), "bas hum uski cheez use karein" = PSP ki API.

**2. App ne PSP ko bola "500 kaato", aur PSP 30 second se chup. FAILED dikhaya to Arpan dobara pay karega aur 1,000 kat sakte. SUCCESS dikhaya to ho sakta paisa kata hi na ho, aur dukaan wala bina paise saamaan de de.**
Arpan: Hum apne paas ek parchi rakh lein, jisse dekh sakein PSP pe kya ho raha. Pay dabaya, to maine apne paas parchi rakh di: INITIATED ho gaya. Parchi pe likha rahega kya-kya hua: PSP ne ye kara, ab ye kara. Fail ho gaya to FAIL mark kar do, pass ho gaya to PASS mark kar do.
Technical me: "apne paas ek parchi" = payment table me status, "INITIATED ho gaya" = INITIATED status, "fail hua to FAIL, pass hua to PASS mark" = pakka jawab aane pe hi SUCCESS / FAILED.

**3. Parchi pe jawab ka intezaar hai. PSP ka pakka jawab pata kaise chale? Aur PSP batana bhool jaaye, ya uska message raste me kho jaaye, to payment hamesha atka rahega?**
Arpan: Beech me ek aisa banda bitha do jo humein batata rahe: PSP hua ya nahi hua, bata do. Beech me ek aisa system bithao jo permanent dono se baat karta rahe: haan bhai, hua, nahi hua.
Technical me: "beech me banda jo batata rahe hua ya nahi" = poochne wala job (PENDING utha ke PSP se status poochhe), "permanent dono se baat karta rahe" = PSP aur hamare DB ka milaan.

**4. Aaj PSP-1 poora band, uski har call fail. Dukaan pe log pay kar rahe aur sabka payment ruk raha.**
Arpan: PSP-1 band ho gaya to unka payment alag PSP-2 pe bhej do. Hamare paas fallback tareeka hoga na koi, unhe intezaar kyun karwana. Aur maan lo PSP-1 band hai lekin kuch log apna status jaanna chah rahe, to unhe thodi der rok do: "bhai abhi thoda down hai, aate-jaate hum tumhe bata denge."
Technical me: "PSP-2 pe bhej do, fallback tareeka" = fallback PSP, "status wale ko thodi der rok do" = PSP-1 pe atke payment wahin PENDING, PSP-2 pe dobara nahi.

**5. Server parchi pe INITIATED likh chuka tha, aur PSP ko call bhejte waqt hi gir gaya. Restart pe yaad nahi ki call PSP tak pahunchi ya nahi.**
Arpan: Beech me ek register rakho, usme likh lo "initiate hua". Server gira to aake dekhega: haan yaar, ye cheez likhi hui hai, chalo PSP se dobara poochte hain, "bata PSP, iska kya hua". To pehle register me likho, phir bhejo. Aur agar dobara bhejna pade to idempotency key laga denge, us key se pata pad jaayega ki do baar hua. Key process ho chuki to do baar nahi hoga.
Technical me: "pehle register me likho, phir bhejo" = PSP call se pehle INITIATED commit, "aake dekhega, PSP se dobara poochte" = restart pe job INITIATED utha ke status poochhe, "idempotency key" = payment id hi PSP ki idempotency key.

**6. Dukaan ke 500 wallet se ICICI me nikalne hain. ICICI hamara DB nahi, to ek transaction me nahi. Wallet se kaat liye, aur ICICI me jodte waqt PSP bola "FAILED".**
Arpan: Ulta kar do, sab ulta kar do, sab wapas paise bhej do. Maan le main HDFC bank hoon, bagal wale bank ko bhejne gaya, usne mana kar diya: "nahi bhai, bank band ho gaya, kal aana, abhi server bekaar hai." Fail ho gaya, baatcheet khatam. To main apne register me sab kaat dunga: "maine yahan se paise liye the, ise wapas kar raha hoon", aur sab wapas paise bhej dunga.
Technical me: "sab ulta kar do" = saga (fail pe ulta kadam), "register me sab kaat diya" = ledger me ulti line, "wapas paise bhej dunga" = 500 wapas dukaan ke wallet me.

**7. Wallet se 500 kaate, PSP ka jawab aane me 2 minute lage. Us 2 minute me 500 na dukaan ke wallet me, na ICICI me. Hisaab wala kahega 500 gayab.**
Arpan: Hum likh lenge na bhai: "wallet se 500 kate", sabse pehle kahin likh lo. Apne ek log ya ledger me kahin bhi likh lo, to wo gayab nahi honge. Koi kuch kahe to bata denge: "haan bhai, tere paise abhi yahan hain."
Technical me: "ledger me likh lo" = ledger entry, "tere paise abhi yahan hain" = hold (beech ka khaata): SUCCESS pe hold se nikle, FAILED pe wapas wallet.

**8. Kisi ne kaha: saga ka jhanjhat kyun? Sabse pehle wallet, PSP, ICICI sabse poochh lo "taiyaar ho?", sab haan bolein tab ek saath kaat do aur jod do.**
Arpan: Aisa kar sakte hain, lekin maan le ICICI ki service dheemi pad gayi. Mere paas paanch bande hain, main poochh raha "sab taiyaar ho?". Teesra banda thoda slow hai, chaar taiyaar hain, par ek ki wajah se sab khade. Ye wahi baat hai: main ek dost ko cheating karwa raha, par wo likhne me bahut slow hai, to uski wajah se mera bhi paper chhoot jaayega.
Technical me: "sab taiyaar ho?" = 2PC (prepare round), "ek slow ki wajah se sab khade, mera bhi paper chhoot jaayega" = 2PC me ek dheema to sab ruke, isliye 2PC nahi, saga + hold.

**9. Diwali pe har second 3,000 payment, saare wallet ek hi register (DB) me. Register itni likhai nahi jhel pa raha.**
Arpan: Us register ko kaat do, chhote-chhote tukde kar do. "Le bhai, tu yahan likh, tu ye likh, tu ye likh." Kaam baant diya, dikkat khatam. Ab aaram se sab apna-apna likh rahe.
Technical me: "register ke chhote tukde" = shard, "tu ye likh, kaam baant diya" = har shard ka apna likhne wala, likhai bant gayi.

**10. Arpan ka wallet tukda 1 me, dukaan ka tukda 3 me. Ek se kaatna, doosre me jodna. Do alag register, ek transaction me nahi hoga.**
Arpan: Apne office se ek ladka pakdunga: "ye 500 mera hai, us tukde me jaana hai, use de ke aa ja, aur jo hua mujhe aake bata." Saath me ek diary rakh, usme likh "ye 500 Arpan ne diye, is tukde me dene hain". Beech me koi dikkat aaye to diary me likhta jaaye kya-kya hua. Hisaab poora hua to theek, nahi to wapas aake mere paise de dega.
Technical me: "ladka 500 le ke us tukde me" = saga do shard ke beech, "diary" = saga ka har kadam likha (beech ka khaata, hold), "wapas aake paise de dega" = fail pe ulta kadam.

&nbsp;

Poora design = [06_payment_system.md](06_payment_system.md)
