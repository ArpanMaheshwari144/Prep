// Card 5: PSP ko call, jawab se pehle server gira, DB me kuch nahi. Ilaaj: pehle PENDING, phir webhook ya job.
// Agar: webhook do baar, nakli webhook, webhook aaya hi nahi. Wapas: job ko jawab mila -> SUCCESS / FAILED.
const DUR = 18;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: PSP ko call gayi, jawab se pehle server gira. Paisa gaya ya nahi?'],
  [3, GRN, 'ILAAJ', 'ILAAJ: call se PEHLE DB me PENDING likho. PSP baad me khud bataye (webhook)'],
  [6, YEL, 'AGAR', 'AGAR webhook do baar aaya? -> SUCCESS sirf ek baar, doosra ignore'],
  [9, YEL, 'AGAR', 'AGAR webhook nakli? -> hastakshar jaancho (PSP secret), PENDING se milao'],
  [12, YEL, 'AGAR', 'AGAR webhook aaya hi nahi? -> job purane PENDING ke liye PSP se poochhe'],
  [15, GRN, 'WAPAS', 'WAPAS: job ko PSP ka jawab: hua = SUCCESS, pakka fail = FAILED'],
];
function setup() {}
// o: { sv, ps, db, job } st ; s: { db }
function base(o = {}, s = {}) {
  B(40, 40, 160, 70, 'Payment Svc', '', { st: o.sv });
  B(700, 40, 170, 70, 'PSP', '', { st: o.ps });
  B(40, 240, 160, 80, 'SQL DB', s.db || 'payment ka haal', { st: o.db });
  if (!o.noJob) B(400, 250, 180, 70, 'Reconciliation', 'job', { st: o.job });
}
function frame(ph) {
  if (ph === 0) {
    base({ sv: 'bad', db: 'bad', noJob: 1 }, { db: 'kuch likha nahi' });
    A(200, 60, 698, 60, C.msg, { n: 1 }); L(300, 50, 'call', C.msg);
    num(785, 130, 2, C.acc); L(805, 135, 'paisa kata', C.acc, 'start');
    num(60, 135, 3, C.bad); L(78, 140, 'server gira', C.bad, 'start'); cross(225, 128);
    A(698, 95, 202, 95, C.good, { dash: 1 }); L(450, 120, 'jawab kisi ne nahi suna', C.bad);
    L(500, 230, 'paisa gaya ya nahi? pata nahi', C.bad);
  } else if (ph === 1) {
    base({ sv: 'bad', db: 'good', ps: 'good', noJob: 1 }, { db: 'PENDING -> SUCCESS' });
    A(120, 112, 120, 238, C.acc, { n: 1, at: 0.45 }); L(135, 172, 'pehle: PENDING likha', C.acc, 'start');
    A(200, 75, 698, 75, C.msg, { n: 2, at: 0.5 }); L(450, 62, 'phir: PSP ko call', C.msg);
    cross(225, 128); num(255, 128, 3, C.bad); L(273, 133, 'server gira', C.bad, 'start');
    AP([[785, 112], [785, 300], [202, 300]], C.good, { n: 4, at: 0.55 });
    L(500, 288, 'PSP ka webhook: "hua" -> DB me SUCCESS', C.good);
    L(450, 350, 'server gira tab bhi DB me PENDING tha, isliye haal pata raha', C.good);
  } else if (ph === 2) {
    base({ db: 'good', ps: 'good', noJob: 1 }, { db: 'SUCCESS (ek baar)' });
    AP([[700, 85], [650, 85], [202, 255]], C.good, { n: 1, at: 0.3 }); L(470, 105, 'webhook: SUCCESS likha', C.good);
    AP([[700, 105], [670, 105], [202, 290]], C.acc, { dash: 1, n: 2, at: 0.25 }); L(500, 250, 'webhook dobara: ignore', C.acc);
    L(450, 345, 'hal: SUCCESS sirf ek baar, doosra ignore', C.good);
  } else if (ph === 3) {
    base({ sv: 'good', db: 'good', noJob: 1 }, { db: 'PENDING (badla nahi)' });
    B(400, 150, 160, 60, 'Nakli', 'webhook: hua', { st: 'bad' });
    A(400, 165, 202, 95, C.bad, { n: 1, at: 0.5 }); L(330, 110, 'nakli webhook', C.bad);
    num(60, 135, 2, C.acc); L(78, 140, 'hastakshar galat -> mana', C.acc, 'start');
    L(450, 300, 'hal: hastakshar jaancho (PSP secret)', C.good, 'start');
    L(450, 322, 'aur apne PENDING se milao', C.good, 'start');
  } else if (ph === 4) {
    base({ db: 'good', job: 'good', ps: 'good' }, { db: 'SUCCESS' });
    AP([[700, 100], [650, 100], [420, 185]], C.bad, { dash: 1, n: 1, at: 0.3 }); cross(440, 178);
    L(560, 175, 'webhook kho gaya', C.bad, 'start');
    A(580, 262, 760, 112, C.acc, { n: 2, at: 0.45 }); L(690, 220, 'haal?', C.acc, 'start');
    A(800, 112, 800, 270, C.good); A(800, 285, 582, 290, C.good, { n: 3, at: 0.5 }); L(800, 305, 'hua', C.good, 'start');
    A(398, 290, 202, 285, C.good, { n: 4 }); L(300, 315, 'SUCCESS', C.good);
    L(450, 345, 'hal: job har kuch minute poochhe, raat ko PSP file se milaan', C.good);
  } else {
    base({ db: 'good', job: 'good', ps: 'good' }, { db: 'SUCCESS / FAILED' });
    A(580, 262, 760, 112, C.acc, { n: 1, at: 0.45 }); L(690, 220, 'haal?', C.acc, 'start');
    A(800, 112, 800, 270, C.good); A(800, 285, 582, 290, C.good, { n: 2, at: 0.5 }); L(810, 305, 'jawab', C.good, 'start');
    A(398, 290, 202, 285, C.good, { n: 3 }); L(300, 315, 'hua = SUCCESS', C.good);
    L(300, 345, 'pakka fail = FAILED, kuch wapas nahi', C.good);
  }
}
