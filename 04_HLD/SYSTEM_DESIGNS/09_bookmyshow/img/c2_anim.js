// Card 2: A1 chuni, pay me 3 min, seat available rahi -> Ravi le gaya. Ilaaj: HELD + held_until.
// Agar: umar khatam phir pay (WHERE, refund), koi 10 seat HOLD (had). Wapas: Sweeper seat chhodta.
const DUR = 15;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: maine A1 chuni, pay me 3 min. Seat available rahi, Ravi le gaya'],
  [3, GRN, 'ILAAJ', 'ILAAJ: A1 HELD, held_until = ab + 5 min. Ravi ko A1 held dikhti hai'],
  [6, YEL, 'AGAR', 'AGAR umar khatam, phir pay aaya? -> confirm pe bhi WHERE, 0 row, refund'],
  [9, YEL, 'AGAR', 'AGAR koi 10 seat HOLD karke baith gaya? -> had lagao (ginti + rate limit)'],
  [12, GRN, 'WAPAS', 'WAPAS: app gira, umar khatam -> Sweeper seat chhod deta, A1 phir khaali'],
];
function setup() {}
function clk(s) { T(880, 22, 'samay ' + s, { a: 'end', c: C.muted, size: 13, b: 1 }); }
function ppl(mSub, mSt, rLbl, rSub, rSt) {
  B(20, 50, 130, 60, 'Main', mSub, { st: mSt }); if (rLbl) B(20, 250, 130, 60, rLbl, rSub, { st: rSt });
}
function core(swSt) {
  if (swSt !== 'good') B(310, 145, 110, 80, 'App'); B(580, 100, 270, 120, 'SQL DB', 'seats', { st: 'acc' });
  if (swSt) B(620, 275, 190, 56, 'Sweeper job', 'purane hold saaf', { st: swSt });
}
function frame(ph) {
  if (ph === 0) {
    clk('10:03'); ppl('pay: 3 min', 'bad', 'Ravi', 'ticket A1', 'good'); core();
    A(150, 85, 308, 165, C.msg, { n: 1, at: 0.6 }); L(215, 170, 'A1 chuni', C.msg);
    A(150, 275, 308, 205, C.msg, { n: 2 }); L(205, 225, 'Book A1', C.msg);
    A(420, 190, 578, 190, C.bad, { n: 3 }); T(715, 180, 'A1: available', { c: C.muted, size: 13 });
    T(715, 202, 'A1: booked (Ravi)', { c: C.bad, b: 1, size: 14 });
    A(335, 143, 152, 58, C.bad, { n: 4, at: 0.75 }); L(260, 40, 'pay hua, par A1 gayi', C.bad);
  } else if (ph === 1) {
    clk('10:01'); ppl('pay: seat pakki', 'acc', 'Ravi', 'A1 held', 'bad'); core();
    A(150, 85, 308, 165, C.msg, { n: 1 }); L(215, 170, 'A1 chuni', C.msg);
    A(420, 170, 578, 160, C.acc, { n: 2 }); L(500, 140, 'HELD + umar', C.acc);
    T(715, 180, 'A1: HELD (Main)', { c: C.acc, b: 1, size: 14 }); T(715, 202, 'held_until 10:05', { c: C.acc, size: 13 });
    A(150, 275, 308, 205, C.msg, { n: 3 }); L(205, 225, 'Book A1', C.msg);
    A(420, 205, 578, 205, C.bad, { n: 4 }); L(500, 235, '0 row: held hai', C.bad);
  } else if (ph === 2) {
    clk('10:06'); ppl('seat gayi', 'bad', null); core();
    T(715, 180, 'A1: HELD (Main)', { c: C.acc, b: 1, size: 14 }); T(715, 202, 'held_until 10:05', { c: C.bad, size: 13 });
    num(600, 80, 1, C.acc); L(620, 85, 'umar khatam', C.acc, 'start');
    A(150, 85, 308, 165, C.msg, { n: 2 }); L(215, 170, 'pay aaya', C.msg);
    A(420, 190, 578, 190, C.bad, { n: 3 }); L(500, 222, 'WHERE held + Main', C.bad); L(500, 240, '+ umar: 0 row', C.bad);
    L(300, 335, 'hal: seat gayi, paisa refund', C.good);
  } else if (ph === 3) {
    clk('10:01'); ppl('seat nahi', 'dim', 'Koi', '10 seat HOLD', 'bad'); core();
    A(150, 275, 308, 205, C.bad, { n: 1 }); A(420, 190, 578, 190, C.acc, { n: 2 }); L(500, 170, 'HOLD x10', C.acc);
    dots(10, 620, 175, C.acc, { gap: 20 });
    num(600, 80, 3, C.bad); L(620, 85, 'sabke liye band', C.bad, 'start');
    L(255, 340, 'hal: had lagao: hold ki ginti + rate limit', C.good);
  } else {
    clk('10:05'); ppl('app gira', 'dim', null); core('good');
    num(170, 30, 1, C.bad); cross(170, 62);
    num(600, 80, 2, C.acc); L(620, 85, 'umar khatam', C.acc, 'start');
    A(715, 273, 715, 222, C.good, { n: 3 }); L(775, 252, 'seat chhodo', C.good, 'start');
    T(715, 180, 'A1: available', { c: C.good, b: 1, size: 14 });
    L(715, 202, 'A1 phir khaali', C.good);
  }
}
