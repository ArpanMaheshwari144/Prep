// Card 5: server gira, book gayab. Ilaaj: pehle log, phir book, gira = replay. Agar: replay me der, dono board, paisa. Wapas.
const DUR = 18;
const PH = [
  [0, RED, 'DIKKAT', 'DIKKAT: order seedha RAM ki book me, 10:15 server gira, 4,000 order gayab'],
  [3, GRN, 'ILAAJ', 'ILAAJ: pehle log (disk, #501), phir book. Gira = log dobara chalao (replay)'],
  [6, YEL, 'AGAR', 'AGAR replay me der lagi? -> standby pehle se log padhta, book taiyaar'],
  [9, YEL, 'AGAR', 'AGAR dono board ek saath chala do? -> nahi. Do board = do sach = galat'],
  [12, YEL, 'AGAR', 'AGAR crash me paise ka kya? -> paisa DB me hai, wahan rollback'],
  [15, GRN, 'WAPAS', 'WAPAS: book log se wapas, matching chalu. Dobara chala, kuch nahi badla'],
];
function setup() {}
function orders(n) {
  B(20, 135, 110, 70, 'Orders', '');
  B(200, 120, 220, 100, 'Log (disk)', '', { st: 'good' });
  cells(212, 168, [['#499', C.good], ['#500', C.good], ['#501', C.good]], { w: 62 });
  A(130, 170, 198, 170, C.msg, { n, at: 0.5 });
}
function frame(ph) {
  if (ph === 0) {
    B(20, 135, 110, 70, 'Orders', '');
    A(130, 170, 558, 170, C.msg, { n: 1, at: 0.3 }); L(340, 158, 'seedha book me, kahin likha nahi', C.msg);
    B(560, 100, 300, 140, 'Matching (RAM)', 'book: gayab', { st: 'bad' }); cross(875, 170);
    num(600, 85, 2, C.bad); L(620, 90, '10:15 gira', C.bad, 'start');
    T(710, 290, '4,000 order gayab', { c: C.bad, b: 1, size: 20 });
  } else if (ph === 1) {
    orders(1); L(165, 158, 'pehle', C.msg);
    A(420, 145, 558, 145, C.good, { n: 2, at: 0.3 }); L(500, 133, 'phir book', C.good);
    num(600, 85, 3, C.bad); L(620, 90, '10:15 gira', C.bad, 'start');
    A(420, 200, 558, 200, C.good, { n: 4, at: 0.3, dash: 1 }); L(500, 188, 'replay', C.good);
    B(560, 100, 300, 140, 'Matching (RAM)', 'book: 4,000 wapas', { st: 'good' });
  } else if (ph === 2) {
    orders();
    B(560, 20, 300, 100, 'Matching main', '', { st: 'bad' }); cross(875, 70);
    num(580, 145, 1, C.bad); L(600, 150, 'main gira, replay me der', C.bad, 'start');
    A(420, 200, 558, 255, C.acc, { n: 2, at: 0.3, dash: 1 }); L(480, 262, 'pehle se padhta', C.acc);
    B(560, 220, 300, 100, 'Standby', 'book taiyaar', { st: 'good' });
    num(580, 340, 3, C.good); L(600, 345, 'turant chalu', C.good, 'start');
    L(250, 300, 'hal: standby pehle se taiyaar', C.good);
  } else if (ph === 3) {
    orders();
    B(560, 20, 300, 100, 'Matching main', 'sauda: TCS 10', { st: 'bad' });
    B(560, 220, 300, 100, 'Standby', 'sauda: wahi TCS 10', { st: 'bad' });
    A(420, 140, 558, 75, C.msg, { n: 2, at: 0.5 }); A(420, 200, 558, 265, C.msg, { n: 2, at: 0.5 });
    num(580, 155, 1, C.bad); L(600, 160, 'dono chalu kar diye', C.bad, 'start');
    num(580, 192, 3, C.bad); L(600, 197, 'do board = do sach = galat', C.bad, 'start');
    L(250, 300, 'hal: ek hi chalu, standby sirf padhta', C.good);
  } else if (ph === 4) {
    orders();
    B(560, 20, 300, 100, 'Matching', '', { st: 'bad' }); cross(875, 70);
    num(580, 145, 1, C.bad); L(600, 150, 'crash', C.bad, 'start');
    B(560, 220, 300, 100, 'Wallet DB', 'paisa yahan (SQL)', { st: 'good' });
    num(580, 200, 2, C.acc); L(600, 205, 'rollback', C.acc, 'start');
    A(420, 140, 558, 85, C.good, { n: 3, at: 0.4 }); L(470, 100, 'book log se', C.good);
    L(250, 300, 'hal: paisa DB me rollback, book log se', C.good);
  } else {
    orders(1);
    A(420, 170, 558, 170, C.good, { n: 2, at: 0.3 }); L(500, 158, 'replay', C.good);
    B(560, 100, 300, 140, 'Matching (RAM)', 'book wapas, chalu', { st: 'good' });
    cells(680, 165, [['T1', C.good]], { w: 60 });
    num(420, 295, 3, C.good); L(440, 300, 'dobara chala: kuch nahi badla (trade_id)', C.good, 'start');
  }
}
