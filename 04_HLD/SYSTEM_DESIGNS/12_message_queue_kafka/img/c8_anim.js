// Card 8: Order #101 DB me bana, Kafka bhejne se pehle server gira. Ilaaj: outbox (ek txn) + relay. Agar: relay ne bheja, sent=true se pehle gira.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: order #101 DB me bana, Kafka bhejne se pehle server gira. Khabar gayi hi nahi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: order + khabar EK transaction me (outbox). Relay commit ke BAAD Kafka ko bhejta'],
  [6, YEL, 'AGAR', 'AGAR relay ne bheja, sent = true se pehle gira? -> khabar dobara, consumer E-101 se skip'],
];
function setup() {}
const STEPS = [
  [[C.msg, 'Order svc → DB', 'Ravi ka order #101 DB me bana, COMMIT ho gaya'],
   [C.bad, 'Order svc', 'Kafka ko khabar bhejne se pehle hi server gir gaya'],
   [C.bad, 'Kafka', 'Order hai par khabar kabhi nahi gayi: email, dashboard ko kuch nahi']],
  [[C.msg, 'Order svc → DB', 'Ek transaction: orders(101) + outbox(E-101, sent = false), COMMIT'],
   [C.acc, 'Relay → Kafka', 'Relay har 1 sec sent = false wali row Kafka ko bhejta'],
   [C.good, 'Relay → DB', 'Kafka ka ACK aaya to sent = true; server gira bhi to row DB me thi']],
  [[C.msg, 'Relay → Kafka', 'Relay ne E-101 Kafka ko bheja'],
   [C.bad, 'Relay', 'sent = true likhne se pehle relay gira; uthte hi E-101 phir bheja'],
   [C.good, 'Kafka → Email', 'Email service ne E-101 pehchaana aur skip kiya: Ravi ko EK email']],
];
function frame(ph) {
  B(15, 40, 190, 60, 'Order svc', ph === 0 ? 'gir gaya' : '', { st: ph === 0 ? 'bad' : '', icon: 'server' });
  if (ph === 0) {
    cross(230, 70); num(265, 70, 2, C.bad);
    B(330, 20, 260, 100, 'DB', '', { st: 'good', icon: 'db' }); cells(380, 65, [['orders: #101', C.good]], { w: 190, h: 34 });
    A(110, 102, 110, 140, C.msg); AP([[110, 140], [460, 140], [460, 122]], C.msg, { n: 1, at: 0.4 });
    B(690, 200, 190, 70, 'Kafka', 'E-101 nahi', { st: 'bad', icon: 'queue' });
    A(205, 90, 688, 225, C.bad, { dash: 1, at: 0.5 }); num(785, 300, 3, C.bad);
    L(430, 320, 'order bana, par kisi service ko pata nahi', C.bad);
  } else {
    B(290, 20, 395, 120, 'DB (ek transaction)', '', { st: 'good', icon: 'db' });
    cells(335, 70, [['orders #101', C.good], [ph === 1 ? 'outbox E-101: false' : 'outbox E-101', C.acc]], { w: 168, h: 34 });
    A(205, 70, 288, 70, C.msg, { n: ph === 1 ? 1 : 0 });
    B(300, 250, 200, 65, 'Relay', ph === 2 ? 'gira, phir utha' : 'har 1 sec', { st: ph === 2 ? 'bad' : 'acc', icon: 'clock' });
    B(700, 150, 185, 70, 'Kafka', 'E-101', { st: 'good', icon: 'queue' });
    if (ph === 1) {
      A(400, 142, 400, 248, C.acc, { dash: 1 }); L(470, 195, 'sent = false padha', C.acc);
      A(500, 270, 698, 200, C.acc, { n: 2 });
      AP([[500, 300], [560, 300], [560, 142]], C.good, { n: 3, at: 0.6 }); L(650, 325, 'ACK aaya: sent = true', C.good);
    } else {
      A(500, 270, 698, 200, C.msg, { n: 1, at: 0.35 }); num(400, 232, 2, C.bad);
      B(15, 250, 190, 65, 'Email svc', 'pehle se: skip', { st: 'good', icon: 'mail' });
      AP([[792, 222], [792, 340], [110, 340], [110, 317]], C.good, { n: 3, at: 0.5 }); L(650, 325, 'Ravi ko EK email', C.good);
    }
  }
}
