# Inventory: hand notes (Ravi ki online dukaan, teen godown)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Ravi ki Flipkart jaisi online dukaan hai. Maal teen godown me pada hai: Delhi, Mumbai, Bangalore, aur ek main warehouse Delhi me. System ka kaam bas ek: har pal sach batana ki kaunsa maal, kis godown me, kitna pada hai.

**FR / NFR / Numbers (interview me pehle 2 minute):**
```
FR      maal aaya to ginti badhao · order pe ghatao (jitna sach me hai) · cancel / wapsi pe badhao
        · kabhi bhi "kitne, kahan" · kam ho to alert
NFR     ginti me galti nahi (pakka na ho to "abhi nahi", galat "haan" nahi) · dekhne me thoda purana chalega,
        ghatana ekdum sahi · har badlaav ka hisaab
Numbers product lakhon, godown kuch hi · aam din ek product pe second me 1-2 order
        · sale ke din ek iPhone pe second me hazaaron "Pay", sab ek hi row pe
```

**E-com se aaya:** wahan "cart sirf list hai, ginti pay pe" aur "counter khaali to doosra counter" padha tha. Yahan wahi ginti apne aap me poora system hai, saare godown ke saath.

&nbsp;

**1. Ravi ne Delhi ke aakhri iPhone pe "Buy" dabaya, payment page pe pahunchte hi phone ki battery khatam. Payment hua hi nahi.**
Arpan: Payment hua nahi matlab wo cheez uske cart me hi hai, kharidi nahi gayi. Cart me daalne ka matlab ye nahi ki tumne product kharid liya; jab tak Buy karke pay nahi karoge, maal tumhara nahi. Isi waqt maine app khola, iPhone dabaya aur pay kar diya, to iPhone mera. Ravi wapas aayega to use out of stock dikhega, jo hota bhi hai aur aam baat hai.
Technical me: ginti Buy pe nahi, payment pe ghate (koi reserve nahi). Ghatana ek hi kaam: "ek kam karo, sirf tab jab kam se kam 1 ho" (`UPDATE ... SET qty = qty - 1 WHERE ... AND qty >= 1`, atomic conditional update), do log ek saath aayein to ek ko mila, doosre ko "nahi hai".

**2. Diwali sale, raat 12 baje. Delhi me 1000 iPhone, ek second me 2 lakh log "Pay", sab ek hi row pe "ek kam karo". DB taala laga ke ek-ek ko line me chalata, log 20 second se spinner dekh rahe.**
Arpan: Ek counter pe lambi line lagi hai aur itna saamaan hi nahi. To 1000 ko baant do, 5 line kar lo, har counter ka apna dabba: 200 ek jagah, 200 ek jagah, 200 ek jagah. Log apne dabbe se uthayenge. Sab ek hi dabbe se uthayenge to DB pe taala dobara ek hi jagah lag jaayega, isliye alag-alag tukdon me baanto aur har counter apna dabba ghatata rahe.
Technical me: stock ko tukdon me baanto (bucketing / sharded counter), har tukda alag row, taala ek row pe nahi.

**Maan lo Ravi ki request dabba 3 pe gayi, wo khaali ho chuka, baaki dabbon me 150 pade.**
Arpan: Out of stock tab dikhega jab 1000 ke 1000 khatam ho jaayein, ek counter khatam hone pe nahi. Ek banda ghee lene aaya, teesre counter pe ghee nahi bacha, par baaki counter pe hai, to main use mana thodi karunga; usse kahunga is counter pe aa ja, ya uski request khud us dabbe pe mod dunga. Grahak ko maal chahiye, use pata hi nahi wo kis dabbe me hai. Online me kisi ko kuch nahi dikhta, use bas iPhone chahiye.
Technical me: khaali bucket -> request agle bucket pe, out of stock = saare bucket 0.

**3. Mahine ke ant me manager rack pe ginta hai: 48 iPhone. System bolta 50. "Ye 2 kab gaye, kis order me, ya kabhi aaye hi nahi?" Hamare paas sirf ek number, kitne = 50.**
Arpan: Jaise-jaise iPhone bikta rahe, bill banta rahe, register me likhta jaun: Ravi ne ek iPhone liya, bill ban gaya. Usi waqt system me bhi chadha do. Ya raat ko dukaan pe chhotu chhod jao: "ye aaj ke bill hain, dekh kisne kya kharida aur system me kitne bache." Sab sync rakho. Phir bhi kabhi fark aaya to register me sab likha hai, dekh lenge ki 2 iPhone bike par likhe nahi gaye, ya likha kuch aur gaya. Ye cheez dono taraf ho sakti hai, isliye register hi sach batayega.
Technical me: register = stock ledger (har badlaav ki entry: kab, kitna, kis order se), raat ka chhotu = reconciliation job (ledger aur ginti ka milaan).

**4. Ravi Bangalore me iPhone order karta hai. Bangalore godown khaali, Delhi me 50, Mumbai me 10.**
Arpan: Ek main warehouse hota hai, Amazon jaisi company ka maal asal me wahin se aata hai, phir chhote godown tak pahunchta hai. Pehle dekhunga Bangalore me iPhone hai kya, hai to wahin se. Nahi hai to main warehouse dekhunga, wahan hai to wahan se utha ke Bangalore bhej do, Ravi ko out of stock nahi dikhega. Mumbai ka iPhone Bangalore bhejna, ye FR pe hai. FR bole "maal hai to order poora karo", to Delhi ya Mumbai se bhejo. FR bole har shehar ka maal usi shehar ke liye, to main warehouse aur Bangalore dono khaali hone pe out of stock dikha do, kyunki kal Mumbai me 10 log aaye to unke liye wahan maal rehna chahiye.
Technical me: main warehouse + shehar ke godown = hub and spoke, kahan se maal uthana = fulfilment / sourcing rule (FR tay kare).

**5. Sale se ek din pehle iPhone ka page 1 crore log khol rahe, "sirf X bache". Har page khulne pe DB se saare dabbon ka jod. Order koi nahi, sirf log dekh rahe, phir bhi DB pe bojh.**
Arpan: Bahut log aa rahe aur bas daam / ginti pata karke chale jaate, dukaan me bheed aur faltu ka drama. To ek banner pe chhapwa ke bahar taang do, padh ke nikal jao, andar mat aao; kharidna ho tab andar aao. Ya chhotu ko baar-baar andar bhej raha "bata ye kitne hain", chhotu bina matlab thak raha; to chhotu ek baar copy pe sab likh ke de de, main copy saamne khol ke bata deta hoon. Chhotu aaram se ice cream kha raha, main bhi, DB pe koi nahi ja raha.
Technical me: banner / copy = cache (Redis), page pe thodi purani ginti chalti hai, asli jaanch Pay pe (NFR 2).

**6. Ravi ne iPhone return kiya, courier ne box godown pe chhoda, aate hi ginti +1. 10 minute me wahi iPhone Priya ko bika: box khula, screen tooti.**
Arpan: Jaanch wahin darwaze pe: delivery wale ko bol do jis haalat me gaya usi haalat me phone chahiye. Phone khol ke dekh, kaisa hai; kuch dikkat hai to return wahin reject. Inventory me maan lene ke baad nahi, warna doosre user ko bik gaya to Priya naraaz. Badi company jhel le, chhoti company ke liye ye bahut badi dikkat. Jaise mere se koi ghee ka dabba le gaya, 2-3 ghante baad lautane aaya, seal khuli, thoda ghee nikla hua; nahi lunga, ise main kisi ko bech nahi sakta. Seal pack hai to lenge, nahi to nahi.
Technical me: return pe doorstep quality check, ginti sirf sellable stock me badhe, kharab maal alag khaane me (damaged).

**7. Delhi me iPhone 5 se neeche aa gaye. Ab kya: sirf manager ko email, ya kuch aur?**
Arpan: 5 se kam ka matlab hamara kaam badh gaya, par grahak ko mat roko. Pehle dekho saari inventory me iPhone hai ya nahi: Bangalore se order aaya to Bangalore, phir main warehouse. Maal hai to order chalta rahe, 5 hon ya 100 order aayein. Kitni inventory hai, ye FR me tay karenge. 2-3 inventory hain to batao "is inventory me khatam, doosri inventory me dekh lo". Ek hi inventory hai to usme sab jagah iPhone nahi bhar sakte, aur bhi maal chahiye; to 2-3 bache tab hi har jagah siren baja do, sabko alert. Alert kisi bhi raaste pahunche: email nahi ja raha to SMS, WhatsApp, ek email company nahi to doosri; siren matlab bas alert. Khatam ho gaya to out of stock, aur alert "inventory khaali, jahan se deal karte ho wahan se deal karo". iPhone kahan se mangwana hai, wo manager / purchase team tay karegi, is design ka scope nahi; inventory ka kaam utna hi hai. Naya stock jaldi aa raha ho to order lete raho, 1 mahina lagega to out of stock dikhao.
Technical me: had pe alert (low stock / reorder point alert), line me, order nahi rukta (Kafka event, async), kai raaste (email, SMS, WhatsApp, backup provider). Order lene se pehle saare godown ka jod (available to promise across warehouses), kitne godown = FR. Maal mangwana = purchase / procurement system, inventory ke scope se bahar. Jaldi aane wala maal ho to order lete raho (backorder), der ho to out of stock (restock lead time, FR tay kare).
