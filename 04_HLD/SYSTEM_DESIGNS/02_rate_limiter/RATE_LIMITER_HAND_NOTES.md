# Rate limiter: hand notes (mock se pehle ye padho)

### Shuru

Kaam do hi hain: har user ki ek had ho (login 5 baar ek minute me, baaki API 100 baar ek minute me), aur had paar ho to 429 bhejo, saath me ye bhi ki kitni der baad aana hai (Retry-After). Limiter har request ke aage baitha hai, isliye ise lagbhag 1 ms me haan ya na bolna hai. 10 server hon tab bhi har user ki ginti ek hi jagah honi chahiye. Aur limiter gire to site nahi girni chahiye. Lagbhag 1 crore user hain aur sabse bheed ke waqt 10,000 request har second. Har user ke liye sirf 2 chhote number rakhne hain, to kul data 500 MB hai. Matlab dikkat jagah ki nahi, SPEED ki hai, isliye aage RAM wala Redis aayega, DB nahi.

1. Sabse seedha design hai app server ki apni memory me ek map: kis user ki is minute kitni request aayi. 5 se zyada hui to 429. Jab tak server ek hai, ye chalta hai.

2. Par bheed badhi to 10 server lag gaye. Arpan ki 5 request App 1 pe gayi aur 5 App 2 pe, dono ne alag-alag gina, aur 10 andar chali gayi. Isliye ginti ek jagah rakhi, REDIS me. Har request pe Redis me ginti +1 karte hain (INCR), aur key ki umar 60 second (TTL), taaki minute khatam hote hi ginti khud mit jaaye. DB nahi, kyunki DB disk pe likhta hai aur kai ms lagte, jabki Redis RAM me hai aur 1 ms me jawab deta hai.

3. Par ghadi ke minute se ginne me dikkat hai. Bot ne 10:00:59 pe 5 bheji aur 10:01:00 pe phir 5, kyunki minute badalte hi ginti 0 ho gayi. Ek second me 10 andar. Isliye TOKEN BUCKET liya: ek baalti me max 5 token, har 12 second me ek token bharta hai, har request ek token leti hai, baalti khaali to 429. Isme sirf 2 number rakhne padte hain, asli user ka jhatka (ek saath 5) bhi chal jaata hai, aur minute badalne se kuch reset nahi hota. Token bharne ke liye koi job nahi chalti: request aane pe hi hisaab lagta hai ki beete time me kitne token bane, baalti ki had tak.

4. Par bot ne ek saath 20 request bheji. Sabne Redis se ek saath padha "5 bache hain", sabne socha jagah hai, aur 20 andar chali gayi. Wajah ye thi ki padhna, jaanchna aur likhna teen alag kadam the, aur beech me doosri request ghus gayi. Isliye teeno kaam ek LUA SCRIPT me bheje. Redis ek baar me ek hi command chalata hai, to script ke beech me koi nahi ghus sakta. Pehli 5 ko token mile, baaki 15 ko 429.

5. Par limiter app ke andar tha, to mana wali request bhi LB se hoti hui app tak aati thi, aur app ka time kharch hota tha. Isliye limiter sabse aage API GATEWAY pe lagaya. Mana wali request app tak pahunchti hi nahi. Har service me alag library isliye nahi, kyunki tab har service ko apna limiter likhna padta aur bot ka bojh phir bhi app pe padta.

6. Ab sab Redis pe tika hai, aur Redis gir gaya. Isliye Redis ki COPY rakhte hain, jo kuch second me uski jagah le leti hai. Un kuch second ke liye pehle se faisla: aam API (search, feed) pe FAIL-OPEN, matlab sabko jaane do, kyunki thodi der bina had ke chalna chhota nuksaan hai aur site band hona bada. Login aur OTP pe FAIL-CLOSED, matlab sabko 429, kyunki yahin bot password ka andaza lagata hai aur OTP pe SMS ka paisa lagta hai.

7. Wahi bot har minute wapas aa jaata hai, 429 khaata hai, phir aata hai. Isliye har 429 ki khabar KAFKA me daali. Peeche ek Pattern Service dekhti hai ki kisne 1 ghante me 50 baar had todi, aur use BAN kar deti hai. Ban list WAF (darwaze ka chowkidar) pe hoti hai, to ban wala Redis tak bhi nahi pahunchta. Kafka isliye ki 429 dena tez rahe aur pattern dhoondhna peeche chale. Ban kuch der ka hota hai (TTL), khaaskar IP ka, kyunki IP badalte rehte hain aur ek IP pe kai log hote hain.

8. Had IP pe thi. Office ki night shift me 1,000 log ek hi IP se aaye, mil ke 100 paar kar gaye, aur poora office atak gaya. Ulta, bot har request alag IP se bheje to IP wali had lagti hi nahi. Isliye had USERID pe lagayi, jo login ke baad JWT se milta hai. Login se pehle userId hota hi nahi, isliye login pe had IP pe aur jis account pe koshish ho rahi hai us account pe.

9. Sale ke din har user apni had ke andar tha, par 1 crore log ek saath aaye aur server bhar gaya. Isliye poore system ki ek had bhi lagayi (GLOBAL CAP): usse upar wali request ko 503, matlab "hum abhi bhare hain, thodi der baad aao". 429 kehta hai "tu zyada bhej raha hai", 503 kehta hai "hum bhare hain". Bhar jaayein to pehle kam zaroori kaam chhodo (suggestion, recommendation), payment aur login aakhri tak chalao.

10. Arpan ki script teen region se chali (Mumbai, Singapore, US), har region ka apna Redis tha, aur har ek ne 100 gine. Had 100 thi, 300 nikal gayi. Isliye HOME REGION: har user ka ek ghar region hai, ginti sirf wahin. Arpan Mumbai se aaya to 1 ms. Uski script US se aayi to US wala Mumbai ke Redis se poochhta hai, 200 ms lagta hai, par ginti sahi rehti hai. Asli user zyadatar apne hi region se aata hai.

### Agar pooche

- Redis aur uski copy dono gir gaye: har gateway apni memory me gine, aur had baant le. 10 machine hain to har machine 10 ek minute me. LB barabar baantta hai, to kul lagbhag 100 hi rehta hai.

- Had kahan se aati hai (free / pro): DB me user ka plan hota hai, gateway use cache kar leta hai.

- Had badalni ho: niyam config me rakhte hain, gateway har 30 second me padh leta hai. Naya code nahi chahiye.

- 429 ke saath kya bhejein: Retry-After (thoda aage-peeche, jaise 28 ya 33 second, taaki sab ek saath na lautein) aur kitni request bachi hain (X-RateLimit-Remaining).

- Bhaari aur halki request: report jaisi bhaari request pe kadi had, ya wazan do (report 50 token, login 1 token).

### Wrap

Request pehle WAF pe aati hai (ban list), phir API Gateway pe limiter: user ki had (429) aur poore system ki had (503). Ginti Redis me hai, token bucket ki Lua script se ek jhatke me, key userId (login pe IP), user ke home region me, aur Redis ki copy ke saath.

tez = Redis RAM me + gateway pe + ek Lua jhatka · ginti ek = Redis + Lua + home region · gire to site chale = copy + fail-open (aam API) / fail-closed (login)


Poora design (har kadam ka diagram aur "kyun") = [02_rate_limiter.md](02_rate_limiter.md)
