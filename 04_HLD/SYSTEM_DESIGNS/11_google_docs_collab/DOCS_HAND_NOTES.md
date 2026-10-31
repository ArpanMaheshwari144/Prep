# Google Docs: hand notes (Arpan aur Ravi, "Trip plan" doc)

Sirf block ki heading padho, scene aur hal khud bolo. Atke to neeche dekho.

**Scene:** Arpan aur Ravi ek hi doc "Trip plan" pe saath likh rahe hain. Doc me hai `Goa jaayenge`.
**Shabd:** badlaav = operation · shabd khiskana = OT · doc ka maalik server = owner (ring) · chhota register = Redis buffer · central authority = permission SQL · tez bachcha = pub/sub

Chat se aaya: khula taar (WebSocket) + chhota ladka (pub/sub). Cache / URL se: ring. Naya: operation, OT, owner, ACK kab.

&nbsp;

**1. Ravi ke Save ne Arpan ki "Goa jaayenge" mita di**
Last write wins nahi chalega. Poora text mat bhejo, sirf badlaav bhejo: yahan ye joda, yahan se ye hataya.
**operation + version**
Interview: "App insert / delete bhejti hai saath me baseVersion ('maine v7 dekh ke kiya'). Koi kisi ka poora text nahi mitata."

**2. Har chhote badlaav pe DB me likhna bhaari** (ye dikkat khud uthayi)
Chhote register me jama karo, thodi der me ek saath DB me daal do.
**buffer + batch write**
Interview: "Operation pehle Redis me, har ~100 ms Cassandra me ek saath. DB pe har akshar ki query nahi."

**3. Arpan ne "Hum " joda, Ravi ne "Goa" ke baad " kal", Ravi ka kal galat jagah**
Shabd khiska do. Arpan ka "Hum" pehle laga, to Ravi ki jagah utni aage. Ban gaya `Hum Goa kal jaayenge`.
**OT (Operational Transformation)**
Interview: "Server kram tay karta. Ravi ka op v7 pe tha, beech me Arpan ke 4 akshar jagah 0 pe lage, to Ravi ki jagah 3 + 4 = 7. Sabke paas ek hi shakal."

**4. Do server, dono ne apne kram me OT lagaya, doc ki do shakal**
Ek central jagah tay kare. Par sabke liye ek nahi: har doc ka apna ek maalik, aur maalik ki ek copy.
**har doc ka ek owner server (docId se ring) + copy**
Interview: "docId ke hash se ring pe owner. Arpan, Ravi kahin bhi jude, op owner pe jaata, OT wahi. Owner gira to copy owner bane."

**5. Register me jama tha, "Saved" dikha diya, server gira, para uda**
Server ki apni memory me nahi, ek alag dabbe me likho aur uski copy rakho. Ek gire to doosra.
**ACK tabhi jab pakka likha (Redis buffer + copy)**
Interview: "'Saved' tabhi jab op Redis (copy ke saath) me likh gaya. Naya owner wahi se uthata, phir batch me Cassandra."

**6. Ravi ko hataya, par uske server ki memory me purana "editor" note**
Ek central authority sach rakhe. Badla to tez bachcha sab servers pe chilla de "Ravi editor nahi".
**permission SQL me (CP) + pub/sub invalidation**
Interview: "Permission ka sach SQL me. Badlaav pe pub/sub se sab server note phaadein. Bachav: khabar chhoot jaaye to memory copy ki umar 30 sec, usse zyada purana nahi. Typing AP, permission CP."

&nbsp;

Poora design = [11_google_docs_collab.md](11_google_docs_collab.md)
