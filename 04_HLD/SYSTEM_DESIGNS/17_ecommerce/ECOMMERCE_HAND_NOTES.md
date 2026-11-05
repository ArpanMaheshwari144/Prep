# E-commerce: hand notes (mausi ka mart, ab online bhi)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Mausi ka mart (Reliance Smart jaisa), ab online bhi. Dukaan bhi chal rahi, counter pe billing bhi, aur app se ghar delivery bhi. Stock ek hi: wahi shelf, wahi godown.

**Faisla FR se (interview me pehle):** Sale me pehle hi bol do: ghee 500 hai to 500 ko hi milega, baaki ko "out of stock" dikhega. Yahan consistency chahiye (500 se ek bhi zyada nahi bikna), availability nahi (sabko milna zaroori nahi). Ye CAP wali baat hai: FR me hi ye cut off kar do, phir design usi hisaab se.

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

**8. Net gaya / PSP chup: Ravi ko laga pay ho gaya, par PSP tak request pahunchi nahi, ya paisa kata aur PSP anjaan.**
Arpan: Design har jagah se thoda toot-ta hai. Chahe 10 minute rok lo, chahe paisa aane pe kaato, kinaare pe dikkat rehti hai. Aakhri second pe pay kiya to wahan BMS wali cheez hi leni padegi. Paisa kata to zimmedari hamari, PSP se hum baat karenge.
Technical me: PAYMENT_PENDING (BMS jaisa) + status job PSP se poochta rahe. Paisa kata mila to dabba uska; dabba na bacha ho to refund. Perfect design koi nahi, bas kitne refund.

**Agar pooche: ek order me ghee, aata, daal. Aata khatam.**
Arpan: Poora order mat roko. Jo hai wo bhejo, aate pe out of stock dikha do. Wo chahe to baaki do le le.
Technical me: har cheez alag check. Adhoora order chalta.

**Agar pooche: Lucknow ka store khaali, Kanpur me 20 dabbe.**
Arpan: Lucknow wale ko Lucknow ka store dikhega. Dark store ka matlab hi aas-paas ka store. Whitefield me baithe ko Koramangala wala dikhta.
Technical me: har store ka apna stock, app location dekh ke paas wala store chunta.

&nbsp;

Poora design = [17_ecommerce.md](17_ecommerce.md)
