# Where Is My Train: bina net / GPS ke train kahan hai

[<- saari kahaniyan](README.md)

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

**Sach kitna pakka:** cell tower + offline timetable + logon se data = app ka apna bayaan aur Wikipedia. Andar ki list kaise banti hai,
wo public nahi.
