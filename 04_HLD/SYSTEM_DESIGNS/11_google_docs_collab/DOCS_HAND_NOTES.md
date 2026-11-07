# Google Docs: hand notes (Arpan aur Ravi, "Trip plan" doc)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan aur Ravi ek hi doc "Trip plan" pe saath likh rahe hain. Doc me hai `Goa jaayenge`.

&nbsp;

**1. Jo "Save" dabata hai, uska poora text server pe jaake purane ko hata deta hai. Arpan ne upar "Goa jaayenge" likh ke Save kiya, Ravi ne neeche "budget 20k" likh ke Save kiya. Ravi ki screen pe purana text tha, to Arpan ki line gayab.**
Arpan: Dono ka text dikhao, kisi ko gayab kyun karna. Ye to last write wins wali situation ho rahi, Google Docs me last write wins nahi chalne ka, warna Google Docs ka matlab hi khatam. Poora text nahi bhej sakte, poora bhejenge to overwrite ho jaayega. Operation bhej do: insert, delete, add. Maine likha "Goa jaayenge", Ravi ne kuch kiya to uska operation bhejenge: is letter ke baad add kar do. Main Ravi ka operation laaunga: isne ye likha to "Goa jaayenge" ke baad budget likh dunga, ya neeche, ya beech me kahin. Google andar se kaise karta, wahan jaana bhi nahi chahta, main khud kaise karunga wo dekh raha.
Technical me: "last write wins nahi" = dono ka badlaav bache, "poora text nahi, operation bhej do: insert, delete" = operation (op) bhejna, "is letter ke baad add kar do" = op me jagah (position).

**2. (Ye dikkat Arpan ne khud uthayi) Hazaron-laakhon log dhadadhad save kar rahe, har chhota badlaav DB pe jaaye to?**
Arpan: Sab save DB pe jaayenge kya, kitna DB write karega, Cassandra ki bhi haalat ho jaayegi. Main hi Google Docs hoon: wo baar-baar hata rahe, badal rahe, check kar rahe. To main apna ek chhota register bana lunga, unki saari baatein likhta rahunga, phir kuch der baad DB me save kar dunga. Har write pe DB query nahi maarunga, warna DB baith jaayega.
Technical me: "chhota register, saari baatein likhta rahunga" = buffer (Redis), "kuch der baad DB me save" = batch write (Cassandra), "har write pe DB query nahi" = har akshar pe DB nahi.

**3. Doc me `Goa jaayenge`. Ek hi second me Arpan ne shuru me "Hum " joda, Ravi ne "Goa" ke baad (jagah 3) " kal" joda. Server pe Arpan ka pehle pahuncha, Ravi ka "jagah 3" lagaya to ban gaya `Hum kal Goa jaayenge`, Ravi ka kal galat jagah.**
Arpan: Maine likha "Hum Goa jaayenge", usne likha "Goa kal jaayenge", to poora ban jaayega "Hum Goa kal jaayenge". Uska kal galat jagah nahi aur hamara Hum bhi galat jagah nahi. Humne shabdon ko khiska diya. Main yaad rakhun ki Arpan ne Goa se pehle Hum likha tha, to wahan Hum likh dun; Ravi ne Goa ke baad kal likha tha, to "Hum Goa kal jaayenge". Koi dikkat hi nahi.
Technical me: "shabdon ko khiska diya" = OT (Operational Transformation), "yaad rakhun kisne kahan likha" = op ki jagah pichhle op ke hisaab se aage.

**4. Lakhon doc, 100 server. Arpan server A se, Ravi server B se. Dono server apne kram me OT lagaate, ek hi doc ki do alag shakal.**
Arpan: Ek centralized system bana do. Do ladke apne-apne server pe likh rahe, main Google Docs hoon: dono ka mere tak aaya, main dono ka sentence padh ke sahi se likh dunga, "Hum Goa kal jaayenge", aur dono ki screen pe wahi, ek hi version.
Par sabka ek nahi hoga: har doc ke liye alag. Main aur Ravi ek doc pe to hum dono ka ek centralized system. 100 log ek doc pe to un 100 ka ek, 1 lakh log to unka ek. Us centralized system ka koi na koi maalik hoga, uske hisaab se chalega.
Technical me: "centralized system, ek hi version" = ek jagah kram aur OT, "har doc ka alag centralized system, uska maalik" = har doc ka ek owner server.

**5. Owner server ne 5 second ke badlaav register (apni memory) me jama kiye, Arpan ko "Saved" dikha diya. DB me likhne se pehle server gira, Arpan ka poora para uda.**
Arpan: Use ek chhoti memory me likh de. Chal maan le mere paas ek dabba hai, us dabbe me saari information store kar raha. Kal wo dabba mar jaaye ya gir jaaye, to us dabbe ki ek aur copy bana dunga. Ya main dabbe ko bolunga: ye meri information tu bhi store kar le. Kal mujhe kuch ho jaaye to tu hai, tujhe kuch ho jaaye to main hoon.
Technical me: "ek dabba, saari information store" = Redis buffer, "us dabbe ki ek aur copy" = Redis ki copy (replica), "Saved" tab jab copy ne bhi likh liya; "mujhe kuch ho to tu hai" = owner gira to naya owner usi copy se uthaaye. DB (Cassandra) me batch waise hi.
**Maan lo dabba aur uski copy dono gir gaye, system restart, batch abhi DB me nahi gaya tha?**
Arpan: Dabba apni disk pe bhi likh de, memory me bhi aur disk pe bhi; gire to disk se wapas aa jaayega, jaise Kafka me line disk pe rehti. Phir bhi sab gira diya to haan, kuch second ki lines chhoot sakti hain, user dobara likh lega. Aisa sochenge to design hi nahi banega: poori consistency chahiye to 2 kyun, 500 copy rakho, aur 500 ka paisa bhi do. Interviewer dekhna chahta hai main sochta kaise hoon, har copy girana design nahi.
Technical me: "disk pe bhi likh de" = Redis AOF (persistence), "kuch second chhoot sakti" = chhota data loss window, trade-off: zyada durability = zyada copy / zyada paisa.

**6. Arpan doc ka maalik, usne Ravi ko hata diya. Par Ravi ke server ke paas permission ki purani copy, "Ravi = editor". Hatne ke baad bhi Ravi 2 minute likhta raha.**
Arpan: Main apni memory me chhota note daal dunga: ye banda editor nahi, iska koi bhi operation aaye, accept nahi karunga. Ravi alag server pe hai, to main ek parchi rakhunga "Ravi ab editor nahi", aur wo parchi ek tez bhaagne wale bachche ko dunga: jahan bhi Ravi ki permission hai wahan sab ja aur bata de "Ravi ab editor nahi". Ek centralized authority hai jo sab likh rahi. Usne mana kiya to chhota banda building ke bahar khada hoke chilla de "suno, Ravi editor nahi", to Ravi jis bhi server pe ho wahan se uska editor note hat chuka hoga. Bahut se answer ho sakte, ek sahi answer nahi: Redis pub/sub bolo ya building ke bahar koi chilla de.
Technical me: "centralized authority" = permission ka asli sach (SQL), "iska operation accept nahi" = server pe permission check, "tez bhaagne wala bachcha, building ke bahar chilla de" = pub/sub invalidation.

**7. Ladka chillaya "Ravi editor nahi", par server 7 us pal restart / net se bahar tha, awaaz suni hi nahi. Pub/sub dobara nahi chillata. Server 7 ki memory me abhi bhi "Ravi = editor", aur Ravi wahin se likh raha.**
Arpan: Server akela nahi, uski copy saath khadi hai. Main thodi der bahar ja raha: "kuch bhi ho to note karke likh lena, wapas aake tujhse dekh lunga, tu meri copy hai." Ladka chillaya "Ravi editor nahi", copy ne likh liya. Main wapas aaya: "kya hua?" "Bas ek awaaz thi, Ravi editor nahi." Achha, to Ravi ki permission kaat do, khatam kahani.
Technical me: "server ki copy jo peeche note kare" = standby / replica server bhi pub/sub sunta, "wapas aake copy se dekh lunga" = reconnect pe chhoote hue badlaav copy (ya SQL) se le ke note theek, phir hi Ravi ka op maano.

&nbsp;

Poora design = [11_google_docs_collab.md](11_google_docs_collab.md)
