# Where Is My Train: hand notes (bina net / GPS ke train kahan)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan Delhi se Jaipur train me hai. Net band, GPS band. Phir bhi app bata raha "train yahan, itni late". Station pe dost khada hai, use bhi dikhna chahiye train kahan hai.

**Ye design kaisa:** Where Is My Train bahut chhota app hai; iska HLD me LB, replica DB jaisa kuch bolna faltu. 100 me se ek app hoga jo ye karta, baaki har jagah net chalta. Kaam ki cheez iska tareeka hai: cell tower.

Uber se farak (Arpan): Uber me tower wala design nahi chalta; Uber net pe hi chalta, net off kiya to error dikhata "net on karo". Yahan poora design tower pe tika hai.

**Ek nazar: dhaancha + dikkat chain (round me sabse pehle ye, phir FR / NFR isi se):**
```
phone (net / GPS band)  --kaunsa tower juda (cell ID)-->  phone me tower list (offline)  -->  "train yahan aas-paas, 5-10 km"
                                                                 |
            timetable phone me = shuru ka andaza · asli khabar aaye to "delayed by X min" + expected time
            train me jinka net chalu  -->  server  -->  station pe dost ke phone pe push
            list kahan se: route ke tukdon me, railway / shehar / Jio se, naya-hata tower aata rahe

 1   net band, GPS band, train kahan              ->  phone kis tower se juda, list se jagah, range jitna andaza (cell ID)
 2   outer pe 25 min ruki, timetable bole aage    ->  asli khabar jeete, "delayed by X min" + expected (ETA)
 3   Arpan ka net band, Jio data nahi deta        ->  train me jinka net chalu wo bhejein, server dost ko push (crowdsourcing)
 4   tower ki list kahan se                       ->  route ke hisaab se tukdon me jodo, list phone me (offline lookup)
 4b  5 bande saalon, Jio naya tower lagaye        ->  railway se list + updates (data partner, data refresh)
 4c  railway ke paas sab nahi                     ->  shehar ki authority / Jio, jo mile jodo (kai source)
```

**FR / NFR (interview me pehle 2 minute):**
```
FR      1  net / GPS band ho tab bhi train kahan, ek andaza dikhe              -> kadam 1
        2  train late ho to "delayed by X min" + expected time                   -> kadam 2
        3  station pe khade dost ko bhi dikhe train kahan                        -> kadam 3

NFR     1  battery na khaaye (GPS nahi, tower se andaza)                         -> kadam 1
        2  pakki jagah nahi, range chalegi (station se pehle log taiyaar rahein)  -> kadam 1
        3  dikhaya time expected hai, pakka nahi                                 -> kadam 2
        4  tower ki list taaza rahe (naya tower lage / hate)                     -> kadam 4, 4b, 4c
```
Numbers / API: baad me, saare design ke saath.

**Tasveer (patri, tower, train):**
```
  tower 40815      tower 40816      tower 40817           tower 40818  40819
      /|\              /|\              /|\                    /|\    /|\
   (  range  )      (  range  )      (  range  )             ( station pe ghane )
 ======================================[ TRAIN ]===================================
 DELHI                                     ^                                JAIPUR
                               phone: "main 40817 se juda hoon"
                               list:  40817 = Jaipur se 25 km pehle
                               app:   train yahan aas-paas (5-10 km)
```

&nbsp;

---

**1. Train me Arpan ka net band, GPS band. Phir bhi app ko batana hai train kahan hai.**

**Arpan:** Mera phone, maan le Delhi se kahin ja raha hoon. Where Is My Train dekhega mere phone se kaunsa tower juda hai.

Jaise-jaise train chali, mere cellular me dande aa rahe; mere paas ka koi tower hai to mera phone aur aas-paas ka tower aapas me continuous baat kar rahe.

Usne (app ne) mere cellular data se nikaal liya ki train aas-paas yahan kahin ho sakti hai. Mere phone ka net band hai, usse farak nahi padta; mera sim ka cellular tower connected hai, train ko wahi chahiye.

Yahan WebSocket nahi, yahan cellular tower hai.

Isliye Where Is My Train me kabhi train aa jaati hai par app dikhata hai abhi nahi aayi: wo andaza lagata hai ki train is 10 ya 5 km ke dayre me kahin bhi ho sakti hai, kyunki tower train se kitni door hai wo nahi pata.

Where Is My Train niche likhta bhi hai ki ye cellular se aa raha.
GPS kyun nahi: pehle socha GPS se connect kar dete hain, dikkat kya hai. Phir dekha GPS on-off hota rehta, battery ke saath chalta, gaon me pahunch gaye to dikkat, kahin net nahi. Tower to bhar-bhar ke aa rahe. Mujhe bas range chahiye, pakki jagah nahi: ye is station se pehle hai, to log taiyaar rahein. Train chalti hai to GPS aur net hilte-julte rehte hain, aur train lohe ka dabba hai, uske andar satellite ka signal achhe se aata hi nahi.

*Technical me:* "kaunsa tower juda" = cell ID (har tower ki pehchaan, bina net / GPS ke phone ko pata), "5-10 km ke dayre me" = tower ki range, pakki jagah nahi (accuracy trade-off), GPS = battery + band jagah pe kamzor. · lohe ka dabba = GPS signal kamzor.

---

**2. Train Rewari se nikli, agla station 40 km door. Beech me outer signal pe 25 minute ruki, phone poore 25 minute ek hi tower se juda. Timetable ke hisaab se is waqt tak agle station pahunch jaani thi.**

**Arpan:** Main train me baitha, train bahar outer pe khadi, aur station pe mera dost khada.

Aas-paas ka tower app pakad lega aur info update kar dega: ye abhi pahunchi nahi, abhi yahan khadi hai, aur neeche "train is delayed by 10 minutes", expected time 10 baje tha, ab 10:15 / 10:30.

Zaroori nahi jo train ka data humne store kar rakha, train wahi pahunch jaaye; wo cheez hum update karenge jab train chalegi. Dost ko dikhega train ruki hui hai bahar outer pe, aur expected time, confirm time nahi.

*Technical me:* timetable = shuruaat ka andaza (offline, phone me), asli khabar jeetti hai: "delayed by X min" + expected time (ETA, pakka nahi).

---

**3. Train me Arpan ka net band. Dost station pe khada, uska net chal raha. Dost ke app ko kahin se khabar chahiye "ye train outer pe ruki hai". Jio ka tower apne user ki info Where Is My Train ko nahi deta.**

**Arpan:** Jio ka tower hai aur Where Is My Train alag app; Jio ka tower apne users ki information bahar share nahi karega, bilkul sahi. To train me aur logon ka net bhi khula hoga aur GPS bhi; 500 me se 499 ka net band nahi.

Ek-do-teen aadmiyon ne bhi net aur GPS on kar rakha hai to Where Is My Train ko wahi chahiye, unse location update kar lega aur info pass on kar dega.

Jahan-jahan log is station pe app khol ke pooch rahe, unke app me update chala jaayega ki train abhi yahan khadi hai.

Mere net band hone se farak nahi, par dost ka net on hona chahiye, kyunki update push hoga; dost bhi net band kare to use location nahi dikhegi.

*Technical me:* tower ka data operator (Jio) ke paas, bahar nahi aata; khabar = train me jinka net chalu unka app server ko bheje (crowdsourcing), server se dost ke phone pe push (dost ka net chahiye). Mera apna hisaab mere phone pe offline.

---

**4. Ye list kahan se: "tower 40817 = Jaipur se 25 km pehle". Railway ke paas tower ki list nahi, Jio / Airtel nahi dete. Tu ye list kaise banayega?**

**Arpan:** Ye information hum apne aap se kar sakte hain. App ek jhatke me nahi banta, tukdon me: pehle UP ka, phir Delhi ka, phir iska, phir usko, phir joda.

Railway se hamein train ka bahut data mil sakta hai: kaunsa route kahan ja raha, kaunsi train kahan chalti. Phir maan le main Delhi se Jaipur gaya, dekh raha hoon is station ke beech 5 tower, us station ke beech 3, us ke beech 4.

Information gather karni padegi: ya to kisi online resource se, ya khud se, ya apne bande laga ke. Jahan station ke paas jaal zyada hai wahan badiya, koi dikkat nahi; gaon ke beech kam jaal hai, wahan dekhna kahan tower hai.

Wo information app me daalni padegi, warna app chal hi nahi sakta. Mil gayi to main ise feed kar lun, to ye precisely location bata sakta hai.

*Technical me:* "tukdon me, route ke hisaab se" = route-wise data collection, "app me daal do" = tower -> patri ki jagah wali list, phone me offline (lookup).

**Maan lo tu akela ya 5 bande har route pe safar karke note karoge to saalon lagenge, aur Jio naya tower lagaye to list purani.**

**Arpan:** Koi bhi patri ke aas-paas tower lagayega to rail department se permission leni padegi; tum aise hi rail ki property ke paas tower install nahi kar sakte.

To kisi authority ke paas ye jaankari pehle se hogi: kaunsa tower naya laga, kaunsa kahan gaya. Wo authority kaun hai, isme deep nahi jaayenge, wo design ka part nahi.

Maan le railway ke paas hai: main railway ke paas gaya, meeting ki, "mujhe ek app banana hai jo train ke liye; mujhe data chahiye, kis station ke beech kitne tower aur kahan, bas".

Isme kisi ki private information nahi, wo to Jio ke paas hogi, wo hamein chahiye bhi nahi. Unhone de di, maine list app me daal di.

Phir kaha "kabhi tower hata ya laga to mujhe wo list bhi chahiye", wo mail pe bhejte rahenge, main app me update karta rahunga.

Ab train Delhi se Jaipur chali, list se mila lunga: yahan 4 tower, yahan 5; tower ID nikaali, dekha yahan khadi, to Jaipur se abhi 25 km door.

*Technical me:* list ka source = data partner (jiske paas permission ka record), updates = naya / hata tower aate rahein, app ki list taaza (data refresh).

**Maan lo railway bole: "ye data hamare paas hai hi nahi, kuch tower private zameen pe, unki permission kahin aur se".**

**Arpan:** Private zameen pe bhi tower aise nahi lagta: tower permissible height se upar hota hai, taar-baar sab alag system, aur jiske ghar pe laga use kiraya jaata.

To har jagah koi na koi central authority hogi: Delhi me MCD ke paas jaankari hogi kahan tower lag raha, Mumbai me BMC, Bangalore, Lucknow me unki authority.

2-3 case le raha hoon; kaunsa asal me chalta, ye hamein bhi nahi pata aur tujhe bhi nahi, kyunki wo data public nahi hota. Jio se poochh sakte hain ki tower kahan-kahan lagaye: de diya to well and good, mana kar diya to wo raasta.

Railway ne 50% di to baaki 50% ke liye shehar ki central authority pe chala jaunga. Ya to Jio dega, ya local authorities se milegi, bas ye 2 jagah.

*Technical me:* kai source se list jodna (railway + shehar ki authority + operator), jo mile wahi; asal me kaise bani wo public nahi.

**Reel me ek aur raasta tha (reel ka daava, kahin aur se jaancha nahi):** founder ne doston ke phone me GPS aur tower ID dono saath record karwaye, map pe tower ne patri ki line kheench di, phir logon ke safar se poore desh ka naqsha, sirf 7-8 MB. Patri ek line hai, to tower badla matlab train aage badhi; station pe tower ghane, wahan andaza lagbhag GPS jitna. App 2015 me Bangalore ki Sigmoid Labs ne banaya, 2018 me Google ne kharida.

&nbsp;

Poora design = [22_where_is_my_train.md](22_where_is_my_train.md)
