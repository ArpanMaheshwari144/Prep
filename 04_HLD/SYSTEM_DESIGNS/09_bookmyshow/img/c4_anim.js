// Card 4: 1 lakh log seat map main DB se, booking dheemi. Ilaaj: map Redis (seatmap:show42, ~5 sec), faisla main DB.
// Agar: book hua (DEL key), A1 bik chuki par map me khaali, hot key. Wapas: map nahi mila -> replica se, Redis me.
const DUR = 18;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: 11 baje 1 lakh log seat map main DB se padh rahe, booking dheemi'],
  [3, GRN, 'ILAAJ', 'ILAAJ: seat map Redis se (seatmap:show42, ~5 sec). Faisla sirf main DB'],
  [6, YEL, 'AGAR', 'AGAR kisi ne book kiya, map ka kya? -> booking main DB pe, phir key DEL'],
  [9, YEL, 'AGAR', 'AGAR A1 bik chuki, map me khaali dikhi? -> chalta hai, UPDATE 0 row bolega'],
  [12, YEL, 'AGAR', 'AGAR lakh log ek hi Redis key pe? -> kai copy + App me 1-2 sec ki copy'],
  [15, GRN, 'WAPAS', 'WAPAS: Redis me map nahi mila -> replica se banao, Redis me rakh do'],
];
function setup() {}
function ppl(mSub, mSt) {
  B(20, 40, 130, 70, '1 lakh log', 'map dekh rahe'); if (mSub !== null) B(20, 250, 130, 60, 'Main', mSub, { st: mSt });
  B(230, 145, 110, 80, 'App');
}
function prim(sub, st) { B(690, 200, 180, 80, 'SQL primary', sub, { st: st || 'acc' }); }
function rd(sub, st, rpSt) {
  B(450, 30, 170, 64, 'Redis', sub, { st }); if (rpSt !== 'none') B(710, 30, 150, 64, 'Read replica', 'copy', { st: rpSt });
}
function load(f, good, s) {
  bar(690, 292, 180, 16, f, '', good); T(780, 328, s || 'primary load', { c: good ? C.good : C.bad, size: 12, b: 1 });
}
function frame(ph) {
  if (ph === 0) {
    ppl('Book A1', 'bad'); prim('map + booking', 'bad');
    A(150, 75, 228, 160, C.msg, { n: 1, at: 0.6 });
    A(340, 172, 688, 222, C.msg, { n: 2, at: 0.2 }); dots(8, 450, 186, C.msg, { gap: 26, r: 6 });
    L(520, 165, 'map: main DB se', C.msg);
    A(150, 280, 228, 210, C.acc, { n: 3 }); A(340, 212, 688, 262, C.bad, { dash: 1, n: 4, at: 0.3 });
    L(520, 275, 'booking ruki', C.bad);
    load(0.98, false);
  } else if (ph === 1) {
    ppl('A1 meri, turant', 'good'); prim('faisla yahin'); rd('seatmap:show42', 'good', '');
    A(150, 75, 228, 160, C.msg, { n: 1, at: 0.6 });
    A(340, 160, 448, 75, C.good, { n: 2 }); L(330, 100, 'map: Redis se', C.good);
    T(535, 115, 'umar ~5 sec', { c: C.acc, size: 13, b: 1 });
    A(150, 280, 228, 210, C.msg, { n: 3 }); A(340, 210, 688, 245, C.good, { n: 4 });
    L(520, 250, 'UPDATE: 1 row', C.good);
    load(0.15, true);
  } else if (ph === 2) {
    ppl('Book A1', ''); prim('A1: booked'); rd('key gayi', 'acc', 'none');
    A(150, 280, 228, 210, C.msg, { n: 1 }); A(340, 210, 688, 245, C.good, { n: 2 });
    L(520, 250, 'UPDATE: 1 row', C.good);
    A(340, 160, 448, 75, C.acc, { n: 3 }); L(310, 100, 'DEL seatmap:show42', C.acc);
    L(330, 340, 'hal: booking main DB pe, phir map ki key DEL', C.good);
  } else if (ph === 3) {
    ppl('Book A1', 'bad'); prim('A1: booked'); rd('A1: khaali', 'acc', 'none');
    num(470, 112, 1, C.acc); L(490, 117, 'purana map', C.acc, 'start');
    A(448, 80, 342, 160, C.acc, { n: 2, at: 0.6 }); L(330, 100, 'A1 khaali dikhi', C.acc);
    A(150, 280, 228, 210, C.msg, { n: 3 }); A(340, 210, 688, 245, C.bad, { n: 4 });
    L(520, 250, 'UPDATE: 0 row', C.bad);
    L(300, 340, 'hal: chalta hai, faisla UPDATE ka. Turant: WebSocket / SSE', C.good);
  } else if (ph === 4) {
    ppl(null); rd('hot key', 'bad', 'none');
    dots(6, 30, 125, C.msg, { gap: 20, r: 6 });
    A(150, 75, 228, 160, C.msg, { n: 1, at: 0.6 });
    A(340, 160, 448, 75, C.bad, { n: 2 }); L(320, 100, 'ek key = ek machine', C.bad);
    B(470, 115, 70, 34, 'copy', '', { st: 'good' }); B(550, 115, 70, 34, 'copy', '', { st: 'good' });
    L(285, 245, 'App: 1-2 sec copy', C.good);
    L(330, 340, 'hal: Redis ki kai copy + App me 1-2 sec ki copy', C.good);
  } else {
    ppl(null); rd('seatmap:show42', 'good', 'good');
    A(250, 143, 448, 45, C.acc, { dash: 1, n: 1, at: 0.4 }); L(290, 75, 'miss', C.acc);
    AP([[340, 165], [785, 165], [785, 96]], C.acc, { n: 2, at: 0.7 }); L(640, 188, 'replica se banao', C.acc);
    A(300, 143, 448, 88, C.good, { n: 3, at: 0.6 }); L(440, 135, 'Redis me rakh do', C.good);
  }
}
