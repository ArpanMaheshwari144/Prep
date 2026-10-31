# Rate limiter: hand notes (Arpan aur Ravi, "OTP bhejo" button)

Sirf block ki heading padho, scene aur hal khud bolo. Atke to neeche dekho.

**Scene:** Arpan ki app me "OTP bhejo" ka button hai, har OTP pe SMS company ka bill. Ravi ne mazaak me script chala di jo har second 1000 baar button dabati hai.
**Shabd:** ek bande ki had = rate limit · sabki ginti rakhne wala = Redis · apni raftaar se bharta dabba = token bucket · ek jhatke me check + nikaalo = Lua · gate ka guard = API gateway · papa ke dost vs chor = fail-open / fail-closed · alag dabba = ban list · ghar ka region = home region

Banking se aaya: check + update ek hi jhatke me (yahan Lua). Kafka / notification se: line (queue). Naya: had, token bucket, gate, ban, home region.

&nbsp;

**1. Ravi ki script, dhadadhad SMS, bekaar ka bill**
Ek bande ke liye ek threshold hona chahiye. Aisa nahi ki 1000, 10,000, 20,000 chal le. Limit lagao.
**rate limit (per user)**
Interview: "Har bande ki had, jaise 10 minute me 5 OTP. Had paar to wahin mana, 429 (Too Many Requests)."

**2. 10 server, har server ke paas Ravi ki ginti alag, 5 ki jagah 50 OTP**
Har server galat bata raha hai "abhi had me hai". Unse bolo tu chhod, mujhe bata, ginti main ek jagah rakhunga: 1 + 2 + 3 ...
**Redis (central counter)**
Interview: "Saare server apni ginti chhod ke Redis se poochhte: ravi ki ginti +1 karo, kitni hui? Ginti ek hi jagah."

**3. Ghadi wala khaata, 10:09:58 pe 5 aur 10:10:01 pe 5, 3 second me 10**
Ek dabbe me had rakho: dabbe me max 5 token, aur wo apni raftaar se bharta (har 2 minute me ek token, yaani 10 minute me 5), user apni raftaar se aata, aata hai to token nikaal ke aage. User kuch der na aaye to dabbe me kai token jama, ek saath le sakta hai, koi dikkat nahi.
**token bucket**
Interview: "Dabba raftaar se bharta, khaata badalne ka kinara hi nahi. Jama token se burst allow, par raftaar ki had bani rehti."

**4. Ek token bacha, 20 request ek saath, sab ne padha "1", sab andar**
Redis ek waqt me ek command chalata hai, par app do command bhejta hai: padho, phir nikaalo. Beech me doosre ka "padho" ghus gaya. Sab ek hi jhatke me kar do, andar koi ghus hi na paaye. Banking me check + update ek query, yahan wahi.
**Lua script**
Interview: "Check aur token nikaalna ek Lua script me, Redis use ek hi command ki tarah chalata. Beech me koi nahi."

**5. Limiter OTP Service ke andar, 1000 request saare raaste ka bojh utha ke andar jaake mana**
Guard ghar ke andar kamre me baitha hai, 1000 log ghar me ghus chuke. Guard ko bahar gate pe bithao, andar koi pahunche hi nahi.
**limiter API gateway pe**
Interview: "Limiter sabse aage gateway pe. Mana wali request wahin laut jaati, andar ke server tak bojh nahi."

**6. Guard ki ginti Redis me, Redis gira**
Pehle to guard ka saathi rakho, ek gaya to doosra (replica). Dono gaye to papa ke khaas dost (dekh ke chale jaayenge) aane do, tijori lootne wale chor rok do.
**replica + fail-open / fail-closed**
Interview: "Pehle replica. Wo bhi gaya to feed / home page jaise kaam fail-open (sabko jaane do), OTP jaise paise wale kaam fail-closed (sabko roko)."

**7. Script ruki nahi, din-raat 429 khaati, gate pe karodon "mana"**
Ek baar mana, do baar mana, teesri baar aaya to pakad ke alag dabbe me bitha do: tu yahan se hil nahi sakta, is site ke liye band.
**pattern pakdo, ban list**
Interview: "Baar-baar had todne ka pattern dikhe to naam ban list me. Ban wala gate pe bina gine lautta."

**8. Ravi har baar naya number, number pe had bekaar; IP pe lagai to office ke 500 log atke**
Login ke baad userId hamare paas, wahan koi dikkat nahi, kadi had. Login se pehle thodi dheeli had, par itni dheeli nahi ki script hazaar baar maare. Insaan hazaar baar password haath se nahi daalta. Ek IP se roz wahi hazaar request, ek second me 10,000, ye insaan nahi, us IP ko WAF me daal do. Office ke saare log ek saath roz password nahi bhoolte, isliye pattern dekhte hain. Perfect design koi nahi, Google bhi kabhi toot-ta hai.
**IP pe dheeli had + userId pe kadi had + WAF**
Interview: "Login se pehle IP pe dheeli had taaki office ki jagah bane, login ke baad userId pe kadi. Script jaisa pattern wali IP WAF me."

**9. Result ka din, 1 crore bachche, sab had me, phir bhi OTP Service aur SMS company gire**
Apne server badha do, wo apne haath me. SMS company haath me nahi. To bheed ko seedha mat jaane do, line lagwao: aaram se, ek-ek karke. Beech me ek cheez fit karo jo spike rok le.
**queue (line) + global had (SMS company ki raftaar)**
Interview: "OTP request line (Kafka) me, worker SMS company ki raftaar se ek-ek bhejte. Bheed kitni bhi ho, aage utna hi jaata jitna wo jhel sake. Line bhi bhar jaaye to baaki ko 503, thodi der baad aao."

**10. 3 region, har region ka apna Redis, Ravi VPN se 15 OTP le gaya**
Uska ek region tay karo. Ravi India ka, to ginti India ke Redis me. 3 jagah se maare ya 10 jagah se, ginti wahi ek jagah.
**home region**
Interview: "Har user ka ghar region. Request kisi bhi region me aaye, ginti ghar ke Redis me."

&nbsp;

Poora design = [02_rate_limiter.md](02_rate_limiter.md)
