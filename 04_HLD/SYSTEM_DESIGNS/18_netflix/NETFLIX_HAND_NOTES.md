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

Poora design = main file baad me, jab Arpan bole.
