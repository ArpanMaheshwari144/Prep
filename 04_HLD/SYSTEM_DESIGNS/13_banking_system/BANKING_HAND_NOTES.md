# Banking: hand notes (bank branch)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Mohalle ki bank branch: cashier, aur ek register jisme sabke khaate likhe hain. Arpan apne khaate se Ravi ko 500 bhejta hai, dono ek hi bank me. Paisa na gum ho, na do baar kate, har paise ka hisaab mile.

&nbsp;

**1. Arpan ne parchi di "Ravi ko 500 bhej do". Cashier ne Arpan ke 500 kaate, Ravi me jodne jaata usi pal bijli chali gayi. Arpan ke 500 kat gaye, Ravi ko kuch nahi mila.**
Arpan: Bhai aisa karo na, mere 500 kat gaye to mujhe 500 wapas de do. Ye hai dikkat: mere 500 kate, Ravi ko kuch nahi mila, to mujhe mere 500 wapas de do na, dikkat hi khatam.
Technical me: "mere 500 wapas de do" = transaction rollback (ya dono kaam, ya koi nahi).

**2. Ravi ko 500 pahunch gaye, par receipt milne se pehle Arpan ki line kat gayi. Use pata nahi chala, usne wahi parchi dobara di.**
Arpan: Wo cheez hamare paas hai na, humne likh di thi. Bhale hi uski line kati ho, wo dobara bheje, wo log to hamare paas hai. Hum dekh lenge ki 500 chale gaye, to mana kar do: "nahi bhai, 500 tumhare already bhej diye hain."
Technical me: "log hamare paas hai, dekh ke mana" = DB me pehle wala record dekh ke dobara mat kaato.

**Maan lo agle din Arpan sach me Ravi ko phir 500 bhejna chahta hai. Diary me pehle se "Arpan se Ravi, 500" hai, cashier use bhi mana kar dega. Wahi purani parchi hai ya nayi, kaise pehchaane?**
Arpan: Aisa karo, ek key bana do, ek key le lo. Arpan ne Ravi ko 500 bheje, ab use pata nahi gaye ki nahi, to wo dobara bhejega to usi key ke saath bhejega na, key thodi badli, key to same hi hai. Key kuch aisa rakho jo badle hi na. Maan lo maine ek parchi pe likh diya ki Ravi ko 500 bhejne hain is waqt, aur parchi bank wale ko de di. Mujhe nahi pata gaye ki nahi, to main dobara wahi same parchi dunga. Wo dekhega: "abe yaar ye to same hi hai, ise kyun wapas bhejna, hatao." Matlab ek key aisi banegi jo do baar transaction me same hi rahegi.
Technical me: "key jo badle hi na, same parchi" = idempotency key, "ye to same hi hai, hatao" = key pehle aa chuki to purana jawab (UNIQUE), nayi parchi = nayi key.

**3. Branch me do counter. Arpan ke khaate me 600. Ek hi pal me do parchi, Netflix 500 aur UPI 500, alag counter pe. Dono cashier ne 600 dekha, dono ne kaat diya, balance -400.**
Arpan: Ise ek hi kram me kar lo, dekhna aur kaatna ek hi kram me. Jisne pehle 600 dekha, wo pehle kaat de: Netflix ke 500 turant kat gaye, ab account me 100 rupaye hain, to agla counter 600 nahi, 100 dekhega. Technical terms me jo bhi kehte hain wo mujhe pata nahi, lekin ek kar do, aur doosre wale ko 600 dikhne nahi chahiye.
Technical me: "dekhna aur kaatna ek kram me" = ek query me check + kaato (`balance - 500 WHERE balance >= 500`), "doosre ko 600 dikhne nahi chahiye" = row lock.

**Maan lo ek hi pal me do parchi: counter 1 pe "Arpan se Ravi 500", counter 2 pe "Ravi se Arpan 300". Counter 1 ne Arpan ka khaata pakda, Ravi ka maang raha; counter 2 ne Ravi ka pakda, Arpan ka maang raha. Dono ek doosre ka intezaar, koi nahi hilta.**
Arpan: Dono counter wale apna-apna khaata pakad ke khade, na ye chhodta na wo. Do raaste hain. Pehla: main Ravi se bolunga "chal bhai, hum dono ek hi line me lagenge, ek hi counter pe", to ek ke baad ek kaam hoga. Doosra: main bolunga "koi baat nahi, pehle mere khaate se kaat ke Ravi me daal de". Wo ho gaya to khaata free, phir doosri parchi bhi ho jaayegi. Aur ye atkan sirf ek hi bank ke DB ke andar hota hai, alag bank me nahi. User ko kabhi pata nahi chalta: DB khud tod deta hai, app dobara chala deta hai, papa ke 10 aur mere 10 dono chale jaate hain.
Technical me: "dono ek doosre ka khaata pakad ke baithe" = deadlock. "ek hi line, ek counter" = ek ke baad ek (serialize); 10 counter bane rahein aur phir bhi na atkein, uske liye har counter pehle chhota account number pakde (lock ordering). "pehle ek poora, phir doosra" = DB atkan pakadta hai (deadlock detection), ek parchi rollback, doosri poori, phir pehli dobara (retry).

**4. Hafte baad Ravi aaya: "Arpan ke 500 mile hi nahi". Register me sirf aaj ka balance, purana number har baar kaat ke naya likha. Saabit kaise karein?**
Arpan: Ek alag diary rakh lo, usme kaato mat, uske neeche bas likhte jao, jodte jao. Ya ek copy me do column bana lo: ek me update likhte ja rahe, koi dikkat nahi, lekin doosre me update mat karo, 500 kaato mat, neeche likhte jao: Ravi me +500 hua, yahan se -500 hua. To kabhi koi dikkat nahi. Main bol raha hoon do alag cheez rakho.
Technical me: "alag diary, kaato mat, neeche likhte jao" = ledger (append-only), "+500 / -500" = double-entry, "ek column me update" = balance column, alag.

**5. Paisa jaate hi Ravi ko khabar deni thi. Cashier ne paisa bheja, aur ladke ko "Ravi ko khabar do" bolne se pehle bijli chali gayi. Ladke ko pata hi nahi ki kisko khabar deni hai.**
Arpan: Jo kuch yaad nahi rehta kisi ko, to likh lo na bhai. Hum bhi to yahan likh rahe hain kyunki mujhe kal yaad nahi hoga, same to same cashier ke saath hai. Kahin likh lo, logs me likh lo. Ab cashier kabhi bhi aaye ya jaaye, wo likha hai, agli baar koi bhi ladka aayega to wo pahuncha hi dega.
Technical me: "jo yaad nahi rehta, likh lo" = outbox (usi transaction me), "koi bhi ladka aaye, pahuncha dega" = relay.

**6. Mahine ki 1 tareekh, sabki salary, poora mohalla branch me. Ek hi cashier, line sadak tak.**
Arpan: Cashier badha do. Ek cashier tha, 10 cashier kar do, 10 counter kar do. Bheed badh gayi, kaam khatam, paisa hazam. Aam taur pe bhi yahi hota hai: bank me ek counter pe bheed hai to hum doosre counter pe jaate hain, movies me bhi same, har jagah same.
Technical me: "10 cashier, 10 counter" = kai service machine, "doosre counter pe jaate" = LB khaali machine pe bheje.

**7. Cashier 10 ho gaye, par register abhi bhi ek hi. Raat ko branch me aag lagi, register jal gaya.**
Arpan: Use kahin store karwa lo na, kai jagah store karo. Maan le meri ek important copy hai, usme kuch notes likh rakhe hain. Main use office me chhod gaya, raat me office me aag lagi, wo notes kho jaayenge na. Nahi, main aisa nahi karunga: main use laptop me save kar lunga aur laptop ghar le jaaunga. Ab us copy me aag lage chahe kuch bhi ho, mere laptop me to mera saamaan hai.
Technical me: "laptop me save, ghar le jaao" = replica doosri jagah (Multi-AZ).

**Maan lo laptop me shaam ko ek baar save karta hai. 2 baje copy me zaroori line likhi, 3 baje aag. Copy kab likhi jaani chahiye?**
Arpan: Jaise hi maine copy pe likha, turant laptop pe likh lo na, dikkat hi khatam. Hamare paas proof ho gaya, ab office me aag lage chahe kuch bhi. Matlab wo cheez tab tak pakki nahi jab tak wo mere laptop me nahi aati. Copy pe likh liya, use turant laptop pe lao, phir pakka maano.
Technical me: "turant laptop pe, tabhi pakka" = sync copy, "shaam ko ek baar save" = async (aakhri likha kho jaata). Ek DB ke andar bhi yahi: "pehle diary me pakka, phir haan" = write-ahead log (WAL), DB khud karta (Foundations 05).

**8. Salary ke din sab sirf balance dekhne aa rahe. Saare cashier register pe jhuke, jo sach me paisa bhejne aaya uska kaam atka.**
Arpan: Alag-alag cheez kar lo. Jo sirf dekhne aa rahe unko sirf dikhao, jo karne aa rahe unko sirf karao, alag-alag baant do. Register uske liye khaali chhodo jo waqai kuch kaam kar raha hai. Us register ki photocopy bana do, photocopy ek alag desk pe rakh lo. Jo sirf padh raha, dekh raha, kuch update nahi kar raha, use photocopy pe bhej do. Jo kuch likh raha, use main register pe le aao.
Technical me: "photocopy alag desk pe" = read replica, "main register likhne walon ke liye" = primary.

**9. Photocopy main register se thodi peeche chalti hai. Arpan ne abhi 500 bheje, turant photocopy desk pe gaya, wahan purana balance. Ghabra ke dobara bhej diya.**
Arpan: Use pehle thodi der main file se padho na. Jisne update kiya, use turant main file pe padho. Thodi der baad jab wo replica pe sync ho jaayega, phir wahin se padh lo. Har cheez ka system hai yahan.
Technical me: "jisne update kiya, thodi der main file se" = read-your-writes (primary se padhna), "sync ho jaaye to wahin se" = phir read replica.

**10. Raat ko kisi bank wale ne main register me Arpan ka balance haath se 100 ki jagah 600 kar diya, diary (ledger) me koi line nahi. Agle din Arpan ne 600 nikaal liye.**
Arpan: Use main ledger se milwao na. Asli to ledger hai. Arpan ka balance koi bhi haath se update kar dega, wahan dikkat hai. Apna jo main hai wo ledger se hai: ledger me hum sab cheez likh rahe, -500 +500, ye hai asli sach, na ki wo column jo galti se koi bhi update kar de.
Technical me: "ledger se milwao" = raat ka milaan (reconciliation), "asli sach ledger, column nahi" = ledger = source of truth, balance bas copy.

**11. Arpan ka khaata 5 saal purana, ledger me hazaaron line. Har raat milaan ke liye 5 saal ki line shuru se jodna bhaari, raat khatam par jod poora nahi.**
Arpan: Data tod do na bhai. 5 saal purana data thodi badlega, wo same hi rahega. Maan liya maine ek mahine me 10,000 ka kharcha kiya, wo 10 mahine purana hai, ab wo badlega thodi. Mahine ke mahine me tod do, kaam khatam. Arpan ne January se March tak itna kharch kiya, jud gaya, mil gaya. March se April tak itna kiya, use jod do. Pichhle mahine ka khaata thodi change kar sakte hain, agle mahine ka khaata hota rehta hai. Pichhle ka dekh ke tod do, tod ke likh do, dikkat khatam.
Technical me: "mahine ke mahine tod do" = mahine ke partition, "pichhla jud gaya, likh do" = snapshot (mahine ka total), "purana data badlega nahi" = purani line immutable.

**12. 5 saal me diary itni moti ki branch ki almari me jagah nahi. Niyam (maan lo): purani line phenk nahi sakte, 10 saal rakhni hain.**
Arpan: Use apne bank ki jagah main office me bhej do. Hamara chhota bank hai, meri branch mere shehar me hai, uski koi main jagah to hogi na, head office. Use wahan bhej do, wahan safe rahega. Data kahin gaya nahi.
Technical me: "head office me bhej do" = purana ledger S3 me (sasta, lambe samay ke liye), "data kahin gaya nahi" = archive, phenka nahi.

**13. Ijazat wala aadmi hi godown me ja ke "Arpan -500" ko "Arpan -50" kar de.**
Arpan: Uski multiple copies rakh lo. 3-4 copy alag-alag chupchap kahin aur kone me daba do, taaki wo sab na badal paaye, ek hi badal paaye. Usne ek file badal bhi di to doosri copy me same data hoga. Aakhir me paancho file milao: ek me hi -500 ki jagah -50 hai, baaki me nahi, matlab isne chhedchhaad ki hai. Aur ek aur kar sakte hain: version ka lock laga do, ki wo file koi change hi na kar sake. Maan le wo ek tarah se kohinoor heera hai: na koi chhoo sakta, na chhed sakta, na badal sakta. Use aisi jagah taala maar do: ye main data hai, ispe koi na badlega, wo chupchap wahin pada rahega.
Technical me: "kai copy chhupa ke, milao" = copies (doosri jagah) se chhedchhaad pakadna, "version ka lock" = versioning, "kohinoor, koi badal na sake" = S3 Object Lock.

&nbsp;

## Mahine ki statement (billions of records)

**Scene:** Bank ke crore khaate, arbon transaction. Har customer ki har mahine ki statement banni hai, aur 31 March ko sab ek saath maangte hain. Peeche kaam karne wale = chhotu.

**14. 31 March ko ek hi din 1 crore log statement maangne aa gaye. Statement tab banni shuru hui jab maangi, sab atke.**
Arpan: Pehle ye poochho ki statement turant chahiye kya, aur kya turant ban bhi sakti hai. Kafka me daal ke magic se turant nahi banti, time lagta hai. To intezaar mat karo ki user aaye tab banayenge. Peeche chhotu laga do: January ki January me, February ki February me bana ke rakh do. 31 March ko jo bhi aaye, Ravi ho ya Shubham, utha ke turant de do.
Technical me: "pehle se bana ke rakh do" = precompute (batch job), "peeche chhotu" = background worker (Kafka se kaam baanta), "turant chahiye kya" = pehle requirement poochho.

**15. 28 Feb raat ka transaction der se aaya. Ravi ki Feb statement tab tak ban chuki thi, shayad bhej bhi di.**
Arpan: Statement banane ke baad ek chhotu use bank ke record se milaye ki sach me sahi hai. Phir bhi koi transaction baad me aaya, to do raaste hain. Pehla: use jod ke nayi statement bana do aur Ravi ke account me daal do, wo dobara download kar lega. Doosra: statement thodi der se banao, 1 March ki subah nahi to thoda baad, taaki der wale bhi aa jaayen. Ekdam pakki chahiye to banao, verify karo ki kuch chhoota nahi, tab do.
Technical me: "record se milaye" = reconciliation, "nayi statement, dobara download" = revised statement, "thodi der se banao" = cutoff + grace window, "turant ya ekdam pakki" = consistency vs immediacy.

**16. Chhotu aadha kaam karke gir gaya.**
Arpan: Ye Kafka me kai baar ho chuka. Chhotu jitna karta hai DB me likhta jaata hai, gira to naya chhotu wahin se utha leta hai.
Technical me: "DB me likhta jaata, wahin se utha le" = checkpoint, jo ban chuka wo dobara nahi = idempotency.

**17. 10 me se 9 chhotu kaam khatam karke baithe hain, ek bahut der se laga pada hai. Uske paas bahut saara kaam.**
Arpan: Jab ladke khaali baithe hain to naye ladke kyun lane? Ek bhara pada hai, baaki khaali, to kaam baant lo. Idhar aao sab, is bande ki madad karo.
Technical me: "ek ke paas bahut kaam" = hot key / data skew, "khaali ladke madad karo" = work stealing.

**18. Wo bada kaam ek hi merchant ka hai: 5 crore transaction, aur statement ek hi banni hai.**
Arpan: Ek parchi pe sab mat baitho, kaam baant do: 20 lakh tu le, 30 lakh tu le. Sab apna hissa karke laaye. Phir ek bada aadmi bulao jo isi kaam me expert hai, wo sabke tukde jod ke ek statement bana dega. Thoda time lagega, chalega.
Technical me: "kaam baant do" = map, "bada aadmi jod ke ek banaye" = reduce, dono milke = MapReduce.

**19. Statement me har line ke saath running balance. May wale chhotu ko 1 May ka opening balance chahiye, jo April wale ke khatam hone pe hi pata chalega.**
Arpan: Tukde mahine ke hisaab se do: April wala April, May wala May. Andaze se balance nahi likh sakte, statement galat ho jaayegi. Aur balance to master register me pehle se likha hai. Intezaar kyun? May wala chhotu master register se 1 May ka balance utha le, apne notepad me likh le, aur kaam shuru kar de.
Technical me: "mahine ke tukde" = mahine ke partition, "master register me likha balance" = balance snapshot (kadam 11 wala "pichhla jud gaya, likh do").

**20. Raat 2 baje saare chhotu main register se arbon line padh rahe hain. Usi waqt log UPI kar rahe hain, aur ₹50 ki chai ki payment 20 second atak gayi.**
Arpan: Iska jawab pehle bahut jagah de chuka hoon. Chhotu photocopy se padhein, main register asli payment ke liye khaali rahe.
Technical me: "photocopy se padhein" = read replica (kadam 8 wala "photocopy ka alag desk"), "main register asli payment ke liye" = primary.

&nbsp;

Poora design = [13_banking_system.md](13_banking_system.md)
