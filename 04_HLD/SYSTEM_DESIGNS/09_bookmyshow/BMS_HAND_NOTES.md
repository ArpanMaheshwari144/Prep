# BookMyShow: hand notes (do counter, ek seat)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan BookMyShow pe Avengers ke pehle show ki seat D5 book kar raha hai. Arpan ki misaal: cinema ke do counter ek hi seat bech rahe, aur ek-doosre ko pata nahi.

&nbsp;

**1. Arpan ne D5 chuni aur "book" dabaya. Usi second ek aur ladke ne bhi D5 pe book dabaya. Dono ke app ne seat khaali dekhi, aur dono ko "seat mil gayi" aa gaya.**
Arpan: Seat dekhna aur seat book karna, ise ek saath rakh, alag-alag mat rakh. Maan le do counter hain, main is counter pe, doosra us counter pe, aur andar ek hi seat bachi. Dono counter ko nahi pata ki doosra bhi wahi seat bech raha. Maine kaha "ek seat chahiye, lo mere paise". Mere counter wale ne jaake seat pe ek parchi rakh di: "ye Arpan ki seat hai, Arpan iska payment karega, ye Arpan ko milegi, kisi aur ko nahi." Doosra counter wala dekhega: "achha, yahan Arpan ki parchi lagi hai, ise main bech nahi sakta. Tu ja bhai, seat book ho gayi."
Technical me: "ek saath rakh, alag mat" = dekhna aur rokna ek hi query (UPDATE ... WHERE seat FREE), "seat pe parchi" = hold, "doosra parchi dekh ke mana" = doosre ko 0 row, "seat chali gayi".

**2. Arpan ne D5 pe parchi laga di, phir pay kiye bina phone rakh ke chai peene chala gaya, kabhi wapas nahi aaya. D5 pe hamesha ke liye Arpan ki parchi, koi aur le hi nahi sakta.**
Arpan: Parchi ko thodi der ke liye wahin rehne de. Counter wala dekh raha: "ye payment kar raha, chal main 5 minute tak dekhta hoon. Aa gaya to theek, nahi to hata dunga yahan se." Usne 5 minute intezaar kiya, Arpan nahi aaya, to parchi phaad ke phenk di. Seat agle ke liye.
Technical me: "5 minute tak dekhta hoon" = hold expiry (hold_until), "parchi phaad ke phenk di, seat agle ke liye" = hold hata ke seat wapas FREE.

**3. Parchi phaadne wala counter wala har minute ghoom ke purani parchiyan dekhta hai. Aaj wo chhutti pe. Arpan ki parchi ko 20 minute ho gaye, abhi bhi D5 pe lagi, aur naya aadmi D5 lena chahta hai.**
Arpan: Sabse pehli baat, parchi uthane wala chhutti pe hai to uska ek chhota bhai rakh lo, wo kaam kar lega. Ya us din ke liye uska replacement laana padega. Ya phir hum us parchi pe hi umar laga dein ki wo apne aap wahan se gayab ho jaaye: parchi 5 minute tak yahan padi hai, 5 minute ke baad tu hat ja, parchi khatam.
Technical me: "chhota bhai / replacement" = backup safai job, "parchi pe hi umar" = shart query me hi (seat FREE ho YA hold_until nikal chuka), phaadne wale ka intezaar nahi.

**4. Avengers ki advance booking subah 10 baje khuli. 10 lakh log ek saath app pe toot pade, seat sirf 50 hazaar. Servers dub gaye, jo asli me pay kar rahe the unke page bhi atke.**
Arpan: Counter badha de, server badha de. Jo sirf dekhne aa raha uske liye ek alag diary, aur jo kuch hilane aa raha uske liye alag diary, primary-replica wali cheez. Aur thode-thode log andar jaao, zyada nahi. Sabse pehli baat, cinema ki capacity kitni hai, utne log andar. Maan lo mere hall me 1000 ki jagah aur saamne 5000 khade, to 1000 andar jaao, baaki 4000 ghar jaao. Aur wo line me lago: "le bhai, ye teri parchi, tu jaayega, phir agla number tera."
Technical me: "counter / server badha de" = machine badhana, "dekhne wale ki alag diary" = read replica, "capacity jitne andar, baaki line me" = virtual queue (waiting room), "ye teri parchi, agla number tera" = token.

**5. Line me Arpan ka number aaya, andar jaane ka token mila jo 10 minute chalega. Usne 8ve minute pe D5 chuni, to D5 pe 5 minute ki parchi lagi. 10ve minute pe token khatam, par parchi ke 3 minute baaki. Pay kar paayega? Kaunsi ghadi chalegi?**
Arpan: Humne use token diya, wo andar aa gaya. Iska matlab wo authenticate ho chuka, ab koi dikkat nahi. Andar aa gaya, to haan bhai, seat ruk gayi iske liye. Payment nahi karega to use hata do. Use utne time me payment karna hai jo uske liye allot hua. Kar diya to theek, nahi to agla.
Technical me: "token diya, andar aa gaya, authenticate ho chuka" = token sirf gate ka pass, "seat ruk gayi, allot hue time me payment" = pay ka faisla hold ki ghadi (hold_until) karti. Do alag ghadiyan: token = gate, hold = seat.

**6. Arpan ne 4:30 pe Pay dabaya, PSP dheema. 5:00 pe parchi ki umar khatam, 5:10 pe Priya D5 maang rahi, PSP ka jawab 6:00 pe "paisa kat gaya".**
Arpan: Time ke andar paisa kata to seat Arpan ki, Priya ko nahi; humne vaada kar diya, warna consumer court ka case. PSP dheema hai to galti Arpan ki nahi, hamari taraf ki hai. To parchi pe status likh lo: pay chal raha hai, PSP ka jawab baaki. Jab tak ye likha hai, umar badha do, seat kisi aur ko dikhe hi nahi. Ya query me hi daal do.
Technical me: "status likh lo" = HELD -> PAYMENT_PENDING (Pay dabate hi), "umar badha do" = is status pe hold expire nahi hota (query aur safai job isko nahi chhoote), PSP ka FAILED / timeout aaye tabhi FREE.

&nbsp;

Poora design = [09_bookmyshow.md](09_bookmyshow.md)
