# Rate limiter: hand notes (Arpan aur Ravi, "OTP bhejo" button)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan ki app me "OTP bhejo" ka button hai, har OTP pe SMS company ka bill. Ravi ne mazaak me script chala di jo har second 1000 baar button dabati hai.

**Ek nazar: dhaancha + dikkat chain (round me sabse pehle ye, phir FR / NFR isi se):**
```
Ravi / script
     |
     v
[ GATE: API gateway pe guard ]  <-- ban list: baar-baar todne wala yahin se wapas
     |   har request pe poochhe: "Ravi ke dabbe me token hai?"
     v
[ Redis: har user ka dabba (token bucket) ]
     |   check + token nikaalna EK saath (Lua script); 10 server hon tab bhi ginti ek jagah
     |   had paar -> 429 + "itne second baad aana"
     v
[ OTP Service ] (kai server)
     |
     v
[ Line (queue) ] -> worker / consumer SMS company ki raftaar se bhejta; umar khatam wala OTP phenk do
     |
     v
[ SMS company ]

saath me: Redis gira -> dono khule (OTP + feed), gateway apni kachchi ginti se 5-5 karke (local fallback)
          login se pehle dheeli had (IP / number), login ke baad userId pe kadi
          3 region -> ginti ghar region ke Redis me (+91 = India)

 1  script har second 1000 OTP, bill chadha     ->  ek bande ki had (rate limit, 429)
 2  10 server, har ek ki alag ginti             ->  ginti ek jagah (central counter, Redis)
 3  khaata badla, 3 second me 10 OTP            ->  dabbe se token, apni raftaar se (token bucket)
 4  20 request ek saath, "tokens = 1"           ->  check + nikaalna ek baari me (Lua script)
 5  guard andar kamre me, 1000 ghus gaye        ->  guard gate pe (API gateway)
 6  guard ka Redis gira                         ->  dono khule, bouncer 5-5 karke (fail-open + local fallback)
 7  script 429 khaati, phir bhi maarti          ->  pattern dekho, ban / WAF
 8  naya number har baar, office ek WiFi        ->  login se pehle dheeli had, baad me userId pe kadi
 9  result ka din, SMS company giri             ->  line lagwao, ek-ek karke (queue, Kafka)
10  VPN se 3 region, 15 OTP                     ->  ek ghar region, ginti wahin (home region, +91)
11  feed ka cache usi Redis, Ravi ka dabba gaya ->  alag Redis; ek me ho to LRU hatao + cache ka TTL chhota (noeviction)
```

**FR / NFR kahan se:** hawa se nahi aate. Upar ki dikkat chain dekh ke nikale; har FR / NFR line ke aage likha hai wo kis kadam ki dikkat se aayi.
Arpan: Request aayi, usko dekho, had me hai to jaane do, had me nahi hai to 429 de do, thodi der baad try kar lega. Doosra, mamla theek rehna chahiye, consistent rehna chahiye: 20 request alag-alag server pe jaa rahi to iska matlab ye thodi ki wo alag-alag account ginegi, ek hi hai. Redis gira to NFR. Office ke IP se 500 log aa rahe to unko bhi dekh lenge. VPN se region badal raha to uske hisaab se dekho, uske home server pe bhejna hai. NFR me: consistent rehna chahiye, available hona chahiye, aisa nahi ki wahi down ho gaya to poora server down; bahut saare region aaye to ek hi home region; kuch bhi gira to failover. Ye FR / NFR dikkat ki chain se nikla.
Technical me: "had paar to 429, baad me try" = 429 Too Many Requests + Retry-After, "alag server pe gayi to bhi ek ginti" = central counter (Redis), "consistent" = atomic ginti (Lua), "wahi gira to poora down nahi" = availability (fail-open + local fallback), "office ke IP se 500" = per-user had (IP nahi), "VPN se region badle to ghar region" = home region.

**FR / NFR / Numbers (interview me pehle 2 minute, main file se):**
```
FR      1  had me hai to jaane do, paar to 429, thodi der baad dobara       -> kadam 1, 3
        2  office ke ek IP se 500 log aa rahe, unhe bhi sambhalo            -> kadam 8

NFR     1  ginti consistent rahe, request kisi bhi server pe jaaye          -> kadam 2, 4
        2  hamesha available: limiter / Redis gira to site na gire, failover -> kadam 6
        3  VPN se region badle to bhi had ek hi rahe                        -> kadam 10

Numbers nahi: yahan koi faisla nahi badalte.
```

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

**Maan lo counter wala tareeka (har request pe +1, pehli pe 60 sec ki umar). Ravi ki pehli request pe +1 ho gaya, umar lagane se pehle server gira.**
Arpan: Counter bina umar ke pada rahega, kabhi khatam nahi hoga, Ravi ka hisaab hamesha ke liye bigad gaya. To +1 aur umar do alag kaam nahi, ek hi kaam: ya dono likhe jaayein ya kuch nahi. Ladke ko bula ke bolo "+1 aur 60 dono ek saath likhna, beech me gira to kuch mat likhna, sab pehle jaisa". Kuch nahi hua to dobara try kar lo.
Technical me: "dono ek saath ya kuch nahi" = INCR + EXPIRE ek Lua script me (atomic, ya SET key 1 EX 60 NX + INCR); bina umar ka counter = key kabhi expire nahi, user hamesha block. (HANDS_ON 02 me chala ke dekha.)

**5. Limiter OTP Service ke andar. Script ki 1000 request LB se, login check karke OTP Service tak pahunchti, wahan mana. SMS nahi gaya, par raaste ke saare server bojh utha rahe.**
Arpan: Mere ghar me ek security guard hai, wo kehta 10,000 log andar nahi aa sakte. Par wo main gate pe nahi, andar kamre me baitha hai. Ghar ke andar to 1000 log ghus gaye, galat ho gaya. Use bahar gate pe bitha de, ghar ke andar koi ghus hi na paaye, gate pe hi hata do.
Technical me: "guard andar kamre me" = limiter service ke andar, "guard bahar gate pe" = limiter API gateway pe.

**6. Gate ka guard ginti Redis se dekhta, Redis gira. Ya sabko jaane de ya sabko rok de. OTP button ke liye kya, home page (feed) ke liye kya?**
Arpan: Redis gira hai, API gateway (bouncer) thodi gira. Redis ki to ek copy (replica) bhi hai; worst case sab gira. OTP band nahi kar sakte: kisi ka zaroori bank transfer ho raha, OTP nahi aaya to wo kya karega, rate limiter ka kaam ye nahi. Feed band karna bhi galat, koi zaroori reel upload kar raha ho. To dono khule rakho. Bouncer bahar khada hai, use bol do "ek saath 1000 andar mat bhejna, 5-5 karke bhejna". Gateway ke andar ka kaam mujhe pata nahi, par aisa kuch kar sakte. Interviewer poochhe "OTP kyun band kiya, mera bank transfer atka", to wo jawab galat lagega.
Arpan (aage): OTP band karne ka matlab sabke liye band nahi. Shaq wala kaun, ye dekho: kisi ne 2-3 second me 5 OTP maang liye to wo suspicious, sirf uske liye gate band. Jo apni time range me baith ke OTP maang raha, use mana nahi kar sakte, warna dikkat. Interviewer poochhe "OTP kaise band kar doge, mera bank transaction atka", to bolo: sir sabka nahi, sirf usi ka jisne ek second me 3-4 maang liye.
Arpan (aage, chor kaun): Chor wo jo sach me chor hai, mooh pe mask pehen ke aaya: 1 second me 5-6 OTP dhadadhad maang liye, wo suspicious, uske liye gate band, warna faltu pareshan karta rahega. Bhala-changa aadmi bole "papa ka dost hoon" to pehle papa se verify karao: "naam bata", "Ravi"; "papa, Ravi ko jaante ho?" "haan" to andar bulao. Jo 2 minute me 1 OTP maang raha, wo suspicious nahi. Aur suspicious kaun, ye dekhna rate limiter ka kaam hi nahi; rate limiter ne apna kaam kar diya (OTP bhi chalu, feed bhi), kaun suspicious ye system ka alag hissa dekhe.
Technical me: "dono khule, bouncer 5-5 karke bheje" = Redis gira to fail-open, par har gateway apni memory me kachchi ginti se had lagaye (local fallback limiter, thoda dheela par pura khula nahi); "2-3 second me 5 OTP = shaq, sirf uska band" = OTP pe band sirf us user ka jo pattern tode (per-user block), baaki sabke liye khula; "suspicious kaun, limiter ka kaam nahi" = abuse / fraud detection alag hissa, limiter sirf had lagaye; Redis ki replica pehle.

**7. Script gate pe rukti, 429 khaati, par ruki nahi. Din-raat har second maarti, gate ko roz karodon "mana", Ravi ko farak nahi.**
Arpan: Ek baar mana kiya, do baar mana kiya, teesri baar bhi aaya. "Ravi bhai, tum yahan aao, mere is dabbe se jaake baat karo, wo kuch samjhaayega, chhoti si toffee dega." Us dabbe me Ravi ka gala pakad ke wahin bitha denge: tu ab kahin nahi ja sakta, yahan se hil hi nahi sakta, is site ke liye tu permanent band.
Arpan (doosri kahani, 16-Nov round): Pehle 1-2 baar chot khaayi to bouncer API gateway pe khada kiya. Ab bouncer bhi roz 1000 logon se uljha hai. To pattern dekho: ye banda roz-roz aa ke yahi kar raha? Haan to ban karo, WAF me daal do, block, kahani khatam.
Technical me: "ek baar, do baar, teesri baar" = baar-baar had todne ka pattern, "alag dabba, yahan se hil nahi sakta" = ban list, "permanent band" = ban wala gate pe bina gine lautta.

**8. OTP login se pehle, had phone number pe. Ravi ki script har baar naya number daalti, har number ki ginti 1. Had IP pe lagai to subah 9 baje office ke 500 log ek hi WiFi (IP) se, 5 OTP ke baad poora office atka.**
Arpan: User login ho gaya to uski saari information aa jaati, userId hamare paas, wahan koi dikkat nahi; kuch kare to ban kar do. Login se pehle ki dikkat hai, to login pe thodi dheeli had chhod do. Par itni dheeli nahi ki koi script hazaar baar chala de. Google pe password kitni baar guess karega, 2, 3, 5 baar. Hazaar baar koi haath se type nahi karta, karega to intention theek nahi. Ek banda roz hazaar baar password reset kar raha, kal dekha phir hazaar, ye insaan ke liye possible nahi, us IP ko WAF me daal do. Office me hazaar log hain par hazaar log ek saath password nahi bhoolte, bhoole bhi to ek-do din, isliye pattern notice karte hain. Ek second me login button pe 10,000 request, ek office WiFi bhi ye nahi kar sakta. 1 lakh me 50,000 bhool gaye to aisa to main har design tod dun, perfect design koi nahi, aaj bhi Google, YouTube toot-ta hai.
Technical me: "login ke baad userId hamare paas" = userId pe kadi had, "login pe dheeli had" = login se pehle IP pe dheeli had, "insaan hazaar baar nahi, pattern notice" = script pattern pakadna, "IP ko WAF me daal do" = WAF.

**9. Result ka din, subah 10 baje 1 crore bachche OTP maangte. Har bachcha apni had me, phir bhi OTP Service aur SMS company dono gire.**
Arpan: Mere paas 2-3 log hain jo 10,000 logon ka kaam kar rahe, koi na koi girega hi. To log badha do, server badha do. Aur SMS company ko utni bheed mat jaane do, wo apne hisaab se kaam kar rahi. Line lagwa, aaram-aaram se: "jaao beta, ek-ek karke; tu nikal, ab tu nikal." Hamare aur SMS company ke beech ek cheez fit kar denge jo spike rok le. SMS ab ek ja, ek ja, dheela-dheela, aaram se.
Technical me: "log badha do, server badha do" = apne server scale, "line lagwa, ek-ek karke" = queue (Kafka), "beech me cheez jo spike rok le" = SMS company ki raftaar se worker / consumer, global had.

**Maan lo OTP line me atka, der se nikla, tab tak bekaar.**
Arpan: Line me har OTP pe uski umar likh do. Line se nikalte waqt dekho, umar khatam to phenk do, bhejo hi mat. Agar OTP ki umar khatam ho gayi to use bhej hi kyun rahe ho, rok do; wo 60 second baad dobara maang lega. Tumne kabhi dekha hai, OTP maangte hain to neeche second chalte hain, 60, 59, 58, ki 60 second me OTP aa jaayega; nahi aaya to "Resend OTP" ka button aata hai. Bahut se bank me aisa hi hota hai. Wahi cheez yahan jodni hai.
Technical me: umar = message TTL / expiry (line se nikalte waqt jaancho), purana OTP SMS company tak jaata hi nahi.

**10. App 3 region me: Mumbai, US, Europe, har region ka apna Redis. Ravi VPN badal-badal ke teeno se OTP maangta, har Redis sochta "5 hi hue", 15 OTP le gaya.**
Arpan: VPN se aa raha ho to uska ek particular region decide kar lete hain. Suppose uska region Mumbai, India ke servers. 3 se maara ya 10 se, uska main region India hai to count India ke Redis pe hoga, US aur Europe pe nahi. Wahi se count karo, wahi se dekho, uske hisaab se aage badhenge.
Technical me: "ek particular region decide" = home region, "count India ke Redis pe" = ginti ghar region ke Redis me.

**Maan lo OTP login se pehle, sirf phone number aata. Ravi VPN se US gateway pe. US ko kaise pata is number ka ghar Mumbai?**
Arpan: Phone number har desh ka alag hota hai, hamara +91 se shuru. VPN IP badal sakta hai, number nahi; VPN chala ke call karun to bhi +91 hi jaata. To +91 dekh ke ghar India, ginti India ke Redis me. Ravi sach me US shift hua aur wahan ka number liya, to uska ghar US hi hai.
Technical me: "+91 se pata" = phone number ka country code se home region (koi alag register nahi chahiye), "IP badle, number nahi" = VPN se bachav.

**11. Ravi ka dabba Redis me, abhi khaali (saare token use kar chuka). Ye app sirf OTP login nahi, ek dukaan bhi hai: home page (feed, kadam 6 wala) aur product page. Kharcha bachane ko company ne usi Redis me home page / product page ka cache bhi daal diya. Diwali sale ki raat sab page khol rahe, cache badhta gaya, Redis ki memory bhar gayi. Jagah banane ko Redis purani keys phenkne laga (LRU), Ravi ka dabba bhi chala gaya. Bouncer ko dabba mila nahi, naya Ravi samajh ke bhara dabba (5 token) bana diya, script 5 OTP aur le gayi, kisi ko pata nahi chala.**
Arpan: Pehli baat to hum sochenge ki alag-alag Redis me rakho: home page / feed alag Redis me, Ravi ka alag Redis me, to kabhi dikkat nahi aayegi. Agar ek me hi rakhna hai (kharcha bachana hai) to phir hum LRU nahi laga sakte, kyunki dono important hain, kuch evict nahi kar sakte. Ravi ke token aise evict karne ke nahi, wo to use honge. LRU lagaya to usne Ravi ka token hata diya, wahi dikkat ho jaayegi. To LRU eviction policy hata do, Ravi ka mat hatao. Product page Diwali ke baad ek-do din me thanda ho jaayega, tab use hata denge, baaki Redis apna chalta rahega, dikkat hi nahi.
Technical me: "feed alag Redis, Ravi ka alag" = sach (limiter ka dabba) aur cache alag Redis me, "LRU hata do, Ravi ka mat hatao" = maxmemory-policy noeviction (Redis kuch nahi phenkta), "Diwali ke baad thanda, tab hatao" = cache pe TTL.

**Maan lo ek hi Redis, LRU hata diya (noeviction). Diwali ki raat page itne padhe gaye ki TTL khatam hone se pehle hi memory bhar gayi. Ravi ki agli request pe bouncer ko dabbe se token nikaal ke likhna hai.**
Arpan: Ab hamare paas raaste kam ho gaye, kyunki Redis ko bol diya kuch mat phenk; Redis itna bhar sakta hai ki kuch rakh hi na paaye. OTP wali cheez (Ravi ka dabba) nahi phenk sakte, warna dikkat. To product page me kuch karna padega. Hamari team pehle se soche ki Redis bhar sakta hai: jo product page logon ne dekh liya, uska TTL kam kar do, wo jaldi Redis se nikal jaayega; doosra dekh liya to uska bhi kam kar do, Redis balance rahega. Jo nahi dekha uska TTL wahi, dheere-dheere wo bhi nikalta jaayega. Product page to DB se bhi la sakte hain, thodi loading zyada dikhegi, par Redis theek chalega. Redis gira to replica hai. Phir bhi zyada dikkat hui to sabse baad me Redis tod do: product page ka Redis alag, Ravi (OTP) ka alag. Thodi peak DB aur Redis se sambhal li, Diwali thandi padi, phir Redis se sab hata denge, baaki chalta rahega.
Technical me: "dekh liya to TTL kam kar do" = cache pe chhota TTL (bheed me jaldi expire), "product page DB se, thodi loading" = cache miss pe DB, thoda dheema chalega (graceful degradation), "Ravi ka dabba nahi phenk sakte" = sach wali key kabhi evict nahi, "sabse baad me Redis tod do" = sach aur cache alag Redis.

&nbsp;

Poora design = [02_rate_limiter.md](02_rate_limiter.md)
