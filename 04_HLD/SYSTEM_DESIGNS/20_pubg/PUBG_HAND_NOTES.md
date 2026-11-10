# PUBG: hand notes (Arpan aur Claude ka squad, Pochinki drop)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan aur Claude ek squad me. Plane se Pochinki me utre, Claude ke haath me shotgun. Raat 9 baje India me lakhon log khel rahe, ek match me 100 log, 8 km x 8 km ka Erangel. Uber se aaya: map pe kaun kahan, aas-paas ka gola, gudda, sab wahi.

&nbsp;

**1. Raat 9 baje 1 lakh log ek saath "Start" dabate hain. Ek match me 100 chahiye, Arpan ke match me abhi 37. Tabhi America se ek ladka aaya, Mumbai server tak ping 300 ms, wo bhi isi match me jud gaya.**
Arpan: America wala ladka America ke server pe jaayega, uska yahan matlab nahi. PUBG me aisa hota bhi nahi ki bahar se log aayein; online bhi kholo to India ke log hi judenge. Design India tak, India ke servers pe. PUBG me log hamesha 4-4 ke squad me baithte hain: tere 4 dost hain to tere 4 jud jaayenge. Akela hai aur match chahiye to kisi random squad me daal do jahan 3 log pehle se hain. 4 zaroori bhi nahi, 2 ya 3 ka squad bhi match shuru kar sakta hai. (Raat 3 baje 37 pe atka match: ye matchmaking ki baat hai, main design ye nahi, aage badho.)
Technical me: "America wala America ke server pe" = region-based matchmaking (har desh ka apna server, ping kam), "akele ko 3 wale squad me" = squad fill, "2-3 ka squad bhi chalega" = flexible squad size.

**2. Match shuru. Server har second 20 baar sabki jagah sabko bhej raha: 100 logon ko 99-99 ki khabar. Arpan Pochinki ke ghar me chhupa, 6 km door Military Base me 30 log lad rahe, unki har goli, har kadam Arpan ke phone pe. Phone garam, data jal raha.**
Arpan: Ek khula taar rakh lo, phone mangwa nahi raha, khabar apne aap aa rahi. Par 6 km door ki awaaz PUBG me aati hi nahi, mere aas-paas ki aayegi. Goli chali to ek event bana; server dekhe kaun-kaun us jagah ke aas-paas hai, sirf unko bhejo. Saamne wali building me goli chali to mujhe sunai de, Military Base wali nahi. Ye Uber jaisa hai: hamein aas-paas ka dikhana hai, Arpan ke aas-paas kya ho raha, wahi Arpan ke kaano me jaana chahiye. Map pe bhi aas-paas ke log dikhenge, door ke nahi.
Technical me: "khula taar" = WebSocket / UDP connection (push), "sirf aas-paas walon ko event" = area of interest / interest management (map ko dabbon me baanto, Uber ke geohash jaisa, har player ko sirf aas-paas ke dabbon ki khabar).

**3. Ek hacker ka phone server ko bolta "meri goli Arpan ko lagi", jabki goli deewar pe lagi ya Arpan se 5-6 meter door. Server ne phone ki baat maan li, Arpan mar gaya.**
Arpan: Aisa to wo sabko maar dega, wahi king ban jaayega. Faisla main (server) karunga ki goli kisko, kab, kahan lagi. Goli kahan se chali, beech me koi deewar to nahi aayi? Arpan 5 minute se ek hi jagah khada tha, saanp ban ke chhupa. Goli chalne se pehle, chalne ke baad aur lagne tak Arpan wahin tha, aur goli uske 5-6 meter door lagi; thoda idhar-udhar bhi karun to bhi nahi lagti. To Arpan nahi marega. Aur maan lo maine (baagi motu) Claude ko goli maar di: server ek seedhi line kheenchega, kya Claude us line pe tha? Claude ka gudda bana ke dekha goli kahan lagi: upar lagi to headshot, neeche lagi to "Arpan killed Claude".
Technical me: "server line kheenche, deewar dekhe, gudda bana ke dekhe" = server-side hit validation (raycast: goli ki line, beech me deewar, player ka hitbox), "upar lagi to headshot" = hitbox (sar / body / pair alag), phone sirf "maine is disha me goli chalayi" bhejta, faisla server ka.

**4. Hacker ki goli imaandaar thi. Dikkat ye ki wo 1 second me 500 meter daud ke Arpan ke peeche pahuncha, phone ne bas bola "main ab Arpan ke peeche hoon".**
Arpan: Ye wahi Uber aur Zomato wala design hai. 500 meter to PUBG me ek poori jagah hai, poora School paar. Server ke paas pichhli jagah hai: 1 second baad player ya to wahin hoga ya aas-paas 1 meter me. Jaise Koramangala me khadi gaadi 1 second baad Whitefield nahi pahunch sakti, namumkin. To gadbad hai: ye update phenk do (discard), ye bot hai ya hacker. Is baar ghus gaya to ghus gaya, uski koi baat mat maano, aur agli baar match me ghusne hi mat do.
Technical me: "pichhli jagah se 1 second me itna hi" = server-side movement validation (speed ki had, namumkin chaal reject), "update phenk do" = discard / server ki jagah sach, "agli baar ghusne mat do" = anti-cheat ban.

**5. "Winner winner chicken dinner." Raat 10 baje 5,000 match lagbhag ek saath khatam, 5 lakh players ke kills, damage, rank points ek pal me DB pe. DB ghut gaya, Arpan ki screen pe "Rank points updating..." 2 minute se.**
Arpan: Har cheez turant DB pe? Arpan ne itne kill kiye, Shubham ne itne, Claude ne itne (Claude bechara mar gaya). Mujhe aisa DB chahiye jo likhai dhadadhad pel de: Cassandra, kyunki yahan paise jaisi pakki consistency nahi chahiye, wo kaam server dekh raha. Aur 5,000 to kam bole, lakhon match hote hain. Docs wala scene: har click pe DB pe mat jao; match ke beech parchi pe likho "Arpan ne itna kiya", match khatam ho to ek saath DB me. Lakh match = lakh parchi, parchi kho jaaye to uski copy rakho. Phir sab parchiyan le jao aur DB me bharo, DB pe achanak itna bojh nahi.
Technical me: "likhai dhadadhad" = write-heavy DB (Cassandra), "match ke beech parchi, end me ek saath" = game server memory me jama, match end pe batch, "parchi line me + copy, phir DB" = queue (Kafka) + async write, copy = durable queue (Arpan ko DB ka intezaar nahi, agla match turant).

&nbsp;

Poora design = [20_pubg.md](20_pubg.md)
