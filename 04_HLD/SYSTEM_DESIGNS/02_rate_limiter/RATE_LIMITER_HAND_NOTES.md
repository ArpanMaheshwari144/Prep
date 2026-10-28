# Rate limiter: hand notes (mock se pehle ye padho)

### Shuru

**Kaam:** do hain: har user ki ek had (login pe 5 baar / minute, baaki API pe 100 / minute), aur had paar ho to 429 ke saath "kitni der baad aao" (Retry-After).

**Sabse upar:** limiter har request ke aage baitha hai, isliye ~1 ms me haan ya na bolna chahiye. 10 server hon tab bhi user ki ginti ek hi jagah honi chahiye, aur limiter gire to site nahi girni chahiye.

**Numbers:** 1 crore user hain, sabse bheed ke waqt ~10,000 request har second. Har user ke liye sirf 2 chhote number rakhne hain, to kul ~500 MB banta hai. Matlab dikkat jagah ki nahi, speed ki hai, isliye aage RAM wala Redis aayega.

**Maan lo tujhe kisi aur ne 429 diya:** Retry-After maano, aur ruk-ruk ke dobara bhejo (backoff + jitter).

&nbsp;

**1. MEMORY ME MAP**

Sabse seedha: app server apni memory me map rakhe, "arpan ne is minute kitni bheji". Ek server tak ye chalta hai.

&nbsp;

**2. REDIS**

Bheed badhi to 10 server lage. Arpan ki 5 request App 1 pe gayi aur 5 App 2 pe, dono ne alag gina, aur 10 andar chali gayi. Isliye ginti ek jagah rakhi, **REDIS** me. Har request pe INCR karte hain, aur pehli request pe 60 sec ki umar lagti hai. DB nahi liya, kyunki DB disk pe hai aur har baar kai ms lagte, jabki Redis RAM me hai aur ~1 ms leta hai.

**AWS pe:** Redis hum khud nahi chalate, AWS ka **ElastiCache** lete hain (Redis jise AWS chalata hai). Machine, copy, gira to copy aage karna, ye AWS ka kaam. Hum bas use karte hain.

&nbsp;

**3. TOKEN BUCKET**

Ye 60 sec ki khidki pehli request se shuru hoti hai, aur khatam hote hi ginti 0 ho jaati hai. Bot khidki ke aakhri second pe 5 bhejta hai aur nayi khidki ke pehle second pe 5, to ek second me 10 andar. Isliye **TOKEN BUCKET** lagaya: baalti me max 5 token, har 12 sec me ek naya token, har request ek token leti hai, baalti khaali ho to 429. Isme sirf 2 number rakhne padte hain, aur kinare wali dikkat nahi aati. Token bharne ke liye koi job nahi chalti, request aane pe hi hisaab lagta hai ki beete time me kitne bane.

**Maan lo bhaari request (jaise report):** uspe kadi had, ya use zyada token ka maano.

&nbsp;

**4. LUA SCRIPT**

Bot ne ek saath 20 bheji. Sabne ek saath padha "5 bache hain", aur sab andar chali gayi. Isliye padhna, jaanchna aur ghatana teeno ek **LUA SCRIPT** me, ek jhatke me karte hain. Redis ek baar me ek hi command chalata hai, to beech me koi ghus nahi sakta. Pehli 5 ko token milte hain, baaki 15 ko 429.

&nbsp;

**5. API GATEWAY**

Limiter app ke andar tha, to mana wali request bhi app tak aa ke uska time kha rahi thi. Isliye limiter sabse aage, **API GATEWAY** pe lagaya. Mana wali request app tak pahunchti hi nahi, aur har service ko apna limiter code nahi likhna padta.

**Had kahan se aati hai:** DB me user ka plan (free / pro). Had badalni ho to config me, gateway ~30 sec me padh leta hai, naya code nahi chahiye.

**Maan lo traffic 10 guna:** gateway machine badhao, Redis ko user ke hisaab se shard karo.

**AWS pe:** gateway khud nahi banate, AWS ka **API Gateway** lete hain, usme had (throttling) ki setting pehle se hai. Machine aur bheed AWS sambhalta hai.

&nbsp;

**6. COPY + FAIL-OPEN / CLOSED**

Redis gir gaya. Isliye Redis ki **COPY** rakhi, jo kuch second me uski jagah le leti hai. Un kuch second ke liye faisla pehle se tay hai: aam API pe **FAIL-OPEN** (sabko jaane do, site chalti rahe), aur login / OTP pe **FAIL-CLOSED** (sabko 429), kyunki wahan bot password guess karta hai aur OTP pe SMS ka paisa lagta hai.

**Maan lo Redis aur uski copy dono gaye:** har gateway apni memory me gine, had baant ke (100 / 10 gateway = har ek pe 10).

&nbsp;

**7. KAFKA + BAN**

Wahi bot har minute wapas aata hai. Isliye har 429 ki khabar **KAFKA** me jaati hai. Peeche ek Pattern Service dekhti hai ki "ye ghante me 50 baar had paar kar chuka", aur use **BAN** kar deti hai. Ban list WAF pe hoti hai, to ban wala darwaze pe hi ruk jaata hai. Ban kuch der ka hota hai, hamesha ka nahi, kyunki IP badalte rehte hain.

&nbsp;

**8. USERID + LOGIN PE DO HAD**

Ek naya bot leak hui password list leke 1,000 alag account pe 4-4 password try karta hai, kai IP se (credential stuffing). Humne IP pe had lagayi, par office ke 1,000 log ek IP pe the, poora office atak gaya.

**Login ke BAAD:** had **USERID** pe (JWT se). Har insaan ki apni had, IP ek ho ya hazaar. Office ki dikkat yahin khatam.

**Login PE** userId hota hi nahi, to do had: **ACCOUNT** pe kadi (5 / min), taaki ek account pe password guess karne wala ruke. **IP** pe bas bahut dheeli (jaise 500 / min), sirf ek IP se hazaaron ki baadh rokne ke liye. Ye asli deewar nahi, kyunki office me kitne log hain pata nahi (100 bhi, TCS jaise lakh bhi), aur bot kai IP se aata hai.

**Par credential stuffing (har account pe sirf 4, kai IP) in had se nahi rukta.** Asli ilaaj, parat-dar-parat:
- **MFA / passkey:** password mil bhi gaya, to bina phone ke bekaar.
- **CAPTCHA:** shaq wali koshish pe CAPTCHA, galat password pe account block nahi, taaki asli user na atke.
- **WAF:** cloud / badnaam IP band (cloud wale apni IP list publish karte hain).

Ban me bhi yahi: userId pe lamba ban, IP pe chhota.

**AWS pe:** **AWS WAF** lete hain: rules hum likhte hain (kaunsi IP band), chalana AWS ka kaam.

&nbsp;

**9. GLOBAL CAP (503)**

Sale me 1 crore log aaye. Har koi apni had me tha, phir bhi server bhar gaya. Isliye poore system ki ek had bhi hai, **GLOBAL CAP**: uske upar **503** ("hum bhare hain"), jabki 429 ka matlab hai "tu zyada bhej raha". Bhar jaayein to pehle kam zaroori kaam kaato (suggestion, analytics), aur payment / login aakhri tak chalao.

&nbsp;

**10. HOME REGION**

Arpan ki script 3 region se chali, har region ne alag 100 gine, to 300 nikal gayi. Isliye **HOME REGION** rakha: har user ka ek ghar region hota hai, aur ginti sirf wahin hoti hai. Ghar se aaya to 1 ms. Bahar se aaya to ghar se poochho, ~200 ms. Bahar se aksar bot hi aata hai, to uska dheema hona chalta hai.

&nbsp;

### Wrap

tez = Redis + gateway + Lua · ginti ek = Redis + home region · gire to site chale = copy + fail-open (login pe closed) · bot = Kafka + ban


Poora design (har kadam ka diagram aur "kyun") = [02_rate_limiter.md](02_rate_limiter.md)
