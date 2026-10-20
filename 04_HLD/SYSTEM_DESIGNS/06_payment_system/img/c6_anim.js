// Card 6: merchant Bank B me, wallet hamare DB me. Ek transaction nahi chalta, payout fail = paisa latka.
// Ilaaj: SAGA, har kadam saga table me, fail pe ulta. Agar: beech me service giri -> table dekh ke wahin se.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: Arpan ka wallet hamare DB me, merchant Bank B me. Ek transaction chalta hi nahi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: SAGA. Step-step chalo, har kadam saga table me. Payout fail = ulta, 500 wapas'],
  [6, YEL, 'AGAR', 'AGAR SAGA ke beech service giri? -> uth ke saga table dekhe, wahin se aage'],
];
function setup() {}
function panel(k, title, top, bot, lab, down, c, note) {
  const x = 15 + k * 220;
  el('rect', { x, y: 5, width: 205, height: 265, rx: 8, fill: '#161d25', stroke: C.line }, DYN);
  num(x + 20, 25, k + 1, c); T(x + 112, 31, title, { c: C.fg, b: 1, size: 15 });
  B(x + 12, 50, 181, 58, top[0], top[1], { st: top[2] });
  B(x + 12, 160, 181, 64, bot[0], bot[1], { st: bot[2] });
  if (lab) { if (down) A(x + 60, 110, x + 60, 158, c); else A(x + 60, 158, x + 60, 110, c); } // khaali lab = teer nahi
  T(x + 72, 139, lab, { c, b: 1, size: 14, a: 'start' });
  T(x + 102, 252, note, { c: C.muted, size: 13 });
}
const STEPS = [
  [[C.acc, 'Payment Svc → Wallet DB', 'Payment Svc ne Arpan ke 500 kaat liye'], [C.bad, 'Payment Svc → Bank B', 'Merchant ke bank B ko payout bheja, fail ho gaya'], [C.bad, 'Arpan', 'Arpan ka 500 gaya, merchant ko nahi mila, paisa beech me latka']],
  [[C.acc, 'Saga Svc → Wallet DB', 'Saga ne Arpan ke 500 kaate aur saga table me DEBITED likha'], [C.bad, 'Saga Svc → Bank B', 'Bank B ko payout gaya, fail hua'], [C.good, 'Saga Svc → Wallet DB', 'Saga ne ulta kaam chalaya: Arpan ko 500 wapas'], [C.good, 'Saga Svc → Arpan', 'Arpan ko "fail, 500 wapas" dikha, hisaab saaf']],
  [[C.bad, 'Saga Svc gira', 'Pehla kadam (DEBITED) likhne ke baad Saga Svc gira'], [C.acc, 'Saga table → Saga Svc', 'Uthne pe saga table padha: t9 DEBITED pe ruka tha'], [C.good, 'Saga Svc → Bank B', 'Wahin se payout bheja, paisa dobara nahi kaata']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 140, 160, 70, 'Payment Svc', 'merchant ko 500');
    B(350, 40, 200, 70, 'Wallet DB (hamara)', 'Arpan -500', { st: 'acc' });
    B(350, 240, 200, 70, 'Bank B (bahar)', 'payout fail', { st: 'bad' }); cross(575, 275);
    A(180, 160, 348, 75, C.acc, { n: 1 }); L(250, 95, 'kata', C.acc);
    A(180, 190, 348, 275, C.bad, { n: 2 }); L(250, 260, 'payout', C.bad);
    B(680, 130, 190, 80, 'Arpan', '500 gaya', { st: 'bad' });
    num(700, 245, 3, C.bad); L(720, 250, 'paisa beech me latka', C.bad, 'start');
    L(450, 345, 'do alag system, inke upar ek BEGIN / COMMIT ho hi nahi sakta', C.bad);
  } else if (ph === 1) {
    panel(0, 'kaato', ['Saga Svc', 't9 step 1', ''], ['Wallet DB', 'Arpan -500', 'acc'], 'likha', true, C.acc, 'saga: DEBITED');
    panel(1, 'payout', ['Saga Svc', 't9 step 2', ''], ['Bank B', 'payout fail', 'bad'], 'payout', true, C.bad, 'saga: PAYOUT fail');
    panel(2, 'ulta kaam', ['Saga Svc', 'compensate', 'acc'], ['Wallet DB', 'Arpan +500', 'good'], 'likha', true, C.good, 'saga: WAPAS');
    panel(3, 'Arpan', ['Arpan', 'fail, 500 wapas', 'good'], ['Saga Svc', '', ''], 'khabar', false, C.good, 'hisaab saaf');
    L(450, 320, 'aage nahi badh sake to pichhla kadam ulta. Paisa kabhi beech me nahi latka', C.good);
  } else {
    B(20, 40, 170, 70, 'Saga Svc', 'step 1 ke baad gira', { st: 'bad' }); cross(215, 75);
    B(20, 240, 170, 70, 'Saga Svc', 'uth gaya', { st: 'good' });
    B(350, 130, 200, 90, 'Saga table', 't9: DEBITED', { st: 'acc' });
    B(690, 130, 180, 90, 'Bank B', 'payout 500', { st: 'good' });
    num(260, 75, 1, C.bad); L(280, 80, 'gira', C.bad, 'start');
    A(350, 205, 192, 265, C.acc, { n: 2 }); L(290, 255, 'padha: DEBITED', C.acc, 'start');
    AP([[190, 290], [780, 290], [780, 222]], C.good, { n: 3 }); L(600, 280, 'wahin se: payout', C.good);
    L(450, 345, 'kaatna dobara nahi, kyunki table me likha hai ki ho chuka', C.good);
  }
}
