// Card 5: rent bheja, SMS service giri, SMS kabhi nahi gaya. Ilaaj: outbox + relay + Kafka. Agar: SMS ulte kram me -> key = account_id.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: transfer karte hi seedha SMS bulaya. SMS service band thi, SMS kabhi nahi gaya'],
  [3, GRN, 'ILAAJ', 'ILAAJ: khabar outbox me usi transaction me (sent=false). Relay COMMIT ke baad Kafka me'],
  [6, YEL, 'AGAR', 'AGAR SMS ulte kram me pahunche? -> Kafka key = account_id, ek account ek partition, kram se'],
];
function setup() {}
const STEPS = [
  [[C.good, 'Bank svc → DB', 'Ravi ko rent 20,000 gaya, paisa pakka'],
   [C.bad, 'Bank svc → SMS', 'Seedha SMS service bulayi, par wo band thi'],
   [C.bad, 'SMS', 'Khabar kahin rakhi hi nahi thi: Ravi ko SMS kabhi nahi gaya']],
  [[C.msg, 'Bank svc → DB', 'Transfer + outbox row (sent=false), dono ek hi transaction me'],
   [C.acc, 'Relay → Kafka', 'Relay har second sent=false wali row Kafka me daalta'],
   [C.good, 'Kafka → SMS', 'SMS service uthi, Kafka se padha, SMS gaya; row sent=true']],
  [[C.msg, 'Relay → Kafka', 'Ravi ke 2 SMS: pehle "20,000 aaye", phir "balance 5,000"'],
   [C.acc, 'Kafka', 'key = account_id: dono ek hi partition P3 me, kram se'],
   [C.good, 'Kafka → SMS', 'SMS service ne P3 kram se padha, Ravi ko sahi kram me mile']],
];
function frame(ph) {
  if (ph === 0) {
    B(15, 140, 180, 60, 'Bank svc', 'transfer', { icon: 'server' });
    B(360, 20, 220, 70, 'DB', '20,000 gaya', { st: 'good', icon: 'db' });
    B(360, 250, 220, 70, 'SMS service', 'band', { st: 'bad', icon: 'mail' }); cross(605, 285);
    A(195, 155, 358, 60, C.good, { n: 1 }); A(195, 185, 358, 280, C.bad, { n: 2 });
    num(660, 285, 3, C.bad); L(740, 200, 'khabar gayi', C.bad);
  } else if (ph === 1) {
    B(15, 30, 180, 60, 'Bank svc', 'transfer', { icon: 'server' });
    B(240, 20, 300, 110, 'DB (ek transaction)', '', { st: 'good', icon: 'db' });
    cells(285, 70, [['transfer', C.good], ['outbox: false', C.acc]], { w: 120, h: 34 });
    A(195, 60, 238, 60, C.msg, { n: 1 });
    B(240, 250, 180, 65, 'Relay', 'har 1 sec', { st: 'acc', icon: 'clock' });
    A(330, 132, 330, 248, C.acc, { dash: 1 });
    B(500, 250, 180, 65, 'Kafka', 'khabar', { st: 'good', icon: 'queue' });
    A(420, 282, 498, 282, C.acc, { n: 2 });
    B(720, 140, 165, 65, 'SMS service', 'sent=true', { st: 'good', icon: 'mail' });
    A(680, 270, 760, 207, C.good, { n: 3 }); L(700, 60, 'band thi to bhi khabar', C.good); L(700, 80, 'Kafka me padi rahi', C.good);
  } else {
    B(15, 140, 180, 60, 'Relay', '', { st: 'acc', icon: 'clock' });
    B(260, 60, 380, 220, 'Kafka: sms', '', { st: 'good', icon: 'queue' });
    T(290, 125, 'P3 (key = Ravi ka account):', { c: C.muted, b: 1, size: 13, a: 'start' });
    cells(318, 140, [['1: 20,000 aaye', C.acc], ['2: balance 5,000', C.acc]], { w: 150, h: 36 });
    T(290, 215, 'P1, P2 ... doosre account', { c: C.muted, b: 1, size: 13, a: 'start' });
    A(195, 170, 258, 170, C.msg, { n: 1 }); num(560, 205, 2, C.acc);
    B(720, 140, 165, 65, 'SMS service', 'kram se', { st: 'good', icon: 'mail' });
    A(642, 175, 718, 175, C.good, { n: 3 });
  }
}
