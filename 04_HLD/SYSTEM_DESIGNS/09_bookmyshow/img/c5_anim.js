// Card 5: 2 lakh log seedha DB pe, ek hi rows ke lock pe ruke. Ilaaj: waiting room (~3000 andar) + Kafka (key = show_id).
// Agar: UPDATE me haar, ek hi worker dheema?, Redis restart (ginti). Wapas: andar wala gaya -> agla andar.
const DUR = 18;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 12 baje 2 lakh log seedha DB pe, ek hi rows ke lock pe ruke'],
  [3, GRN, 'ILAAJ', 'ILAAJ: ~3000 andar, baaki line me. Kafka: ek show ek line, ek-ek UPDATE'],
  [6, YEL, 'AGAR', 'AGAR UPDATE me seat haar gaya? -> "seat gayi, doosri lo". Faisla DB ka'],
  [9, YEL, 'AGAR', 'AGAR ek hi Booking worker, dheema nahi? -> UPDATE ~ms ka, hazaar / sec'],
  [12, YEL, 'AGAR', 'AGAR line ki ginti Redis me, Redis restart? -> DB se dobara gin lo'],
  [15, GRN, 'WAPAS', 'WAPAS: andar wala bina liye chala gaya -> line se agla andar aa jaata'],
];
function setup() {}
function ppl() { B(20, 140, 110, 90, '2 lakh log', '12 baje'); }
function wr(sub, st) { B(180, 140, 130, 90, 'Waiting room', sub, { st: st || 'acc' }); }
function app() { B(360, 150, 100, 70, 'App'); }
function db(sub, st) { B(720, 140, 150, 90, 'SQL DB', sub, { st: st || 'acc' }); }
function pipe(kc, wSub, wSt) {
  B(500, 272, 130, 56, 'Kafka', 'key = show_id'); B(720, 275, 150, 56, 'Booking worker', wSub, { st: wSt });
  if (!kc) { AP([[410, 222], [410, 300], [498, 300]], C.line); A(630, 300, 718, 300, C.line); A(795, 273, 795, 232, C.line); }
}
function frame(ph) {
  if (ph === 0) {
    ppl(); app(); db('lock', 'bad');
    A(130, 185, 358, 185, C.msg, { n: 1 }); dots(6, 160, 162, C.msg, { gap: 30, r: 6 });
    A(460, 185, 718, 185, C.bad, { n: 2, at: 0.15 }); dots(8, 530, 210, C.bad, { gap: 22, r: 7 });
    L(590, 150, 'sab ek hi rows ke lock pe ruke', C.bad);
    T(795, 265, 'sab dheema', { c: C.bad, b: 1, size: 16 });
  } else if (ph === 1) {
    ppl(); wr('ginti ~3000'); app(); db('show 42 seats'); pipe(1, 'ek-ek UPDATE', 'good');
    A(130, 185, 178, 185, C.msg, { n: 1 });
    A(310, 185, 358, 185, C.good, { n: 2 }); L(335, 130, '~3000 andar', C.good);
    T(245, 255, 'baaki line me', { c: C.acc, size: 13, b: 1 }); T(245, 273, '(apni jagah dikhti)', { c: C.muted, size: 12 });
    T(75, 255, '"booking ho rahi hai"', { c: C.acc, size: 12 });
    AP([[410, 222], [410, 300], [498, 300]], C.msg, { n: 3, at: 0.5 }); L(565, 350, 'ek show = ek line', C.msg);
    A(630, 300, 718, 300, C.msg, { n: 4 });
    A(795, 273, 795, 232, C.good, { n: 5 });
  } else if (ph === 2) {
    ppl(); wr('ginti ~3000'); app(); db('seat: booked', 'bad'); pipe(0, 'UPDATE', 'acc');
    A(820, 273, 820, 232, C.acc, { n: 1 });
    A(718, 185, 462, 185, C.bad, { n: 2 }); L(590, 170, '0 row: haar gaya', C.bad);
    AP([[410, 148], [410, 105], [75, 105], [75, 138]], C.bad, { n: 3, at: 0.25 }); L(245, 90, '"seat gayi, doosri lo"', C.bad);
    L(180, 330, 'hal: faisla DB ka', C.good);
  } else if (ph === 3) {
    ppl(); wr('ginti ~3000'); app(); db('show 42 seats'); pipe(1, 'hazaar / sec', 'good');
    AP([[410, 222], [410, 300], [498, 300]], C.line);
    A(630, 300, 718, 300, C.msg, { n: 1 }); L(675, 350, 'ek line', C.msg);
    A(795, 273, 795, 232, C.good, { n: 2 }); L(780, 257, '~ms ka UPDATE', C.good, 'end');
    L(240, 330, 'hal: aur chahiye to show ke hisse', C.good); L(240, 348, '(balcony / stall)', C.good);
  } else if (ph === 4) {
    ppl(); wr('ginti: ?', 'bad'); app(); db('sach: seats', 'good'); pipe(0, '', '');
    B(180, 20, 130, 56, 'Redis', 'line ginti', { st: 'bad' }); cross(334, 48); num(334, 15, 1, C.bad);
    L(360, 53, 'restart', C.bad, 'start');
    AP([[795, 138], [795, 108], [245, 108], [245, 138]], C.good, { n: 2, at: 0.4 }); L(420, 98, 'DB se dobara gin lo', C.good);
    L(160, 330, 'hal: ginti sirf darwaza, sach DB ki seats', C.good);
  } else {
    ppl(); wr('ginti ~3000', 'good'); app(); db('show 42 seats'); pipe(0, '', '');
    A(410, 148, 410, 95, C.muted, { n: 1 }); L(420, 85, 'bina liye chala gaya', C.muted, 'start');
    A(310, 185, 358, 185, C.good, { n: 2 }); L(335, 250, 'agla andar', C.good);
  }
}
