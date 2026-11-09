# File upload: hand notes (YouTube jaisa "peeche chal raha")

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** ek aam file-sharing app. Arpan 2 GB ki video / 10 GB file / "Goa trip" folder daalta hai, Ravi ke saath share karta hai.

**Pehle poochho kaunsa product** (Arpan: "har file upload alag, koi scan karta koi nahi, koi 10 GB / folder leta hi nahi"): core = 1, 3, 4, 6 har jagah (seedha S3 bhi core, kyunki 3, 4, 6 usi pe tike); agar badi file = 2 ka tukde wala hissa; agar folder = 5; agar bahar ki file (virus scan) = 8.

&nbsp;

**1. Server khud sab kar raha (lena, virus scan, thumbnail, save), Arpan 10 min spinner dekh raha, 1000 log aaye to list kholne wale bhi atke.**
Arpan: YouTube kuch rokta nahi, bol deta "upload chal raha, chai pi aao". Matlab peeche kuch chal raha. Stadium jaisa: line me lag jao, baaki main peeche karta rahunga. File lo, line (Kafka) me daalo, worker peeche kaam kare. Status apne DB me, user UI pe dekh le. Aur jaanch: PDF bol ke exe na chadha de, to virus scan (third party) + file ka chhota tukda dekh lo ki sach me PDF / CSV hai.
Technical me: "line" = Kafka + worker (async; Kafka me file nahi jaati, sirf file ki id / S3 ka pata, file S3 me), "status" = DB me file ka status, "chhota tukda" = magic bytes, "scan" = third party virus scan.

**2. 1000 log x 2 GB = 2 TB hamare server se guzar raha, network bhar gaya.**
Arpan: Badi file apne server se guzaro hi mat, seedha bade dabbe (S3) me. 10 GB ko tukdon me todo; 8 GB pe net gaya to sirf baaki 2 GB dobara. Client ne dobara upload kiya hi nahi to adhoore tukde thodi der baad S3 ki safai hata de.
Technical me: "bada dabba" = S3, "tukde" = multipart upload (resume), "safai" = lifecycle rule (adhoora multipart hatao).

**3. Phone seedha S3 me daal raha, par S3 kaise maane ye hamara user hai? App me AWS chaabi rakhi to koi bhi nikaal le.**
Arpan: Bank jaisa: koi seedha manager ke cabin me nahi ghusta (cabin = S3). Pehle receptionist (hamara server) JWT dekh ke tasalli kare, phir ladke ke haath me token de (token = presigned URL, sirf is file ke liye, thodi der ka): "ye le, jab teri baari aaye to file de dena". 10 GB ka bora receptionist khud nahi uthata (file hamare server se nahi guzarti, phone seedha S3 me daalta hai). Badi faltu file pe had ya paisa.
Technical me: "receptionist ka token" = presigned URL (hamara server deta, chaabi phone ko nahi), "had" = size limit / quota.
**Maan lo 10 GB ki file, upload 40 minute chal raha, aur receptionist ka token thodi der ka tha, beech me hi khatam ho gaya.**
Arpan: Token poori file ka ek nahi, har tukde ka apna. Kadam 2 me file tukdon me ja rahi hai, to har tukda bhejne se pehle receptionist se us tukde ka chhota token le lo. Beech me net gaya ya token ki umar khatam hui, to koi baat nahi: jo tukde pahunch gaye wo S3 me pade hain, bas baaki tukdon ke liye naya token maango aur wahin se aage badho. Is tarah token chhota bhi rehta hai (leak hua to nuksaan kam) aur bada upload bhi poora ho jaata hai.
Technical me: har tukde ka token = har part ka alag presigned URL (multipart upload), resume pe naya token = sirf baaki parts ke liye naye URL.

**4. File S3 me aa gayi, par server ko pata hi nahi, DB me "UPLOADING" pada, scan shuru hi nahi hua.**
Arpan: S3 ko bol do: "jab file aa jaaye, mujhe bata dena". Hum DB me mark kar denge aur ladke ko bata denge. Poori nahi hui to wo dobara try karega.
Technical me: "S3 bataye" = S3 event notification -> DB status + Kafka me scan ka kaam.
**Maan lo server ne DB me UPLOADED likh diya, par Kafka me scan ka kaam daalne se pehle gira. File UPLOADED pe atki, scan kabhi shuru nahi.**
Arpan: Wahi Kafka aur Notification wala: vasiyat likh do. UPLOADED ke saath hi likh do "iska scan karwana hai", ek hi baar me. Beech me gira to baad me koi utha ke Kafka me daal dega.
Technical me: "UPLOADED ke saath vasiyat" = outbox (status + scan ka kaam ek hi DB transaction me), "baad me koi daal dega" = relay.
**Maan lo S3 ne "file aa gayi" do baar bataya, ya ulte kram me.**
Arpan: Do baar aa sakta hai, koi dikkat nahi. Parchi rakho: is file ka kaam ho gaya, likh liya; dobara aaya to dekha ho gaya, chhod do. Ulta aaya to dekho kaunsa naya hai, purana aaya to chhod do.
S3 khud likhta hai: "Amazon S3 event notifications are designed to be delivered at least once." (https://docs.aws.amazon.com/AmazonS3/latest/userguide/EventNotifications.html) aur "Event notifications aren't guaranteed to arrive in the same order that the events occurred." (https://docs.aws.amazon.com/AmazonS3/latest/userguide/notification-content-structure.html)
Technical me: "parchi" = idempotency (file id + version pe ek hi baar), "kaunsa naya" = S3 event ka `sequencer` (bada = baad wala, usi file ke liye).

**5. "Goa trip" folder (Day1, Day2, 500 photo). S3 me folder hote hi nahi; naam path me likha to rename = 500 copy + 500 delete.**
Arpan: Folder ka hisaab alag, apne server me. Ek parent, baaki uske child, tree jaisa (DSA wala tree: root, phir level 1, level 2). Naam badalna = sirf parent ka naam badlo, saare bachche usi ke neeche. Parent bulaya to saari files saath; delete kiya to poora. S3 sirf files rakhe.
Technical me: "tree" = DB me folders table (id, parent_id), file ki row me folder_id; S3 key file ki id se, naam se nahi.

**6. Ravi ne URL me 1042 ki jagah 1043 likha aur Priya ka bank statement khul gaya. Download bhi hamare server se ja raha.**
Arpan: Ravi asli member hai (JWT), par Priya ki statement maange to "Priya ko saath la" (server DB me dekhe file kiski hai / kisse share hui, warna mana). Server DB me dekhe file kiski hai / kisse share hui, tabhi link de. Link me chhed-chhaad ki to link bekaar. Download truck seedha ghar (S3 se seedha user ke phone pe), hamare bank me nahi aata (hamare server se data nahi guzarta).
Technical me: "kiski hai" = owner / share check (authorization, JWT sirf pehchaan), "link bekaar" = presigned download URL (signature), "truck seedha ghar" = S3 se direct download.

**7. Ravi ne link 200 logon ke WhatsApp group me daal diya.**
Arpan: Link ki umar chhoti (presigned URL expiry), file ke hisaab se: bank statement jaisi 1-2 minute, baaki 5-6 minute. Umar hamara faisla, S3 ka nahi. User khud baante to wo uski zimmedari, system pe har cheez nahi thop sakte.
Technical me: "umar" = presigned URL expiry (file ki sensitivity pe).

**8. Scan me 10 minute baaki, Ravi ko download chahiye. Scan me virus nikla to?**
Arpan: Scan ki alag tez line, jaise SMS me OTP ki (doctor line me nahi lagta = scan ka alag Kafka topic, baaki kaam ke peeche nahi rukta). Scan poora hone tak link hi nahi; ho gaya to user ko bata do. Virus nikla to file lo hi mat. Hum antivirus nahi, file system hain; mall vs airport ki security, hum utna jaanchte jitna system ko chahiye (magic bytes + virus scan, poora antivirus nahi). Zaroori ho to bahar wala (third party) pehle jaanche, phir hamare paas.
Technical me: "alag line" = scan ka alag Kafka topic (Kafka me priority nahi hoti, isliye zaroori kaam ki alag line), "link nahi" = status CLEAN tak download band, "lo hi mat" = INFECTED -> file hatao (quarantine / delete) + user ko batao.
**Maan lo bahar wala scanner 1 ghante se jawab hi nahi de raha, Ravi ki file "scan baaki" pe atki.**
Arpan: Ye wahi Notification wali company ki kahani hai. Scanner hamara nahi, bahar ka hai, to wo chup bhi ho sakta hai. Thodi der ruk ke dobara bhejo; do-teen baar bhi chup rahe to file ko dead letter queue me daal do aur insaan ko alert, investigate karo, taaki baaki files ki line na atke. Tab tak file "scan baaki" pe hi rahegi aur link band rahega, kyunki bina scan ke file kisi ko dena khatra hai. Ravi ko app pe dikhao "file jaanch me hai".
Technical me: dobara try = retry with backoff, dead letter queue = DLQ + alert (scan wali tez line se alag), link band = CLEAN tak download nahi (fail-closed).

&nbsp;

Poora design = [07_file_upload.md](07_file_upload.md)
