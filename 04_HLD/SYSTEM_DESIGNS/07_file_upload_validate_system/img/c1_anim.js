// Card 1: jaanch 3 sec, user ghoomta screen dekhta. Ilaaj: async + tracking id. Agar: Validator 1 ghanta band.
const DUR = 9;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: report.pdf daali. Upload Svc jaanch ke 3 sec ruka, screen ghoomti rahi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: turant T-101 + VALIDATING. Khabar Kafka me, worker peeche jaanch ke DONE likhta'],
  [6, YEL, 'AGAR', 'AGAR Validator 1 ghanta band? -> 4,000 khabar line me padi rehti'],
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
  [[C.msg, 'Arpan → Upload Svc', 'report.pdf bheji'], [C.acc, 'Upload Svc → Validator', 'jaanch karwayi, jawab ka intezaar'], [C.acc, 'Validator → Upload Svc', '3 sec baad "OK"'], [C.bad, 'Upload Svc → Arpan', 'tab jaake jawab. Itni der|screen ghoomti rahi']],
  [[C.acc, 'Upload Svc → Kafka', 'T-101 ki khabar line me daali,|jaanch abhi nahi'], [C.good, 'Upload Svc → Arpan', 'turant T-101, haal VALIDATING'], [C.acc, 'Worker → Validator', 'worker ne line se T-101 uthaya,|peeche jaanch karwayi'], [C.good, 'Worker → Status DB', 'T-101: DONE likha,|tab hi offset aage']],
  [[C.acc, 'Kafka → Worker', 'T-101 uthaya'], [C.bad, 'Worker → Validator', 'call fail, Validator 1 ghanta band'], [C.acc, 'Kafka line', 'DONE nahi likha, offset wahin.|4,000 khabar padi rahi, khoi nahi'], [C.good, 'Worker → Validator', 'Validator wapas aaya,|line ki saari khabar jaanch li']],
];
function frame(ph) {
  if (ph === 0) {
    B(20, 130, 150, 70, 'Arpan', 'screen ghoom rahi', { st: 'bad' });
    B(330, 110, 220, 110, 'Upload Svc', 'thread: wait...', { st: 'bad' });
    B(700, 130, 170, 70, 'Validator', 'jaanch: 3 sec');
    A(170, 150, 328, 150, C.msg, { n: 1 }); L(250, 135, 'report.pdf', C.msg);
    A(550, 150, 698, 150, C.acc, { n: 2 }); L(625, 135, 'jaancho', C.acc);
    A(698, 180, 552, 180, C.acc, { n: 3 }); L(625, 205, '3 sec baad: OK', C.acc);
    A(328, 190, 172, 190, C.bad, { n: 4 }); L(250, 215, 'tab jawab', C.bad);
    L(450, 300, '31 March: 200 upload / sec, har thread 3 sec atka, threads khatam', C.bad);
  } else if (ph === 1) {
    panel(0, 'line me', ['Upload Svc', 'report.pdf', ''], ['Kafka', 'T-101', 'acc'], 'daala', true, C.acc, 'jaanch abhi nahi');
    panel(1, 'turant id', ['Arpan', 'T-101: VALIDATING', 'good'], ['Upload Svc', '', ''], 'turant', false, C.good, 'screen nahi ghoomi');
    panel(2, 'worker', ['Worker', 'Kafka se T-101', 'acc'], ['Validator', 'report.pdf OK', 'good'], 'jaancho', true, C.acc, 'peeche, apni speed');
    panel(3, 'DONE', ['Worker', '', ''], ['Status DB', 'T-101: DONE', 'good'], 'likha', true, C.good, 'phir hi offset aage');
    L(450, 320, 'dheema kaam line me peeche. Arpan T-101 se haal dekhta', C.good);
  } else {
    B(20, 40, 380, 100, 'Kafka line', '', { st: 'acc' });
    cells(35, 80, [['T-101', C.acc], ['T-102', C.acc], ['...', C.line], ['T-4000', C.acc]], { w: 85, h: 32 });
    B(480, 55, 150, 70, 'Worker', 'T-101 pe ruka', { st: 'acc' });
    B(720, 55, 160, 70, 'Validator', '1 ghanta band', { st: 'bad' });
    B(720, 240, 160, 70, 'Validator', 'wapas aaya', { st: 'good' });
    A(400, 90, 478, 90, C.acc, { n: 1 });
    A(630, 90, 718, 90, C.bad, { n: 2, dash: 1 }); L(675, 72, 'call fail', C.bad);
    num(60, 175, 3, C.acc); L(80, 180, 'offset wahin, khabar padi', C.acc, 'start');
    A(555, 127, 718, 270, C.good, { n: 4 }); L(570, 225, 'ab sab jaancho', C.good, 'end');
    L(450, 345, 'khabar khoi nahi. Upload bhi chalta raha, sirf jaanch der se', C.good);
  }
}
