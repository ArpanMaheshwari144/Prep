# Netflix: hand notes (Arpan, Ravi aur Money Heist ka naya season)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Money Heist ka naya season Friday dopahar 12:30 baje poori duniya me ek saath. Arpan, Ravi aur India ke 2 crore log 12:30 pe ek saath play dabate hain, saath me US, Spain, Brazil bhi.

&nbsp;

**1. Saari episode US ke store me. 12:30 pe karodon log wahi file maang rahe, har tukda samundar paar se, Ravi ke TV pe buffering, data pahunchane wali company ka bill aasmaan pe.**
Arpan: Main roz wahi file lene doosre shehar jaata hoon, bus me tel phoonk ke. File ghar nahi la sakta, par apne shehar me to la sakta hoon. CA doosre shehar me hai, to mera dost jo roz aata-jaata hai, wo file mere shehar le aaye. Main chupchaap jaao, sign karo, wapas aao. Bekaar me baar-baar shehar kyun.
Technical me: "file apne shehar me" = ISP ke andar Netflix ka dabba (Open Connect), "ghar nahi la sakta" = poora video phone pe nahi rakhte, "dost roz aata-jaata" = dabba US ke store se bharta rehta.

**2. Dabba lag gaya, par season 12:30 pe aaya, 12:29 tak dabba khaali. 12:30 pe 2 crore ek saath, dabba sabko "laata hoon" bolta, saari request phir samundar paar.**
Arpan: Jab pata hai log kis liye aa rahe, to wo cheez pehle se laa ke rakh lo. Pen drive me Money Heist rakh liya, ab sabko baant raha: le tu bhi le, tu bhi le, buffering hi nahi. Aur ye cheez ban gayi hai, ab badlegi nahi, isliye rakhne me koi dikkat nahi.
Technical me: "pehle se laa ke rakho" = release se pehle raat ko dabba bharna (proactive fill), "badlegi nahi" = video static hai, isliye CDN pe rakhna safe.

**3. Mumbai ke dabbe ki jagah seemit, Netflix ka saara maal (har film x kai size) nahi samaata.**
Arpan: Ek dabba nahi, kai dabbe le lo, jaise maal ek truck me na aaye to 3-4 truck. Par dabbe ki bhi seema hai, isliye dabbe me wahi daalo jiska craze hai. Garam cheez paas rakho, thandi nahi. Purana season koi aaj bhi dekhe to bade dabbe se mangwa lo, thodi der chhote dabbe me rakh lo, garmaahat khatam to wapas.
Technical me: "garam paas" = popularity se kya rakhna (region-wise), "bade dabbe se mangwa lo" = upar wala bada store / tier, "garmaahat khatam to wapas" = thanda maal dabbe se nikalna.

**4. Mumbai me 20 dabbe, kuch me Money Heist hai kuch me nahi, ek ki saans phool rahi. Ravi ke TV ko kaise pata kis dabbe se maange?**
Arpan: Akela bhaar kyun uthaun, 10 dost bulao, sabko dabba do, nikaal-nikaal ke do. Garam season ko zyada se zyada dabbon me rakho. Request hamare server pe aati hai, use pata hai kis dabbe me kya hai, wo mod dega. Hospital ka guard: wheelchair wale is line, mahila us line, buzurg us line.
Technical me: "zyada dabbon me" = hot content ki kai copy, "server mod dega" = play pe server batata kaunse dabbe se lo (steering), "guard" = load + maal dekh ke sahi dabba.

**5. Ravi ne TV pe 27 minute dekha, band kiya. Subah metro me phone pe khola, 27:00 se chahiye.**
Arpan: Ravi ka account hai, Netflix account se yaad rakhta. Jaise kisi ke 10,000 rakhe, parchi pe uska naam likha, bus stand pe mila to pehchaan liya. TV, phone, kahin bhi khole, yaad wahi. Parchi DB me.
Technical me: "account se yaad" = watch position user ke account pe server me, device me nahi, "parchi DB me" = persistent store.
**Maan lo karodon TV har kuch second "26:50 pe hoon" bhej rahe, DB ki saans phool gayi.**
Arpan: Race me har kadam thodi likhte, jab ruk jaaye tab likho. Abhi parchi paas rakho, 27 pe ruk gaya to register me daal do.
Technical me: "parchi paas" = position pehle fast store me, "ruk gaya to register" = DB me sirf aakhri / kabhi-kabhi likhna (write kam).
**Maan lo tukde ISP ke dabbe se aa rahe, AWS ke server ko kaise pata ki ruk gaya?**
Arpan: Do log baat kar rahe, ek mujhse baat nahi karta. Maine dost ko bola, jo baat hui sankshep me mujhe bata diya kar. Jis din kuch nahi bataya, matlab aaj aage nahi badha.
Technical me: "dost sankshep me bataye" = dabba / player playback ki report server ko bhejta.

**6. App khola, server "tere liye" line banane baitha: 3 saal ka itihaas, karodon logon se milaan, 6 second khaali screen.**
Arpan: Game parlour wala jaanta hai Arpan 3 baje aata, GTA pehle se laga ke rakhta. Ravi ka 3 saal ka pattern hai, kya dekha, kya search kiya. App khole to tayaar cheez saamne. Ye Twitter ki feed jaisa, pehle se push.
Technical me: "pehle se laga ke rakhta" = list raat ko pehle se banti (batch), "app khole to saamne" = cache se uthao, sochna nahi.
**Maan lo list raat 2 baje bani, Ravi ne 11 baje Dhurandhar khatam ki, 11:05 pe sabse upar wahi.**
Arpan: Dekhi hui cheez turant parchi pe likh lo. List kab bhi bane, dikhate waqt parchi milao, lal patti lag jaati. Dobara dikhana galti nahi, Netflix ka dhandha tujhe app pe rokna hai.
Technical me: "turant parchi" = abhi ka dekha alag fast store me, "dikhate waqt milao" = pehle se bani list + abhi ki baat mila ke dikhana.
**Maan lo Priya ne pehli baar join kiya, koi itihaas nahi.**
Arpan: Parlour wala mere bhai ko nahi jaanta, wo kahega ye hit game hai, khelo. India me hai to India ka garam maal dikhao. Dheere-dheere lat lagegi, pattern banega.
Technical me: "garam maal" = naye user ko region ka trending (cold start), "pattern banega" = dekhne se apni list.

&nbsp;

**Padhai wala hissa (mock se pehle samjha, scene me nahi poocha gaya)**

**P1. AWS ka CDN (CloudFront / Akamai) kyun nahi, apna dabba kyun?**
Doodh company ki kahani: courier company har litre ka paisa leti hai, aur Netflix ke liye ye "litre" roz ke hazaaron TB hain, bill aasmaan pe. Netflix ne har mohalle ki dukaan me apna fridge muft me rakh diya: Jio / Airtel jaise ISP ke andar apna dabba. Teen fayde: paisa (dabba ek baar ka kharcha, per-GB bill nahi), paas (video ISP ke andar se aata, buffering kam), ISP bhi khush (uska bahar jaane wala bandwidth bachta). Aur aam CDN tab file laata jab koi pehli baar maange, Netflix raat ko jab net khaali hota tab pehle se bhar deta. Doodh wala subah se pehle fridge bhar deta, maangne ka intezaar nahi karta.
Technical me: courier = per-GB bill wala CDN (CloudFront / Akamai), mohalle ka fridge = Open Connect (ISP ke andar, free), raat ko pehle se = proactive fill (off-peak).

**P2. Phir AWS kis kaam ka?**
AWS chhoda nahi. Login, browse, "tere liye" line, aur play dabane pe kaunsa paas wala dabba video dega, ye sab AWS pe chalta. Sirf video ke bytes Open Connect se aate.
`tu play dabata -> AWS (login, kaunsa episode, kaunsa paas wala dabba) -> ISP ke andar Netflix ka dabba -> video ke tukde`
Technical me: AWS = login / browse / recommendation / steering (control plane), Open Connect = sirf video ke bytes.

**P3. Ek video ke kai size: Twitter wala Ferrari / Alto, teen twist ke saath**
Twitter me khud nikala tha: Ferrari highway ke liye, Alto gaon ke liye, phone net dekh ke size chunta, net gira to tasveer thodi dhundhli par chalti rahe. Video 4-4 second ke tukdon me, aage khiskao to player manifest dekh ke seedha us tukde pe (copy ka 51wa page). Player aage ke 2-3 tukde pehle se le ke rakhta: net thoda hile to bhi video nahi rukti, aur net zyada gire to agla tukda Alto wala. Netflix me teen twist:
(1) Har film ki apni seedhi. Cartoon (Doraemon) me rang saade, kam hilta, to kam bits me bhi saaf. Action film (Money Heist ka bank wala goli-baari scene) me bahut hilta, zyada bits chahiye. Ek hi niyam sab pe lagao to cartoon pe data barbaad aur action film dhundhli.
(2) Ek size ke bhi kai roop. Purana TV ek tarah ki file samajhta, naya phone doosri tarah ki jo aur chhoti hoti. Play dabate hi app batata "main ye wala samajhta hoon", wahi roop milta.
(3) Ek baar banao, karodon baar chalao. Twitter me video abhi daala abhi chalana tha, jaldi thi. Netflix ke paas studio ki film release se din-hafte pehle aa jaati. Kai machine (AWS pe) tukde baant ke aaraam se saare size aur roop banati (wahi "video ki id ki parchi, 10 log kaam baanto"), phir raat ko dabbon me.
Awaaz aur subtitle alag files: ek hi 1080p ke saath Hindi ya English kuch bhi chuno, video ki 10 copy nahi banti.
Technical me: Ferrari / Alto = transcoding (kai size) + ABR, 4 sec tukde + manifest = segments + seek, aage ke tukde pehle se = player buffer, har film ki apni seedhi = per-title encode ladder, kai roop = codec, ek baar banao = AWS pe batch encoding (parallel workers), awaaz / subtitle alag = alag audio / subtitle tracks.

**P4. "Tere liye" line ke baaki tukde**
Mohalle ka chaiwala tujhe roz dekhta, tere aane se pehle jaanta tu adrak wali kam cheeni maangega. Netflix tera itihaas leta (kya dekha, kitna dekha, beech me chhoda, kya search kiya) aur tere jaise logon ka bhi ("jinhone Money Heist dekha, unhone Squid Game bhi dekha"). Raat ko badi machines har user ki list bana ke rakh deti. App khola to list uthao aur thoda abhi ke hisaab se badlo: "abhi raat hai to thriller upar", "tu TV pe hai". Tasveer bhi teri: ek film ki 5-6 tasveer, tu action zyada dekhta to Money Heist ki goli wali, koi romance dekhta to usi film ki pyaar wali. Film wahi, darwaza alag.
`tera dekhna / chhodna / search -> Kafka -> raat ka batch: har user ki list; app khola -> list uthao (cache) + thoda abhi ka mod`
Technical me: chaiwala = offline / batch recommendation (tera + tere jaise logon ka itihaas), thoda abhi ka mod = online halka re-rank, apni tasveer = personalized thumbnail, dekhna -> Kafka = events ka stream batch tak.

&nbsp;

Poora design = [18_netflix.md](18_netflix.md)
