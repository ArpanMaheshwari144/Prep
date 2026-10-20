// Card 5: deadline, sab har 2 sec haal poochh rahe, DB ghira. Ilaaj: Redis + read replica. Agar: DB DONE, app VALIDATING.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: deadline pe 10,000 log har 2 sec GET /status. Sab seedha primary DB pe, ghira'],
  [3, GRN, 'ILAAJ', 'ILAAJ: GET /status pehle Redis se (1 ms). Nahi mila to replica se, aur Redis me rakh do'],
  [6, YEL, 'AGAR', 'AGAR DB me DONE, app me VALIDATING? -> DONE pe Redis key hatao'],
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
  [[C.msg, 'Users → Status Svc', '10,000 log, har 2 sec: haal?'], [C.bad, 'Status Svc → Primary DB', 'har poochh seedha DB pe'], [C.bad, 'Primary DB', 'ghira: upload ki likhai|bhi dheemi']],
  [[C.msg, 'Users → Status Svc', 'T-101 ka haal?'], [C.good, 'Status Svc → Redis', 'Redis me mila: 1 ms'], [C.acc, 'Status Svc → Read replica', 'nahi mila to replica se laao,|phir Redis me rakh do']],
  [[C.good, 'Worker → Status DB', 'T-101: DONE likha'], [C.acc, 'Worker → Redis', 'usi waqt Redis key hatayi'], [C.acc, 'Status Svc → Redis', 'agla poochh: Redis me nahi mila'], [C.good, 'Status DB → Status Svc', 'DB se naya DONE,|Redis me phir rakha']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 120, 180, 90, 'Users', '10,000, har 2 sec');
    dots(5, 40, 230, C.bad);
    B(330, 120, 200, 90, 'Status Svc', '');
    B(660, 110, 210, 110, 'Primary DB', 'ghira', { st: 'bad' });
    bar(680, 175, 170, 12, 1, '');
    A(200, 165, 328, 165, C.msg, { n: 1 }); L(265, 150, 'haal?', C.msg);
    A(530, 165, 658, 165, C.bad, { n: 2 }); L(595, 150, 'har baar', C.bad);
    num(765, 255, 3, C.bad); L(745, 280, 'likhai bhi dheemi', C.bad);
    L(450, 340, 'padhna bahut zyada, likhna kam. Par sab ek hi DB pe', C.bad);
  } else if (ph === 1) {
    B(20, 130, 150, 70, 'Users', 'T-101?');
    B(260, 130, 170, 70, 'Status Svc', '');
    B(560, 30, 200, 70, 'Redis', 'T-101: VALIDATING', { st: 'good' });
    B(560, 240, 200, 70, 'Read replica', 'copy', { st: 'acc' });
    A(170, 165, 258, 165, C.msg, { n: 1 });
    A(430, 145, 558, 65, C.good, { n: 2 }); L(470, 90, 'mila: 1 ms', C.good, 'end');
    A(430, 185, 558, 275, C.acc, { n: 3, dash: 1 }); L(470, 255, 'nahi mila to', C.acc, 'end');
    
    L(450, 345, 'primary sirf likhai ke liye. Padhna Redis aur replica se', C.good);
  } else {
    panel(0, 'DONE likha', ['Worker', 'T-101 jaancha', 'good'], ['Status DB', 'T-101: DONE', 'good'], 'likha', true, C.good, 'sach DB me');
    panel(1, 'key hatao', ['Worker', 'usi waqt', ''], ['Redis', 'T-101: (hata)', 'acc'], 'DEL key', true, C.acc, 'purana VALIDATING gaya');
    panel(2, 'agla poochh', ['Status Svc', 'T-101?', ''], ['Redis', 'nahi mila', 'acc'], 'poochha', true, C.acc, 'miss');
    panel(3, 'naya haal', ['Status Svc', 'DONE dikha', 'good'], ['Status DB', 'T-101: DONE', 'good'], 'DONE', false, C.good, 'Redis me naya rakha');
    L(450, 320, 'key hatna chhoot bhi jaaye, to umar (TTL) baad khud naya', C.good);
  }
}
