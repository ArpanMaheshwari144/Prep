# Where Is My Train: bina net / GPS ke train kahan hai

[<- saari kahaniyan](README.md) · Poora design (notes + main) = [22_where_is_my_train](../SYSTEM_DESIGNS/22_where_is_my_train/22_where_is_my_train.md)

Chhota app hai, 2015 me Bangalore ki Sigmoid Labs ne banaya, 2018 me Google ne kharida. Iska HLD koi nahi poochta (LB, replica
jaisa kuch bolna yahan faltu hai). Kaam ki cheez iska tareeka hai: mobile tower se jagah ka andaza.

**Dikkat kya thi:**
Train me baithe ho, net nahi, GPS band. Phir bhi jaanna hai ki train kahan hai aur kitni late hai.

**Hal (teen tukde):**

```
phone hamesha kisi tower se juda   ->  tower ki pehchaan (cell ID) bina net / GPS ke bhi phone ko pata
app me pehle se list               ->  "kaunsa tower, patri pe kahan" (data pehle se daala hua, design ka hissa nahi)
app me railway ka timetable        ->  saari train + station + time, phone me hi
```

Phone bola "tower X se juda hoon" -> list me tower X = Kanpur se 20 km pehle -> timetable se mila ke "train yahan, itni late".

**Kadam 1: tower ki range = andaza, pakki jagah nahi.**
Tower ki range ke andar train kahin bhi ho sakti hai, 5-10 km ke dayre me. Isliye kabhi train platform pe aa jaati hai par app abhi bhi
"nahi aayi" dikhata hai. App ko pata hai tower kahan hai, par train tower se kitni door hai, ye nahi.

**Kadam 2: asli khabar timetable pe jeetti hai.**
**Maan lo train outer pe 25 min ruki,** timetable ke hisaab se ab tak agle station pahunch jaani thi. Timetable sirf shuruaat ka andaza
hai. Train chalti hai to jo khabar aati hai wahi jeetti hai: "train outer pe ruki hai, delayed by 25 min", aur naya EXPECTED time
(pakka nahi) dikhao: 10 baje tha, ab 10:25.

**Kadam 3: khabar dost tak kaise pahunchti hai.**
Tera net band hai to tere phone pe hisaab offline nikal aata hai, tujhe dikh jaata hai. Par Jio ka tower sirf Jio ka hai; app company
tower se nahi poochh sakti ki kaunsa phone kahan juda hai. Train me jinka net chalu hai (log time pass ke liye net chalu rakhte hi
hain), unka app "is train ka phone abhi tower X pe" server ko bhejta hai, aur server se dost ke phone pe jaata hai. Dost ka net chalu hona
chahiye, kyunki usko khabar net se hi aati hai. Isi ka naam crowdsourcing: logon ke safar se hi list aur khabar dono bante hain.

**Kadam 4: GPS kyun nahi, list kaise bani, aur late kaise pata.**
GPS battery khaata hai aur train ki metal body uska signal kamzor kar deti hai; train ke liye meter nahi, kuch km ka andaza kaafi.
List aise bani (reel ka daava): founder ne doston ke phone me GPS aur tower ID dono saath record karwaye. Map pe daala to towers
ne railway line kheench di, phir logon ke safar se poore desh ka naqsha bana, sirf 7-8 MB. Train sirf patri pe chalti hai, aur patri
ek line hai, to tower badla matlab train aage badhi. Station pe tower ghane hote hain, wahan andaza lagbhag GPS jitna sahi. Late
kitni: poora timetable app me offline hai, to train abhi kahan hai aur timetable ke hisaab se kahan honi chahiye thi, wahi farak
delay hai.

**Maan lo tower ki list koi deta hi nahi (railway ke paas nahi, Jio / Airtel nahi dete), to list kahan se?**
Pehla raasta, khud: app ek jhatke me nahi banta, tukdon me banta (pehle UP, phir Delhi, phir aage). Patri ka raasta railway ke
data se mil jaata (kaunsi train kahan chalti). Phir khud train me chalo aur note karo: is station ke beech 5 tower, us ke beech
3, us ke beech 1. Gaon ke beech jaal kam hai, wahan dhyaan se dekho kahan tower hai. Ye list app me daalni hi padegi, warna app
chal hi nahi sakta.
Doosra raasta, jinke paas jaankari hai: tower koi bhi kahin bhi nahi laga sakta, wo permissible height se upar hota hai, to
permission leni padti hai aur jaankari kisi authority ke paas rehti hai (patri ke paas ho to railway, Delhi me MCD, Mumbai me
BMC). Railway ne 50% di to baaki 50% shehar ki authority se. Jio / Airtel de dein to well and good. Naya tower lage ya hate to
wo update bhejte rahein, app me list taaza karte raho. Kaunsa raasta asal me chalta, ye data public nahi, isliye 2-3 raaste
saath rakho.

**Sach kitna pakka:** cell tower + offline timetable + logon se data = app ka apna bayaan aur Wikipedia. Doston ke phone se list
banne wali baat ek reel ka daava hai, kahin aur se jaancha nahi.
