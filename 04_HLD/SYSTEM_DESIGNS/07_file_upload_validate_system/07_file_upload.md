# File upload: YouTube jaisa "peeche chal raha" ki kahani

Ek aam file-sharing app. Arpan 2 GB ki video, 10 GB ki file ya "Goa trip" folder daalta hai, aur Ravi ke saath
share karta hai. Kaam: user ko rukna na pade, badi file bhi bina hamare server ko dabaye pahunche, file kabhi na
khoye, aur ek ki file doosra kabhi na dekh paaye.

Har kadam = **kahani** (dikkat aur hal, bolne wali bhasha) -> **Asli naam** (interview me yahi shabd) -> tasveer
(kahani ka shabd, bracket me asli naam).

---

## FR / NFR / Numbers (interview me pehle 2 minute)

```
FR      file upload (badi bhi, folder bhi) · jaanch (virus + sach me wahi type) · status dikhe · share + download
NFR     user ruke nahi · file khoye nahi · ek ki file doosra na dekhe · bheed (shaam ko hazaaron upload) me bhi chale
Numbers 1000 log x 2 GB ek saath = 2 TB -> ye data hamare server se guzarna hi nahi chahiye
        status log baar-baar dekhte -> padhna zyada, likhna kam
```

---

## 1. Server khud sab kar raha, Arpan 10 minute spinner dekh raha (peeche bhejo)

Arpan ne 2 GB ki video daali. Server file leta, virus scan karta, thumbnail banata, save karta, aur ye sab hone ke
baad "done" bolta. Arpan 10 minute spinner dekh raha, aur 1000 log aaye to jo sirf apni list khol rahe wo bhi atke.
YouTube kuch rokta nahi, bol deta "upload chal raha, chai pi aao". Matlab peeche kuch chal raha. Stadium jaisa: line
me lag jao, baaki main peeche karta rahunga. File lo, line me daalo, worker peeche kaam kare. Status apne DB me rakho,
user UI pe dekh le.

Aur jaanch: koi PDF bol ke exe na chadha de. To virus scan (bahar ki company ka) aur file ka chhota tukda dekh lo ki
sach me PDF / CSV hai.

**Asli naam:** line = Kafka + worker (async), status = DB me file ki row ka status, chhota tukda = magic bytes,
scan = third party virus scan.

```mermaid
flowchart LR
    A["Arpan: Upload"] --> U["Upload Server"]
    U -- "status: UPLOADED" --> DB[("DB<br/>(file ka status)")]
    U --> K["Line<br/>(Kafka)"]
    K --> W["Worker<br/>(scan + magic bytes + thumbnail)"]
    W --> DB
    style K fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style W fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

## 2. 2 TB hamare server se guzar raha (seedha bada dabba + tukde)

Shaam ko 1000 log ek saath 2 GB daalte hain, 2 TB hamare server se guzar raha, network bhar gaya. Badi file apne
server se guzaro hi mat, seedha bade dabbe me daalo.

**Maan lo 10 GB ki file, 8 GB pe net gaya:** file ko tukdon me todo. 8 GB ke tukde pahunch chuke, sirf baaki 2 GB
dobara. Aur client ne dobara upload kiya hi nahi, to adhoore tukde S3 me pade rahenge aur paisa lagega. To thodi der
baad S3 ki safai unhe hata de.

**Asli naam:** bada dabba = S3, tukde = multipart upload (resume, sirf toota tukda dobara), safai = lifecycle rule
(adhoora multipart upload kuch din baad hatao).

## 3. S3 kaise maane ye hamara user hai (receptionist ka token)

Phone seedha S3 me daal raha, par S3 ko kaise pata ki ye Arpan hai aur use ijaazat hai? App me AWS ki chaabi rakhi
to koi bhi nikaal ke bucket bhar dega ya doosron ki files padh lega. Bank jaisa: koi seedha manager ke cabin me nahi
ghusta. Pehle receptionist (hamara server) JWT dekh ke tasalli kare, phir ladke ke haath me token de: "ye le, jab
teri baari aaye to file de dena". 10 GB ka bora receptionist khud nahi uthata, ladka khud andar le jaata.

**Maan lo koi 10 GB ki faltu file daal raha:** size ki had lagao, ya badi file pe paisa lo, S3 ka kharcha bhi hamara.

**Asli naam:** receptionist ka token = presigned URL (hamara server JWT dekh ke banata, chaabi phone ko kabhi nahi),
had = size limit / quota.

```mermaid
flowchart LR
    A["Arpan ka phone"] -- "1. JWT: upload karna hai" --> U["Receptionist<br/>(Upload Server)"]
    U -- "2. token (presigned URL)" --> A
    A -- "3. file seedha, tukdon me" --> S3[("Bada dabba<br/>(S3)")]
    style U fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
    style S3 fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

## 4. File S3 me aa gayi, server ko pata hi nahi (S3 khud bataye)

File hamare server ke paas se guzri hi nahi, to server ko pata nahi chala ki upload kab poora hua. DB me "UPLOADING"
pada hai, na Kafka me kuch gaya, na scan shuru hua. S3 ko bol do: "jab file aa jaaye, mujhe bata dena". Hum DB me
mark kar denge aur ladke ko bata denge. Poori nahi hui to wo dobara try karega.

**Asli naam:** S3 event notification -> hamara server DB me status UPLOADED + Kafka me scan ka kaam.

```mermaid
flowchart LR
    S3[("S3")] -- "file aa gayi (event)" --> U["Upload Server"]
    U --> DB[("DB: UPLOADED")]
    U --> K["Kafka"] --> W["Worker (scan)"]
    style S3 fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

## 5. "Goa trip" folder, naam badla to 500 copy (ped hamare DB me)

Arpan ne "Goa trip" folder daala: Day1, Day2, kul 500 photo. S3 me folder hote hi nahi, bas lambe naam wali files.
Naam path me likha to "Goa trip" se "Goa 2026" karna = 500 copy + 500 delete. Folder ka hisaab alag, apne server
me: ek parent, baaki uske child, ped jaisa (DSA wala tree: root, phir level 1, level 2). Naam badalna = sirf parent
ka naam badlo, saare bachche usi ke neeche. Parent bulaya to saari files saath; delete kiya to poora. S3 sirf files
rakhe.

**Asli naam:** DB me folders table (id, parent_id, name), file ki row me folder_id; S3 key file ki id se, naam se
nahi. Rename = ek row update.

```mermaid
flowchart TB
    G["Goa 2026<br/>(parent, id 1)"] --> D1["Day1<br/>(parent_id 1)"]
    G --> D2["Day2<br/>(parent_id 1)"]
    D1 --> F1["img1.jpg -> S3 key f-901"]
    D2 --> F2["img2.jpg -> S3 key f-902"]
    style G fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

## 6. Ravi ne 1043 likha, Priya ka statement khula (maalik check + truck seedha ghar)

Arpan ne Ravi ko `app.com/files/1042` bheja. Ravi ne 1043 likha aur Priya ka bank statement khul gaya. Ravi asli
member hai (JWT sahi), par Priya ki statement maange to "Priya ko saath la". Server DB me dekhe file kiski hai, kisse
share hui, tabhi link de, warna mana. Link me chhed-chhaad ki to link bekaar. Aur download bhi hamare server se mat
guzaro: truck seedha ghar, S3 se seedha user ko.

**Asli naam:** owner / share check = authorization (JWT sirf pehchaan batata, ijaazat DB batata), presigned download
URL (signature, badla to bekaar), S3 se direct download.

```mermaid
flowchart LR
    R["Ravi: file 1043"] --> U["Server<br/>(kiski hai? kisse share?)"]
    U -- "Priya ki, Ravi ko nahi -> mana" --> R
    A["Arpan: file 1042"] --> U
    U -- "token (presigned download)" --> A
    A -- "seedha" --> S3[("S3")]
    style U fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

**Maan lo Ravi ne apna link 200 logon ke WhatsApp group me daal diya:** link ki umar chhoti, file ke hisaab se: bank
statement jaisi 1-2 minute, baaki 5-6 minute. Umar hamara faisla, S3 ka nahi. User khud baante to wo uski zimmedari,
system pe har cheez nahi thop sakte.
**Asli naam:** presigned URL expiry, file ki sensitivity pe.

## 7. Scan baaki, Ravi ko download chahiye; virus nikla to (alag line + lo hi mat)

File S3 me aa gayi, scan me 10 minute baaki, aur Ravi download maang raha. Scan ki alag tez line, jaise SMS me OTP ki
(doctor line me nahi lagta). Scan poora hone tak link hi nahi; ho gaya to user ko bata do.

**Maan lo scan me virus nikla:** file lo hi mat, hata do aur user ko batao. Hum antivirus nahi, file system hain;
mall aur airport ki security jaisa, hum utna jaanchte jitna system ko chahiye. Zaroori ho to bahar wala (third party)
pehle jaanche, phir hamare paas.

**Asli naam:** scan ke liye alag topic / priority queue, status CLEAN tak download band, INFECTED -> file hatao
(quarantine / delete) + user ko batao.

```mermaid
flowchart LR
    S3[("S3: file aayi")] -- "event" --> U["Upload Server"]
    U --> K["Scan ki alag line<br/>(priority topic)"]
    K --> W["Scan worker"]
    W -- "CLEAN" --> DB[("DB: CLEAN<br/>ab link milega")]
    W -- "INFECTED" --> X["File hatao<br/>+ user ko batao"]
    style K fill:#ffe8b3,stroke:#f59f00,stroke-width:2px,color:#000
```

---

## Wrap (30 second me poora design)

User ko server se token milta hai aur file seedha S3 me jaati hai, badi ho to tukdon me; adhoore tukde S3 ki safai
hatati hai. S3 khud batata hai ki file aa gayi, server DB me status likhta hai aur Kafka ki alag tez line pe scan
jaata hai. Scan CLEAN tak link nahi, virus nikla to file hatao. Folder ka ped hamare DB me, rename = ek row. Download
pe maalik / share check, phir chhoti umar ka link aur file S3 se seedha.

```
user ruke nahi       -> Kafka + worker, status DB me
server dabe nahi     -> presigned URL, seedha S3, multipart
file khoye nahi      -> S3 + S3 event (status atke nahi)
kachra na bache      -> lifecycle safai (adhoore tukde)
doosra na dekhe      -> owner / share check, chhota link, scan tak band
```

---

Notes (Arpan ke shabd) = [FILE_UPLOAD_HAND_NOTES.md](FILE_UPLOAD_HAND_NOTES.md) · [<- MASTER SHEET](../../00_MASTER_SHEET.md)
