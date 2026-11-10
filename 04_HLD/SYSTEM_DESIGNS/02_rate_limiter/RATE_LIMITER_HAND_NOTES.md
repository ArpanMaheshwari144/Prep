# Rate limiter: hand notes (Arpan aur Ravi, "OTP bhejo" button)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan ki app me "OTP bhejo" ka button hai, har OTP pe SMS company ka bill. Ravi ne mazaak me script chala di jo har second 1000 baar button dabati hai.

&nbsp;

**1. Server dab gaya, har request pe SMS ja raha, SMS company ka bill chadh raha.**
Arpan: Mera ek app hai, ek service hai jiska main per din bill de raha. Kisi bande ne aake kuch chala diya, ab dhadadhad SMS ja rahe, bematlab. To limit lagao. Aisa nahi ki 1000 hai, 10,000 hai, 20,000 chal le. Ek bande ke liye kuch threshold hona chahiye. Rate limit ka kaam yahi hai, limit lagana.
Technical me: "ek bande ke liye threshold" = rate limit per user, "limit lagao" = had paar to mana (429).

**2. Ginti server apni memory me rakh raha. App 10 server pe, load balancer Ravi ki requests baant deta. Har server ke paas Ravi ki ginti kam, har server sochta "had me hai", Ravi 5 ki jagah 50 OTP le gaya.**
Arpan: Maan le mere paas 5 aadmi khade, har ek bol raha "abhi had me hai, ek aur bhej sakta". Ye app to galat bata rahe. Main inko kahunga: tu chhod, tu mujhe bata, phir main bataunga kitni request hain. Ek server ne bataya 1, doosre ne 2, main count karta rahunga 1 + 2 + 3 + 4. Hisaab-kitaab main apne aap rakhunga.
Technical me: "tu chhod, mujhe bata, hisaab main rakhunga" = central counter (Redis), "1 + 2 + 3 + 4" = har request pe ek hi jagah ginti +1.

**3. Ginti ka khaata ghadi se: 10:00-10:10 ek khaata, 10:10-10:20 agla, har khaate me 5. Ravi 10:09:58 pe 5 leta, khaata badla, 10:10:01 pe phir 5. 3 second me 10.**
Arpan: Iska answer mujhe pehle se pata hai, padha tha tabhi ban pa raha. Had ek dabbe me rakh lete hain, har bande ke liye: is dabbe se token nikaalo aur aage badh jaao. Dabbe pe ek had hoti hai, 5 second me ek token ya 3 second me ek token. Wo apni raftaar se badhega, user apni raftaar se aayega. User aaye nahi to uske paas bahut saare token, to burst allow hai, koi dikkat nahi. Ye token bucket naam ka algorithm hai.
Technical me: "dabbe se token nikaalo, aage badho" = token bucket, "apni raftaar se badhega" = refill rate (OTP pe: max 5, har 2 minute me 1, aam raftaar 10 min me 5, khaali baitha user ek saath 5 le sakta), "user aaye nahi to bahut token" = burst allow.

**4. Ravi ke dabbe me 1 token. Script ne ek pal me 20 request maari, 10 server pe. Har server ne Redis se padha "tokens = 1", ek nikaala, 20 andar ghus gayi.**
Arpan: Pehle bola: Ravi ko 10 alag server pe jaane hi mat do, uska ek home server, wahi se jaaye. Phir: Redis single thread application hai, wo ek waqt me ek hi chalata. Iska answer bhi pata hai: Lua script. Ye sab cheez ek hi baari me kar de, andar koi ghus hi na paaye. Jaise banking me update aur check ek hi me karte hain, wahi Lua script kar degi.
Technical me: "Redis single thread" = Redis ek command ek waqt (par alag-alag GET aur DECR ke beech doosra server ghus sakta, isliye single thread akela kaafi nahi), "sab ek hi baari me, andar koi ghus na paaye" = Lua script (check + nikaalna atomic), "banking me update aur check ek me" = wahi atomic check-and-update.

**5. Limiter OTP Service ke andar. Script ki 1000 request LB se, login check karke OTP Service tak pahunchti, wahan mana. SMS nahi gaya, par raaste ke saare server bojh utha rahe.**
Arpan: Mere ghar me ek security guard hai, wo kehta 10,000 log andar nahi aa sakte. Par wo main gate pe nahi, andar kamre me baitha hai. Ghar ke andar to 1000 log ghus gaye, galat ho gaya. Use bahar gate pe bitha de, ghar ke andar koi ghus hi na paaye, gate pe hi hata do.
Technical me: "guard andar kamre me" = limiter service ke andar, "guard bahar gate pe" = limiter API gateway pe.

**6. Gate ka guard ginti Redis se dekhta, Redis gira. Ya sabko jaane de ya sabko rok de. OTP button ke liye kya, home page (feed) ke liye kya?**
Arpan: Mere papa ke kuch khaas dost hain, unko andar aane do, wo dekh ke chale jaayenge, koi nuksaan nahi. "Papa ji namaste, chalte hain." Par beech me kuch chor bhi hain jo hamari tijori lootne aa rahe, unhe rok do.
Technical me: "papa ke dost, dekh ke chale jaayenge" = feed / home page fail-open, "tijori lootne wale chor rok do" = OTP fail-closed.

**7. Script gate pe rukti, 429 khaati, par ruki nahi. Din-raat har second maarti, gate ko roz karodon "mana", Ravi ko farak nahi.**
Arpan: Ek baar mana kiya, do baar mana kiya, teesri baar bhi aaya. "Ravi bhai, tum yahan aao, mere is dabbe se jaake baat karo, wo kuch samjhaayega, chhoti si toffee dega." Us dabbe me Ravi ka gala pakad ke wahin bitha denge: tu ab kahin nahi ja sakta, yahan se hil hi nahi sakta, is site ke liye tu permanent band.
Technical me: "ek baar, do baar, teesri baar" = baar-baar had todne ka pattern, "alag dabba, yahan se hil nahi sakta" = ban list, "permanent band" = ban wala gate pe bina gine lautta.

**8. OTP login se pehle, had phone number pe. Ravi ki script har baar naya number daalti, har number ki ginti 1. Had IP pe lagai to subah 9 baje office ke 500 log ek hi WiFi (IP) se, 5 OTP ke baad poora office atka.**
Arpan: User login ho gaya to uski saari information aa jaati, userId hamare paas, wahan koi dikkat nahi; kuch kare to ban kar do. Login se pehle ki dikkat hai, to login pe thodi dheeli had chhod do. Par itni dheeli nahi ki koi script hazaar baar chala de. Google pe password kitni baar guess karega, 2, 3, 5 baar. Hazaar baar koi haath se type nahi karta, karega to intention theek nahi. Ek banda roz hazaar baar password reset kar raha, kal dekha phir hazaar, ye insaan ke liye possible nahi, us IP ko WAF me daal do. Office me hazaar log hain par hazaar log ek saath password nahi bhoolte, bhoole bhi to ek-do din, isliye pattern notice karte hain. Ek second me login button pe 10,000 request, ek office WiFi bhi ye nahi kar sakta. 1 lakh me 50,000 bhool gaye to aisa to main har design tod dun, perfect design koi nahi, aaj bhi Google, YouTube toot-ta hai.
Technical me: "login ke baad userId hamare paas" = userId pe kadi had, "login pe dheeli had" = login se pehle IP pe dheeli had, "insaan hazaar baar nahi, pattern notice" = script pattern pakadna, "IP ko WAF me daal do" = WAF.

**9. Result ka din, subah 10 baje 1 crore bachche OTP maangte. Har bachcha apni had me, phir bhi OTP Service aur SMS company dono gire.**
Arpan: Mere paas 2-3 log hain jo 10,000 logon ka kaam kar rahe, koi na koi girega hi. To log badha do, server badha do. Aur SMS company ko utni bheed mat jaane do, wo apne hisaab se kaam kar rahi. Line lagwa, aaram-aaram se: "jaao beta, ek-ek karke; tu nikal, ab tu nikal." Hamare aur SMS company ke beech ek cheez fit kar denge jo spike rok le. SMS ab ek ja, ek ja, dheela-dheela, aaram se.
Technical me: "log badha do, server badha do" = apne server scale, "line lagwa, ek-ek karke" = queue (Kafka), "beech me cheez jo spike rok le" = SMS company ki raftaar se worker, global had.

**Maan lo OTP line me atka, der se nikla, tab tak bekaar.**
Arpan: Line me har OTP pe uski umar likh do. Line se nikalte waqt dekho, umar khatam to phenk do, bhejo mat; bachcha naya OTP maang lega.
Technical me: umar = message TTL / expiry (line se nikalte waqt jaancho), purana OTP SMS company tak jaata hi nahi.

**10. App 3 region me: Mumbai, US, Europe, har region ka apna Redis. Ravi VPN badal-badal ke teeno se OTP maangta, har Redis sochta "5 hi hue", 15 OTP le gaya.**
Arpan: VPN se aa raha ho to uska ek particular region decide kar lete hain. Suppose uska region Mumbai, India ke servers. 3 se maara ya 10 se, uska main region India hai to count India ke Redis pe hoga, US aur Europe pe nahi. Wahi se count karo, wahi se dekho, uske hisaab se aage badhenge.
Technical me: "ek particular region decide" = home region, "count India ke Redis pe" = ginti ghar region ke Redis me.

**Maan lo OTP login se pehle, sirf phone number aata. Ravi VPN se US gateway pe. US ko kaise pata is number ka ghar Mumbai?**
Arpan: Phone number har desh ka alag hota hai, hamara +91 se shuru. VPN IP badal sakta hai, number nahi; VPN chala ke call karun to bhi +91 hi jaata. To +91 dekh ke ghar India, ginti India ke Redis me. Ravi sach me US shift hua aur wahan ka number liya, to uska ghar US hi hai.
Technical me: "+91 se pata" = phone number ka country code se home region (koi alag register nahi chahiye), "IP badle, number nahi" = VPN se bachav.

&nbsp;

Poora design = [02_rate_limiter.md](02_rate_limiter.md)
