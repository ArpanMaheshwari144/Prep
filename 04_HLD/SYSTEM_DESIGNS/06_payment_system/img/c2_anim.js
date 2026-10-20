// Card 2: wallet 600, phone + laptop dono ne 600 padha, dono ne kaata -> -400. Ilaaj: check + kaato ek atomic UPDATE.
// Agar: WHERE check bhool gaye -> DB ka CHECK pakadta. Wapas: dono deewar, jeetta ek.
const DUR = 12;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: pehle padho, phir kaato. Dono server ne 600 dekha, wallet -400'],
  [3, GRN, 'ILAAJ', 'ILAAJ: check + kaato ek atomic UPDATE. Phone jeeta, laptop ko 0 row'],
  [6, YEL, 'AGAR', 'AGAR WHERE wala check bhool gaye? -> DB ka CHECK (balance >= 0) pakadta'],
  [9, GRN, 'WAPAS', 'WAPAS: WHERE + CHECK dono. Jeetta ek, doosre ko turant "paisa kam"'],
];
function setup() {}
function base(s1, s2, st1, st2, db, dst, dsub) {
  B(20, 40, 130, 60, 'Phone', 'dukaan 500'); B(20, 240, 130, 60, 'Laptop', 'Swiggy 500');
  B(270, 40, 140, 60, 'Server-1', s1, { st: st1 }); B(270, 240, 140, 60, 'Server-2', s2, { st: st2 });
  B(620, 125, 240, 90, 'DB: Arpan wallet', db, { st: dst });
  if (dsub) T(740, 200, dsub, { c: C.muted, size: 12 });
  A(150, 70, 268, 70, C.msg, { n: 1 }); A(150, 270, 268, 270, C.msg, { n: 1 });
}
function frame(ph) {
  if (ph === 0) {
    base('600 dekha', '600 dekha', 'bad', 'bad', 'balance: -400', 'bad');
    A(410, 60, 618, 135, C.acc, { n: 2, at: 0.3 }); A(410, 280, 618, 205, C.acc, { n: 2, at: 0.3 });
    L(500, 45, 'padho: 600', C.acc); L(500, 315, 'padho: 600', C.acc);
    A(410, 90, 618, 160, C.bad, { n: 3, at: 0.6 }); A(410, 250, 618, 185, C.bad, { n: 3, at: 0.6 });
    L(470, 140, 'kaato 500', C.bad); L(470, 210, 'kaato 500', C.bad);
    L(740, 250, 'dono kate, wallet -400', C.bad);
  } else if (ph === 1) {
    base('1 row', '0 row', 'good', 'bad', 'balance: 100', 'good');
    A(410, 75, 618, 150, C.good, { n: 2, at: 0.45 }); L(520, 75, 'UPDATE: 600 se 100', C.good);
    A(410, 265, 618, 190, C.bad, { n: 3, at: 0.45 }); L(540, 290, 'UPDATE: 100 < 500', C.bad);
    L(85, 130, 'hua, 100 bacha', C.good); L(85, 330, '"paisa kam"', C.bad);
    T(450, 160, 'DB ek-ek chalata', { c: C.muted, size: 12 });
    T(450, 180, 'jeetta ek', { c: C.muted, size: 12 });
    L(450, 346, 'SET balance = balance - 500 WHERE id = ? AND balance >= 500', C.acc);
  } else if (ph === 2) {
    base('WHERE nahi', 'WHERE nahi', 'acc', 'bad', 'balance: 100', 'good', 'CHECK (balance >= 0)');
    num(340, 125, 1, C.bad); L(358, 130, 'check bhool gaye', C.bad, 'start');
    A(410, 75, 618, 150, C.good, { n: 2, at: 0.45 }); L(520, 75, '600 se 100', C.good);
    A(410, 265, 618, 190, C.bad, { n: 3, at: 0.45 }); L(540, 290, '100 - 500 = -400 ?', C.bad);
    num(668, 245, 4, C.bad); L(686, 250, 'CHECK ne mana kiya', C.bad, 'start');
    L(450, 346, 'hal: DB ka CHECK (balance >= 0) aakhri deewar hai', C.good);
  } else {
    base('1 row', '0 row', 'good', 'good', 'balance: 100', 'good', 'WHERE + CHECK');
    A(410, 75, 618, 150, C.good, { n: 2, at: 0.45 }); L(520, 75, 'UPDATE: jeeta', C.good);
    A(410, 265, 618, 190, C.good, { n: 3, at: 0.45 }); L(540, 290, '0 row = reject', C.good);
    L(85, 130, 'hua, 100 bacha', C.good); L(85, 330, '"paisa kam" turant', C.good);
  }
}
