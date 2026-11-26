# File upload: hand notes (YouTube jaisa "peeche chal raha")

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** ek aam file-sharing app. Arpan 2 GB ki video / 10 GB file / "Goa trip" folder daalta hai, Ravi ke saath share karta hai.

**Ek nazar: dhaancha + dikkat chain (round me sabse pehle ye, phir FR / NFR isi se):**
```
Phone  -->  hamara server (JWT dekhe, token de)          DB: file ka status, folder tree, kiski file
  |                    ^
  |  tukde seedha      |  "file aa gayi" (S3 event)
  v                    |
 S3  ------------------+        server -> line (Kafka) -> scan worker -> CLEAN tak link band
  |
  +--> download: server haq dekhe -> chhoti umar ka link -> S3 / CDN se seedha phone pe

 1   server khud sab kare, 10 min spinner          ->  chai pi aao, peeche line (Kafka + worker); chhota tukda dekh lo (magic bytes)
 2   1000 x 2 GB hamare server se, 8 GB pe net gaya ->  seedha S3, tukdon me, sirf baaki dobara; adhoore tukde S3 ki safai (multipart, lifecycle)
 3   S3 kaise maane ye hamara user                 ->  receptionist ka token (presigned URL)
 3b  10 GB upload, token beech me khatam           ->  har tukde ka apna token (presigned per part)
 4   file S3 me aayi, server ko pata nahi          ->  S3 bataye "aa gayi" (event notification)
 4b  UPLOADED likha, scan daalne se pehle gira     ->  UPLOADED ke saath vasiyat (outbox = DB table, + relay)
 4c  S3 ne "aa gayi" do baar / ulta bataya         ->  parchi + apna thappa (idempotency, sequencer / version)
 5   folder rename = 500 copy + 500 delete         ->  tree jaisa, sirf parent badlo (parent_id)
 6   1043 likha, Priya ka statement khula          ->  "Priya ko saath la", truck seedha ghar (owner check, S3 direct download)
 6b  CDN dabbe me Priya ka statement               ->  bina token CDN na de (CDN signed URL)
 7   link 200 logon ke WhatsApp group me           ->  link ki umar chhoti (presigned expiry)
 8   scan me 10 min baaki / virus nikla            ->  CLEAN tak link band, virus to file hatao (quarantine)
 8b  bahar wala scanner 1 ghante se chup           ->  ruk ke dobara, phir insaan ko alert (retry + backoff, DLQ)
 9   S3 bill double, ek hi file lakhon baar        ->  file ke maal se number, register me milao, pehle se to upload nahi (content hash, dedup)
 9b  Priya wahi meme daale, list me dikhe hi nahi  ->  Priya ka upload mat roko, uske naam row, purani file pe bhejo (reference, cross-user dedup)
 9c  Ravi ne delete kiya, Priya ka download toota  ->  sirf Ravi ki row hatao, aakhri wala hataye tab S3 se (reference counting)
```

**FR / NFR kahan se:** hawa se nahi aate. Upar ki dikkat chain dekh ke nikale; har FR / NFR line ke aage likha hai wo kis kadam ki dikkat se aayi.

**FR / NFR / Numbers (interview me pehle 2 minute, main file se):**
```
FR      1  file upload, badi bhi                                  -> kadam 2, 3
        2  folder bhi (rename / delete poora ek saath)            -> kadam 5
        3  jaanch: virus + sach me wahi type                      -> kadam 1, 8
        4  status dikhe (upload chal raha / scan / taiyaar)       -> kadam 1, 4
        5  share + download                                       -> kadam 6, 7

NFR     1  user ruke nahi                                         -> kadam 1
        2  file khoye nahi                                        -> kadam 2, 4b
        3  ek ki file doosra na dekhe                             -> kadam 3, 6, 7
        4  ek hi file S3 me baar-baar na rakhein (bill), har user ko apni file dikhe,
           ek ke delete karne se doosre ki na jaaye              -> kadam 9, 9b, 9c

Numbers 1000 log x 2 GB ek saath = 2 TB -> ye data hamare server se guzarna hi nahi chahiye (kadam 2)
        status log baar-baar dekhte -> padhna zyada, likhna kam
```

**Pehle poochho kaunsa product** (Arpan: "har file upload alag, koi scan karta koi nahi, koi 10 GB / folder leta hi nahi"): core = 1, 3, 4, 6, 7 har jagah (seedha S3 bhi core, kyunki 3, 4, 6 usi pe tike; 7 = 6 wale download link ki umar); agar badi file = 2 ka tukde wala hissa; agar folder = 5; agar bahar ki file (virus scan) = 8.

&nbsp;

---

**1. Server khud sab kar raha (lena, virus scan, thumbnail, save), Arpan 10 min spinner dekh raha, 1000 log aaye to list kholne wale bhi atke.**

**Arpan:** YouTube kuch rokta nahi, bol deta "upload chal raha, chai pi aao". Matlab peeche kuch chal raha. Stadium jaisa: line me lag jao, baaki main peeche karta rahunga. File lo, line (Kafka) me daalo, worker / consumer peeche kaam kare. Status apne DB me, user UI pe dekh le. Aur jaanch: PDF bol ke exe na chadha de, to virus scan (third party) + file ka chhota tukda dekh lo ki sach me PDF / CSV hai.

*Technical me:* "line" = Kafka + worker / consumer (async; Kafka me file nahi jaati, sirf file ki id / S3 ka pata, file S3 me), "status" = DB me file ka status, "chhota tukda" = magic bytes, "scan" = third party virus scan.

**Maan lo bheed badhi, kahan tootega:**

**Arpan:** Ek hi service hai to server banane padenge, aage LB laga do. S3 ke liye koi dikkat nahi. DB pe bojh pade to cache (Redis); DB me writes aur badhe to shard kar sakte, lekin sharding zaroori nahi: pehle cache aur indexing se dekho, theek chal raha to theek, nahi to sharding last option hai, kyunki uski bahut cost hai, sharding free nahi.

*Technical me:* LB + zyada server, padhai pe cache, DB pe index; shard aakhri (mehnga, operational cost).

---

**2. 1000 log x 2 GB = 2 TB hamare server se guzar raha, network bhar gaya.**

**Arpan:** Badi file apne server se guzaro hi mat, seedha bade dabbe (S3) me. 10 GB ko tukdon me todo; 8 GB pe net gaya to sirf baaki 2 GB dobara. Client ne dobara upload kiya hi nahi to adhoore tukde thodi der baad S3 ki safai hata de.

*Technical me:* "bada dabba" = S3, "tukde" = multipart upload (resume), "safai" = lifecycle rule (adhoora multipart hatao).

---

**3. Phone seedha S3 me daal raha, par S3 kaise maane ye hamara user hai? App me AWS chaabi rakhi to koi bhi nikaal le.**

**Arpan:** Bank jaisa: koi seedha manager ke cabin me nahi ghusta (cabin = S3). Pehle receptionist (hamara server) JWT dekh ke tasalli kare, phir ladke ke haath me token de (token = presigned URL, sirf is file ke liye, thodi der ka): "ye le, jab teri baari aaye to file de dena". 10 GB ka bora receptionist khud nahi uthata (file hamare server se nahi guzarti, phone seedha S3 me daalta hai). Badi faltu file pe had ya paisa.

*Technical me:* ladka = user ka phone (client) · "receptionist ka token" = presigned URL (hamara server deta, chaabi phone ko nahi), "had" = size limit / quota.

**Maan lo 10 GB ki file, upload 40 minute chal raha, aur receptionist ka token thodi der ka tha, beech me hi khatam ho gaya.**

**Arpan:** Token poori file ka ek nahi, har tukde ka apna. Kadam 2 me file tukdon me ja rahi hai, to har tukda bhejne se pehle receptionist se us tukde ka chhota token le lo. Beech me net gaya ya token ki umar khatam hui, to koi baat nahi: jo tukde pahunch gaye wo S3 me pade hain, bas baaki tukdon ke liye naya token maango aur wahin se aage badho. Is tarah token chhota bhi rehta hai (leak hua to nuksaan kam) aur bada upload bhi poora ho jaata hai.

*Technical me:* har tukde ka token = har part ka alag presigned URL (multipart upload), resume pe naya token = sirf baaki parts ke liye naye URL.

---

**4. File S3 me aa gayi, par server ko pata hi nahi, DB me "UPLOADING" pada, scan shuru hi nahi hua.**

**Arpan:** S3 ko bol do: "jab file aa jaaye, mujhe bata dena". Hum DB me mark kar denge aur ladke ko bata denge. Poori nahi hui to wo dobara try karega.

*Technical me:* ladka = user ka phone (client) · "S3 bataye" = S3 event notification -> DB status + Kafka me scan ka kaam.

**Maan lo server ne DB me UPLOADED likh diya, par Kafka me scan ka kaam daalne se pehle gira. File UPLOADED pe atki, scan kabhi shuru nahi.**

**Arpan:** Wahi Kafka aur Notification wala: vasiyat likh do. UPLOADED ke saath hi likh do "iska scan karwana hai", ek hi baar me. Beech me gira to baad me koi utha ke Kafka me daal dega.

*Technical me:* "UPLOADED ke saath vasiyat" = outbox (hamare DB ki table; status + scan ka kaam ek hi DB transaction me, server gire to bhi row bachi), "baad me koi daal dega" = relay.

**Maan lo S3 ne "file aa gayi" do baar bataya, ya ulte kram me.**

**Arpan:** Do baar aa sakta hai, koi dikkat nahi. Parchi rakho: is file ka kaam ho gaya, likh liya; dobara aaya to dekha ho gaya, chhod do. Ulta aaya to dekho kaunsa naya hai, purana aaya to chhod do.
Maan lo resume me v1 phir v2 aaye aur event ulte pahunche. Arpan: Apna thappa lagao, pehli = current, doosri = updated. V1 / V2 ke naam se nahi, apne thappe se faisla; purana thappa baad me aaya to phenk do.
S3 ka event kabhi do baar ya ulte kram me aa sakta (AWS docs).

*Technical me:* "parchi" = idempotency (file id + version pe ek hi baar), "kaunsa naya" / "apna thappa" = S3 event ka `sequencer` ya hamara version number (bada = baad wala, usi file ke liye).

**Maan lo user har 2-3 second status poochh raha:**

**Arpan:** User har second, har do-teen second baad poll kar raha, "meri file kahan hai", to read bahut zyada ho jaayenge. Cache use karo, user cache se hi laut jaayega, server pe load direct nahi. Par cache stale ho sakta hai: worker ne file validate kar di aur cache purana, user ko galat status. To TTL rakh sakte, ya jab validate ho jaaye tab cache bhi update kar do.

*Technical me:* status poll = read-heavy -> status Redis me, "validate hote hi update" = write-through / invalidate on status change, TTL backup.

**Aur neeche (status poochhne wale bahut):**

**Arpan:** Padhna zyada hai to DB ki read replica bhi laga sakte, Redis ke saath.

*Technical me:* Redis + read replica.

**Maan lo worker beech me gira, file VALIDATING pe atki:**

**Arpan:** Sweeper job chala sakte: har minute dekhe, UPLOADING agar bahut der se (upload jitne time se zyada) nahi badla to adhoori file hata do, wo bekaar hai; VALIDATING pe atki file hatani nahi, neeche wala retry. S3 me lifecycle rule bhi purani temporary aur adhoori multipart files apne aap clean kar deta.

*Technical me:* sweeper / reaper job (status + updated_at dekh ke), S3 lifecycle rule (adhoora multipart abort).
**Isi pe ek aur raasta (atki files dobara try):**

**Arpan:** Status DB me hai, yahi uska fayda. Ek worker VALIDATING pe atki files nikaale, dekhe kaunsi fail hui, aur unhe dobara try kare, file hamesha atki na rahe.

*Technical me:* atke VALIDATING (purana updated_at) pe retry job.

**DB kaunsa:** Arpan (21-Nov): File upload me main kaam to S3 karta hai, file wahin jaati hai. DB me hamein metadata chahiye: file ka naam, type (PDF, EXE, document), kab aur kitne baje upload hui, kitne GB ki, status.

Iske liye NoSQL ka koi point nahi banta: JSON me kya store karna, MongoDB, DynamoDB, Cassandra ki zaroorat nahi, Cassandra to bahut badi baat. Requirement me bhi kya badlega.

Isliye lean aaj bhi ACID / SQL wala: AWS RDS (Postgres / MySQL), jo chahiye wo SQL me mil jaata. (4-Oct bhi yahi: tracking aur status consistent chahiye.)

*Technical me:* file = S3, metadata + status = SQL (RDS Postgres), seedhi row + consistency, NoSQL ki zaroorat nahi.

---

**5. "Goa trip" folder (Day1, Day2, 500 photo). S3 me folder hote hi nahi; naam path me likha to rename = 500 copy + 500 delete.**

**Arpan:** Folder ka hisaab alag, apne server me. Ek parent, baaki uske child, tree jaisa (DSA wala tree: root, phir level 1, level 2). Naam badalna = sirf parent ka naam badlo, saare bachche usi ke neeche. Parent bulaya to saari files saath; delete kiya to poora. S3 sirf files rakhe.

*Technical me:* "tree" = DB me folders table (id, parent_id), file ki row me folder_id; S3 key file ki id se, naam se nahi.

**Maan lo Arpan ne "Goa trip" folder delete kiya. DB me folder aur 500 file ki rows "deleted", app me folder gayab. Par S3 me 500 photo abhi bhi padi, jagah aur paisa.**

**Arpan:** S3 se kaam hatana user ka nahi. Hum apna DB theek kar denge, S3 se hum hata denge. File mere mote store (kamre) me padi, uska saaraansh mere DB me. User bola file delete karni, maine uske saamne faad ke phenk di.

Bahut log nahi jaante peeche S3 hai, wo sochte file bas hamare paas hai. To S3 pe abhi request nahi bhejenge.

Do raaste: ya delete pe ek event jaaye jo S3 ko bole in files ko delete kar do, ya baad me ek sync job chale: DB me nahi hai to S3 se hata do.

*Technical me:* "saamne faad di, S3 baad me" = soft delete (DB me turant), "event jaaye, S3 se hatao" = async delete (queue + worker), "sync job" = orphan cleanup / reconciliation job.

**Maan lo folder ki jaanch, ek file fail:**

**Arpan:** Folder parent aur child ke hisaab se: parent ki tracking id aur har child ki tracking id, ek-ek karke upload. Ek child fail hua to poora parent folder fail; ek bhi fail nahi, sab pass, to poora folder pass.

**Maan lo ek saath 10 file aayi:**

**Arpan:** Apni copy badha dunga, chaar copy ho gayi: le bhai 10 file, thodi tu sambhal, thodi tu, thodi main. Inhe scan karo aur dheere-dheere us bade ladke ko dete raho jiska naam S3 hai. Beech me net chala gaya to dobara upload, hamare paas status hai ki kahan tak hua tha.

*Technical me:* bada ladka = S3 · "parent ka haal bachchon se" = folder status = sab child ka AND, "copy badha do, baant ke" = kai worker parallel, "kahan tak hua" = har file ka apna status (resume).

---

**6. Ravi ne URL me 1042 ki jagah 1043 likha aur Priya ka bank statement khul gaya. Download bhi hamare server se ja raha.**

**Arpan:** Ravi asli member hai (JWT), par Priya ki statement maange to "Priya ko saath la". Server DB me dekhe file kiski hai / kisse share hui, tabhi link de. Link me chhed-chhaad ki to link bekaar. Download truck seedha ghar (S3 se seedha user ke phone pe), hamare bank me nahi aata (hamare server se data nahi guzarta).

*Technical me:* "kiski hai" = owner / share check (authorization, JWT sirf pehchaan), "link bekaar" = presigned download URL (signature), "truck seedha ghar" = S3 se direct download.

**Maan lo beech me CDN laga diya, Priya ka statement Jaipur ke CDN dabbe me pada, Ravi CDN wala URL khole:**

**Arpan:** Faisla hamara server karega, S3 / CDN sirf rakhte aur dete. Kamre ke bahar main khada: ID dikha, file kiski hai dekho, tabhi token; bina token CDN file nahi deta.

*Technical me:* CDN pe bhi signed URL / signed cookie (server sign kare, CDN jaanche).

**Aur neeche (sirf link ki umar chhoti karna kaafi nahi):**

**Arpan:** Sirf link ki umar chhoti karne se fayda nahi, Ravi ne thodi der bhi dekh li to cheez uske paas chali gayi; isliye pehle haq dekho, tabhi token.

*Technical me:* expiry sirf leak ka nuksaan ghatata, asli rok = authorization.

---

**7. Ravi ne link 200 logon ke WhatsApp group me daal diya.**

**Arpan:** Link ki umar chhoti (presigned URL expiry), file ke hisaab se: bank statement jaisi 1-2 minute, baaki 5-6 minute. Umar hamara faisla, S3 ka nahi. User khud baante to wo uski zimmedari, system pe har cheez nahi thop sakte.

*Technical me:* "umar" = presigned URL expiry (file ki sensitivity pe).

**Aur neeche (app pehle hi bata de):**

**Arpan:** Private file ho to app bol de "ye private hai, link share mat karna"; phir bhi baante to chhoti umar sambhal legi.

*Technical me:* share warning (UI) + expiry backup.

---

**8. Scan me 10 minute baaki, Ravi ko download chahiye. Scan me virus nikla to?**

**Arpan:** Scan ki alag tez line, jaise SMS me OTP ki (doctor line me nahi lagta = scan ka alag Kafka topic, baaki kaam ke peeche nahi rukta). Scan poora hone tak link hi nahi; ho gaya to user ko bata do.

Paise ginne jaisa: kisi ne kaha "gin do", maine andaza bola 10,000, wo sabko bata aaya, par asal me 20-25 hazaar nikle; pehle poora gino, phir batao.

Upload hote hi Priya ne manager ko link bheja to bhi manager ko "file jaanch me hai, thoda ruko" dikhe, file nahi. Virus nikla to file lo hi mat.

Hum antivirus nahi, file system hain; mall vs airport ki security, hum utna jaanchte jitna system ko chahiye (magic bytes + virus scan, poora antivirus nahi). Zaroori ho to bahar wala (third party) pehle jaanche, phir hamare paas.

*Technical me:* "alag line" = scan ka alag Kafka topic (Kafka me priority nahi hoti, isliye zaroori kaam ki alag line), "link nahi" = status CLEAN tak download band, "lo hi mat" = INFECTED -> file hatao (quarantine / delete) + user ko batao.

**Maan lo bahar wala scanner 1 ghante se jawab hi nahi de raha, Ravi ki file "scan baaki" pe atki.**

**Arpan:** Ye wahi Notification wali company ki kahani hai. Scanner hamara nahi, bahar ka hai, to wo chup bhi ho sakta hai. Thodi der ruk ke dobara bhejo; do-teen baar bhi chup rahe to file ko dead letter queue me daal do aur insaan ko alert, investigate karo, taaki baaki files ki line na atke. Tab tak file "scan baaki" pe hi rahegi aur link band rahega, kyunki bina scan ke file kisi ko dena khatra hai. Ravi ko app pe dikhao "file jaanch me hai".

*Technical me:* dobara try = retry with backoff, dead letter queue = DLQ + alert (scan wali tez line se alag), link band = CLEAN tak download nahi (fail-closed).

**Isi pe ek aur raasta (scanner pe health check):**

**Arpan:** Scanner pe health check lagao, jaise LB me karte the: har 5-10 second dekho wo theek hai ya nahi. Teen baar jawab nahi aaya to use dead maan lo aur doosre scanner pe bhejo, taaki ek scanner pe sab na tike. Scanner wali company ko alert bhejo, aur kyun gira wo logs (CloudWatch / Papertrail) se pata chalega.

*Technical me:* health check + had (3 fail = dead), backup scanner, alert + logs.

**Maan lo complete kabhi aaya hi nahi:**

**Arpan:** Pehli baat, file ko temporary jagah rakho; jaanch pass ho tab asli jagah, phir complete bhejo. Aur complete nahi aaya to S3 ka lifecycle rule purani temporary aur adhoori multipart files apne aap clean kar dega, ya sweeper job chala sakte jo bekaar files khud hata de.

*Technical me:* quarantine / temp prefix pe upload, pass hone pe asli bucket / prefix me copy.

---

**9. App ko 6 mahine ho gaye. S3 ka bill pichhle mahine se double. S3 me ek hi meme lakhon baar padi, Ravi ne apni shaadi ki 2 GB video teen baar daali (bhool gaya tha ki pehle daal chuka).**

**Arpan:** Pehli baat, S3 duplicate file handle nahi karta, to hamein apne server se kehna padega duplicate handle karo. Main server pe baitha hoon, ek aadmi moti file le ke aaya, maine bhej di.

Kuch din baad wahi file dobara le aaya, phir bhej di; 3-4 baar aa raha. To uski file pe ek number chipka do, maan lo ABC123, aur DB (mote register) me rakh do.

Number hash hoga: file ka content dekh ke koi bhi algorithm chalao, number nikla ABC123. Wahi file dobara aayi to dobara number nikala, wahi ABC123 nikla kyunki same file hai; to wo file upload nahi hone denge.

Number phone nikaale ya server, wo design ka hissa nahi; hamein bas ek number chahiye jisse pata chale ye file pehle se hai ya nahi. Presigned URL me number jod sakte, par pata nahi wahan hota hai ya nahi, to wahan nahi.

Number ka roop kuch bhi (string, number); mote register me rakho, pehle se hai to theek, nahi to nahi.

*Technical me:* "file ke content se number" = content hash (jaise SHA-256), "mote register me number" = DB me hash, "pehle se hai to upload nahi" = deduplication (S3 khud nahi karta).

**Maan lo pooche S3 khud dedup kyun nahi karta:**

**Arpan:** S3 ko to paise chahiye na. Tumne jitna GB use kiya uske hisaab se paise katenge, uske liye to fayda hi hai. Andar wo override bhi kar de, tab bhi tumse double charge karega, kyunki S3 ka business model wahi hai.

*Technical me:* S3 ka bill = jitni jagah (GB-month), dedup hamari zimmedari.

**Maan lo Priya wahi viral meme upload karti hai jo Ravi sabse pehle daal chuka (aur uske baad lakhon log bhi). Number register me mil gaya, upload ruk gaya, par Priya ki list me meme dikh hi nahi raha, kyunki uske naam pe kuch likha hi nahi gaya.**

**Arpan:** Priya aur Ravi same meme upload kar rahe, to iske liye sochna padega. Priya ka upload hum rok nahi sakte, kyunki Ravi ne apna kiya, Priya ne apna.

Agar app me login hi na hota to dekhte; par yahan login hai, Priya ki alag userId, Ravi ki alag. Hash same ho sakta hai kyunki meme same hai, par userId alag.

Pehle Priya ke hisaab me dekhenge: kya Priya ne ye file kabhi upload ki thi? Nahi. Phir poore DB me dekha: ye file pehle se uploaded hai.

To Priya ko bolo ki file upload ho gayi, aur jab Priya kabhi download kare to usi meme pe redirect kar denge, meme download ho jaayega, dikkat hi nahi. S3 andar duplicate ka kya karta, wo S3 jaane.

*Technical me:* "Priya ka upload mat roko, pehle uske hisaab me dekho" = hash + userId pe check, "poore DB me hai to usi pe redirect" = Priya ke naam nayi row jo purani S3 file ki taraf ishaara kare (reference / pointer, ek file kai maalik), alag users ke beech = cross-user dedup.

**Maan lo Ravi ne wo meme delete kiya, peeche ki safai ne S3 se file hata di. Agle din Priya ne download dabaya, uska redirect usi file pe tha, error aaya.**

**Arpan:** Har user ki alag copy rakh sakte, jagah nahi gherti, ek chhoti copy: ye Ravi ka hai, ye Priya ka. Ravi ne delete kiya, par dekh lo aur kisi user ka bhi hai kya; Priya ka nikla.

To Ravi ke hisaab se hata denge, Priya ka S3 me pada rahega; jab Priya delete karegi tab S3 se bhi gayab. Dobara upload nahi karenge; server userId se Priya ka alag, Ravi ka alag rakhta.

Ravi ne hataya to Ravi ki userId se khatam, Priya ki userId me hai to Priya ke paas jaayega, Ravi se lena-dena nahi.

*Technical me:* "ye Ravi ka, ye Priya ka" = har user ki apni row (reference), "aur kisi ka bhi hai to mat hatao, aakhri wala hataye tab S3 se" = reference counting (ginti 0 tabhi S3 se safai).

&nbsp;

Poora design = [07_file_upload.md](07_file_upload.md)
