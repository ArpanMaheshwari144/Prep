# Twitter feed: hand notes (Arpan, Ravi aur King Kohli)

Sirf block ki heading padho, scene aur hal khud bolo. Atke to neeche dekho.

**Scene:** Arpan ne tweet kiya "aaj ka mock 5/5". Ravi Arpan ko follow karta hai, aur King Kohli ko bhi (27 crore followers). Ravi app kholta hai to home feed pe sabke naye tweet, naye pehle.
**Shabd:** taiyaar dabba = inbox, fanout on write · Kohli ki ek parchi = celeb pe fanout nahi, padhte waqt lao (hybrid) · sab milake sort = merge at read · beech ka dabba = Kafka + outbox · parchi ki 15 copy = hot key replicas · aakhri 500 + mota register purana = inbox chhota + S3 · 30 din wala = inactive ka dabba nahi · bada bhai = Cassandra (user id se tod do), follow ka hisaab = Neo4j · apni branch = region · branches ki baatcheet = cross-region copy

Kafka se aaya: beech ka dabba, outbox. Cache se: copy, ek key ek jagah. Banking se: branch. Naya: fanout, hybrid celeb, merge at read.
Video upload: kal padh ke isi scene me (Arpan ne video daala).

&nbsp;

**1. Ravi app khole, server 500 logon ke tweet DB se nikaale, milaye, sort kare; 8 baje 5 crore log, feed 6-7 second, DB hile**
Har baar ek hi kaam, har baar DB kyun khodna. Doston se kaho tum apna mujhe de do, main apne paas rakh lunga aur Ravi aaye to apne paas se turant de dunga. DB dobara hit nahi.
**har user ka taiyaar dabba (inbox), fanout on write**
Interview: "Har user ki feed pehle se Redis inbox me. Tweet aate hi followers ke inbox me jaata hai, Ravi khole to seedha inbox se, DB nahi."

**2. Kohli ne tweet kiya, 27 crore dabbon me daalna, ghante lage, Ravi ko 2 ghante baad dikha, baaki sabke tweet bhi peeche atke**
27 crore ek bahut bada number hai, aur kal 28, 29, 30 crore honge, dikkat badhti rahegi. 100-200, 2000-3000 ke liye dabba theek, Kohli ke liye nahi. Kohli ne century ka tweet kiya to ek chhoti parchi pe likh ke laga do, 27 crore wahi se padh lenge. Na 27 crore dabbe, na DB.
**celeb pe fanout nahi, ek parchi (hybrid fanout)**
Interview: "Bade account ka tweet followers ke inbox me nahi jaata. Uski apni list hoti hai, padhne wala feed khole tab wahan se aata hai."

**3. Ravi ke dabbe me doston ke tweet, par Kohli, Dhoni, SRK ki parchiyan alag; feed ek chahiye, time ke kram me**
Ravi aaye tab sab ek saath lao: uska dabba, aur jin bade logon ko wo follow karta hai unki parchiyan. Jaise DSA me sort karte, sabko time se sort karo aur Ravi ko de do: "le, ye tera aaj ka poora samachar."
**merge at read (inbox + celeb parchiyan, time se sort)**
Interview: "Feed = apna inbox + follow kiye celeb ke naye tweet, dono milake time se sort, phir user ko."

**4. Arpan ke 8,000 followers, "Post" ghoomta rahe jab tak sab dabbe na bharein; 3,000 pe server gira, 5,000 ko kabhi nahi mila**
Pehle ek had rakho: followers had ke paar gaye to Arpan bhi Kohli wale raaste pe. Phir beech me dabba lagao, mera favourite stadium wala bhai Kafka. Post dabate hi bol do "ho gaya, nikal", peeche Kafka dabbe bharta rahe. Server gira to parchi (outbox) pe pehle se likha hai, kuch nahi khoya. Kafka gira to uski copy hai.
**had (threshold) + Kafka async fanout + outbox + copy**
Interview: "Tweet save karke turant OK. Fanout Kafka se peeche hota hai, outbox se crash pe bhi nahi khota. Had ke upar wale account celeb raaste pe."

**5. Kohli ne 99 se six maara, 1 minute me 5 crore refresh, sabko wahi ek parchi chahiye, wo ek machine pe, machine hili, Dhoni-SRK bhi atke**
Parchi pe lakhon ki bheed hai to us parchi ki 10-15 copy bana do, alag-alag machine pe, aur padhne wale bant jaayein. Ek machine pe 5 crore nahi aayenge.
**hot key ki copies (replicas), padhne wale bant jaayein**
Interview: "Viral tweet ek key pe bheed laata hai. Us key ki kai copy alag machine pe, read unme baant do."
**Maan lo Kohli ne "Inida" likh ke 10 second me "India" kar diya, kuch copies purana dikha rahi:** thodi der galat dikhega, chalega. System nahi rokna. Peeche ek ladka (job) sab copies ko naya pahuncha deta, kuch second me sab theek.

**6. 50 crore dabbe memory me, Ravi ke dabbe me 3 saal ke tweet; 30 crore log mahino se aaye nahi, unke dabbe bhi roz bhar rahe; memory ka bill aasmaan pe**
Dabbe me sirf aakhri 500. Koi itna neeche scroll karta nahi, karega to DB se le aayenge. Saal bhar se purana mote register me mat rakho, alag permanent diary (S3, archive) me. Aur jo 30 din se nahi aaya, us pe mehnat bekaar, uska dabba banana band. Wo 40 din baad aaya to "ruk, 2-3 second me la raha", DB se dabba bana ke wapas Redis me.
**inbox me aakhri ~500, purana S3, 30 din se inactive ka dabba nahi (lautne pe DB se banao)**
Interview: "Inbox chhota, sirf aakhri kuch sau. Inactive user ka inbox nahi bharte, wapas aaye to DB se bana dete. Purana data S3 archive me."

**7. Saare tweet ek SQL DB me, din me 50 crore naye, likhai ki line lambi; aur har baar "Arpan ke naye 500" ke liye poori table**
Likhai bahut hai to bade bhai Cassandra ko bulao, wo bhaari likhai jhel leta hai, copy bhi khud rakhta hai. Tweet naam se nahi, user id se todo (do Arpan ho sakte hain): ek id ka saara maal ek jagah, to "Arpan ke naye 500" seedha wahin se. Kaun kisko follow karta hai, uska hisaab Neo4j bhai rakhega.
**Cassandra (user id se partition, naye pehle) + follow graph Neo4j**
Interview: "Tweets Cassandra me, partition key = author id, andar time se, to ek user ke naye tweet ek jagah se. Follow relation alag graph DB me."

**8. Saare server America me, Ravi Delhi me, har request samundar paar; IPL pe India ke 10 crore America ki machine hilayein**
Ravi India me hai to baat India me hi rakho. Lajpat Nagar ke log doosri branch me jaake bheed kar rahe to Lajpat Nagar me hi branch khol do.
**India me apna region**
Interview: "India region me apni Feed / Tweet Service, Redis aur DB ki copy. Indian user ki request India me hi nipat jaati."

**9. Arpan ne India branch me tweet kiya, follower John New York me, America branch ko pata hi nahi; ulta Kohli America se tweet kare to Ravi ko nahi dikhe**
Do branch hain to unhe aapas me baat karni padegi, jaise SBI ki Delhi aur Mumbai branch: Ravi ne Mumbai me 500 daale, Arpan Delhi me poochhe "mere liye kuch aaya?" to bata dete. Tweet apni branch ke register me likho aur doosri branch ko bhi bhej do.
**cross-region copy (branches aapas me tweet bhejti hain)**
Interview: "Har region apne tweet doosre region ko bhi bhejta hai, taaki follower kisi bhi region me ho, feed me aaye."
**Maan lo dono branch ki baat thodi der ruk gayi aur Arpan ne beech me 5 tweet kiye:** ek banda unhe sambhal ke rakhta hai aur dekhta rehta "doosri taraf aaya?". Judte hi bache 5 bhej deta hai, ek bhi nahi chhootta.

&nbsp;

Poora design = [03_twitter_feed.md](03_twitter_feed.md)
