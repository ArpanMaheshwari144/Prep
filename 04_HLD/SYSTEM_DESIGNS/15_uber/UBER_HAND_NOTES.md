# Uber: hand notes (Connaught Place se ghar)

Har block = sawaal (scene) -> Arpan ne jo bola, wahi shabd -> Technical me (Arpan ke shabd ka naam).

**Scene:** Arpan shaam 7 baje Connaught Place pe khada hai, ghar ke liye Uber book karna hai. Sheher me 5 lakh driver ghoom rahe hain.

&nbsp;

**1. "Book" dabaya, par server ko pata hi nahi kaunsa driver abhi kahan hai.**
Arpan: Location to phone me khud hai, GPS ka latitude-longitude. Phone khud bhejega, har hilne pe nahi, thodi-thodi der me. Beech me ek dabba rakh lo jiska kaam sirf ek: har driver ki sabse taaza jagah batana.
Technical me: "beech ka dabba" = Location Service, phone har ~4 sec lat/long bhejta.

**2. Har 4 sec, 5 lakh driver = 1.25 lakh update / sec. Normal DB ghut gaya, jagah 30 sec peeche.**
Arpan: Ya to chhoti parchi, kaat ke nayi jagah likho; ya bada dabba jo likhne me tez ho. Crore-crore likhai pe parchi dheemi ho sakti, gir sakti, aur kab tak replica lagaunga, kal driver aur badhenge. To bada bhai bulao, jo isi kaam ke liye bana hai, apni copies khud rakhta: Cassandra.
Technical me: "bada dabba" = Cassandra (likhai tez, apni copies, machine jodo to badhta).

**3. Book pe server 5 lakh driver ka fasla naap raha, 40 sec "dhoondh rahe hain".**
Arpan: Connaught Place pe khade ko ISBT ka driver kyun? Sirf 8-9 km ke andar wale chahiye. Aur 5 lakh me se chhaanto mat: naksha pehle se dabbon me kata ho, server sirf usi dabbe me dekhe jahan Arpan khada. Usme 3 driver, unme jo khaali wo Arpan ko. Baaki 5 lakh kahin bhi ghoomein, lena-dena nahi.
Technical me: "naksha dabbon me" = geohash (grid), "sirf apna dabba" = 5 lakh ki jagah gine-chune driver.

**4. Arpan dabbe ke kinare pe, 200 meter door bagal wale dabbe me khaali driver, app bola "koi nahi".**
Arpan: Dabbe ki seema pe mat chalo. Arpan jahan khada wahan se 8 km ka gola khincho; jin dabbon ko gola chhoota, unke driver bhi dekho.
Technical me: "gola + chhoote dabbe" = apna geohash + aas-paas ke dabbe, phir asli fasla.

**5. Gole me ek khaali driver Raju. Arpan aur Priya ne ek hi second me book dabaya, Raju ke phone pe do ride.**
Arpan: BMS wali parchi: "Raju khaali hai to Arpan ka" ek kadam me. Jiski pehle pahunchi uski parchi, doosre ko "Raju chala gaya", uske liye agla driver. Parchi ki umar: Raju 15 sec me accept na kare to parchi hatao, agla driver.
Technical me: "parchi" = driver pe hold / lock (atomic, sirf FREE ho to), "umar" = TTL.

**6. Diljit ka concert khatam, 50 hazaar log, cab sirf 500.**
Arpan: BMS jaisa: 500 cab hain to 500 log andar, baaki line me, agli baari. 50 hazaar ko server pe bitha ke kya karunga. Pehle 500 sambhalo, phir kiraya badha ke door ki cab bulao.
Technical me: "500 andar, baaki line" = virtual queue, "kiraya badhao, door ki cab aaye" = surge pricing.

**7. Raju ne 14.9 sec pe accept dabaya, server pe 16 sec pe pahuncha, tab tak Priya ko de diya.**
Arpan: Ghadi server ki. 15 sec tak ruka, phir chala gaya; der se aaya to uski parchi phaad do, accept radd. Uber me bhi aise booking cancel hoti dikhti.
Technical me: "ghadi server ki" = TTL ka faisla server pe, der se aaya accept reject.

**8. Raju Connaught Place ke dabbe se Janpath ke dabbe me gaya, purane dabbe me naam pada; Arpan aur Priya dono ko dikh raha.**
Arpan: Sach wahi jo GPS ne abhi bheja: Raju Janpath me. Book pe Raju ki taaza jagah dekho; Arpan ke gole me nahi to use mat do, Arpan ke liye CP me koi aur. Dono ek saath dabaayein to wahi taala.
Technical me: "taaza jagah dekho" = assign se pehle driver ki latest location check (ek driver ki ek sach jagah), lock double se bachata.

&nbsp;

Poora design = [15_uber.md](15_uber.md)
