# BookMyShow: hand notes (do counter, ek seat)

Sirf block ki heading padho, scene aur hal khud bolo. Atke to neeche dekho.

**Scene:** cinema me do counter ek hi seat bech rahe hain, aur ek-doosre ko pata nahi.
**Shabd:** parchi = hold · gate ki line = waiting room · token = gate ka pass

Banking aur payment se aaya: SQL transaction, pay ki key (booking id), PSP + webhook. Naya sirf seat ka jhagda aur bheed.

&nbsp;

**1. Dono counter ne D5 khaali dekhi aur dono ne bech di**
Dekhna aur parchi lagana ek saath, ek kaam me. Jisne pehle lagayi, seat uski. Doosra counter parchi dekhta hai: "ye Arpan ki hai, nahi bech sakta".
**hold, ek query (UPDATE ... WHERE FREE)**
Interview: "`UPDATE seats SET HELD, Arpan WHERE seat = D5 AND FREE`. Row lock se doosre ko 0 row milti hai, 'seat chali gayi'."

**2. Arpan parchi laga ke chai peene chala gaya**
Parchi thodi der ki: 5 minute me pay nahi kiya, to parchi phaad do, seat agle ki.
**hold expiry (5 min)**
Interview: "HELD ke saath hold_until. Pay sirf tab BOOKED jab hold Arpan ka ho aur umar baaki ho."

**3. Parchi phaadne wala chhutti pe, purani parchi lagi padi**
Parchi pe hi waqt likh do, naya aadmi aaye to counter khud dekh le ki waqt nikal gaya. Chaho to phaadne wale ka backup bhi.
**shart query me (hold_until < now)**
Interview: "HOLD query: seat FREE ho YA hold_until nikal chuka ho. Safai job band ho tab bhi seat milti hai, job sirf map saaf karta."

**4. 10 lakh log, 50 hazaar seat, server dub gaye**
Hall jitni jagah, utne andar. Baaki line me lagao, "agla number tera". Counter badhana kaafi nahi, seat utni hi hain aur sab unhi pe toot padte.
**waiting room + token**
Interview: "Pehle Redis line, har minute kuch hazaar ko token. Sirf token wale HOLD karte. Jhagda chand rows pe hai, machine badhane se lock nahi batt-ta."

**5. Token 10 min ka, parchi 5 min ki, token pehle khatam**
Token sirf gate ka pass tha, andar aa gaya to kaam ho gaya. Pay ke liye seat ki parchi wali ghadi chalti hai.
**do alag ghadiyan (token = gate, hold = seat)**
Interview: "Token sirf HOLD karne ki ijazat. HOLD ho gaya to faisla hold_until karta, token khatam hone se seat nahi jaati."

&nbsp;

Poora design = [09_bookmyshow.md](09_bookmyshow.md)
